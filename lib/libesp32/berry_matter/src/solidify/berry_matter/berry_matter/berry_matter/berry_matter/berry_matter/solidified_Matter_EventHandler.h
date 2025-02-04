/* Solidification of Matter_EventHandler.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_EventHandler' ktab size: 58, total: 86 (saved 224 bytes)
static const bvalue be_ktab_class_Matter_EventHandler[58] = {
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
  /* K12  */  be_nested_str_weak(counter_event_no),
  /* K13  */  be_nested_str_weak(add),
  /* K14  */  be_nested_str_weak(counter_event_no_persisted),
  /* K15  */  be_nested_str_weak(load_event_no_persisted),
  /* K16  */  be_nested_str_weak(persist),
  /* K17  */  be_nested_str_weak(find),
  /* K18  */  be_nested_str_weak(EVENT_NO_KEY),
  /* K19  */  be_nested_str_weak(0),
  /* K20  */  be_nested_str_weak(int64),
  /* K21  */  be_nested_str_weak(fromstring),
  /* K22  */  be_nested_str_weak(EVENT_NO_INCR),
  /* K23  */  be_nested_str_weak(setmember),
  /* K24  */  be_nested_str_weak(tostring),
  /* K25  */  be_nested_str_weak(save),
  /* K26  */  be_nested_str_weak(push),
  /* K27  */  be_nested_str_weak(EVENT_QUEUE_SIZE_MAX),
  /* K28  */  be_nested_str_weak(pop),
  /* K29  */  be_nested_str_weak(matter),
  /* K30  */  be_nested_str_weak(EVENT_DEBUG),
  /* K31  */  be_nested_str_weak(EVENT_INFO),
  /* K32  */  be_nested_str_weak(compact),
  /* K33  */  be_nested_str_weak(device),
  /* K34  */  be_nested_str_weak(EventQueued),
  /* K35  */  be_nested_str_weak(get_next_event_no),
  /* K36  */  be_nested_str_weak(loglevel),
  /* K37  */  be_const_int(3),
  /* K38  */  be_nested_str_weak(),
  /* K39  */  be_nested_str_weak(_X2C_X20),
  /* K40  */  be_nested_str_weak(val),
  /* K41  */  be_nested_str_weak(_X25i_X2E_X25i_X2E_X25i_X2E_X25i),
  /* K42  */  be_nested_str_weak(CRIT_X20_X20),
  /* K43  */  be_nested_str_weak(INFO_X20_X20),
  /* K44  */  be_nested_str_weak(DEBUG_X20),
  /* K45  */  be_nested_str_weak(get_event_name),
  /* K46  */  be_nested_str_weak(_X28),
  /* K47  */  be_nested_str_weak(_X29_X20),
  /* K48  */  be_nested_str_weak(MTR_X3A_X20_X2BAdd_Event_X20_X28_X25s_X258s_X29_X20_X5B_X2502X_X5D_X2504X_X2F_X2502X_X20_X25s_X2D_X20_X25s),
  /* K49  */  be_nested_str_weak(event_no),
  /* K50  */  be_nested_str_weak(endpoint),
  /* K51  */  be_nested_str_weak(cluster),
  /* K52  */  be_nested_str_weak(event_id),
  /* K53  */  be_nested_str_weak(queue_event),
  /* K54  */  be_nested_str_weak(message_handler),
  /* K55  */  be_nested_str_weak(im),
  /* K56  */  be_nested_str_weak(subs_shop),
  /* K57  */  be_nested_str_weak(event_published),
};


extern const bclass be_class_Matter_EventHandler;

