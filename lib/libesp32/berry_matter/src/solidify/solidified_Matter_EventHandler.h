/* Solidification of Matter_EventHandler.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_EventHandler;

/********************************************************************
** Solidified function: queue_event
********************************************************************/
extern const bclass be_class_Matter_EventHandler;
be_local_closure(class_Matter_EventHandler_queue_event,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventHandler, 
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(EventQueued),
    /* K2   */  be_nested_str_weak(priority),
    /* K3   */  be_nested_str_weak(queue_debug),
    /* K4   */  be_nested_str_weak(push),
    /* K5   */  be_nested_str_weak(EVENT_QUEUE_SIZE_MAX),
    /* K6   */  be_nested_str_weak(pop),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(EVENT_DEBUG),
    /* K9   */  be_nested_str_weak(queue_info),
    /* K10  */  be_nested_str_weak(EVENT_INFO),
    /* K11  */  be_nested_str_weak(queue_critical),
    }),
    be_str_weak(queue_event),
    &be_const_str_solidified,
    ( &(const binstruction[58]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x880C0502,  //  0004  GETMBR	R3	R2	K2
      0x88100103,  //  0005  GETMBR	R4	R0	K3
      0x8C100904,  //  0006  GETMET	R4	R4	K4
      0x5C180400,  //  0007  MOVE	R6	R2
      0x7C100400,  //  0008  CALL	R4	2
      0x6010000C,  //  0009  GETGBL	R4	G12
      0x88140103,  //  000A  GETMBR	R5	R0	K3
      0x7C100200,  //  000B  CALL	R4	1
      0x88140105,  //  000C  GETMBR	R5	R0	K5
      0x24100805,  //  000D  GT	R4	R4	R5
      0x78120029,  //  000E  JMPF	R4	#0039
      0x88100103,  //  000F  GETMBR	R4	R0	K3
      0x8C100906,  //  0010  GETMET	R4	R4	K6
      0x58180007,  //  0011  LDCONST	R6	K7
      0x7C100400,  //  0012  CALL	R4	2
      0x88140902,  //  0013  GETMBR	R5	R4	K2
      0xB81A0000,  //  0014  GETNGBL	R6	K0
      0x88180D08,  //  0015  GETMBR	R6	R6	K8
      0x24140A06,  //  0016  GT	R5	R5	R6
      0x78160020,  //  0017  JMPF	R5	#0039
      0x88140109,  //  0018  GETMBR	R5	R0	K9
      0x8C140B04,  //  0019  GETMET	R5	R5	K4
      0x5C1C0800,  //  001A  MOVE	R7	R4
      0x7C140400,  //  001B  CALL	R5	2
      0x6014000C,  //  001C  GETGBL	R5	G12
      0x88180109,  //  001D  GETMBR	R6	R0	K9
      0x7C140200,  //  001E  CALL	R5	1
      0x88180105,  //  001F  GETMBR	R6	R0	K5
      0x24140A06,  //  0020  GT	R5	R5	R6
      0x78160016,  //  0021  JMPF	R5	#0039
      0x88140109,  //  0022  GETMBR	R5	R0	K9
      0x8C140B06,  //  0023  GETMET	R5	R5	K6
      0x581C0007,  //  0024  LDCONST	R7	K7
      0x7C140400,  //  0025  CALL	R5	2
      0x88180B02,  //  0026  GETMBR	R6	R5	K2
      0xB81E0000,  //  0027  GETNGBL	R7	K0
      0x881C0F0A,  //  0028  GETMBR	R7	R7	K10
      0x24180C07,  //  0029  GT	R6	R6	R7
      0x781A000D,  //  002A  JMPF	R6	#0039
      0x8818010B,  //  002B  GETMBR	R6	R0	K11
      0x8C180D04,  //  002C  GETMET	R6	R6	K4
      0x5C200A00,  //  002D  MOVE	R8	R5
      0x7C180400,  //  002E  CALL	R6	2
      0x6018000C,  //  002F  GETGBL	R6	G12
      0x881C010B,  //  0030  GETMBR	R7	R0	K11
      0x7C180200,  //  0031  CALL	R6	1
      0x881C0105,  //  0032  GETMBR	R7	R0	K5
      0x24180C07,  //  0033  GT	R6	R6	R7
      0x781A0003,  //  0034  JMPF	R6	#0039
      0x8818010B,  //  0035  GETMBR	R6	R0	K11
      0x8C180D06,  //  0036  GETMET	R6	R6	K6
      0x58200007,  //  0037  LDCONST	R8	K7
      0x7C180400,  //  0038  CALL	R6	2
      0x80000000,  //  0039  RET	0
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
    /* K2   */  be_nested_str_weak(EVENT_NO_FILENAME),
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
** Solidified class: Matter_EventHandler
********************************************************************/
be_local_class(Matter_EventHandler,
    6,
    NULL,
    be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(EVENT_NO_INCR, -1), be_const_int(1000) },
        { be_const_key_weak(queue_info, -1), be_const_var(1) },
        { be_const_key_weak(queue_critical, 11), be_const_var(2) },
        { be_const_key_weak(queue_event, -1), be_const_closure(class_Matter_EventHandler_queue_event_closure) },
        { be_const_key_weak(counter_event_no_persisted, 6), be_const_var(5) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_EventHandler_init_closure) },
        { be_const_key_weak(get_next_event_no, -1), be_const_closure(class_Matter_EventHandler_get_next_event_no_closure) },
        { be_const_key_weak(dump, -1), be_const_closure(class_Matter_EventHandler_dump_closure) },
        { be_const_key_weak(queue_debug, 10), be_const_var(0) },
        { be_const_key_weak(EVENT_NO_FILENAME, -1), be_nested_str_weak(_matter_event_no) },
        { be_const_key_weak(counter_event_no, -1), be_const_var(4) },
        { be_const_key_weak(EVENT_QUEUE_SIZE_MAX, -1), be_const_int(10) },
        { be_const_key_weak(load_event_no_persisted, -1), be_const_closure(class_Matter_EventHandler_load_event_no_persisted_closure) },
        { be_const_key_weak(device, -1), be_const_var(3) },
    })),
    be_str_weak(Matter_EventHandler)
);