/********************************************************************
** Solidified function: dump
********************************************************************/
be_local_closure(class_Matter_EventHandler_dump,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventHandler,     /* shared constants */
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
be_local_closure(class_Matter_EventHandler_get_last_event_no,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventHandler,     /* shared constants */
    be_str_weak(get_last_event_no),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x8804010C,  //  0000  GETMBR	R1	R0	K12
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_next_event_no
********************************************************************/
be_local_closure(class_Matter_EventHandler_get_next_event_no,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventHandler,     /* shared constants */
    be_str_weak(get_next_event_no),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x8804010C,  //  0000  GETMBR	R1	R0	K12
      0x8C04030D,  //  0001  GETMET	R1	R1	K13
      0x580C0009,  //  0002  LDCONST	R3	K9
      0x7C040400,  //  0003  CALL	R1	2
      0x90021801,  //  0004  SETMBR	R0	K12	R1
      0x8804010C,  //  0005  GETMBR	R1	R0	K12
      0x8808010E,  //  0006  GETMBR	R2	R0	K14
      0x28040202,  //  0007  GE	R1	R1	R2
      0x78060001,  //  0008  JMPF	R1	#000B
      0x8C04010F,  //  0009  GETMET	R1	R0	K15
      0x7C040200,  //  000A  CALL	R1	1
      0x8804010C,  //  000B  GETMBR	R1	R0	K12
      0x80040200,  //  000C  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_event_no_persisted
********************************************************************/
be_local_closure(class_Matter_EventHandler_load_event_no_persisted,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventHandler,     /* shared constants */
    be_str_weak(load_event_no_persisted),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0xA4062000,  //  0000  IMPORT	R1	K16
      0x60080008,  //  0001  GETGBL	R2	G8
      0x8C0C0311,  //  0002  GETMET	R3	R1	K17
      0x88140112,  //  0003  GETMBR	R5	R0	K18
      0x58180013,  //  0004  LDCONST	R6	K19
      0x7C0C0600,  //  0005  CALL	R3	3
      0x7C080200,  //  0006  CALL	R2	1
      0xB80E2800,  //  0007  GETNGBL	R3	K20
      0x8C0C0715,  //  0008  GETMET	R3	R3	K21
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C0C0400,  //  000A  CALL	R3	2
      0x90021803,  //  000B  SETMBR	R0	K12	R3
      0x880C010C,  //  000C  GETMBR	R3	R0	K12
      0x8C0C070D,  //  000D  GETMET	R3	R3	K13
      0x88140116,  //  000E  GETMBR	R5	R0	K22
      0x7C0C0400,  //  000F  CALL	R3	2
      0x90021C03,  //  0010  SETMBR	R0	K14	R3
      0x8C0C0317,  //  0011  GETMET	R3	R1	K23
      0x88140112,  //  0012  GETMBR	R5	R0	K18
      0x8818010E,  //  0013  GETMBR	R6	R0	K14
      0x8C180D18,  //  0014  GETMET	R6	R6	K24
      0x7C180200,  //  0015  CALL	R6	1
      0x7C0C0600,  //  0016  CALL	R3	3
      0x8C0C0319,  //  0017  GETMET	R3	R1	K25
      0x7C0C0200,  //  0018  CALL	R3	1
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: queue_event
********************************************************************/
be_local_closure(class_Matter_EventHandler_queue_event,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventHandler,     /* shared constants */
    be_str_weak(queue_event),
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
      0x88080308,  //  0000  GETMBR	R2	R1	K8
      0x880C0105,  //  0001  GETMBR	R3	R0	K5
      0x8C0C071A,  //  0002  GETMET	R3	R3	K26
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C0C0400,  //  0004  CALL	R3	2
      0x600C000C,  //  0005  GETGBL	R3	G12
      0x88100105,  //  0006  GETMBR	R4	R0	K5
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8810011B,  //  0008  GETMBR	R4	R0	K27
      0x240C0604,  //  0009  GT	R3	R3	R4
      0x780E0029,  //  000A  JMPF	R3	#0035
      0x880C0105,  //  000B  GETMBR	R3	R0	K5
      0x8C0C071C,  //  000C  GETMET	R3	R3	K28
      0x58140007,  //  000D  LDCONST	R5	K7
      0x7C0C0400,  //  000E  CALL	R3	2
      0x88100708,  //  000F  GETMBR	R4	R3	K8
      0xB8163A00,  //  0010  GETNGBL	R5	K29
      0x88140B1E,  //  0011  GETMBR	R5	R5	K30
      0x24100805,  //  0012  GT	R4	R4	R5
      0x78120020,  //  0013  JMPF	R4	#0035
      0x88100104,  //  0014  GETMBR	R4	R0	K4
      0x8C10091A,  //  0015  GETMET	R4	R4	K26
      0x5C180600,  //  0016  MOVE	R6	R3
      0x7C100400,  //  0017  CALL	R4	2
      0x6010000C,  //  0018  GETGBL	R4	G12
      0x88140104,  //  0019  GETMBR	R5	R0	K4
      0x7C100200,  //  001A  CALL	R4	1
      0x8814011B,  //  001B  GETMBR	R5	R0	K27
      0x24100805,  //  001C  GT	R4	R4	R5
      0x78120016,  //  001D  JMPF	R4	#0035
      0x88100104,  //  001E  GETMBR	R4	R0	K4
      0x8C10091C,  //  001F  GETMET	R4	R4	K28
      0x58180007,  //  0020  LDCONST	R6	K7
      0x7C100400,  //  0021  CALL	R4	2
      0x88140908,  //  0022  GETMBR	R5	R4	K8
      0xB81A3A00,  //  0023  GETNGBL	R6	K29
      0x88180D1F,  //  0024  GETMBR	R6	R6	K31
      0x24140A06,  //  0025  GT	R5	R5	R6
      0x7816000D,  //  0026  JMPF	R5	#0035
      0x88140103,  //  0027  GETMBR	R5	R0	K3
      0x8C140B1A,  //  0028  GETMET	R5	R5	K26
      0x5C1C0800,  //  0029  MOVE	R7	R4
      0x7C140400,  //  002A  CALL	R5	2
      0x6014000C,  //  002B  GETGBL	R5	G12
      0x88180103,  //  002C  GETMBR	R6	R0	K3
      0x7C140200,  //  002D  CALL	R5	1
      0x8818011B,  //  002E  GETMBR	R6	R0	K27
      0x24140A06,  //  002F  GT	R5	R5	R6
      0x78160003,  //  0030  JMPF	R5	#0035
      0x88140103,  //  0031  GETMBR	R5	R0	K3
      0x8C140B1C,  //  0032  GETMET	R5	R5	K28
      0x581C0007,  //  0033  LDCONST	R7	K7
      0x7C140400,  //  0034  CALL	R5	2
      0x80000000,  //  0035  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(class_Matter_EventHandler_every_second,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventHandler,     /* shared constants */
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x8C040120,  //  0000  GETMET	R1	R0	K32
      0x7C040200,  //  0001  CALL	R1	1
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_EventHandler_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventHandler,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x90024201,  //  0000  SETMBR	R0	K33	R1
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x90020A02,  //  0003  SETMBR	R0	K5	R2
      0x60080012,  //  0004  GETGBL	R2	G18
      0x7C080000,  //  0005  CALL	R2	0
      0x90020802,  //  0006  SETMBR	R0	K4	R2
      0x60080012,  //  0007  GETGBL	R2	G18
      0x7C080000,  //  0008  CALL	R2	0
      0x90020602,  //  0009  SETMBR	R0	K3	R2
      0x8C08010F,  //  000A  GETMET	R2	R0	K15
      0x7C080200,  //  000B  CALL	R2	1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_min_no
********************************************************************/
be_local_closure(class_Matter_EventHandler_find_min_no,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        7,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
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
    }),
    1,                          /* has constants */
    &be_ktab_class_Matter_EventHandler,     /* shared constants */
    be_str_weak(find_min_no),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0004,  //  0002  JMPF	R2	#0008
      0x8808010C,  //  0003  GETMBR	R2	R0	K12
      0x28080202,  //  0004  GE	R2	R1	R2
      0x780A0001,  //  0005  JMPF	R2	#0008
      0x4C080000,  //  0006  LDNIL	R2
      0x80040400,  //  0007  RET	1	R2
      0x84080000,  //  0008  CLOSURE	R2	P0
      0x4C0C0000,  //  0009  LDNIL	R3
      0x5C100400,  //  000A  MOVE	R4	R2
      0x5C140600,  //  000B  MOVE	R5	R3
      0x88180105,  //  000C  GETMBR	R6	R0	K5
      0x5C1C0200,  //  000D  MOVE	R7	R1
      0x7C100600,  //  000E  CALL	R4	3
      0x5C0C0800,  //  000F  MOVE	R3	R4
      0x5C100400,  //  0010  MOVE	R4	R2
      0x5C140600,  //  0011  MOVE	R5	R3
      0x88180104,  //  0012  GETMBR	R6	R0	K4
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
be_local_closure(class_Matter_EventHandler_publish_event,   /* name */
  be_nested_proto(
    23,                          /* nstack */
    9,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventHandler,     /* shared constants */
    be_str_weak(publish_event),
    &be_const_str_solidified,
    ( &(const binstruction[112]) {  /* code */
      0xB8263A00,  //  0000  GETNGBL	R9	K29
      0x8C241322,  //  0001  GETMET	R9	R9	K34
      0x8C2C0123,  //  0002  GETMET	R11	R0	K35
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
      0xB82A0000,  //  000D  GETNGBL	R10	K0
      0x8C281524,  //  000E  GETMET	R10	R10	K36
      0x58300025,  //  000F  LDCONST	R12	K37
      0x7C280400,  //  0010  CALL	R10	2
      0x782A0052,  //  0011  JMPF	R10	#0065
      0x58280026,  //  0012  LDCONST	R10	K38
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
      0x002E4E0B,  //  0020  ADD	R11	K39	R11
      0x0028140B,  //  0021  ADD	R10	R10	R11
      0x4C2C0000,  //  0022  LDNIL	R11
      0x202C100B,  //  0023  NE	R11	R8	R11
      0x782E0004,  //  0024  JMPF	R11	#002A
      0x602C0008,  //  0025  GETGBL	R11	G8
      0x5C301000,  //  0026  MOVE	R12	R8
      0x7C2C0200,  //  0027  CALL	R11	1
      0x002E4E0B,  //  0028  ADD	R11	K39	R11
      0x0028140B,  //  0029  ADD	R10	R10	R11
      0x542E0027,  //  002A  LDINT	R11	40
      0x1C2C040B,  //  002B  EQ	R11	R2	R11
      0x782E0014,  //  002C  JMPF	R11	#0042
      0x1C2C0707,  //  002D  EQ	R11	R3	K7
      0x782E0012,  //  002E  JMPF	R11	#0042
      0x882C0D28,  //  002F  GETMBR	R11	R6	K40
      0x60300018,  //  0030  GETGBL	R12	G24
      0x58340029,  //  0031  LDCONST	R13	K41
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
      0x1C2C0B06,  //  0042  EQ	R11	R5	K6
      0x782E0001,  //  0043  JMPF	R11	#0046
      0x582C002A,  //  0044  LDCONST	R11	K42
      0x70020004,  //  0045  JMP		#004B
      0x1C2C0B09,  //  0046  EQ	R11	R5	K9
      0x782E0001,  //  0047  JMPF	R11	#004A
      0x582C002B,  //  0048  LDCONST	R11	K43
      0x70020000,  //  0049  JMP		#004B
      0x582C002C,  //  004A  LDCONST	R11	K44
      0xB8323A00,  //  004B  GETNGBL	R12	K29
      0x8C30192D,  //  004C  GETMET	R12	R12	K45
      0x5C380400,  //  004D  MOVE	R14	R2
      0x5C3C0600,  //  004E  MOVE	R15	R3
      0x7C300600,  //  004F  CALL	R12	3
      0x4C340000,  //  0050  LDNIL	R13
      0x2034180D,  //  0051  NE	R13	R12	R13
      0x78360002,  //  0052  JMPF	R13	#0056
      0x00365C0C,  //  0053  ADD	R13	K46	R12
      0x00341B2F,  //  0054  ADD	R13	R13	K47
      0x70020000,  //  0055  JMP		#0057
      0x58340026,  //  0056  LDCONST	R13	K38
      0x5C301A00,  //  0057  MOVE	R12	R13
      0xB8360200,  //  0058  GETNGBL	R13	K1
      0x60380018,  //  0059  GETGBL	R14	G24
      0x583C0030,  //  005A  LDCONST	R15	K48
      0x5C401600,  //  005B  MOVE	R16	R11
      0x88441331,  //  005C  GETMBR	R17	R9	K49
      0x88481332,  //  005D  GETMBR	R18	R9	K50
      0x884C1333,  //  005E  GETMBR	R19	R9	K51
      0x88501334,  //  005F  GETMBR	R20	R9	K52
      0x5C541800,  //  0060  MOVE	R21	R12
      0x5C581400,  //  0061  MOVE	R22	R10
      0x7C381000,  //  0062  CALL	R14	8
      0x583C0006,  //  0063  LDCONST	R15	K6
      0x7C340400,  //  0064  CALL	R13	2
      0x8C280135,  //  0065  GETMET	R10	R0	K53
      0x5C301200,  //  0066  MOVE	R12	R9
      0x7C280400,  //  0067  CALL	R10	2
      0x88280121,  //  0068  GETMBR	R10	R0	K33
      0x88281536,  //  0069  GETMBR	R10	R10	K54
      0x88281537,  //  006A  GETMBR	R10	R10	K55
      0x88281538,  //  006B  GETMBR	R10	R10	K56
      0x8C281539,  //  006C  GETMET	R10	R10	K57
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
be_local_closure(class_Matter_EventHandler_compact,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        4,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
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
    }),
    1,                          /* has constants */
    &be_ktab_class_Matter_EventHandler,     /* shared constants */
    be_str_weak(compact),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x84040000,  //  0000  CLOSURE	R1	P0
      0x5C080200,  //  0001  MOVE	R2	R1
      0x880C0105,  //  0002  GETMBR	R3	R0	K5
      0x7C080200,  //  0003  CALL	R2	1
      0x5C080200,  //  0004  MOVE	R2	R1
      0x880C0104,  //  0005  GETMBR	R3	R0	K4
      0x7C080200,  //  0006  CALL	R2	1
      0x5C080200,  //  0007  MOVE	R2	R1
      0x880C0103,  //  0008  GETMBR	R3	R0	K3
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
// compact class 'Matter_EventQueued' ktab size: 28, total: 43 (saved 120 bytes)
static const bvalue be_ktab_class_Matter_EventQueued[28] = {
  /* K0   */  be_nested_str_weak(raw_tlv),
  /* K1   */  be_nested_str_weak(eventreport2raw),
  /* K2   */  be_nested_str_weak(endpoint),
  /* K3   */  be_nested_str_weak(cluster),
  /* K4   */  be_nested_str_weak(event_id),
  /* K5   */  be_nested_str_weak(is_urgent),
  /* K6   */  be_nested_str_weak(priority),
  /* K7   */  be_nested_str_weak(event_no),
  /* K8   */  be_nested_str_weak(epoch_timestamp),
  /* K9   */  be_nested_str_weak(data0),
  /* K10  */  be_nested_str_weak(data1),
  /* K11  */  be_nested_str_weak(data2),
  /* K12  */  be_const_int(0),
  /* K13  */  be_nested_str_weak(matter),
  /* K14  */  be_nested_str_weak(EVENT_CRITICAL),
  /* K15  */  be_nested_str_weak(tasmota),
  /* K16  */  be_nested_str_weak(rtc),
  /* K17  */  be_nested_str_weak(utc),
  /* K18  */  be_const_int(1700000000),
  /* K19  */  be_nested_str_weak(config_time),
  /* K20  */  be_nested_str_weak(add),
  /* K21  */  be_const_int(1),
  /* K22  */  be_const_int(2),
  /* K23  */  be_nested_str_weak(eventpath2raw),
  /* K24  */  be_nested_str_weak(low32),
  /* K25  */  be_nested_str_weak(high32),
  /* K26  */  be_nested_str_weak(tag_sub),
  /* K27  */  be_nested_str_weak(tlv2raw),
};


extern const bclass be_class_Matter_EventQueued;

/********************************************************************
** Solidified function: to_raw_bytes
********************************************************************/
be_local_closure(class_Matter_EventQueued_to_raw_bytes,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventQueued,     /* shared constants */
    be_str_weak(to_raw_bytes),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060010,  //  0003  JMPF	R1	#0015
      0x60040015,  //  0004  GETGBL	R1	G21
      0x540A003F,  //  0005  LDINT	R2	64
      0x7C040200,  //  0006  CALL	R1	1
      0x90020001,  //  0007  SETMBR	R0	K0	R1
      0x8C040101,  //  0008  GETMET	R1	R0	K1
      0x880C0100,  //  0009  GETMBR	R3	R0	K0
      0x88100102,  //  000A  GETMBR	R4	R0	K2
      0x88140103,  //  000B  GETMBR	R5	R0	K3
      0x88180104,  //  000C  GETMBR	R6	R0	K4
      0x881C0105,  //  000D  GETMBR	R7	R0	K5
      0x88200106,  //  000E  GETMBR	R8	R0	K6
      0x88240107,  //  000F  GETMBR	R9	R0	K7
      0x88280108,  //  0010  GETMBR	R10	R0	K8
      0x882C0109,  //  0011  GETMBR	R11	R0	K9
      0x8830010A,  //  0012  GETMBR	R12	R0	K10
      0x8834010B,  //  0013  GETMBR	R13	R0	K11
      0x7C041800,  //  0014  CALL	R1	12
      0x88040100,  //  0015  GETMBR	R1	R0	K0
      0x80040200,  //  0016  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_EventQueued_init,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    10,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventQueued,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x90020E01,  //  0000  SETMBR	R0	K7	R1
      0x90020402,  //  0001  SETMBR	R0	K2	R2
      0x90020603,  //  0002  SETMBR	R0	K3	R3
      0x90020804,  //  0003  SETMBR	R0	K4	R4
      0x90020A05,  //  0004  SETMBR	R0	K5	R5
      0x90020C06,  //  0005  SETMBR	R0	K6	R6
      0x88280106,  //  0006  GETMBR	R10	R0	K6
      0x1428150C,  //  0007  LT	R10	R10	K12
      0x782A0000,  //  0008  JMPF	R10	#000A
      0x90020D0C,  //  0009  SETMBR	R0	K6	K12
      0x88280106,  //  000A  GETMBR	R10	R0	K6
      0xB82E1A00,  //  000B  GETNGBL	R11	K13
      0x882C170E,  //  000C  GETMBR	R11	R11	K14
      0x2428140B,  //  000D  GT	R10	R10	R11
      0x782A0002,  //  000E  JMPF	R10	#0012
      0xB82A1A00,  //  000F  GETNGBL	R10	K13
      0x8828150E,  //  0010  GETMBR	R10	R10	K14
      0x90020C0A,  //  0011  SETMBR	R0	K6	R10
      0xB82A1E00,  //  0012  GETNGBL	R10	K15
      0x8C281510,  //  0013  GETMET	R10	R10	K16
      0x58300011,  //  0014  LDCONST	R12	K17
      0x7C280400,  //  0015  CALL	R10	2
      0x9002100A,  //  0016  SETMBR	R0	K8	R10
      0x88280108,  //  0017  GETMBR	R10	R0	K8
      0x14281512,  //  0018  LT	R10	R10	K18
      0x782A0004,  //  0019  JMPF	R10	#001F
      0xB82A1E00,  //  001A  GETNGBL	R10	K15
      0x8C281510,  //  001B  GETMET	R10	R10	K16
      0x58300013,  //  001C  LDCONST	R12	K19
      0x7C280400,  //  001D  CALL	R10	2
      0x9002100A,  //  001E  SETMBR	R0	K8	R10
      0x90021207,  //  001F  SETMBR	R0	K9	R7
      0x90021408,  //  0020  SETMBR	R0	K10	R8
      0x90021609,  //  0021  SETMBR	R0	K11	R9
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compact
********************************************************************/
be_local_closure(class_Matter_EventQueued_compact,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventQueued,     /* shared constants */
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
** Solidified function: eventpath2raw
********************************************************************/
be_local_closure(class_Matter_EventQueued_eventpath2raw,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    6,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventQueued,     /* shared constants */
    be_str_weak(eventpath2raw),
    &be_const_str_solidified,
    ( &(const binstruction[91]) {  /* code */
      0x8C180314,  //  0000  GETMET	R6	R1	K20
      0x542236FF,  //  0001  LDINT	R8	14080
      0x5425FFFD,  //  0002  LDINT	R9	-2
      0x7C180600,  //  0003  CALL	R6	3
      0x541A00FE,  //  0004  LDINT	R6	255
      0x18180406,  //  0005  LE	R6	R2	R6
      0x781A0008,  //  0006  JMPF	R6	#0010
      0x8C180314,  //  0007  GETMET	R6	R1	K20
      0x54222400,  //  0008  LDINT	R8	9217
      0x5425FFFD,  //  0009  LDINT	R9	-2
      0x7C180600,  //  000A  CALL	R6	3
      0x8C180314,  //  000B  GETMET	R6	R1	K20
      0x5C200400,  //  000C  MOVE	R8	R2
      0x58240015,  //  000D  LDCONST	R9	K21
      0x7C180600,  //  000E  CALL	R6	3
      0x70020007,  //  000F  JMP		#0018
      0x8C180314,  //  0010  GETMET	R6	R1	K20
      0x54222500,  //  0011  LDINT	R8	9473
      0x5425FFFD,  //  0012  LDINT	R9	-2
      0x7C180600,  //  0013  CALL	R6	3
      0x8C180314,  //  0014  GETMET	R6	R1	K20
      0x5C200400,  //  0015  MOVE	R8	R2
      0x58240016,  //  0016  LDCONST	R9	K22
      0x7C180600,  //  0017  CALL	R6	3
      0x541A00FE,  //  0018  LDINT	R6	255
      0x18180606,  //  0019  LE	R6	R3	R6
      0x781A0008,  //  001A  JMPF	R6	#0024
      0x8C180314,  //  001B  GETMET	R6	R1	K20
      0x54222401,  //  001C  LDINT	R8	9218
      0x5425FFFD,  //  001D  LDINT	R9	-2
      0x7C180600,  //  001E  CALL	R6	3
      0x8C180314,  //  001F  GETMET	R6	R1	K20
      0x5C200600,  //  0020  MOVE	R8	R3
      0x58240015,  //  0021  LDCONST	R9	K21
      0x7C180600,  //  0022  CALL	R6	3
      0x70020013,  //  0023  JMP		#0038
      0x541AFFFE,  //  0024  LDINT	R6	65535
      0x18180606,  //  0025  LE	R6	R3	R6
      0x781A0008,  //  0026  JMPF	R6	#0030
      0x8C180314,  //  0027  GETMET	R6	R1	K20
      0x54222501,  //  0028  LDINT	R8	9474
      0x5425FFFD,  //  0029  LDINT	R9	-2
      0x7C180600,  //  002A  CALL	R6	3
      0x8C180314,  //  002B  GETMET	R6	R1	K20
      0x5C200600,  //  002C  MOVE	R8	R3
      0x58240016,  //  002D  LDCONST	R9	K22
      0x7C180600,  //  002E  CALL	R6	3
      0x70020007,  //  002F  JMP		#0038
      0x8C180314,  //  0030  GETMET	R6	R1	K20
      0x54222601,  //  0031  LDINT	R8	9730
      0x5425FFFD,  //  0032  LDINT	R9	-2
      0x7C180600,  //  0033  CALL	R6	3
      0x8C180314,  //  0034  GETMET	R6	R1	K20
      0x5C200600,  //  0035  MOVE	R8	R3
      0x54260003,  //  0036  LDINT	R9	4
      0x7C180600,  //  0037  CALL	R6	3
      0x541A00FE,  //  0038  LDINT	R6	255
      0x18180806,  //  0039  LE	R6	R4	R6
      0x781A0008,  //  003A  JMPF	R6	#0044
      0x8C180314,  //  003B  GETMET	R6	R1	K20
      0x54222402,  //  003C  LDINT	R8	9219
      0x5425FFFD,  //  003D  LDINT	R9	-2
      0x7C180600,  //  003E  CALL	R6	3
      0x8C180314,  //  003F  GETMET	R6	R1	K20
      0x5C200800,  //  0040  MOVE	R8	R4
      0x58240015,  //  0041  LDCONST	R9	K21
      0x7C180600,  //  0042  CALL	R6	3
      0x70020007,  //  0043  JMP		#004C
      0x8C180314,  //  0044  GETMET	R6	R1	K20
      0x54222502,  //  0045  LDINT	R8	9475
      0x5425FFFD,  //  0046  LDINT	R9	-2
      0x7C180600,  //  0047  CALL	R6	3
      0x8C180314,  //  0048  GETMET	R6	R1	K20
      0x5C200800,  //  0049  MOVE	R8	R4
      0x58240016,  //  004A  LDCONST	R9	K22
      0x7C180600,  //  004B  CALL	R6	3
      0x78160004,  //  004C  JMPF	R5	#0052
      0x8C180314,  //  004D  GETMET	R6	R1	K20
      0x54222903,  //  004E  LDINT	R8	10500
      0x5425FFFD,  //  004F  LDINT	R9	-2
      0x7C180600,  //  0050  CALL	R6	3
      0x70020003,  //  0051  JMP		#0056
      0x8C180314,  //  0052  GETMET	R6	R1	K20
      0x54222803,  //  0053  LDINT	R8	10244
      0x5425FFFD,  //  0054  LDINT	R9	-2
      0x7C180600,  //  0055  CALL	R6	3
      0x8C180314,  //  0056  GETMET	R6	R1	K20
      0x54220017,  //  0057  LDINT	R8	24
      0x58240015,  //  0058  LDCONST	R9	K21
      0x7C180600,  //  0059  CALL	R6	3
      0x80000000,  //  005A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: eventreport2raw
********************************************************************/
be_local_closure(class_Matter_EventQueued_eventreport2raw,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    12,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_EventQueued,     /* shared constants */
    be_str_weak(eventreport2raw),
    &be_const_str_solidified,
    ( &(const binstruction[83]) {  /* code */
      0x8C300314,  //  0000  GETMET	R12	R1	K20
      0x543A0014,  //  0001  LDINT	R14	21
      0x583C0015,  //  0002  LDCONST	R15	K21
      0x7C300600,  //  0003  CALL	R12	3
      0x8C300314,  //  0004  GETMET	R12	R1	K20
      0x543A3500,  //  0005  LDINT	R14	13569
      0x543DFFFD,  //  0006  LDINT	R15	-2
      0x7C300600,  //  0007  CALL	R12	3
      0x8C300117,  //  0008  GETMET	R12	R0	K23
      0x5C380200,  //  0009  MOVE	R14	R1
      0x5C3C0400,  //  000A  MOVE	R15	R2
      0x5C400600,  //  000B  MOVE	R16	R3
      0x5C440800,  //  000C  MOVE	R17	R4
      0x5C480A00,  //  000D  MOVE	R18	R5
      0x7C300C00,  //  000E  CALL	R12	6
      0x8C300314,  //  000F  GETMET	R12	R1	K20
      0x543A2700,  //  0010  LDINT	R14	9985
      0x543DFFFD,  //  0011  LDINT	R15	-2
      0x7C300600,  //  0012  CALL	R12	3
      0x8C300314,  //  0013  GETMET	R12	R1	K20
      0x8C380F18,  //  0014  GETMET	R14	R7	K24
      0x7C380200,  //  0015  CALL	R14	1
      0x543E0003,  //  0016  LDINT	R15	4
      0x7C300600,  //  0017  CALL	R12	3
      0x8C300314,  //  0018  GETMET	R12	R1	K20
      0x8C380F19,  //  0019  GETMET	R14	R7	K25
      0x7C380200,  //  001A  CALL	R14	1
      0x543E0003,  //  001B  LDINT	R15	4
      0x7C300600,  //  001C  CALL	R12	3
      0x8C300314,  //  001D  GETMET	R12	R1	K20
      0x543A2401,  //  001E  LDINT	R14	9218
      0x543DFFFD,  //  001F  LDINT	R15	-2
      0x7C300600,  //  0020  CALL	R12	3
      0x8C300314,  //  0021  GETMET	R12	R1	K20
      0x5C380C00,  //  0022  MOVE	R14	R6
      0x583C0015,  //  0023  LDCONST	R15	K21
      0x7C300600,  //  0024  CALL	R12	3
      0x8C300314,  //  0025  GETMET	R12	R1	K20
      0x543A2602,  //  0026  LDINT	R14	9731
      0x543DFFFD,  //  0027  LDINT	R15	-2
      0x7C300600,  //  0028  CALL	R12	3
      0x8C300314,  //  0029  GETMET	R12	R1	K20
      0x5C381000,  //  002A  MOVE	R14	R8
      0x543E0003,  //  002B  LDINT	R15	4
      0x7C300600,  //  002C  CALL	R12	3
      0x8C300314,  //  002D  GETMET	R12	R1	K20
      0x543A3506,  //  002E  LDINT	R14	13575
      0x543DFFFD,  //  002F  LDINT	R15	-2
      0x7C300600,  //  0030  CALL	R12	3
      0x4C300000,  //  0031  LDNIL	R12
      0x2030120C,  //  0032  NE	R12	R9	R12
      0x78320003,  //  0033  JMPF	R12	#0038
      0x9026350C,  //  0034  SETMBR	R9	K26	K12
      0x8C30131B,  //  0035  GETMET	R12	R9	K27
      0x5C380200,  //  0036  MOVE	R14	R1
      0x7C300400,  //  0037  CALL	R12	2
      0x4C300000,  //  0038  LDNIL	R12
      0x2030140C,  //  0039  NE	R12	R10	R12
      0x78320003,  //  003A  JMPF	R12	#003F
      0x902A3515,  //  003B  SETMBR	R10	K26	K21
      0x8C30151B,  //  003C  GETMET	R12	R10	K27
      0x5C380200,  //  003D  MOVE	R14	R1
      0x7C300400,  //  003E  CALL	R12	2
      0x4C300000,  //  003F  LDNIL	R12
      0x2030160C,  //  0040  NE	R12	R11	R12
      0x78320003,  //  0041  JMPF	R12	#0046
      0x902E3516,  //  0042  SETMBR	R11	K26	K22
      0x8C30171B,  //  0043  GETMET	R12	R11	K27
      0x5C380200,  //  0044  MOVE	R14	R1
      0x7C300400,  //  0045  CALL	R12	2
      0x8C300314,  //  0046  GETMET	R12	R1	K20
      0x543A0017,  //  0047  LDINT	R14	24
      0x583C0015,  //  0048  LDCONST	R15	K21
      0x7C300600,  //  0049  CALL	R12	3
      0x8C300314,  //  004A  GETMET	R12	R1	K20
      0x543A0017,  //  004B  LDINT	R14	24
      0x583C0015,  //  004C  LDCONST	R15	K21
      0x7C300600,  //  004D  CALL	R12	3
      0x8C300314,  //  004E  GETMET	R12	R1	K20
      0x543A0017,  //  004F  LDINT	R14	24
      0x583C0015,  //  0050  LDCONST	R15	K21
      0x7C300600,  //  0051  CALL	R12	3
      0x80000000,  //  0052  RET	0
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
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(data0, -1), be_const_var(5) },
        { be_const_key_weak(eventreport2raw, -1), be_const_closure(class_Matter_EventQueued_eventreport2raw_closure) },
        { be_const_key_weak(event_id, 11), be_const_var(2) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_EventQueued_init_closure) },
        { be_const_key_weak(data1, 14), be_const_var(6) },
        { be_const_key_weak(data2, 1), be_const_var(7) },
        { be_const_key_weak(compact, -1), be_const_closure(class_Matter_EventQueued_compact_closure) },
        { be_const_key_weak(is_urgent, -1), be_const_var(3) },
        { be_const_key_weak(eventpath2raw, -1), be_const_closure(class_Matter_EventQueued_eventpath2raw_closure) },
        { be_const_key_weak(epoch_timestamp, 10), be_const_var(9) },
        { be_const_key_weak(priority, -1), be_const_var(4) },
        { be_const_key_weak(cluster, -1), be_const_var(1) },
        { be_const_key_weak(raw_tlv, 4), be_const_var(10) },
        { be_const_key_weak(to_raw_bytes, 2), be_const_closure(class_Matter_EventQueued_to_raw_bytes_closure) },
        { be_const_key_weak(endpoint, -1), be_const_var(0) },
        { be_const_key_weak(event_no, 0), be_const_var(8) },
    })),
    be_str_weak(Matter_EventQueued)
);
/********************************************************************/
/* End of solidification */