extern const bclass be_class_Matter_EventQueued;

/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_EventQueued;
be_local_closure(class_Matter_EventQueued_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_EventQueued, 
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(endpoint),
    /* K1   */  be_nested_str_weak(path),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(event_id),
    /* K4   */  be_nested_str_weak(event),
    /* K5   */  be_nested_str_weak(is_urgent),
    /* K6   */  be_nested_str_weak(priority),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(matter),
    /* K9   */  be_nested_str_weak(EVENT_CRITICAL),
    /* K10  */  be_nested_str_weak(event_no),
    /* K11  */  be_nested_str_weak(int64),
    /* K12  */  be_nested_str_weak(toint64),
    /* K13  */  be_nested_str_weak(event_number),
    /* K14  */  be_nested_str_weak(raw_tlv),
    /* K15  */  be_nested_str_weak(to_TLV),
    /* K16  */  be_nested_str_weak(tlv2raw),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x88080301,  //  0000  GETMBR	R2	R1	K1
      0x88080500,  //  0001  GETMBR	R2	R2	K0
      0x90020002,  //  0002  SETMBR	R0	K0	R2
      0x88080301,  //  0003  GETMBR	R2	R1	K1
      0x88080502,  //  0004  GETMBR	R2	R2	K2
      0x90020402,  //  0005  SETMBR	R0	K2	R2
      0x88080301,  //  0006  GETMBR	R2	R1	K1
      0x88080504,  //  0007  GETMBR	R2	R2	K4
      0x90020602,  //  0008  SETMBR	R0	K3	R2
      0x88080301,  //  0009  GETMBR	R2	R1	K1
      0x88080505,  //  000A  GETMBR	R2	R2	K5
      0x90020A02,  //  000B  SETMBR	R0	K5	R2
      0x88080306,  //  000C  GETMBR	R2	R1	K6
      0x90020C02,  //  000D  SETMBR	R0	K6	R2
      0x88080106,  //  000E  GETMBR	R2	R0	K6
      0x14080507,  //  000F  LT	R2	R2	K7
      0x780A0000,  //  0010  JMPF	R2	#0012
      0x90020D07,  //  0011  SETMBR	R0	K6	K7
      0x88080106,  //  0012  GETMBR	R2	R0	K6
      0xB80E1000,  //  0013  GETNGBL	R3	K8
      0x880C0709,  //  0014  GETMBR	R3	R3	K9
      0x24080403,  //  0015  GT	R2	R2	R3
      0x780A0002,  //  0016  JMPF	R2	#001A
      0xB80A1000,  //  0017  GETNGBL	R2	K8
      0x88080509,  //  0018  GETMBR	R2	R2	K9
      0x90020C02,  //  0019  SETMBR	R0	K6	R2
      0xB80A1600,  //  001A  GETNGBL	R2	K11
      0x8C08050C,  //  001B  GETMET	R2	R2	K12
      0x8810030D,  //  001C  GETMBR	R4	R1	K13
      0x7C080400,  //  001D  CALL	R2	2
      0x90021402,  //  001E  SETMBR	R0	K10	R2
      0x8C08030F,  //  001F  GETMET	R2	R1	K15
      0x7C080200,  //  0020  CALL	R2	1
      0x8C080510,  //  0021  GETMET	R2	R2	K16
      0x7C080200,  //  0022  CALL	R2	1
      0x90021C02,  //  0023  SETMBR	R0	K14	R2
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_EventQueued
********************************************************************/
be_local_class(Matter_EventQueued,
    7,
    NULL,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(is_urgent, -1), be_const_var(3) },
        { be_const_key_weak(priority, -1), be_const_var(4) },
        { be_const_key_weak(endpoint, -1), be_const_var(0) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_EventQueued_init_closure) },
        { be_const_key_weak(raw_tlv, 2), be_const_var(6) },
        { be_const_key_weak(event_id, 6), be_const_var(2) },
        { be_const_key_weak(cluster, -1), be_const_var(1) },
        { be_const_key_weak(event_no, 0), be_const_var(5) },
    })),
    be_str_weak(Matter_EventQueued)
);
/********************************************************************/
/* End of solidification */
