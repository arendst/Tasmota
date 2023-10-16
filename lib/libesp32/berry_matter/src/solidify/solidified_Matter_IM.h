/* Solidification of Matter_IM.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_IM;

/********************************************************************
** Solidified function: every_250ms
********************************************************************/
be_local_closure(Matter_IM_every_250ms,   /* name */
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
    /* K0   */  be_nested_str_weak(subs_shop),
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
** Solidified function: process_incoming_ack
********************************************************************/
be_local_closure(Matter_IM_process_incoming_ack,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(find_sendqueue_by_exchangeid),
    /* K1   */  be_nested_str_weak(exchange_id),
    /* K2   */  be_nested_str_weak(ack_received),
    }),
    be_str_weak(process_incoming_ack),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x88100301,  //  0001  GETMBR	R4	R1	K1
      0x7C080400,  //  0002  CALL	R2	2
      0x780A0003,  //  0003  JMPF	R2	#0008
      0x8C0C0502,  //  0004  GETMET	R3	R2	K2
      0x5C140200,  //  0005  MOVE	R5	R1
      0x7C0C0400,  //  0006  CALL	R3	2
      0x80040600,  //  0007  RET	1	R3
      0x500C0000,  //  0008  LDBOOL	R3	0	0
      0x80040600,  //  0009  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: subscribe_response
********************************************************************/
be_local_closure(Matter_IM_subscribe_response,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(SubscribeResponseMessage),
    /* K2   */  be_nested_str_weak(from_TLV),
    }),
    be_str_weak(subscribe_response),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x50100000,  //  0006  LDBOOL	R4	0	0
      0x80040800,  //  0007  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_write_response
********************************************************************/
be_local_closure(Matter_IM_process_write_response,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(WriteResponseMessage),
    /* K2   */  be_nested_str_weak(from_TLV),
    }),
    be_str_weak(process_write_response),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x50100000,  //  0006  LDBOOL	R4	0	0
      0x80040800,  //  0007  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_status_response
********************************************************************/
be_local_closure(Matter_IM_process_status_response,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(findsubval),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(find_sendqueue_by_exchangeid),
    /* K3   */  be_nested_str_weak(exchange_id),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(SUCCESS),
    /* K6   */  be_nested_str_weak(status_ok_received),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20_X3EOK_X20_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20exch_X3D_X25i_X20not_X20found),
    /* K10  */  be_nested_str_weak(session),
    /* K11  */  be_nested_str_weak(local_session_id),
    /* K12  */  be_nested_str_weak(MTR_X3A_X20_X3EStatus_X20_X20_X20_X20ERROR_X20_X3D_X200x_X2502X),
    /* K13  */  be_const_int(3),
    /* K14  */  be_nested_str_weak(status_error_received),
    /* K15  */  be_nested_str_weak(remove_sendqueue_by_exchangeid),
    }),
    be_str_weak(process_status_response),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x8C0C0500,  //  0000  GETMET	R3	R2	K0
      0x58140001,  //  0001  LDCONST	R5	K1
      0x541A00FE,  //  0002  LDINT	R6	255
      0x7C0C0600,  //  0003  CALL	R3	3
      0x8C100102,  //  0004  GETMET	R4	R0	K2
      0x88180303,  //  0005  GETMBR	R6	R1	K3
      0x7C100400,  //  0006  CALL	R4	2
      0xB8160800,  //  0007  GETNGBL	R5	K4
      0x88140B05,  //  0008  GETMBR	R5	R5	K5
      0x1C140605,  //  0009  EQ	R5	R3	R5
      0x78160010,  //  000A  JMPF	R5	#001C
      0x78120004,  //  000B  JMPF	R4	#0011
      0x8C140906,  //  000C  GETMET	R5	R4	K6
      0x5C1C0200,  //  000D  MOVE	R7	R1
      0x7C140400,  //  000E  CALL	R5	2
      0x80040A00,  //  000F  RET	1	R5
      0x70020009,  //  0010  JMP		#001B
      0xB8160E00,  //  0011  GETNGBL	R5	K7
      0x8C140B08,  //  0012  GETMET	R5	R5	K8
      0x601C0018,  //  0013  GETGBL	R7	G24
      0x58200009,  //  0014  LDCONST	R8	K9
      0x8824030A,  //  0015  GETMBR	R9	R1	K10
      0x8824130B,  //  0016  GETMBR	R9	R9	K11
      0x88280303,  //  0017  GETMBR	R10	R1	K3
      0x7C1C0600,  //  0018  CALL	R7	3
      0x54220003,  //  0019  LDINT	R8	4
      0x7C140600,  //  001A  CALL	R5	3
      0x7002000E,  //  001B  JMP		#002B
      0xB8160E00,  //  001C  GETNGBL	R5	K7
      0x8C140B08,  //  001D  GETMET	R5	R5	K8
      0x601C0018,  //  001E  GETGBL	R7	G24
      0x5820000C,  //  001F  LDCONST	R8	K12
      0x5C240600,  //  0020  MOVE	R9	R3
      0x7C1C0400,  //  0021  CALL	R7	2
      0x5820000D,  //  0022  LDCONST	R8	K13
      0x7C140600,  //  0023  CALL	R5	3
      0x78120005,  //  0024  JMPF	R4	#002B
      0x8C14090E,  //  0025  GETMET	R5	R4	K14
      0x5C1C0200,  //  0026  MOVE	R7	R1
      0x7C140400,  //  0027  CALL	R5	2
      0x8C14010F,  //  0028  GETMET	R5	R0	K15
      0x881C0303,  //  0029  GETMBR	R7	R1	K3
      0x7C140400,  //  002A  CALL	R5	2
      0x50140000,  //  002B  LDBOOL	R5	0	0
      0x80040A00,  //  002C  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_timed_request
********************************************************************/
be_local_closure(Matter_IM_process_timed_request,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TimedRequestMessage),
    /* K2   */  be_nested_str_weak(from_TLV),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(MTR_X3A_X20_X3ECommand_X20_X20_X20_X28_X256i_X29_X20TimedRequest_X3D_X25i),
    /* K6   */  be_nested_str_weak(session),
    /* K7   */  be_nested_str_weak(local_session_id),
    /* K8   */  be_nested_str_weak(timeout),
    /* K9   */  be_const_int(3),
    /* K10  */  be_nested_str_weak(send_status),
    /* K11  */  be_nested_str_weak(SUCCESS),
    }),
    be_str_weak(process_timed_request),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0xB8120600,  //  0006  GETNGBL	R4	K3
      0x8C100904,  //  0007  GETMET	R4	R4	K4
      0x60180018,  //  0008  GETGBL	R6	G24
      0x581C0005,  //  0009  LDCONST	R7	K5
      0x88200306,  //  000A  GETMBR	R8	R1	K6
      0x88201107,  //  000B  GETMBR	R8	R8	K7
      0x88240708,  //  000C  GETMBR	R9	R3	K8
      0x7C180600,  //  000D  CALL	R6	3
      0x581C0009,  //  000E  LDCONST	R7	K9
      0x7C100600,  //  000F  CALL	R4	3
      0x8C10010A,  //  0010  GETMET	R4	R0	K10
      0x5C180200,  //  0011  MOVE	R6	R1
      0xB81E0000,  //  0012  GETNGBL	R7	K0
      0x881C0F0B,  //  0013  GETMBR	R7	R7	K11
      0x7C100600,  //  0014  CALL	R4	3
      0x50100200,  //  0015  LDBOOL	R4	1	0
      0x80040800,  //  0016  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: attributedata2raw
********************************************************************/
be_local_closure(Matter_IM_attributedata2raw,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(add),
    /* K1   */  be_const_int(355795236),
    /* K2   */  be_const_int(1),
    /* K3   */  be_nested_str_weak(path2raw),
    /* K4   */  be_nested_str_weak(tag_sub),
    /* K5   */  be_const_int(2),
    /* K6   */  be_nested_str_weak(tlv2raw),
    }),
    be_str_weak(attributedata2raw),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x8C140300,  //  0000  GETMET	R5	R1	K0
      0x581C0001,  //  0001  LDCONST	R7	K1
      0x5421FFFB,  //  0002  LDINT	R8	-4
      0x7C140600,  //  0003  CALL	R5	3
      0x8C140300,  //  0004  GETMET	R5	R1	K0
      0x581C0002,  //  0005  LDCONST	R7	K2
      0x5421FFFD,  //  0006  LDINT	R8	-2
      0x7C140600,  //  0007  CALL	R5	3
      0x8C140103,  //  0008  GETMET	R5	R0	K3
      0x5C1C0200,  //  0009  MOVE	R7	R1
      0x5C200400,  //  000A  MOVE	R8	R2
      0x58240002,  //  000B  LDCONST	R9	K2
      0x5C280800,  //  000C  MOVE	R10	R4
      0x7C140A00,  //  000D  CALL	R5	5
      0x900E0905,  //  000E  SETMBR	R3	K4	K5
      0x8C140706,  //  000F  GETMET	R5	R3	K6
      0x5C1C0200,  //  0010  MOVE	R7	R1
      0x7C140400,  //  0011  CALL	R5	2
      0x8C140300,  //  0012  GETMET	R5	R1	K0
      0x541E1817,  //  0013  LDINT	R7	6168
      0x5421FFFD,  //  0014  LDINT	R8	-2
      0x7C140600,  //  0015  CALL	R5	3
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_invoke_request_solo
********************************************************************/
be_local_closure(Matter_IM_process_invoke_request_solo,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[42]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(profiler),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(invoke_request_solo_start),
    /* K4   */  be_nested_str_weak(msg),
    /* K5   */  be_nested_str_weak(status),
    /* K6   */  be_nested_str_weak(UNSUPPORTED_COMMAND),
    /* K7   */  be_nested_str_weak(get_command_name),
    /* K8   */  be_nested_str_weak(cluster),
    /* K9   */  be_nested_str_weak(command),
    /* K10  */  be_nested_str_weak(device),
    /* K11  */  be_nested_str_weak(invoke_request),
    /* K12  */  be_nested_str_weak(session),
    /* K13  */  be_nested_str_weak(command_fields),
    /* K14  */  be_nested_str_weak(COMMAND_X20DONE),
    /* K15  */  be_nested_str_weak(_X28),
    /* K16  */  be_nested_str_weak(_X29_X20),
    /* K17  */  be_nested_str_weak(),
    /* K18  */  be_nested_str_weak(tasmota),
    /* K19  */  be_nested_str_weak(loglevel),
    /* K20  */  be_const_int(3),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20_X3ECommand1_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s_X20_X25s),
    /* K22  */  be_nested_str_weak(local_session_id),
    /* K23  */  be_nested_str_weak(add),
    /* K24  */  be_const_int(354943030),
    /* K25  */  be_const_int(1),
    /* K26  */  be_nested_str_weak(SUCCESS),
    /* K27  */  be_nested_str_weak(invokeresponse2raw),
    /* K28  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20OK_X20exch_X3D_X25i),
    /* K29  */  be_nested_str_weak(exchange_id),
    /* K30  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s),
    /* K31  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20Status_X3D0x_X2502X_X20exch_X3D_X25i),
    /* K32  */  be_nested_str_weak(MTR_X3A_X20_Ignore_X20_X20_X20_X20_X28_X256i_X29_X20exch_X3D_X25i),
    /* K33  */  be_const_int(405077761),
    /* K34  */  be_nested_str_weak(build_response),
    /* K35  */  be_nested_str_weak(message_handler),
    /* K36  */  be_nested_str_weak(raw),
    /* K37  */  be_nested_str_weak(clear),
    /* K38  */  be_nested_str_weak(encode_frame),
    /* K39  */  be_nested_str_weak(encrypt),
    /* K40  */  be_nested_str_weak(send_response_frame),
    /* K41  */  be_nested_str_weak(RESPONSE_X20SENT),
    }),
    be_str_weak(process_invoke_request_solo),
    &be_const_str_solidified,
    ( &(const binstruction[208]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0x900A0801,  //  0005  SETMBR	R2	K4	R1
      0xB80E0000,  //  0006  GETNGBL	R3	K0
      0x880C0706,  //  0007  GETMBR	R3	R3	K6
      0x900A0A03,  //  0008  SETMBR	R2	K5	R3
      0xB80E0000,  //  0009  GETNGBL	R3	K0
      0x8C0C0707,  //  000A  GETMET	R3	R3	K7
      0x88140508,  //  000B  GETMBR	R5	R2	K8
      0x88180509,  //  000C  GETMBR	R6	R2	K9
      0x7C0C0600,  //  000D  CALL	R3	3
      0x60100008,  //  000E  GETGBL	R4	G8
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C100200,  //  0010  CALL	R4	1
      0x8814010A,  //  0011  GETMBR	R5	R0	K10
      0x8C140B0B,  //  0012  GETMET	R5	R5	K11
      0x881C030C,  //  0013  GETMBR	R7	R1	K12
      0x8820050D,  //  0014  GETMBR	R8	R2	K13
      0x5C240400,  //  0015  MOVE	R9	R2
      0x7C140800,  //  0016  CALL	R5	4
      0xB81A0000,  //  0017  GETNGBL	R6	K0
      0x88180D01,  //  0018  GETMBR	R6	R6	K1
      0x8C180D02,  //  0019  GETMET	R6	R6	K2
      0x5820000E,  //  001A  LDCONST	R8	K14
      0x7C180400,  //  001B  CALL	R6	2
      0x88180502,  //  001C  GETMBR	R6	R2	K2
      0x4C1C0000,  //  001D  LDNIL	R7
      0x20180C07,  //  001E  NE	R6	R6	R7
      0x781A0005,  //  001F  JMPF	R6	#0026
      0x60180008,  //  0020  GETGBL	R6	G8
      0x881C0502,  //  0021  GETMBR	R7	R2	K2
      0x7C180200,  //  0022  CALL	R6	1
      0x001A1E06,  //  0023  ADD	R6	K15	R6
      0x00180D10,  //  0024  ADD	R6	R6	K16
      0x70020000,  //  0025  JMP		#0027
      0x58180011,  //  0026  LDCONST	R6	K17
      0xB81E2400,  //  0027  GETNGBL	R7	K18
      0x8C1C0F13,  //  0028  GETMET	R7	R7	K19
      0x58240014,  //  0029  LDCONST	R9	K20
      0x7C1C0400,  //  002A  CALL	R7	2
      0x781E000E,  //  002B  JMPF	R7	#003B
      0xB81E2400,  //  002C  GETNGBL	R7	K18
      0x8C1C0F02,  //  002D  GETMET	R7	R7	K2
      0x60240018,  //  002E  GETGBL	R9	G24
      0x58280015,  //  002F  LDCONST	R10	K21
      0x882C030C,  //  0030  GETMBR	R11	R1	K12
      0x882C1716,  //  0031  GETMBR	R11	R11	K22
      0x5C300800,  //  0032  MOVE	R12	R4
      0x780E0001,  //  0033  JMPF	R3	#0036
      0x5C340600,  //  0034  MOVE	R13	R3
      0x70020000,  //  0035  JMP		#0037
      0x58340011,  //  0036  LDCONST	R13	K17
      0x5C380C00,  //  0037  MOVE	R14	R6
      0x7C240A00,  //  0038  CALL	R9	5
      0x58280014,  //  0039  LDCONST	R10	K20
      0x7C1C0600,  //  003A  CALL	R7	3
      0x4C1C0000,  //  003B  LDNIL	R7
      0x900A0407,  //  003C  SETMBR	R2	K2	R7
      0x601C0015,  //  003D  GETGBL	R7	G21
      0x5422002F,  //  003E  LDINT	R8	48
      0x7C1C0200,  //  003F  CALL	R7	1
      0x8C200F17,  //  0040  GETMET	R8	R7	K23
      0x58280018,  //  0041  LDCONST	R10	K24
      0x542DFFFB,  //  0042  LDINT	R11	-4
      0x7C200600,  //  0043  CALL	R8	3
      0x8C200F17,  //  0044  GETMET	R8	R7	K23
      0x58280019,  //  0045  LDCONST	R10	K25
      0x582C0019,  //  0046  LDCONST	R11	K25
      0x7C200600,  //  0047  CALL	R8	3
      0x50200200,  //  0048  LDBOOL	R8	1	0
      0x1C200A08,  //  0049  EQ	R8	R5	R8
      0x74220004,  //  004A  JMPT	R8	#0050
      0x88200505,  //  004B  GETMBR	R8	R2	K5
      0xB8260000,  //  004C  GETNGBL	R9	K0
      0x8824131A,  //  004D  GETMBR	R9	R9	K26
      0x1C201009,  //  004E  EQ	R8	R8	R9
      0x78220017,  //  004F  JMPF	R8	#0068
      0xB8220000,  //  0050  GETNGBL	R8	K0
      0x8820111A,  //  0051  GETMBR	R8	R8	K26
      0x900A0A08,  //  0052  SETMBR	R2	K5	R8
      0x8C20011B,  //  0053  GETMET	R8	R0	K27
      0x5C280E00,  //  0054  MOVE	R10	R7
      0x5C2C0400,  //  0055  MOVE	R11	R2
      0x4C300000,  //  0056  LDNIL	R12
      0x7C200800,  //  0057  CALL	R8	4
      0xB8222400,  //  0058  GETNGBL	R8	K18
      0x8C201113,  //  0059  GETMET	R8	R8	K19
      0x58280014,  //  005A  LDCONST	R10	K20
      0x7C200400,  //  005B  CALL	R8	2
      0x78220009,  //  005C  JMPF	R8	#0067
      0xB8222400,  //  005D  GETNGBL	R8	K18
      0x8C201102,  //  005E  GETMET	R8	R8	K2
      0x60280018,  //  005F  GETGBL	R10	G24
      0x582C001C,  //  0060  LDCONST	R11	K28
      0x8830030C,  //  0061  GETMBR	R12	R1	K12
      0x88301916,  //  0062  GETMBR	R12	R12	K22
      0x8834031D,  //  0063  GETMBR	R13	R1	K29
      0x7C280600,  //  0064  CALL	R10	3
      0x582C0014,  //  0065  LDCONST	R11	K20
      0x7C200600,  //  0066  CALL	R8	3
      0x70020046,  //  0067  JMP		#00AF
      0x4C200000,  //  0068  LDNIL	R8
      0x20200A08,  //  0069  NE	R8	R5	R8
      0x78220018,  //  006A  JMPF	R8	#0084
      0x8C20011B,  //  006B  GETMET	R8	R0	K27
      0x5C280E00,  //  006C  MOVE	R10	R7
      0x5C2C0400,  //  006D  MOVE	R11	R2
      0x5C300A00,  //  006E  MOVE	R12	R5
      0x7C200800,  //  006F  CALL	R8	4
      0x5C200600,  //  0070  MOVE	R8	R3
      0x74220000,  //  0071  JMPT	R8	#0073
      0x580C0011,  //  0072  LDCONST	R3	K17
      0xB8222400,  //  0073  GETNGBL	R8	K18
      0x8C201113,  //  0074  GETMET	R8	R8	K19
      0x58280014,  //  0075  LDCONST	R10	K20
      0x7C200400,  //  0076  CALL	R8	2
      0x7822000A,  //  0077  JMPF	R8	#0083
      0xB8222400,  //  0078  GETNGBL	R8	K18
      0x8C201102,  //  0079  GETMET	R8	R8	K2
      0x60280018,  //  007A  GETGBL	R10	G24
      0x582C001E,  //  007B  LDCONST	R11	K30
      0x8830030C,  //  007C  GETMBR	R12	R1	K12
      0x88301916,  //  007D  GETMBR	R12	R12	K22
      0x5C340400,  //  007E  MOVE	R13	R2
      0x5C380600,  //  007F  MOVE	R14	R3
      0x7C280800,  //  0080  CALL	R10	4
      0x582C0014,  //  0081  LDCONST	R11	K20
      0x7C200600,  //  0082  CALL	R8	3
      0x7002002A,  //  0083  JMP		#00AF
      0x88200505,  //  0084  GETMBR	R8	R2	K5
      0x4C240000,  //  0085  LDNIL	R9
      0x20201009,  //  0086  NE	R8	R8	R9
      0x78220015,  //  0087  JMPF	R8	#009E
      0x8C20011B,  //  0088  GETMET	R8	R0	K27
      0x5C280E00,  //  0089  MOVE	R10	R7
      0x5C2C0400,  //  008A  MOVE	R11	R2
      0x4C300000,  //  008B  LDNIL	R12
      0x7C200800,  //  008C  CALL	R8	4
      0xB8222400,  //  008D  GETNGBL	R8	K18
      0x8C201113,  //  008E  GETMET	R8	R8	K19
      0x58280014,  //  008F  LDCONST	R10	K20
      0x7C200400,  //  0090  CALL	R8	2
      0x7822000A,  //  0091  JMPF	R8	#009D
      0xB8222400,  //  0092  GETNGBL	R8	K18
      0x8C201102,  //  0093  GETMET	R8	R8	K2
      0x60280018,  //  0094  GETGBL	R10	G24
      0x582C001F,  //  0095  LDCONST	R11	K31
      0x8830030C,  //  0096  GETMBR	R12	R1	K12
      0x88301916,  //  0097  GETMBR	R12	R12	K22
      0x88340505,  //  0098  GETMBR	R13	R2	K5
      0x8838031D,  //  0099  GETMBR	R14	R1	K29
      0x7C280800,  //  009A  CALL	R10	4
      0x582C0014,  //  009B  LDCONST	R11	K20
      0x7C200600,  //  009C  CALL	R8	3
      0x70020010,  //  009D  JMP		#00AF
      0xB8222400,  //  009E  GETNGBL	R8	K18
      0x8C201113,  //  009F  GETMET	R8	R8	K19
      0x58280014,  //  00A0  LDCONST	R10	K20
      0x7C200400,  //  00A1  CALL	R8	2
      0x78220009,  //  00A2  JMPF	R8	#00AD
      0xB8222400,  //  00A3  GETNGBL	R8	K18
      0x8C201102,  //  00A4  GETMET	R8	R8	K2
      0x60280018,  //  00A5  GETGBL	R10	G24
      0x582C0020,  //  00A6  LDCONST	R11	K32
      0x8830030C,  //  00A7  GETMBR	R12	R1	K12
      0x88301916,  //  00A8  GETMBR	R12	R12	K22
      0x8834031D,  //  00A9  GETMBR	R13	R1	K29
      0x7C280600,  //  00AA  CALL	R10	3
      0x582C0014,  //  00AB  LDCONST	R11	K20
      0x7C200600,  //  00AC  CALL	R8	3
      0x50200000,  //  00AD  LDBOOL	R8	0	0
      0x80041000,  //  00AE  RET	1	R8
      0x8C200F17,  //  00AF  GETMET	R8	R7	K23
      0x58280021,  //  00B0  LDCONST	R10	K33
      0x542DFFFB,  //  00B1  LDINT	R11	-4
      0x7C200600,  //  00B2  CALL	R8	3
      0x8C200F17,  //  00B3  GETMET	R8	R7	K23
      0x542A0017,  //  00B4  LDINT	R10	24
      0x582C0019,  //  00B5  LDCONST	R11	K25
      0x7C200600,  //  00B6  CALL	R8	3
      0x8C200322,  //  00B7  GETMET	R8	R1	K34
      0x542A0008,  //  00B8  LDINT	R10	9
      0x502C0200,  //  00B9  LDBOOL	R11	1	0
      0x7C200600,  //  00BA  CALL	R8	3
      0x8824010A,  //  00BB  GETMBR	R9	R0	K10
      0x88241323,  //  00BC  GETMBR	R9	R9	K35
      0x88280324,  //  00BD  GETMBR	R10	R1	K36
      0x8C2C1525,  //  00BE  GETMET	R11	R10	K37
      0x7C2C0200,  //  00BF  CALL	R11	1
      0x8C2C1126,  //  00C0  GETMET	R11	R8	K38
      0x5C340E00,  //  00C1  MOVE	R13	R7
      0x5C381400,  //  00C2  MOVE	R14	R10
      0x7C2C0600,  //  00C3  CALL	R11	3
      0x8C2C1127,  //  00C4  GETMET	R11	R8	K39
      0x7C2C0200,  //  00C5  CALL	R11	1
      0x8C2C1328,  //  00C6  GETMET	R11	R9	K40
      0x5C341000,  //  00C7  MOVE	R13	R8
      0x7C2C0400,  //  00C8  CALL	R11	2
      0xB82E0000,  //  00C9  GETNGBL	R11	K0
      0x882C1701,  //  00CA  GETMBR	R11	R11	K1
      0x8C2C1702,  //  00CB  GETMET	R11	R11	K2
      0x58340029,  //  00CC  LDCONST	R13	K41
      0x7C2C0400,  //  00CD  CALL	R11	2
      0x502C0200,  //  00CE  LDBOOL	R11	1	0
      0x80041600,  //  00CF  RET	1	R11
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invokeresponse2raw
********************************************************************/
be_local_closure(Matter_IM_invokeresponse2raw,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(add),
    /* K1   */  be_const_int(1),
    /* K2   */  be_nested_str_weak(endpoint),
    /* K3   */  be_const_int(2),
    /* K4   */  be_nested_str_weak(cluster),
    /* K5   */  be_nested_str_weak(command),
    /* K6   */  be_nested_str_weak(status),
    /* K7   */  be_nested_str_weak(matter),
    /* K8   */  be_nested_str_weak(SUCCESS),
    /* K9   */  be_nested_str_weak(tag_sub),
    /* K10  */  be_nested_str_weak(tlv2raw),
    }),
    be_str_weak(invokeresponse2raw),
    &be_const_str_solidified,
    ( &(const binstruction[148]) {  /* code */
      0x8C100300,  //  0000  GETMET	R4	R1	K0
      0x541A0014,  //  0001  LDINT	R6	21
      0x581C0001,  //  0002  LDCONST	R7	K1
      0x7C100600,  //  0003  CALL	R4	3
      0x4C100000,  //  0004  LDNIL	R4
      0x1C100604,  //  0005  EQ	R4	R3	R4
      0x78120004,  //  0006  JMPF	R4	#000C
      0x8C100300,  //  0007  GETMET	R4	R1	K0
      0x541A3500,  //  0008  LDINT	R6	13569
      0x541DFFFD,  //  0009  LDINT	R7	-2
      0x7C100600,  //  000A  CALL	R4	3
      0x70020003,  //  000B  JMP		#0010
      0x8C100300,  //  000C  GETMET	R4	R1	K0
      0x541A34FF,  //  000D  LDINT	R6	13568
      0x541DFFFD,  //  000E  LDINT	R7	-2
      0x7C100600,  //  000F  CALL	R4	3
      0x8C100300,  //  0010  GETMET	R4	R1	K0
      0x541A36FF,  //  0011  LDINT	R6	14080
      0x541DFFFD,  //  0012  LDINT	R7	-2
      0x7C100600,  //  0013  CALL	R4	3
      0x88100502,  //  0014  GETMBR	R4	R2	K2
      0x541600FE,  //  0015  LDINT	R5	255
      0x18100805,  //  0016  LE	R4	R4	R5
      0x78120008,  //  0017  JMPF	R4	#0021
      0x8C100300,  //  0018  GETMET	R4	R1	K0
      0x541A23FF,  //  0019  LDINT	R6	9216
      0x541DFFFD,  //  001A  LDINT	R7	-2
      0x7C100600,  //  001B  CALL	R4	3
      0x8C100300,  //  001C  GETMET	R4	R1	K0
      0x88180502,  //  001D  GETMBR	R6	R2	K2
      0x581C0001,  //  001E  LDCONST	R7	K1
      0x7C100600,  //  001F  CALL	R4	3
      0x70020007,  //  0020  JMP		#0029
      0x8C100300,  //  0021  GETMET	R4	R1	K0
      0x541A24FF,  //  0022  LDINT	R6	9472
      0x541DFFFD,  //  0023  LDINT	R7	-2
      0x7C100600,  //  0024  CALL	R4	3
      0x8C100300,  //  0025  GETMET	R4	R1	K0
      0x88180502,  //  0026  GETMBR	R6	R2	K2
      0x581C0003,  //  0027  LDCONST	R7	K3
      0x7C100600,  //  0028  CALL	R4	3
      0x88100504,  //  0029  GETMBR	R4	R2	K4
      0x541600FE,  //  002A  LDINT	R5	255
      0x18100805,  //  002B  LE	R4	R4	R5
      0x78120008,  //  002C  JMPF	R4	#0036
      0x8C100300,  //  002D  GETMET	R4	R1	K0
      0x541A2400,  //  002E  LDINT	R6	9217
      0x541DFFFD,  //  002F  LDINT	R7	-2
      0x7C100600,  //  0030  CALL	R4	3
      0x8C100300,  //  0031  GETMET	R4	R1	K0
      0x88180504,  //  0032  GETMBR	R6	R2	K4
      0x581C0001,  //  0033  LDCONST	R7	K1
      0x7C100600,  //  0034  CALL	R4	3
      0x70020014,  //  0035  JMP		#004B
      0x88100504,  //  0036  GETMBR	R4	R2	K4
      0x5416FFFE,  //  0037  LDINT	R5	65535
      0x18100805,  //  0038  LE	R4	R4	R5
      0x78120008,  //  0039  JMPF	R4	#0043
      0x8C100300,  //  003A  GETMET	R4	R1	K0
      0x541A2500,  //  003B  LDINT	R6	9473
      0x541DFFFD,  //  003C  LDINT	R7	-2
      0x7C100600,  //  003D  CALL	R4	3
      0x8C100300,  //  003E  GETMET	R4	R1	K0
      0x88180504,  //  003F  GETMBR	R6	R2	K4
      0x581C0003,  //  0040  LDCONST	R7	K3
      0x7C100600,  //  0041  CALL	R4	3
      0x70020007,  //  0042  JMP		#004B
      0x8C100300,  //  0043  GETMET	R4	R1	K0
      0x541A2600,  //  0044  LDINT	R6	9729
      0x541DFFFD,  //  0045  LDINT	R7	-2
      0x7C100600,  //  0046  CALL	R4	3
      0x8C100300,  //  0047  GETMET	R4	R1	K0
      0x88180504,  //  0048  GETMBR	R6	R2	K4
      0x541E0003,  //  0049  LDINT	R7	4
      0x7C100600,  //  004A  CALL	R4	3
      0x88100505,  //  004B  GETMBR	R4	R2	K5
      0x541600FE,  //  004C  LDINT	R5	255
      0x18100805,  //  004D  LE	R4	R4	R5
      0x78120008,  //  004E  JMPF	R4	#0058
      0x8C100300,  //  004F  GETMET	R4	R1	K0
      0x541A2401,  //  0050  LDINT	R6	9218
      0x541DFFFD,  //  0051  LDINT	R7	-2
      0x7C100600,  //  0052  CALL	R4	3
      0x8C100300,  //  0053  GETMET	R4	R1	K0
      0x88180505,  //  0054  GETMBR	R6	R2	K5
      0x581C0001,  //  0055  LDCONST	R7	K1
      0x7C100600,  //  0056  CALL	R4	3
      0x70020014,  //  0057  JMP		#006D
      0x88100505,  //  0058  GETMBR	R4	R2	K5
      0x5416FFFE,  //  0059  LDINT	R5	65535
      0x18100805,  //  005A  LE	R4	R4	R5
      0x78120008,  //  005B  JMPF	R4	#0065
      0x8C100300,  //  005C  GETMET	R4	R1	K0
      0x541A2501,  //  005D  LDINT	R6	9474
      0x541DFFFD,  //  005E  LDINT	R7	-2
      0x7C100600,  //  005F  CALL	R4	3
      0x8C100300,  //  0060  GETMET	R4	R1	K0
      0x88180505,  //  0061  GETMBR	R6	R2	K5
      0x581C0003,  //  0062  LDCONST	R7	K3
      0x7C100600,  //  0063  CALL	R4	3
      0x70020007,  //  0064  JMP		#006D
      0x8C100300,  //  0065  GETMET	R4	R1	K0
      0x541A2601,  //  0066  LDINT	R6	9730
      0x541DFFFD,  //  0067  LDINT	R7	-2
      0x7C100600,  //  0068  CALL	R4	3
      0x8C100300,  //  0069  GETMET	R4	R1	K0
      0x88180505,  //  006A  GETMBR	R6	R2	K5
      0x541E0003,  //  006B  LDINT	R7	4
      0x7C100600,  //  006C  CALL	R4	3
      0x8C100300,  //  006D  GETMET	R4	R1	K0
      0x541A0017,  //  006E  LDINT	R6	24
      0x581C0001,  //  006F  LDCONST	R7	K1
      0x7C100600,  //  0070  CALL	R4	3
      0x4C100000,  //  0071  LDNIL	R4
      0x1C100604,  //  0072  EQ	R4	R3	R4
      0x78120016,  //  0073  JMPF	R4	#008B
      0x88100506,  //  0074  GETMBR	R4	R2	K6
      0x4C140000,  //  0075  LDNIL	R5
      0x1C140805,  //  0076  EQ	R5	R4	R5
      0x78160001,  //  0077  JMPF	R5	#007A
      0xB8160E00,  //  0078  GETNGBL	R5	K7
      0x88100B08,  //  0079  GETMBR	R4	R5	K8
      0x8C140300,  //  007A  GETMET	R5	R1	K0
      0x541E3500,  //  007B  LDINT	R7	13569
      0x5421FFFD,  //  007C  LDINT	R8	-2
      0x7C140600,  //  007D  CALL	R5	3
      0x8C140300,  //  007E  GETMET	R5	R1	K0
      0x541E23FF,  //  007F  LDINT	R7	9216
      0x5421FFFD,  //  0080  LDINT	R8	-2
      0x7C140600,  //  0081  CALL	R5	3
      0x8C140300,  //  0082  GETMET	R5	R1	K0
      0x881C0506,  //  0083  GETMBR	R7	R2	K6
      0x58200001,  //  0084  LDCONST	R8	K1
      0x7C140600,  //  0085  CALL	R5	3
      0x8C140300,  //  0086  GETMET	R5	R1	K0
      0x541E0017,  //  0087  LDINT	R7	24
      0x58200001,  //  0088  LDCONST	R8	K1
      0x7C140600,  //  0089  CALL	R5	3
      0x70020003,  //  008A  JMP		#008F
      0x900E1301,  //  008B  SETMBR	R3	K9	K1
      0x8C10070A,  //  008C  GETMET	R4	R3	K10
      0x5C180200,  //  008D  MOVE	R6	R1
      0x7C100400,  //  008E  CALL	R4	2
      0x8C100300,  //  008F  GETMET	R4	R1	K0
      0x541A1817,  //  0090  LDINT	R6	6168
      0x541DFFFD,  //  0091  LDINT	R7	-2
      0x7C100600,  //  0092  CALL	R4	3
      0x80000000,  //  0093  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_ack_now
********************************************************************/
be_local_closure(Matter_IM_send_ack_now,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(session),
    /* K1   */  be_nested_str_weak(_message_handler),
    /* K2   */  be_nested_str_weak(send_encrypted_ack),
    }),
    be_str_weak(send_ack_now),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x80000400,  //  0003  RET	0
      0x88080300,  //  0004  GETMBR	R2	R1	K0
      0x88080501,  //  0005  GETMBR	R2	R2	K1
      0x8C080502,  //  0006  GETMET	R2	R2	K2
      0x5C100200,  //  0007  MOVE	R4	R1
      0x50140000,  //  0008  LDBOOL	R5	0	0
      0x7C080600,  //  0009  CALL	R2	3
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_invoke_request
********************************************************************/
be_local_closure(Matter_IM_process_invoke_request,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[43]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(profiler),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(invoke_request_start),
    /* K4   */  be_nested_str_weak(Path),
    /* K5   */  be_nested_str_weak(msg),
    /* K6   */  be_nested_str_weak(InvokeRequestMessage),
    /* K7   */  be_nested_str_weak(from_TLV),
    /* K8   */  be_nested_str_weak(invoke_requests),
    /* K9   */  be_nested_str_weak(InvokeResponseMessage),
    /* K10  */  be_nested_str_weak(suppress_response),
    /* K11  */  be_nested_str_weak(invoke_responses),
    /* K12  */  be_nested_str_weak(endpoint),
    /* K13  */  be_nested_str_weak(command_path),
    /* K14  */  be_nested_str_weak(cluster),
    /* K15  */  be_nested_str_weak(command),
    /* K16  */  be_nested_str_weak(status),
    /* K17  */  be_nested_str_weak(UNSUPPORTED_COMMAND),
    /* K18  */  be_nested_str_weak(get_command_name),
    /* K19  */  be_nested_str_weak(device),
    /* K20  */  be_nested_str_weak(invoke_request),
    /* K21  */  be_nested_str_weak(session),
    /* K22  */  be_nested_str_weak(command_fields),
    /* K23  */  be_nested_str_weak(COMMAND_X20DONE),
    /* K24  */  be_nested_str_weak(_X28),
    /* K25  */  be_nested_str_weak(_X29_X20),
    /* K26  */  be_nested_str_weak(),
    /* K27  */  be_nested_str_weak(tasmota),
    /* K28  */  be_nested_str_weak(MTR_X3A_X20_X3ECommand_X20_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s_X20_X25s),
    /* K29  */  be_nested_str_weak(local_session_id),
    /* K30  */  be_const_int(3),
    /* K31  */  be_nested_str_weak(SUCCESS),
    /* K32  */  be_nested_str_weak(invokeresponse2raw),
    /* K33  */  be_nested_str_weak(push),
    /* K34  */  be_nested_str_weak(loglevel),
    /* K35  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20OK_X20exch_X3D_X25i),
    /* K36  */  be_nested_str_weak(exchange_id),
    /* K37  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s),
    /* K38  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20Status_X3D0x_X2502X_X20exch_X3D_X25i),
    /* K39  */  be_nested_str_weak(MTR_X3A_X20_Ignore_X20_X20_X20_X20_X28_X256i_X29_X20exch_X3D_X25i),
    /* K40  */  be_nested_str_weak(stop_iteration),
    /* K41  */  be_const_int(0),
    /* K42  */  be_nested_str_weak(send_invoke_response),
    }),
    be_str_weak(process_invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[233]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0xB80E0000,  //  0005  GETNGBL	R3	K0
      0x8C0C0704,  //  0006  GETMET	R3	R3	K4
      0x7C0C0200,  //  0007  CALL	R3	1
      0x900E0A01,  //  0008  SETMBR	R3	K5	R1
      0xB8120000,  //  0009  GETNGBL	R4	K0
      0x8C100906,  //  000A  GETMET	R4	R4	K6
      0x7C100200,  //  000B  CALL	R4	1
      0x8C100907,  //  000C  GETMET	R4	R4	K7
      0x5C180400,  //  000D  MOVE	R6	R2
      0x7C100400,  //  000E  CALL	R4	2
      0x88140908,  //  000F  GETMBR	R5	R4	K8
      0x4C180000,  //  0010  LDNIL	R6
      0x20140A06,  //  0011  NE	R5	R5	R6
      0x781600D4,  //  0012  JMPF	R5	#00E8
      0xB8160000,  //  0013  GETNGBL	R5	K0
      0x8C140B09,  //  0014  GETMET	R5	R5	K9
      0x7C140200,  //  0015  CALL	R5	1
      0x50180000,  //  0016  LDBOOL	R6	0	0
      0x90161406,  //  0017  SETMBR	R5	K10	R6
      0x60180012,  //  0018  GETGBL	R6	G18
      0x7C180000,  //  0019  CALL	R6	0
      0x90161606,  //  001A  SETMBR	R5	K11	R6
      0x60180010,  //  001B  GETGBL	R6	G16
      0x881C0908,  //  001C  GETMBR	R7	R4	K8
      0x7C180200,  //  001D  CALL	R6	1
      0xA80200B7,  //  001E  EXBLK	0	#00D7
      0x5C1C0C00,  //  001F  MOVE	R7	R6
      0x7C1C0000,  //  0020  CALL	R7	0
      0x88200F0D,  //  0021  GETMBR	R8	R7	K13
      0x8820110C,  //  0022  GETMBR	R8	R8	K12
      0x900E1808,  //  0023  SETMBR	R3	K12	R8
      0x88200F0D,  //  0024  GETMBR	R8	R7	K13
      0x8820110E,  //  0025  GETMBR	R8	R8	K14
      0x900E1C08,  //  0026  SETMBR	R3	K14	R8
      0x88200F0D,  //  0027  GETMBR	R8	R7	K13
      0x8820110F,  //  0028  GETMBR	R8	R8	K15
      0x900E1E08,  //  0029  SETMBR	R3	K15	R8
      0xB8220000,  //  002A  GETNGBL	R8	K0
      0x88201111,  //  002B  GETMBR	R8	R8	K17
      0x900E2008,  //  002C  SETMBR	R3	K16	R8
      0xB8220000,  //  002D  GETNGBL	R8	K0
      0x8C201112,  //  002E  GETMET	R8	R8	K18
      0x8828070E,  //  002F  GETMBR	R10	R3	K14
      0x882C070F,  //  0030  GETMBR	R11	R3	K15
      0x7C200600,  //  0031  CALL	R8	3
      0x60240008,  //  0032  GETGBL	R9	G8
      0x5C280600,  //  0033  MOVE	R10	R3
      0x7C240200,  //  0034  CALL	R9	1
      0x88280113,  //  0035  GETMBR	R10	R0	K19
      0x8C281514,  //  0036  GETMET	R10	R10	K20
      0x88300315,  //  0037  GETMBR	R12	R1	K21
      0x88340F16,  //  0038  GETMBR	R13	R7	K22
      0x5C380600,  //  0039  MOVE	R14	R3
      0x7C280800,  //  003A  CALL	R10	4
      0xB82E0000,  //  003B  GETNGBL	R11	K0
      0x882C1701,  //  003C  GETMBR	R11	R11	K1
      0x8C2C1702,  //  003D  GETMET	R11	R11	K2
      0x58340017,  //  003E  LDCONST	R13	K23
      0x7C2C0400,  //  003F  CALL	R11	2
      0x882C0702,  //  0040  GETMBR	R11	R3	K2
      0x4C300000,  //  0041  LDNIL	R12
      0x202C160C,  //  0042  NE	R11	R11	R12
      0x782E0005,  //  0043  JMPF	R11	#004A
      0x602C0008,  //  0044  GETGBL	R11	G8
      0x88300702,  //  0045  GETMBR	R12	R3	K2
      0x7C2C0200,  //  0046  CALL	R11	1
      0x002E300B,  //  0047  ADD	R11	K24	R11
      0x002C1719,  //  0048  ADD	R11	R11	K25
      0x70020000,  //  0049  JMP		#004B
      0x582C001A,  //  004A  LDCONST	R11	K26
      0xB8323600,  //  004B  GETNGBL	R12	K27
      0x8C301902,  //  004C  GETMET	R12	R12	K2
      0x60380018,  //  004D  GETGBL	R14	G24
      0x583C001C,  //  004E  LDCONST	R15	K28
      0x88400315,  //  004F  GETMBR	R16	R1	K21
      0x8840211D,  //  0050  GETMBR	R16	R16	K29
      0x5C441200,  //  0051  MOVE	R17	R9
      0x78220001,  //  0052  JMPF	R8	#0055
      0x5C481000,  //  0053  MOVE	R18	R8
      0x70020000,  //  0054  JMP		#0056
      0x5848001A,  //  0055  LDCONST	R18	K26
      0x5C4C1600,  //  0056  MOVE	R19	R11
      0x7C380A00,  //  0057  CALL	R14	5
      0x583C001E,  //  0058  LDCONST	R15	K30
      0x7C300600,  //  0059  CALL	R12	3
      0x4C300000,  //  005A  LDNIL	R12
      0x900E040C,  //  005B  SETMBR	R3	K2	R12
      0x60300015,  //  005C  GETGBL	R12	G21
      0x5436001F,  //  005D  LDINT	R13	32
      0x7C300200,  //  005E  CALL	R12	1
      0x50340200,  //  005F  LDBOOL	R13	1	0
      0x1C34140D,  //  0060  EQ	R13	R10	R13
      0x74360004,  //  0061  JMPT	R13	#0067
      0x88340710,  //  0062  GETMBR	R13	R3	K16
      0xB83A0000,  //  0063  GETNGBL	R14	K0
      0x88381D1F,  //  0064  GETMBR	R14	R14	K31
      0x1C341A0E,  //  0065  EQ	R13	R13	R14
      0x7836001B,  //  0066  JMPF	R13	#0083
      0xB8360000,  //  0067  GETNGBL	R13	K0
      0x88341B1F,  //  0068  GETMBR	R13	R13	K31
      0x900E200D,  //  0069  SETMBR	R3	K16	R13
      0x8C340120,  //  006A  GETMET	R13	R0	K32
      0x5C3C1800,  //  006B  MOVE	R15	R12
      0x5C400600,  //  006C  MOVE	R16	R3
      0x4C440000,  //  006D  LDNIL	R17
      0x7C340800,  //  006E  CALL	R13	4
      0x88340B0B,  //  006F  GETMBR	R13	R5	K11
      0x8C341B21,  //  0070  GETMET	R13	R13	K33
      0x5C3C1800,  //  0071  MOVE	R15	R12
      0x7C340400,  //  0072  CALL	R13	2
      0xB8363600,  //  0073  GETNGBL	R13	K27
      0x8C341B22,  //  0074  GETMET	R13	R13	K34
      0x583C001E,  //  0075  LDCONST	R15	K30
      0x7C340400,  //  0076  CALL	R13	2
      0x78360009,  //  0077  JMPF	R13	#0082
      0xB8363600,  //  0078  GETNGBL	R13	K27
      0x8C341B02,  //  0079  GETMET	R13	R13	K2
      0x603C0018,  //  007A  GETGBL	R15	G24
      0x58400023,  //  007B  LDCONST	R16	K35
      0x88440315,  //  007C  GETMBR	R17	R1	K21
      0x8844231D,  //  007D  GETMBR	R17	R17	K29
      0x88480324,  //  007E  GETMBR	R18	R1	K36
      0x7C3C0600,  //  007F  CALL	R15	3
      0x5840001E,  //  0080  LDCONST	R16	K30
      0x7C340600,  //  0081  CALL	R13	3
      0x70020052,  //  0082  JMP		#00D6
      0x4C340000,  //  0083  LDNIL	R13
      0x2034140D,  //  0084  NE	R13	R10	R13
      0x78360022,  //  0085  JMPF	R13	#00A9
      0x8C340120,  //  0086  GETMET	R13	R0	K32
      0x5C3C1800,  //  0087  MOVE	R15	R12
      0x5C400600,  //  0088  MOVE	R16	R3
      0x5C441400,  //  0089  MOVE	R17	R10
      0x7C340800,  //  008A  CALL	R13	4
      0x88340B0B,  //  008B  GETMBR	R13	R5	K11
      0x8C341B21,  //  008C  GETMET	R13	R13	K33
      0x5C3C1800,  //  008D  MOVE	R15	R12
      0x7C340400,  //  008E  CALL	R13	2
      0xB8360000,  //  008F  GETNGBL	R13	K0
      0x8C341B12,  //  0090  GETMET	R13	R13	K18
      0x883C070E,  //  0091  GETMBR	R15	R3	K14
      0x8840070F,  //  0092  GETMBR	R16	R3	K15
      0x7C340600,  //  0093  CALL	R13	3
      0x5C201A00,  //  0094  MOVE	R8	R13
      0x5C341000,  //  0095  MOVE	R13	R8
      0x74360000,  //  0096  JMPT	R13	#0098
      0x5820001A,  //  0097  LDCONST	R8	K26
      0xB8363600,  //  0098  GETNGBL	R13	K27
      0x8C341B22,  //  0099  GETMET	R13	R13	K34
      0x583C001E,  //  009A  LDCONST	R15	K30
      0x7C340400,  //  009B  CALL	R13	2
      0x7836000A,  //  009C  JMPF	R13	#00A8
      0xB8363600,  //  009D  GETNGBL	R13	K27
      0x8C341B02,  //  009E  GETMET	R13	R13	K2
      0x603C0018,  //  009F  GETGBL	R15	G24
      0x58400025,  //  00A0  LDCONST	R16	K37
      0x88440315,  //  00A1  GETMBR	R17	R1	K21
      0x8844231D,  //  00A2  GETMBR	R17	R17	K29
      0x5C480600,  //  00A3  MOVE	R18	R3
      0x5C4C1000,  //  00A4  MOVE	R19	R8
      0x7C3C0800,  //  00A5  CALL	R15	4
      0x5840001E,  //  00A6  LDCONST	R16	K30
      0x7C340600,  //  00A7  CALL	R13	3
      0x7002002C,  //  00A8  JMP		#00D6
      0x88340710,  //  00A9  GETMBR	R13	R3	K16
      0x4C380000,  //  00AA  LDNIL	R14
      0x20341A0E,  //  00AB  NE	R13	R13	R14
      0x78360019,  //  00AC  JMPF	R13	#00C7
      0x8C340120,  //  00AD  GETMET	R13	R0	K32
      0x5C3C1800,  //  00AE  MOVE	R15	R12
      0x5C400600,  //  00AF  MOVE	R16	R3
      0x4C440000,  //  00B0  LDNIL	R17
      0x7C340800,  //  00B1  CALL	R13	4
      0x88340B0B,  //  00B2  GETMBR	R13	R5	K11
      0x8C341B21,  //  00B3  GETMET	R13	R13	K33
      0x5C3C1800,  //  00B4  MOVE	R15	R12
      0x7C340400,  //  00B5  CALL	R13	2
      0xB8363600,  //  00B6  GETNGBL	R13	K27
      0x8C341B22,  //  00B7  GETMET	R13	R13	K34
      0x583C001E,  //  00B8  LDCONST	R15	K30
      0x7C340400,  //  00B9  CALL	R13	2
      0x7836000A,  //  00BA  JMPF	R13	#00C6
      0xB8363600,  //  00BB  GETNGBL	R13	K27
      0x8C341B02,  //  00BC  GETMET	R13	R13	K2
      0x603C0018,  //  00BD  GETGBL	R15	G24
      0x58400026,  //  00BE  LDCONST	R16	K38
      0x88440315,  //  00BF  GETMBR	R17	R1	K21
      0x8844231D,  //  00C0  GETMBR	R17	R17	K29
      0x88480710,  //  00C1  GETMBR	R18	R3	K16
      0x884C0324,  //  00C2  GETMBR	R19	R1	K36
      0x7C3C0800,  //  00C3  CALL	R15	4
      0x5840001E,  //  00C4  LDCONST	R16	K30
      0x7C340600,  //  00C5  CALL	R13	3
      0x7002000E,  //  00C6  JMP		#00D6
      0xB8363600,  //  00C7  GETNGBL	R13	K27
      0x8C341B22,  //  00C8  GETMET	R13	R13	K34
      0x583C001E,  //  00C9  LDCONST	R15	K30
      0x7C340400,  //  00CA  CALL	R13	2
      0x78360009,  //  00CB  JMPF	R13	#00D6
      0xB8363600,  //  00CC  GETNGBL	R13	K27
      0x8C341B02,  //  00CD  GETMET	R13	R13	K2
      0x603C0018,  //  00CE  GETGBL	R15	G24
      0x58400027,  //  00CF  LDCONST	R16	K39
      0x88440315,  //  00D0  GETMBR	R17	R1	K21
      0x8844231D,  //  00D1  GETMBR	R17	R17	K29
      0x88480324,  //  00D2  GETMBR	R18	R1	K36
      0x7C3C0600,  //  00D3  CALL	R15	3
      0x5840001E,  //  00D4  LDCONST	R16	K30
      0x7C340600,  //  00D5  CALL	R13	3
      0x7001FF47,  //  00D6  JMP		#001F
      0x58180028,  //  00D7  LDCONST	R6	K40
      0xAC180200,  //  00D8  CATCH	R6	1	0
      0xB0080000,  //  00D9  RAISE	2	R0	R0
      0x6018000C,  //  00DA  GETGBL	R6	G12
      0x881C0B0B,  //  00DB  GETMBR	R7	R5	K11
      0x7C180200,  //  00DC  CALL	R6	1
      0x24180D29,  //  00DD  GT	R6	R6	K41
      0x781A0004,  //  00DE  JMPF	R6	#00E4
      0x8C18012A,  //  00DF  GETMET	R6	R0	K42
      0x5C200200,  //  00E0  MOVE	R8	R1
      0x5C240A00,  //  00E1  MOVE	R9	R5
      0x7C180600,  //  00E2  CALL	R6	3
      0x70020001,  //  00E3  JMP		#00E6
      0x50180000,  //  00E4  LDBOOL	R6	0	0
      0x80040C00,  //  00E5  RET	1	R6
      0x50180200,  //  00E6  LDBOOL	R6	1	0
      0x80040C00,  //  00E7  RET	1	R6
      0x80000000,  //  00E8  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_read_request
********************************************************************/
be_local_closure(Matter_IM_process_read_request,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(profiler),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(read_request_start),
    /* K4   */  be_nested_str_weak(ReadRequestMessage),
    /* K5   */  be_nested_str_weak(from_TLV),
    /* K6   */  be_nested_str_weak(attributes_requests),
    /* K7   */  be_nested_str_weak(_inner_process_read_request),
    /* K8   */  be_nested_str_weak(session),
    /* K9   */  be_nested_str_weak(send_report_data),
    }),
    be_str_weak(process_read_request),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x880C0701,  //  0001  GETMBR	R3	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C0C0400,  //  0004  CALL	R3	2
      0xB80E0000,  //  0005  GETNGBL	R3	K0
      0x8C0C0704,  //  0006  GETMET	R3	R3	K4
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C0C0705,  //  0008  GETMET	R3	R3	K5
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C0C0400,  //  000A  CALL	R3	2
      0x88100706,  //  000B  GETMBR	R4	R3	K6
      0x4C140000,  //  000C  LDNIL	R5
      0x20100805,  //  000D  NE	R4	R4	R5
      0x78120008,  //  000E  JMPF	R4	#0018
      0x8C100107,  //  000F  GETMET	R4	R0	K7
      0x88180308,  //  0010  GETMBR	R6	R1	K8
      0x5C1C0600,  //  0011  MOVE	R7	R3
      0x5C200200,  //  0012  MOVE	R8	R1
      0x7C100800,  //  0013  CALL	R4	4
      0x8C140109,  //  0014  GETMET	R5	R0	K9
      0x5C1C0200,  //  0015  MOVE	R7	R1
      0x5C200800,  //  0016  MOVE	R8	R4
      0x7C140600,  //  0017  CALL	R5	3
      0x50100200,  //  0018  LDBOOL	R4	1	0
      0x80040800,  //  0019  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_invoke_response
********************************************************************/
be_local_closure(Matter_IM_send_invoke_response,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(send_queue),
    /* K1   */  be_nested_str_weak(push),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(IM_InvokeResponse),
    }),
    be_str_weak(send_invoke_response),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0xB8160400,  //  0002  GETNGBL	R5	K2
      0x8C140B03,  //  0003  GETMET	R5	R5	K3
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C140600,  //  0006  CALL	R5	3
      0x7C0C0400,  //  0007  CALL	R3	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_write_response
********************************************************************/
be_local_closure(Matter_IM_send_write_response,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(send_queue),
    /* K1   */  be_nested_str_weak(push),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(IM_WriteResponse),
    }),
    be_str_weak(send_write_response),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0xB8160400,  //  0002  GETNGBL	R5	K2
      0x8C140B03,  //  0003  GETMET	R5	R5	K3
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C140600,  //  0006  CALL	R5	3
      0x7C0C0400,  //  0007  CALL	R3	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_write_request
********************************************************************/
be_local_closure(Matter_IM_process_write_request,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        17,                          /* nstack */
        5,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 1),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[26]) {     /* constants */
        /* K0   */  be_nested_str_weak(matter),
        /* K1   */  be_nested_str_weak(get_attribute_name),
        /* K2   */  be_nested_str_weak(cluster),
        /* K3   */  be_nested_str_weak(attribute),
        /* K4   */  be_nested_str_weak(_X20_X28),
        /* K5   */  be_nested_str_weak(_X29),
        /* K6   */  be_nested_str_weak(),
        /* K7   */  be_nested_str_weak(status),
        /* K8   */  be_nested_str_weak(UNSUPPORTED_WRITE),
        /* K9   */  be_nested_str_weak(write_attribute),
        /* K10  */  be_nested_str_weak(session),
        /* K11  */  be_nested_str_weak(SUCCESS),
        /* K12  */  be_nested_str_weak(AttributeStatusIB),
        /* K13  */  be_nested_str_weak(path),
        /* K14  */  be_nested_str_weak(AttributePathIB),
        /* K15  */  be_nested_str_weak(StatusIB),
        /* K16  */  be_nested_str_weak(endpoint),
        /* K17  */  be_nested_str_weak(write_responses),
        /* K18  */  be_nested_str_weak(push),
        /* K19  */  be_nested_str_weak(tasmota),
        /* K20  */  be_nested_str_weak(log),
        /* K21  */  be_nested_str_weak(MTR_X3A_X20Write_Attr_X20_X25s_X25s_X20_X2D_X20STATUS_X3A_X200x_X2502X_X20_X25s),
        /* K22  */  be_const_int(0),
        /* K23  */  be_const_int(2),
        /* K24  */  be_const_int(3),
        /* K25  */  be_nested_str_weak(MTR_X3A_X20Write_Attr_X20_X25s_X25s_X20_X2D_X20IGNORED),
        }),
        be_str_weak(write_single_attribute),
        &be_const_str_solidified,
        ( &(const binstruction[101]) {  /* code */
          0xB8160000,  //  0000  GETNGBL	R5	K0
          0x8C140B01,  //  0001  GETMET	R5	R5	K1
          0x881C0502,  //  0002  GETMBR	R7	R2	K2
          0x88200503,  //  0003  GETMBR	R8	R2	K3
          0x7C140600,  //  0004  CALL	R5	3
          0x78160002,  //  0005  JMPF	R5	#0009
          0x001A0805,  //  0006  ADD	R6	K4	R5
          0x00180D05,  //  0007  ADD	R6	R6	K5
          0x70020000,  //  0008  JMP		#000A
          0x58180006,  //  0009  LDCONST	R6	K6
          0x5C140C00,  //  000A  MOVE	R5	R6
          0xB81A0000,  //  000B  GETNGBL	R6	K0
          0x88180D08,  //  000C  GETMBR	R6	R6	K8
          0x900A0E06,  //  000D  SETMBR	R2	K7	R6
          0x4C180000,  //  000E  LDNIL	R6
          0x20180206,  //  000F  NE	R6	R1	R6
          0x781A0006,  //  0010  JMPF	R6	#0018
          0x8C180309,  //  0011  GETMET	R6	R1	K9
          0x68200000,  //  0012  GETUPV	R8	U0
          0x8820110A,  //  0013  GETMBR	R8	R8	K10
          0x5C240400,  //  0014  MOVE	R9	R2
          0x5C280600,  //  0015  MOVE	R10	R3
          0x7C180800,  //  0016  CALL	R6	4
          0x70020000,  //  0017  JMP		#0019
          0x4C180000,  //  0018  LDNIL	R6
          0x781A0002,  //  0019  JMPF	R6	#001D
          0xB81E0000,  //  001A  GETNGBL	R7	K0
          0x881C0F0B,  //  001B  GETMBR	R7	R7	K11
          0x900A0E07,  //  001C  SETMBR	R2	K7	R7
          0x881C0507,  //  001D  GETMBR	R7	R2	K7
          0x4C200000,  //  001E  LDNIL	R8
          0x201C0E08,  //  001F  NE	R7	R7	R8
          0x781E0037,  //  0020  JMPF	R7	#0059
          0x78120035,  //  0021  JMPF	R4	#0058
          0xB81E0000,  //  0022  GETNGBL	R7	K0
          0x8C1C0F0C,  //  0023  GETMET	R7	R7	K12
          0x7C1C0200,  //  0024  CALL	R7	1
          0xB8220000,  //  0025  GETNGBL	R8	K0
          0x8C20110E,  //  0026  GETMET	R8	R8	K14
          0x7C200200,  //  0027  CALL	R8	1
          0x901E1A08,  //  0028  SETMBR	R7	K13	R8
          0xB8220000,  //  0029  GETNGBL	R8	K0
          0x8C20110F,  //  002A  GETMET	R8	R8	K15
          0x7C200200,  //  002B  CALL	R8	1
          0x901E0E08,  //  002C  SETMBR	R7	K7	R8
          0x88200F0D,  //  002D  GETMBR	R8	R7	K13
          0x88240510,  //  002E  GETMBR	R9	R2	K16
          0x90222009,  //  002F  SETMBR	R8	K16	R9
          0x88200F0D,  //  0030  GETMBR	R8	R7	K13
          0x88240502,  //  0031  GETMBR	R9	R2	K2
          0x90220409,  //  0032  SETMBR	R8	K2	R9
          0x88200F0D,  //  0033  GETMBR	R8	R7	K13
          0x88240503,  //  0034  GETMBR	R9	R2	K3
          0x90220609,  //  0035  SETMBR	R8	K3	R9
          0x88200F07,  //  0036  GETMBR	R8	R7	K7
          0x88240507,  //  0037  GETMBR	R9	R2	K7
          0x90220E09,  //  0038  SETMBR	R8	K7	R9
          0x88200111,  //  0039  GETMBR	R8	R0	K17
          0x8C201112,  //  003A  GETMET	R8	R8	K18
          0x5C280E00,  //  003B  MOVE	R10	R7
          0x7C200400,  //  003C  CALL	R8	2
          0xB8222600,  //  003D  GETNGBL	R8	K19
          0x8C201114,  //  003E  GETMET	R8	R8	K20
          0x60280018,  //  003F  GETGBL	R10	G24
          0x582C0015,  //  0040  LDCONST	R11	K21
          0x60300008,  //  0041  GETGBL	R12	G8
          0x5C340400,  //  0042  MOVE	R13	R2
          0x7C300200,  //  0043  CALL	R12	1
          0x5C340A00,  //  0044  MOVE	R13	R5
          0x88380507,  //  0045  GETMBR	R14	R2	K7
          0x883C0507,  //  0046  GETMBR	R15	R2	K7
          0xB8420000,  //  0047  GETNGBL	R16	K0
          0x8840210B,  //  0048  GETMBR	R16	R16	K11
          0x1C3C1E10,  //  0049  EQ	R15	R15	R16
          0x783E0001,  //  004A  JMPF	R15	#004D
          0x583C000B,  //  004B  LDCONST	R15	K11
          0x70020000,  //  004C  JMP		#004E
          0x583C0006,  //  004D  LDCONST	R15	K6
          0x7C280A00,  //  004E  CALL	R10	5
          0x882C0510,  //  004F  GETMBR	R11	R2	K16
          0x202C1716,  //  0050  NE	R11	R11	K22
          0x782E0001,  //  0051  JMPF	R11	#0054
          0x582C0017,  //  0052  LDCONST	R11	K23
          0x70020000,  //  0053  JMP		#0055
          0x582C0018,  //  0054  LDCONST	R11	K24
          0x7C200600,  //  0055  CALL	R8	3
          0x50200200,  //  0056  LDBOOL	R8	1	0
          0x80041000,  //  0057  RET	1	R8
          0x7002000A,  //  0058  JMP		#0064
          0xB81E2600,  //  0059  GETNGBL	R7	K19
          0x8C1C0F14,  //  005A  GETMET	R7	R7	K20
          0x60240018,  //  005B  GETGBL	R9	G24
          0x58280019,  //  005C  LDCONST	R10	K25
          0x602C0008,  //  005D  GETGBL	R11	G8
          0x5C300400,  //  005E  MOVE	R12	R2
          0x7C2C0200,  //  005F  CALL	R11	1
          0x5C300A00,  //  0060  MOVE	R12	R5
          0x7C240600,  //  0061  CALL	R9	3
          0x58280018,  //  0062  LDCONST	R10	K24
          0x7C1C0600,  //  0063  CALL	R7	3
          0x80000000,  //  0064  RET	0
        })
      ),
      be_nested_proto(
        9,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 3]) {  /* upvals */
          be_local_const_upval(1, 6),
          be_local_const_upval(1, 8),
          be_local_const_upval(1, 12),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 8]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x68100001,  //  0001  GETUPV	R4	U1
          0x5C140000,  //  0002  MOVE	R5	R0
          0x5C180200,  //  0003  MOVE	R6	R1
          0x681C0002,  //  0004  GETUPV	R7	U2
          0x5C200400,  //  0005  MOVE	R8	R2
          0x7C0C0A00,  //  0006  CALL	R3	5
          0x80040600,  //  0007  RET	1	R3
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(WriteRequestMessage),
    /* K2   */  be_nested_str_weak(from_TLV),
    /* K3   */  be_nested_str_weak(suppress_response),
    /* K4   */  be_nested_str_weak(device),
    /* K5   */  be_nested_str_weak(get_active_endpoints),
    /* K6   */  be_nested_str_weak(Path),
    /* K7   */  be_nested_str_weak(msg),
    /* K8   */  be_nested_str_weak(write_requests),
    /* K9   */  be_nested_str_weak(WriteResponseMessage),
    /* K10  */  be_nested_str_weak(write_responses),
    /* K11  */  be_nested_str_weak(path),
    /* K12  */  be_nested_str_weak(data),
    /* K13  */  be_nested_str_weak(endpoint),
    /* K14  */  be_nested_str_weak(cluster),
    /* K15  */  be_nested_str_weak(attribute),
    /* K16  */  be_nested_str_weak(status),
    /* K17  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K18  */  be_nested_str_weak(INVALID_ACTION),
    /* K19  */  be_nested_str_weak(get_attribute_name),
    /* K20  */  be_nested_str_weak(tasmota),
    /* K21  */  be_nested_str_weak(log),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20Write_Attr_X20),
    /* K23  */  be_nested_str_weak(_X20_X28),
    /* K24  */  be_nested_str_weak(_X29),
    /* K25  */  be_nested_str_weak(),
    /* K26  */  be_const_int(3),
    /* K27  */  be_nested_str_weak(process_attribute_expansion),
    /* K28  */  be_nested_str_weak(stop_iteration),
    /* K29  */  be_nested_str_weak(send_write_response),
    }),
    be_str_weak(process_write_request),
    &be_const_str_solidified,
    ( &(const binstruction[104]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x88100703,  //  0006  GETMBR	R4	R3	K3
      0x88140104,  //  0007  GETMBR	R5	R0	K4
      0x8C140B05,  //  0008  GETMET	R5	R5	K5
      0x7C140200,  //  0009  CALL	R5	1
      0x84180000,  //  000A  CLOSURE	R6	P0
      0xB81E0000,  //  000B  GETNGBL	R7	K0
      0x8C1C0F06,  //  000C  GETMET	R7	R7	K6
      0x7C1C0200,  //  000D  CALL	R7	1
      0x901E0E01,  //  000E  SETMBR	R7	K7	R1
      0x88200708,  //  000F  GETMBR	R8	R3	K8
      0x4C240000,  //  0010  LDNIL	R9
      0x20201009,  //  0011  NE	R8	R8	R9
      0x78220051,  //  0012  JMPF	R8	#0065
      0xB8220000,  //  0013  GETNGBL	R8	K0
      0x8C201109,  //  0014  GETMET	R8	R8	K9
      0x7C200200,  //  0015  CALL	R8	1
      0x60240012,  //  0016  GETGBL	R9	G18
      0x7C240000,  //  0017  CALL	R9	0
      0x90221409,  //  0018  SETMBR	R8	K10	R9
      0x60240010,  //  0019  GETGBL	R9	G16
      0x88280708,  //  001A  GETMBR	R10	R3	K8
      0x7C240200,  //  001B  CALL	R9	1
      0xA802003D,  //  001C  EXBLK	0	#005B
      0x5C281200,  //  001D  MOVE	R10	R9
      0x7C280000,  //  001E  CALL	R10	0
      0x882C150B,  //  001F  GETMBR	R11	R10	K11
      0x8830150C,  //  0020  GETMBR	R12	R10	K12
      0x8834170D,  //  0021  GETMBR	R13	R11	K13
      0x901E1A0D,  //  0022  SETMBR	R7	K13	R13
      0x8834170E,  //  0023  GETMBR	R13	R11	K14
      0x901E1C0D,  //  0024  SETMBR	R7	K14	R13
      0x8834170F,  //  0025  GETMBR	R13	R11	K15
      0x901E1E0D,  //  0026  SETMBR	R7	K15	R13
      0xB8360000,  //  0027  GETNGBL	R13	K0
      0x88341B11,  //  0028  GETMBR	R13	R13	K17
      0x901E200D,  //  0029  SETMBR	R7	K16	R13
      0x88340F0E,  //  002A  GETMBR	R13	R7	K14
      0x4C380000,  //  002B  LDNIL	R14
      0x1C341A0E,  //  002C  EQ	R13	R13	R14
      0x74360003,  //  002D  JMPT	R13	#0032
      0x88340F0F,  //  002E  GETMBR	R13	R7	K15
      0x4C380000,  //  002F  LDNIL	R14
      0x1C341A0E,  //  0030  EQ	R13	R13	R14
      0x7836000A,  //  0031  JMPF	R13	#003D
      0xB8360000,  //  0032  GETNGBL	R13	K0
      0x88341B12,  //  0033  GETMBR	R13	R13	K18
      0x901E200D,  //  0034  SETMBR	R7	K16	R13
      0x5C340C00,  //  0035  MOVE	R13	R6
      0x5C381000,  //  0036  MOVE	R14	R8
      0x4C3C0000,  //  0037  LDNIL	R15
      0x5C400E00,  //  0038  MOVE	R16	R7
      0x4C440000,  //  0039  LDNIL	R17
      0x50480200,  //  003A  LDBOOL	R18	1	0
      0x7C340A00,  //  003B  CALL	R13	5
      0x7001FFDF,  //  003C  JMP		#001D
      0x88340F0D,  //  003D  GETMBR	R13	R7	K13
      0x4C380000,  //  003E  LDNIL	R14
      0x1C341A0E,  //  003F  EQ	R13	R13	R14
      0x78360012,  //  0040  JMPF	R13	#0054
      0xB8360000,  //  0041  GETNGBL	R13	K0
      0x8C341B13,  //  0042  GETMET	R13	R13	K19
      0x883C0F0E,  //  0043  GETMBR	R15	R7	K14
      0x88400F0F,  //  0044  GETMBR	R16	R7	K15
      0x7C340600,  //  0045  CALL	R13	3
      0xB83A2800,  //  0046  GETNGBL	R14	K20
      0x8C381D15,  //  0047  GETMET	R14	R14	K21
      0x60400008,  //  0048  GETGBL	R16	G8
      0x5C440E00,  //  0049  MOVE	R17	R7
      0x7C400200,  //  004A  CALL	R16	1
      0x00422C10,  //  004B  ADD	R16	K22	R16
      0x78360002,  //  004C  JMPF	R13	#0050
      0x00462E0D,  //  004D  ADD	R17	K23	R13
      0x00442318,  //  004E  ADD	R17	R17	K24
      0x70020000,  //  004F  JMP		#0051
      0x58440019,  //  0050  LDCONST	R17	K25
      0x00402011,  //  0051  ADD	R16	R16	R17
      0x5844001A,  //  0052  LDCONST	R17	K26
      0x7C380600,  //  0053  CALL	R14	3
      0x88340104,  //  0054  GETMBR	R13	R0	K4
      0x8C341B1B,  //  0055  GETMET	R13	R13	K27
      0x5C3C0E00,  //  0056  MOVE	R15	R7
      0x84400001,  //  0057  CLOSURE	R16	P1
      0x7C340600,  //  0058  CALL	R13	3
      0xA0240000,  //  0059  CLOSE	R9
      0x7001FFC1,  //  005A  JMP		#001D
      0x5824001C,  //  005B  LDCONST	R9	K28
      0xAC240200,  //  005C  CATCH	R9	1	0
      0xB0080000,  //  005D  RAISE	2	R0	R0
      0x5C240800,  //  005E  MOVE	R9	R4
      0x74260003,  //  005F  JMPT	R9	#0064
      0x8C24011D,  //  0060  GETMET	R9	R0	K29
      0x5C2C0200,  //  0061  MOVE	R11	R1
      0x5C301000,  //  0062  MOVE	R12	R8
      0x7C240600,  //  0063  CALL	R9	3
      0xA0200000,  //  0064  CLOSE	R8
      0x50200200,  //  0065  LDBOOL	R8	1	0
      0xA0000000,  //  0066  CLOSE	R0
      0x80041000,  //  0067  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: path2raw
********************************************************************/
be_local_closure(Matter_IM_path2raw,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(add),
    /* K1   */  be_const_int(1),
    /* K2   */  be_nested_str_weak(endpoint),
    /* K3   */  be_const_int(2),
    /* K4   */  be_nested_str_weak(cluster),
    /* K5   */  be_nested_str_weak(attribute),
    }),
    be_str_weak(path2raw),
    &be_const_str_solidified,
    ( &(const binstruction[107]) {  /* code */
      0x8C140300,  //  0000  GETMET	R5	R1	K0
      0x541E0036,  //  0001  LDINT	R7	55
      0x58200001,  //  0002  LDCONST	R8	K1
      0x7C140600,  //  0003  CALL	R5	3
      0x8C140300,  //  0004  GETMET	R5	R1	K0
      0x5C1C0600,  //  0005  MOVE	R7	R3
      0x58200001,  //  0006  LDCONST	R8	K1
      0x7C140600,  //  0007  CALL	R5	3
      0x88140502,  //  0008  GETMBR	R5	R2	K2
      0x541A00FE,  //  0009  LDINT	R6	255
      0x18140A06,  //  000A  LE	R5	R5	R6
      0x78160008,  //  000B  JMPF	R5	#0015
      0x8C140300,  //  000C  GETMET	R5	R1	K0
      0x541E2401,  //  000D  LDINT	R7	9218
      0x5421FFFD,  //  000E  LDINT	R8	-2
      0x7C140600,  //  000F  CALL	R5	3
      0x8C140300,  //  0010  GETMET	R5	R1	K0
      0x881C0502,  //  0011  GETMBR	R7	R2	K2
      0x58200001,  //  0012  LDCONST	R8	K1
      0x7C140600,  //  0013  CALL	R5	3
      0x70020007,  //  0014  JMP		#001D
      0x8C140300,  //  0015  GETMET	R5	R1	K0
      0x541E2501,  //  0016  LDINT	R7	9474
      0x5421FFFD,  //  0017  LDINT	R8	-2
      0x7C140600,  //  0018  CALL	R5	3
      0x8C140300,  //  0019  GETMET	R5	R1	K0
      0x881C0502,  //  001A  GETMBR	R7	R2	K2
      0x58200003,  //  001B  LDCONST	R8	K3
      0x7C140600,  //  001C  CALL	R5	3
      0x88140504,  //  001D  GETMBR	R5	R2	K4
      0x541A00FE,  //  001E  LDINT	R6	255
      0x18140A06,  //  001F  LE	R5	R5	R6
      0x78160008,  //  0020  JMPF	R5	#002A
      0x8C140300,  //  0021  GETMET	R5	R1	K0
      0x541E2402,  //  0022  LDINT	R7	9219
      0x5421FFFD,  //  0023  LDINT	R8	-2
      0x7C140600,  //  0024  CALL	R5	3
      0x8C140300,  //  0025  GETMET	R5	R1	K0
      0x881C0504,  //  0026  GETMBR	R7	R2	K4
      0x58200001,  //  0027  LDCONST	R8	K1
      0x7C140600,  //  0028  CALL	R5	3
      0x70020014,  //  0029  JMP		#003F
      0x88140504,  //  002A  GETMBR	R5	R2	K4
      0x541AFFFE,  //  002B  LDINT	R6	65535
      0x18140A06,  //  002C  LE	R5	R5	R6
      0x78160008,  //  002D  JMPF	R5	#0037
      0x8C140300,  //  002E  GETMET	R5	R1	K0
      0x541E2502,  //  002F  LDINT	R7	9475
      0x5421FFFD,  //  0030  LDINT	R8	-2
      0x7C140600,  //  0031  CALL	R5	3
      0x8C140300,  //  0032  GETMET	R5	R1	K0
      0x881C0504,  //  0033  GETMBR	R7	R2	K4
      0x58200003,  //  0034  LDCONST	R8	K3
      0x7C140600,  //  0035  CALL	R5	3
      0x70020007,  //  0036  JMP		#003F
      0x8C140300,  //  0037  GETMET	R5	R1	K0
      0x541E2602,  //  0038  LDINT	R7	9731
      0x5421FFFD,  //  0039  LDINT	R8	-2
      0x7C140600,  //  003A  CALL	R5	3
      0x8C140300,  //  003B  GETMET	R5	R1	K0
      0x881C0504,  //  003C  GETMBR	R7	R2	K4
      0x54220003,  //  003D  LDINT	R8	4
      0x7C140600,  //  003E  CALL	R5	3
      0x88140505,  //  003F  GETMBR	R5	R2	K5
      0x541A00FE,  //  0040  LDINT	R6	255
      0x18140A06,  //  0041  LE	R5	R5	R6
      0x78160008,  //  0042  JMPF	R5	#004C
      0x8C140300,  //  0043  GETMET	R5	R1	K0
      0x541E2403,  //  0044  LDINT	R7	9220
      0x5421FFFD,  //  0045  LDINT	R8	-2
      0x7C140600,  //  0046  CALL	R5	3
      0x8C140300,  //  0047  GETMET	R5	R1	K0
      0x881C0505,  //  0048  GETMBR	R7	R2	K5
      0x58200001,  //  0049  LDCONST	R8	K1
      0x7C140600,  //  004A  CALL	R5	3
      0x70020014,  //  004B  JMP		#0061
      0x88140505,  //  004C  GETMBR	R5	R2	K5
      0x541AFFFE,  //  004D  LDINT	R6	65535
      0x18140A06,  //  004E  LE	R5	R5	R6
      0x78160008,  //  004F  JMPF	R5	#0059
      0x8C140300,  //  0050  GETMET	R5	R1	K0
      0x541E2503,  //  0051  LDINT	R7	9476
      0x5421FFFD,  //  0052  LDINT	R8	-2
      0x7C140600,  //  0053  CALL	R5	3
      0x8C140300,  //  0054  GETMET	R5	R1	K0
      0x881C0505,  //  0055  GETMBR	R7	R2	K5
      0x58200003,  //  0056  LDCONST	R8	K3
      0x7C140600,  //  0057  CALL	R5	3
      0x70020007,  //  0058  JMP		#0061
      0x8C140300,  //  0059  GETMET	R5	R1	K0
      0x541E2603,  //  005A  LDINT	R7	9732
      0x5421FFFD,  //  005B  LDINT	R8	-2
      0x7C140600,  //  005C  CALL	R5	3
      0x8C140300,  //  005D  GETMET	R5	R1	K0
      0x881C0505,  //  005E  GETMBR	R7	R2	K5
      0x54220003,  //  005F  LDINT	R8	4
      0x7C140600,  //  0060  CALL	R5	3
      0x78120003,  //  0061  JMPF	R4	#0066
      0x8C140300,  //  0062  GETMET	R5	R1	K0
      0x541E3404,  //  0063  LDINT	R7	13317
      0x5421FFFD,  //  0064  LDINT	R8	-2
      0x7C140600,  //  0065  CALL	R5	3
      0x8C140300,  //  0066  GETMET	R5	R1	K0
      0x541E0017,  //  0067  LDINT	R7	24
      0x58200001,  //  0068  LDCONST	R8	K1
      0x7C140600,  //  0069  CALL	R5	3
      0x80000000,  //  006A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_subscribe_update
********************************************************************/
be_local_closure(Matter_IM_send_subscribe_update,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[25]) {     /* constants */
    /* K0   */  be_nested_str_weak(session),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(ReadRequestMessage),
    /* K3   */  be_nested_str_weak(fabric_filtered),
    /* K4   */  be_nested_str_weak(attributes_requests),
    /* K5   */  be_nested_str_weak(updates),
    /* K6   */  be_nested_str_weak(AttributePathIB),
    /* K7   */  be_nested_str_weak(endpoint),
    /* K8   */  be_nested_str_weak(cluster),
    /* K9   */  be_nested_str_weak(attribute),
    /* K10  */  be_nested_str_weak(push),
    /* K11  */  be_nested_str_weak(stop_iteration),
    /* K12  */  be_nested_str_weak(tasmota),
    /* K13  */  be_nested_str_weak(log),
    /* K14  */  be_nested_str_weak(MTR_X3A_X20_X3CSub_Data_X20_X20_X28_X256i_X29_X20sub_X3D_X25i),
    /* K15  */  be_nested_str_weak(local_session_id),
    /* K16  */  be_nested_str_weak(subscription_id),
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(is_keep_alive),
    /* K19  */  be_nested_str_weak(_inner_process_read_request),
    /* K20  */  be_nested_str_weak(suppress_response),
    /* K21  */  be_nested_str_weak(IM_ReportDataSubscribed),
    /* K22  */  be_nested_str_weak(_message_handler),
    /* K23  */  be_nested_str_weak(send_queue),
    /* K24  */  be_nested_str_weak(send_enqueued),
    }),
    be_str_weak(send_subscribe_update),
    &be_const_str_solidified,
    ( &(const binstruction[67]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x7C0C0200,  //  0003  CALL	R3	1
      0x50100000,  //  0004  LDBOOL	R4	0	0
      0x900E0604,  //  0005  SETMBR	R3	K3	R4
      0x60100012,  //  0006  GETGBL	R4	G18
      0x7C100000,  //  0007  CALL	R4	0
      0x900E0804,  //  0008  SETMBR	R3	K4	R4
      0x60100010,  //  0009  GETGBL	R4	G16
      0x88140305,  //  000A  GETMBR	R5	R1	K5
      0x7C100200,  //  000B  CALL	R4	1
      0xA802000F,  //  000C  EXBLK	0	#001D
      0x5C140800,  //  000D  MOVE	R5	R4
      0x7C140000,  //  000E  CALL	R5	0
      0xB81A0200,  //  000F  GETNGBL	R6	K1
      0x8C180D06,  //  0010  GETMET	R6	R6	K6
      0x7C180200,  //  0011  CALL	R6	1
      0x881C0B07,  //  0012  GETMBR	R7	R5	K7
      0x901A0E07,  //  0013  SETMBR	R6	K7	R7
      0x881C0B08,  //  0014  GETMBR	R7	R5	K8
      0x901A1007,  //  0015  SETMBR	R6	K8	R7
      0x881C0B09,  //  0016  GETMBR	R7	R5	K9
      0x901A1207,  //  0017  SETMBR	R6	K9	R7
      0x881C0704,  //  0018  GETMBR	R7	R3	K4
      0x8C1C0F0A,  //  0019  GETMET	R7	R7	K10
      0x5C240C00,  //  001A  MOVE	R9	R6
      0x7C1C0400,  //  001B  CALL	R7	2
      0x7001FFEF,  //  001C  JMP		#000D
      0x5810000B,  //  001D  LDCONST	R4	K11
      0xAC100200,  //  001E  CATCH	R4	1	0
      0xB0080000,  //  001F  RAISE	2	R0	R0
      0xB8121800,  //  0020  GETNGBL	R4	K12
      0x8C10090D,  //  0021  GETMET	R4	R4	K13
      0x60180018,  //  0022  GETGBL	R6	G24
      0x581C000E,  //  0023  LDCONST	R7	K14
      0x8820050F,  //  0024  GETMBR	R8	R2	K15
      0x88240310,  //  0025  GETMBR	R9	R1	K16
      0x7C180600,  //  0026  CALL	R6	3
      0x581C0011,  //  0027  LDCONST	R7	K17
      0x7C100600,  //  0028  CALL	R4	3
      0x50100000,  //  0029  LDBOOL	R4	0	0
      0x90062404,  //  002A  SETMBR	R1	K18	R4
      0x8C100113,  //  002B  GETMET	R4	R0	K19
      0x5C180400,  //  002C  MOVE	R6	R2
      0x5C1C0600,  //  002D  MOVE	R7	R3
      0x4C200000,  //  002E  LDNIL	R8
      0x7C100800,  //  002F  CALL	R4	4
      0x50140000,  //  0030  LDBOOL	R5	0	0
      0x90122805,  //  0031  SETMBR	R4	K20	R5
      0x88140310,  //  0032  GETMBR	R5	R1	K16
      0x90122005,  //  0033  SETMBR	R4	K16	R5
      0xB8160200,  //  0034  GETNGBL	R5	K1
      0x8C140B15,  //  0035  GETMET	R5	R5	K21
      0x881C0516,  //  0036  GETMBR	R7	R2	K22
      0x5C200400,  //  0037  MOVE	R8	R2
      0x5C240800,  //  0038  MOVE	R9	R4
      0x5C280200,  //  0039  MOVE	R10	R1
      0x7C140A00,  //  003A  CALL	R5	5
      0x88180117,  //  003B  GETMBR	R6	R0	K23
      0x8C180D0A,  //  003C  GETMET	R6	R6	K10
      0x5C200A00,  //  003D  MOVE	R8	R5
      0x7C180400,  //  003E  CALL	R6	2
      0x8C180118,  //  003F  GETMET	R6	R0	K24
      0x88200516,  //  0040  GETMBR	R8	R2	K22
      0x7C180400,  //  0041  CALL	R6	2
      0x80000000,  //  0042  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_sendqueue_by_exchangeid
********************************************************************/
be_local_closure(Matter_IM_remove_sendqueue_by_exchangeid,   /* name */
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
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(send_queue),
    /* K2   */  be_nested_str_weak(get_exchangeid),
    /* K3   */  be_nested_str_weak(remove),
    /* K4   */  be_const_int(1),
    }),
    be_str_weak(remove_sendqueue_by_exchangeid),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x80000400,  //  0003  RET	0
      0x58080000,  //  0004  LDCONST	R2	K0
      0x600C000C,  //  0005  GETGBL	R3	G12
      0x88100101,  //  0006  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0007  CALL	R3	1
      0x140C0403,  //  0008  LT	R3	R2	R3
      0x780E000C,  //  0009  JMPF	R3	#0017
      0x880C0101,  //  000A  GETMBR	R3	R0	K1
      0x940C0602,  //  000B  GETIDX	R3	R3	R2
      0x8C0C0702,  //  000C  GETMET	R3	R3	K2
      0x7C0C0200,  //  000D  CALL	R3	1
      0x1C0C0601,  //  000E  EQ	R3	R3	R1
      0x780E0004,  //  000F  JMPF	R3	#0015
      0x880C0101,  //  0010  GETMBR	R3	R0	K1
      0x8C0C0703,  //  0011  GETMET	R3	R3	K3
      0x5C140400,  //  0012  MOVE	R5	R2
      0x7C0C0400,  //  0013  CALL	R3	2
      0x70020000,  //  0014  JMP		#0016
      0x00080504,  //  0015  ADD	R2	R2	K4
      0x7001FFED,  //  0016  JMP		#0005
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_incoming
********************************************************************/
be_local_closure(Matter_IM_process_incoming,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[25]) {     /* constants */
    /* K0   */  be_nested_str_weak(opcode),
    /* K1   */  be_const_int(2),
    /* K2   */  be_nested_str_weak(read_request_solo),
    /* K3   */  be_nested_str_weak(from_raw),
    /* K4   */  be_nested_str_weak(raw),
    /* K5   */  be_nested_str_weak(app_payload_idx),
    /* K6   */  be_nested_str_weak(process_read_request_solo),
    /* K7   */  be_nested_str_weak(invoke_request_solo),
    /* K8   */  be_nested_str_weak(process_invoke_request_solo),
    /* K9   */  be_nested_str_weak(matter),
    /* K10  */  be_nested_str_weak(TLV),
    /* K11  */  be_nested_str_weak(parse),
    /* K12  */  be_const_int(1),
    /* K13  */  be_nested_str_weak(process_status_response),
    /* K14  */  be_nested_str_weak(process_read_request),
    /* K15  */  be_const_int(3),
    /* K16  */  be_nested_str_weak(send_ack_now),
    /* K17  */  be_nested_str_weak(subscribe_request),
    /* K18  */  be_nested_str_weak(subscribe_response),
    /* K19  */  be_nested_str_weak(report_data),
    /* K20  */  be_nested_str_weak(process_write_request),
    /* K21  */  be_nested_str_weak(process_write_response),
    /* K22  */  be_nested_str_weak(process_invoke_request),
    /* K23  */  be_nested_str_weak(process_invoke_response),
    /* K24  */  be_nested_str_weak(process_timed_request),
    }),
    be_str_weak(process_incoming),
    &be_const_str_solidified,
    ( &(const binstruction[133]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0x1C0C0501,  //  0001  EQ	R3	R2	K1
      0x780E000D,  //  0002  JMPF	R3	#0011
      0x880C0102,  //  0003  GETMBR	R3	R0	K2
      0x8C0C0703,  //  0004  GETMET	R3	R3	K3
      0x88140304,  //  0005  GETMBR	R5	R1	K4
      0x88180305,  //  0006  GETMBR	R6	R1	K5
      0x7C0C0600,  //  0007  CALL	R3	3
      0x4C100000,  //  0008  LDNIL	R4
      0x20100604,  //  0009  NE	R4	R3	R4
      0x78120004,  //  000A  JMPF	R4	#0010
      0x8C100106,  //  000B  GETMET	R4	R0	K6
      0x5C180200,  //  000C  MOVE	R6	R1
      0x5C1C0600,  //  000D  MOVE	R7	R3
      0x7C100600,  //  000E  CALL	R4	3
      0x80040800,  //  000F  RET	1	R4
      0x7002000F,  //  0010  JMP		#0021
      0x540E0007,  //  0011  LDINT	R3	8
      0x1C0C0403,  //  0012  EQ	R3	R2	R3
      0x780E000C,  //  0013  JMPF	R3	#0021
      0x880C0107,  //  0014  GETMBR	R3	R0	K7
      0x8C0C0703,  //  0015  GETMET	R3	R3	K3
      0x88140304,  //  0016  GETMBR	R5	R1	K4
      0x88180305,  //  0017  GETMBR	R6	R1	K5
      0x7C0C0600,  //  0018  CALL	R3	3
      0x4C100000,  //  0019  LDNIL	R4
      0x20100604,  //  001A  NE	R4	R3	R4
      0x78120004,  //  001B  JMPF	R4	#0021
      0x8C100108,  //  001C  GETMET	R4	R0	K8
      0x5C180200,  //  001D  MOVE	R6	R1
      0x5C1C0600,  //  001E  MOVE	R7	R3
      0x7C100600,  //  001F  CALL	R4	3
      0x80040800,  //  0020  RET	1	R4
      0xB80E1200,  //  0021  GETNGBL	R3	K9
      0x880C070A,  //  0022  GETMBR	R3	R3	K10
      0x8C0C070B,  //  0023  GETMET	R3	R3	K11
      0x88140304,  //  0024  GETMBR	R5	R1	K4
      0x88180305,  //  0025  GETMBR	R6	R1	K5
      0x7C0C0600,  //  0026  CALL	R3	3
      0x1C10050C,  //  0027  EQ	R4	R2	K12
      0x78120005,  //  0028  JMPF	R4	#002F
      0x8C10010D,  //  0029  GETMET	R4	R0	K13
      0x5C180200,  //  002A  MOVE	R6	R1
      0x5C1C0600,  //  002B  MOVE	R7	R3
      0x7C100600,  //  002C  CALL	R4	3
      0x80040800,  //  002D  RET	1	R4
      0x70020053,  //  002E  JMP		#0083
      0x1C100501,  //  002F  EQ	R4	R2	K1
      0x78120005,  //  0030  JMPF	R4	#0037
      0x8C10010E,  //  0031  GETMET	R4	R0	K14
      0x5C180200,  //  0032  MOVE	R6	R1
      0x5C1C0600,  //  0033  MOVE	R7	R3
      0x7C100600,  //  0034  CALL	R4	3
      0x80040800,  //  0035  RET	1	R4
      0x7002004B,  //  0036  JMP		#0083
      0x1C10050F,  //  0037  EQ	R4	R2	K15
      0x78120008,  //  0038  JMPF	R4	#0042
      0x8C100110,  //  0039  GETMET	R4	R0	K16
      0x5C180200,  //  003A  MOVE	R6	R1
      0x7C100400,  //  003B  CALL	R4	2
      0x8C100111,  //  003C  GETMET	R4	R0	K17
      0x5C180200,  //  003D  MOVE	R6	R1
      0x5C1C0600,  //  003E  MOVE	R7	R3
      0x7C100600,  //  003F  CALL	R4	3
      0x80040800,  //  0040  RET	1	R4
      0x70020040,  //  0041  JMP		#0083
      0x54120003,  //  0042  LDINT	R4	4
      0x1C100404,  //  0043  EQ	R4	R2	R4
      0x78120005,  //  0044  JMPF	R4	#004B
      0x8C100112,  //  0045  GETMET	R4	R0	K18
      0x5C180200,  //  0046  MOVE	R6	R1
      0x5C1C0600,  //  0047  MOVE	R7	R3
      0x7C100600,  //  0048  CALL	R4	3
      0x80040800,  //  0049  RET	1	R4
      0x70020037,  //  004A  JMP		#0083
      0x54120004,  //  004B  LDINT	R4	5
      0x1C100404,  //  004C  EQ	R4	R2	R4
      0x78120005,  //  004D  JMPF	R4	#0054
      0x8C100113,  //  004E  GETMET	R4	R0	K19
      0x5C180200,  //  004F  MOVE	R6	R1
      0x5C1C0600,  //  0050  MOVE	R7	R3
      0x7C100600,  //  0051  CALL	R4	3
      0x80040800,  //  0052  RET	1	R4
      0x7002002E,  //  0053  JMP		#0083
      0x54120005,  //  0054  LDINT	R4	6
      0x1C100404,  //  0055  EQ	R4	R2	R4
      0x78120008,  //  0056  JMPF	R4	#0060
      0x8C100110,  //  0057  GETMET	R4	R0	K16
      0x5C180200,  //  0058  MOVE	R6	R1
      0x7C100400,  //  0059  CALL	R4	2
      0x8C100114,  //  005A  GETMET	R4	R0	K20
      0x5C180200,  //  005B  MOVE	R6	R1
      0x5C1C0600,  //  005C  MOVE	R7	R3
      0x7C100600,  //  005D  CALL	R4	3
      0x80040800,  //  005E  RET	1	R4
      0x70020022,  //  005F  JMP		#0083
      0x54120006,  //  0060  LDINT	R4	7
      0x1C100404,  //  0061  EQ	R4	R2	R4
      0x78120005,  //  0062  JMPF	R4	#0069
      0x8C100115,  //  0063  GETMET	R4	R0	K21
      0x5C180200,  //  0064  MOVE	R6	R1
      0x5C1C0600,  //  0065  MOVE	R7	R3
      0x7C100600,  //  0066  CALL	R4	3
      0x80040800,  //  0067  RET	1	R4
      0x70020019,  //  0068  JMP		#0083
      0x54120007,  //  0069  LDINT	R4	8
      0x1C100404,  //  006A  EQ	R4	R2	R4
      0x78120005,  //  006B  JMPF	R4	#0072
      0x8C100116,  //  006C  GETMET	R4	R0	K22
      0x5C180200,  //  006D  MOVE	R6	R1
      0x5C1C0600,  //  006E  MOVE	R7	R3
      0x7C100600,  //  006F  CALL	R4	3
      0x80040800,  //  0070  RET	1	R4
      0x70020010,  //  0071  JMP		#0083
      0x54120008,  //  0072  LDINT	R4	9
      0x1C100404,  //  0073  EQ	R4	R2	R4
      0x78120005,  //  0074  JMPF	R4	#007B
      0x8C100117,  //  0075  GETMET	R4	R0	K23
      0x5C180200,  //  0076  MOVE	R6	R1
      0x5C1C0600,  //  0077  MOVE	R7	R3
      0x7C100600,  //  0078  CALL	R4	3
      0x80040800,  //  0079  RET	1	R4
      0x70020007,  //  007A  JMP		#0083
      0x54120009,  //  007B  LDINT	R4	10
      0x1C100404,  //  007C  EQ	R4	R2	R4
      0x78120004,  //  007D  JMPF	R4	#0083
      0x8C100118,  //  007E  GETMET	R4	R0	K24
      0x5C180200,  //  007F  MOVE	R6	R1
      0x5C1C0600,  //  0080  MOVE	R7	R3
      0x7C100600,  //  0081  CALL	R4	3
      0x80040800,  //  0082  RET	1	R4
      0x50100000,  //  0083  LDBOOL	R4	0	0
      0x80040800,  //  0084  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_invoke_response
********************************************************************/
be_local_closure(Matter_IM_process_invoke_response,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(InvokeResponseMessage),
    /* K2   */  be_nested_str_weak(from_TLV),
    }),
    be_str_weak(process_invoke_response),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x50100000,  //  0006  LDBOOL	R4	0	0
      0x80040800,  //  0007  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_IM_every_second,   /* name */
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
    /* K0   */  be_nested_str_weak(expire_sendqueue),
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
** Solidified function: send_status
********************************************************************/
be_local_closure(Matter_IM_send_status,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(send_queue),
    /* K1   */  be_nested_str_weak(push),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(IM_Status),
    }),
    be_str_weak(send_status),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0xB8160400,  //  0002  GETNGBL	R5	K2
      0x8C140B03,  //  0003  GETMET	R5	R5	K3
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C140600,  //  0006  CALL	R5	3
      0x7C0C0400,  //  0007  CALL	R3	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _inner_process_read_request
********************************************************************/
be_local_closure(Matter_IM__inner_process_read_request,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        20,                          /* nstack */
        4,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 3]) {  /* upvals */
          be_local_const_upval(1, 1),
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 4),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[35]) {     /* constants */
        /* K0   */  be_nested_str_weak(matter),
        /* K1   */  be_nested_str_weak(TLV),
        /* K2   */  be_nested_str_weak(get_attribute_name),
        /* K3   */  be_nested_str_weak(cluster),
        /* K4   */  be_nested_str_weak(attribute),
        /* K5   */  be_nested_str_weak(_X20_X28),
        /* K6   */  be_nested_str_weak(_X29),
        /* K7   */  be_nested_str_weak(),
        /* K8   */  be_nested_str_weak(read_attribute),
        /* K9   */  be_nested_str_weak(tlv_solo),
        /* K10  */  be_nested_str_weak(to_str_val),
        /* K11  */  be_nested_str_weak(is_list),
        /* K12  */  be_nested_str_weak(is_array),
        /* K13  */  be_nested_str_weak(encode_len),
        /* K14  */  be_nested_str_weak(IM_ReportData),
        /* K15  */  be_nested_str_weak(MAX_MESSAGE),
        /* K16  */  be_nested_str_weak(Matter_TLV_array),
        /* K17  */  be_nested_str_weak(attributedata2raw),
        /* K18  */  be_nested_str_weak(push),
        /* K19  */  be_nested_str_weak(val),
        /* K20  */  be_nested_str_weak(stop_iteration),
        /* K21  */  be_nested_str_weak(tasmota),
        /* K22  */  be_nested_str_weak(log),
        /* K23  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20_X25s),
        /* K24  */  be_nested_str_weak(local_session_id),
        /* K25  */  be_const_int(3),
        /* K26  */  be_nested_str_weak(status),
        /* K27  */  be_nested_str_weak(attributestatus2raw),
        /* K28  */  be_nested_str_weak(loglevel),
        /* K29  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20STATUS_X3A_X200x_X2502X_X20_X25s),
        /* K30  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
        /* K31  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20IGNORED),
        /* K32  */  be_const_int(0),
        /* K33  */  be_nested_str_weak(attribute_reports),
        /* K34  */  be_const_int(1),
        }),
        be_str_weak(read_single_attribute),
        &be_const_str_solidified,
        ( &(const binstruction[233]) {  /* code */
          0xB8120000,  //  0000  GETNGBL	R4	K0
          0x88100901,  //  0001  GETMBR	R4	R4	K1
          0xB8160000,  //  0002  GETNGBL	R5	K0
          0x8C140B02,  //  0003  GETMET	R5	R5	K2
          0x881C0503,  //  0004  GETMBR	R7	R2	K3
          0x88200504,  //  0005  GETMBR	R8	R2	K4
          0x7C140600,  //  0006  CALL	R5	3
          0x78160002,  //  0007  JMPF	R5	#000B
          0x001A0A05,  //  0008  ADD	R6	K5	R5
          0x00180D06,  //  0009  ADD	R6	R6	K6
          0x70020000,  //  000A  JMP		#000C
          0x58180007,  //  000B  LDCONST	R6	K7
          0x5C140C00,  //  000C  MOVE	R5	R6
          0x4C180000,  //  000D  LDNIL	R6
          0x20180206,  //  000E  NE	R6	R1	R6
          0x781A0006,  //  000F  JMPF	R6	#0017
          0x8C180308,  //  0010  GETMET	R6	R1	K8
          0x68200000,  //  0011  GETUPV	R8	U0
          0x5C240400,  //  0012  MOVE	R9	R2
          0x68280001,  //  0013  GETUPV	R10	U1
          0x88281509,  //  0014  GETMBR	R10	R10	K9
          0x7C180800,  //  0015  CALL	R6	4
          0x70020000,  //  0016  JMP		#0018
          0x4C180000,  //  0017  LDNIL	R6
          0x501C0200,  //  0018  LDBOOL	R7	1	0
          0x4C200000,  //  0019  LDNIL	R8
          0x4C240000,  //  001A  LDNIL	R9
          0x20240C09,  //  001B  NE	R9	R6	R9
          0x78260054,  //  001C  JMPF	R9	#0072
          0x58240007,  //  001D  LDCONST	R9	K7
          0x68280002,  //  001E  GETUPV	R10	U2
          0x742A0002,  //  001F  JMPT	R10	#0023
          0x8C280D0A,  //  0020  GETMET	R10	R6	K10
          0x7C280200,  //  0021  CALL	R10	1
          0x5C241400,  //  0022  MOVE	R9	R10
          0x88280D0B,  //  0023  GETMBR	R10	R6	K11
          0x742A0001,  //  0024  JMPT	R10	#0027
          0x88280D0C,  //  0025  GETMBR	R10	R6	K12
          0x782A0031,  //  0026  JMPF	R10	#0059
          0x8C280D0D,  //  0027  GETMET	R10	R6	K13
          0x7C280200,  //  0028  CALL	R10	1
          0xB82E0000,  //  0029  GETNGBL	R11	K0
          0x882C170E,  //  002A  GETMBR	R11	R11	K14
          0x882C170F,  //  002B  GETMBR	R11	R11	K15
          0x2428140B,  //  002C  GT	R10	R10	R11
          0x782A002A,  //  002D  JMPF	R10	#0059
          0x60280012,  //  002E  GETGBL	R10	G18
          0x7C280000,  //  002F  CALL	R10	0
          0x5C201400,  //  0030  MOVE	R8	R10
          0x60280015,  //  0031  GETGBL	R10	G21
          0x542E002F,  //  0032  LDINT	R11	48
          0x7C280200,  //  0033  CALL	R10	1
          0x8C2C0910,  //  0034  GETMET	R11	R4	K16
          0x7C2C0200,  //  0035  CALL	R11	1
          0x68300001,  //  0036  GETUPV	R12	U1
          0x8C301911,  //  0037  GETMET	R12	R12	K17
          0x5C381400,  //  0038  MOVE	R14	R10
          0x5C3C0400,  //  0039  MOVE	R15	R2
          0x5C401600,  //  003A  MOVE	R16	R11
          0x50440000,  //  003B  LDBOOL	R17	0	0
          0x7C300A00,  //  003C  CALL	R12	5
          0x8C301112,  //  003D  GETMET	R12	R8	K18
          0x5C381400,  //  003E  MOVE	R14	R10
          0x7C300400,  //  003F  CALL	R12	2
          0x60300010,  //  0040  GETGBL	R12	G16
          0x88340D13,  //  0041  GETMBR	R13	R6	K19
          0x7C300200,  //  0042  CALL	R12	1
          0xA8020010,  //  0043  EXBLK	0	#0055
          0x5C341800,  //  0044  MOVE	R13	R12
          0x7C340000,  //  0045  CALL	R13	0
          0x60380015,  //  0046  GETGBL	R14	G21
          0x543E002F,  //  0047  LDINT	R15	48
          0x7C380200,  //  0048  CALL	R14	1
          0x5C281C00,  //  0049  MOVE	R10	R14
          0x68380001,  //  004A  GETUPV	R14	U1
          0x8C381D11,  //  004B  GETMET	R14	R14	K17
          0x5C401400,  //  004C  MOVE	R16	R10
          0x5C440400,  //  004D  MOVE	R17	R2
          0x5C481A00,  //  004E  MOVE	R18	R13
          0x504C0200,  //  004F  LDBOOL	R19	1	0
          0x7C380A00,  //  0050  CALL	R14	5
          0x8C381112,  //  0051  GETMET	R14	R8	K18
          0x5C401400,  //  0052  MOVE	R16	R10
          0x7C380400,  //  0053  CALL	R14	2
          0x7001FFEE,  //  0054  JMP		#0044
          0x58300014,  //  0055  LDCONST	R12	K20
          0xAC300200,  //  0056  CATCH	R12	1	0
          0xB0080000,  //  0057  RAISE	2	R0	R0
          0x70020009,  //  0058  JMP		#0063
          0x60280015,  //  0059  GETGBL	R10	G21
          0x542E002F,  //  005A  LDINT	R11	48
          0x7C280200,  //  005B  CALL	R10	1
          0x5C201400,  //  005C  MOVE	R8	R10
          0x68280001,  //  005D  GETUPV	R10	U1
          0x8C281511,  //  005E  GETMET	R10	R10	K17
          0x5C301000,  //  005F  MOVE	R12	R8
          0x5C340400,  //  0060  MOVE	R13	R2
          0x5C380C00,  //  0061  MOVE	R14	R6
          0x7C280800,  //  0062  CALL	R10	4
          0x68280002,  //  0063  GETUPV	R10	U2
          0x742A000B,  //  0064  JMPT	R10	#0071
          0xB82A2A00,  //  0065  GETNGBL	R10	K21
          0x8C281516,  //  0066  GETMET	R10	R10	K22
          0x60300018,  //  0067  GETGBL	R12	G24
          0x58340017,  //  0068  LDCONST	R13	K23
          0x68380000,  //  0069  GETUPV	R14	U0
          0x88381D18,  //  006A  GETMBR	R14	R14	K24
          0x5C3C0400,  //  006B  MOVE	R15	R2
          0x5C400A00,  //  006C  MOVE	R16	R5
          0x5C441200,  //  006D  MOVE	R17	R9
          0x7C300A00,  //  006E  CALL	R12	5
          0x58340019,  //  006F  LDCONST	R13	K25
          0x7C280600,  //  0070  CALL	R10	3
          0x70020038,  //  0071  JMP		#00AB
          0x8824051A,  //  0072  GETMBR	R9	R2	K26
          0x4C280000,  //  0073  LDNIL	R10
          0x2024120A,  //  0074  NE	R9	R9	R10
          0x78260026,  //  0075  JMPF	R9	#009D
          0x780E0024,  //  0076  JMPF	R3	#009C
          0x60240015,  //  0077  GETGBL	R9	G21
          0x542A002F,  //  0078  LDINT	R10	48
          0x7C240200,  //  0079  CALL	R9	1
          0x5C201200,  //  007A  MOVE	R8	R9
          0x68240001,  //  007B  GETUPV	R9	U1
          0x8C24131B,  //  007C  GETMET	R9	R9	K27
          0x5C2C1000,  //  007D  MOVE	R11	R8
          0x5C300400,  //  007E  MOVE	R12	R2
          0x8834051A,  //  007F  GETMBR	R13	R2	K26
          0x7C240800,  //  0080  CALL	R9	4
          0xB8262A00,  //  0081  GETNGBL	R9	K21
          0x8C24131C,  //  0082  GETMET	R9	R9	K28
          0x582C0019,  //  0083  LDCONST	R11	K25
          0x7C240400,  //  0084  CALL	R9	2
          0x78260015,  //  0085  JMPF	R9	#009C
          0xB8262A00,  //  0086  GETNGBL	R9	K21
          0x8C241316,  //  0087  GETMET	R9	R9	K22
          0x602C0018,  //  0088  GETGBL	R11	G24
          0x5830001D,  //  0089  LDCONST	R12	K29
          0x68340000,  //  008A  GETUPV	R13	U0
          0x88341B18,  //  008B  GETMBR	R13	R13	K24
          0x60380008,  //  008C  GETGBL	R14	G8
          0x5C3C0400,  //  008D  MOVE	R15	R2
          0x7C380200,  //  008E  CALL	R14	1
          0x5C3C0A00,  //  008F  MOVE	R15	R5
          0x8840051A,  //  0090  GETMBR	R16	R2	K26
          0x8844051A,  //  0091  GETMBR	R17	R2	K26
          0xB84A0000,  //  0092  GETNGBL	R18	K0
          0x8848251E,  //  0093  GETMBR	R18	R18	K30
          0x1C442212,  //  0094  EQ	R17	R17	R18
          0x78460001,  //  0095  JMPF	R17	#0098
          0x5844001E,  //  0096  LDCONST	R17	K30
          0x70020000,  //  0097  JMP		#0099
          0x58440007,  //  0098  LDCONST	R17	K7
          0x7C2C0C00,  //  0099  CALL	R11	6
          0x58300019,  //  009A  LDCONST	R12	K25
          0x7C240600,  //  009B  CALL	R9	3
          0x7002000D,  //  009C  JMP		#00AB
          0xB8262A00,  //  009D  GETNGBL	R9	K21
          0x8C241316,  //  009E  GETMET	R9	R9	K22
          0x602C0018,  //  009F  GETGBL	R11	G24
          0x5830001F,  //  00A0  LDCONST	R12	K31
          0x68340000,  //  00A1  GETUPV	R13	U0
          0x88341B18,  //  00A2  GETMBR	R13	R13	K24
          0x60380008,  //  00A3  GETGBL	R14	G8
          0x5C3C0400,  //  00A4  MOVE	R15	R2
          0x7C380200,  //  00A5  CALL	R14	1
          0x5C3C0A00,  //  00A6  MOVE	R15	R5
          0x7C2C0800,  //  00A7  CALL	R11	4
          0x58300019,  //  00A8  LDCONST	R12	K25
          0x7C240600,  //  00A9  CALL	R9	3
          0x501C0000,  //  00AA  LDBOOL	R7	0	0
          0x6024000F,  //  00AB  GETGBL	R9	G15
          0x5C281000,  //  00AC  MOVE	R10	R8
          0x602C0012,  //  00AD  GETGBL	R11	G18
          0x7C240400,  //  00AE  CALL	R9	2
          0x78260001,  //  00AF  JMPF	R9	#00B2
          0x58240020,  //  00B0  LDCONST	R9	K32
          0x70020000,  //  00B1  JMP		#00B3
          0x4C240000,  //  00B2  LDNIL	R9
          0x4C280000,  //  00B3  LDNIL	R10
          0x2028100A,  //  00B4  NE	R10	R8	R10
          0x782A0031,  //  00B5  JMPF	R10	#00E8
          0x4C280000,  //  00B6  LDNIL	R10
          0x1C28120A,  //  00B7  EQ	R10	R9	R10
          0x782A0001,  //  00B8  JMPF	R10	#00BB
          0x5C281000,  //  00B9  MOVE	R10	R8
          0x70020000,  //  00BA  JMP		#00BC
          0x94281009,  //  00BB  GETIDX	R10	R8	R9
          0x602C000C,  //  00BC  GETGBL	R11	G12
          0x88300121,  //  00BD  GETMBR	R12	R0	K33
          0x7C2C0200,  //  00BE  CALL	R11	1
          0x1C2C1720,  //  00BF  EQ	R11	R11	K32
          0x782E0004,  //  00C0  JMPF	R11	#00C6
          0x882C0121,  //  00C1  GETMBR	R11	R0	K33
          0x8C2C1712,  //  00C2  GETMET	R11	R11	K18
          0x5C341400,  //  00C3  MOVE	R13	R10
          0x7C2C0400,  //  00C4  CALL	R11	2
          0x70020014,  //  00C5  JMP		#00DB
          0x882C0121,  //  00C6  GETMBR	R11	R0	K33
          0x5431FFFE,  //  00C7  LDINT	R12	-1
          0x942C160C,  //  00C8  GETIDX	R11	R11	R12
          0x6034000C,  //  00C9  GETGBL	R13	G12
          0x5C381600,  //  00CA  MOVE	R14	R11
          0x7C340200,  //  00CB  CALL	R13	1
          0x6038000C,  //  00CC  GETGBL	R14	G12
          0x5C3C1400,  //  00CD  MOVE	R15	R10
          0x7C380200,  //  00CE  CALL	R14	1
          0x00341A0E,  //  00CF  ADD	R13	R13	R14
          0xB83A0000,  //  00D0  GETNGBL	R14	K0
          0x88381D0E,  //  00D1  GETMBR	R14	R14	K14
          0x88381D0F,  //  00D2  GETMBR	R14	R14	K15
          0x18341A0E,  //  00D3  LE	R13	R13	R14
          0x78360001,  //  00D4  JMPF	R13	#00D7
          0x4034160A,  //  00D5  CONNECT	R13	R11	R10
          0x70020003,  //  00D6  JMP		#00DB
          0x88300121,  //  00D7  GETMBR	R12	R0	K33
          0x8C301912,  //  00D8  GETMET	R12	R12	K18
          0x5C381400,  //  00D9  MOVE	R14	R10
          0x7C300400,  //  00DA  CALL	R12	2
          0x4C2C0000,  //  00DB  LDNIL	R11
          0x1C2C120B,  //  00DC  EQ	R11	R9	R11
          0x782E0001,  //  00DD  JMPF	R11	#00E0
          0x4C200000,  //  00DE  LDNIL	R8
          0x70020006,  //  00DF  JMP		#00E7
          0x00241322,  //  00E0  ADD	R9	R9	K34
          0x602C000C,  //  00E1  GETGBL	R11	G12
          0x5C301000,  //  00E2  MOVE	R12	R8
          0x7C2C0200,  //  00E3  CALL	R11	1
          0x282C120B,  //  00E4  GE	R11	R9	R11
          0x782E0000,  //  00E5  JMPF	R11	#00E7
          0x4C200000,  //  00E6  LDNIL	R8
          0x7001FFCA,  //  00E7  JMP		#00B3
          0x80040E00,  //  00E8  RET	1	R7
        })
      ),
      be_nested_proto(
        8,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 5),
          be_local_const_upval(1, 8),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 7]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x68100001,  //  0001  GETUPV	R4	U1
          0x5C140000,  //  0002  MOVE	R5	R0
          0x5C180200,  //  0003  MOVE	R6	R1
          0x5C1C0400,  //  0004  MOVE	R7	R2
          0x7C0C0800,  //  0005  CALL	R3	4
          0x80040600,  //  0006  RET	1	R3
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[25]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(get_active_endpoints),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(Path),
    /* K4   */  be_nested_str_weak(msg),
    /* K5   */  be_nested_str_weak(ReportDataMessage),
    /* K6   */  be_nested_str_weak(attribute_reports),
    /* K7   */  be_nested_str_weak(attributes_requests),
    /* K8   */  be_nested_str_weak(endpoint),
    /* K9   */  be_nested_str_weak(cluster),
    /* K10  */  be_nested_str_weak(attribute),
    /* K11  */  be_nested_str_weak(fabric_filtered),
    /* K12  */  be_nested_str_weak(status),
    /* K13  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K14  */  be_nested_str_weak(get_attribute_name),
    /* K15  */  be_nested_str_weak(tasmota),
    /* K16  */  be_nested_str_weak(log),
    /* K17  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s),
    /* K18  */  be_nested_str_weak(local_session_id),
    /* K19  */  be_nested_str_weak(_X20_X28),
    /* K20  */  be_nested_str_weak(_X29),
    /* K21  */  be_nested_str_weak(),
    /* K22  */  be_const_int(3),
    /* K23  */  be_nested_str_weak(process_attribute_expansion),
    /* K24  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(_inner_process_read_request),
    &be_const_str_solidified,
    ( &(const binstruction[96]) {  /* code */
      0x84140000,  //  0000  CLOSURE	R5	P0
      0x88180100,  //  0001  GETMBR	R6	R0	K0
      0x8C180D01,  //  0002  GETMET	R6	R6	K1
      0x7C180200,  //  0003  CALL	R6	1
      0xB81E0400,  //  0004  GETNGBL	R7	K2
      0x8C1C0F03,  //  0005  GETMET	R7	R7	K3
      0x7C1C0200,  //  0006  CALL	R7	1
      0x901E0803,  //  0007  SETMBR	R7	K4	R3
      0xB8220400,  //  0008  GETNGBL	R8	K2
      0x8C201105,  //  0009  GETMET	R8	R8	K5
      0x7C200200,  //  000A  CALL	R8	1
      0x60240012,  //  000B  GETGBL	R9	G18
      0x7C240000,  //  000C  CALL	R9	0
      0x90220C09,  //  000D  SETMBR	R8	K6	R9
      0x60240010,  //  000E  GETGBL	R9	G16
      0x88280507,  //  000F  GETMBR	R10	R2	K7
      0x7C240200,  //  0010  CALL	R9	1
      0xA8020048,  //  0011  EXBLK	0	#005B
      0x5C281200,  //  0012  MOVE	R10	R9
      0x7C280000,  //  0013  CALL	R10	0
      0x882C1508,  //  0014  GETMBR	R11	R10	K8
      0x901E100B,  //  0015  SETMBR	R7	K8	R11
      0x882C1509,  //  0016  GETMBR	R11	R10	K9
      0x901E120B,  //  0017  SETMBR	R7	K9	R11
      0x882C150A,  //  0018  GETMBR	R11	R10	K10
      0x901E140B,  //  0019  SETMBR	R7	K10	R11
      0x882C050B,  //  001A  GETMBR	R11	R2	K11
      0x901E160B,  //  001B  SETMBR	R7	K11	R11
      0xB82E0400,  //  001C  GETNGBL	R11	K2
      0x882C170D,  //  001D  GETMBR	R11	R11	K13
      0x901E180B,  //  001E  SETMBR	R7	K12	R11
      0x882C0F08,  //  001F  GETMBR	R11	R7	K8
      0x4C300000,  //  0020  LDNIL	R12
      0x1C2C160C,  //  0021  EQ	R11	R11	R12
      0x742E0007,  //  0022  JMPT	R11	#002B
      0x882C0F09,  //  0023  GETMBR	R11	R7	K9
      0x4C300000,  //  0024  LDNIL	R12
      0x1C2C160C,  //  0025  EQ	R11	R11	R12
      0x742E0003,  //  0026  JMPT	R11	#002B
      0x882C0F0A,  //  0027  GETMBR	R11	R7	K10
      0x4C300000,  //  0028  LDNIL	R12
      0x1C2C160C,  //  0029  EQ	R11	R11	R12
      0x782E0029,  //  002A  JMPF	R11	#0055
      0x882C0F09,  //  002B  GETMBR	R11	R7	K9
      0x4C300000,  //  002C  LDNIL	R12
      0x202C160C,  //  002D  NE	R11	R11	R12
      0x782E001A,  //  002E  JMPF	R11	#004A
      0x882C0F0A,  //  002F  GETMBR	R11	R7	K10
      0x4C300000,  //  0030  LDNIL	R12
      0x202C160C,  //  0031  NE	R11	R11	R12
      0x782E0016,  //  0032  JMPF	R11	#004A
      0xB82E0400,  //  0033  GETNGBL	R11	K2
      0x8C2C170E,  //  0034  GETMET	R11	R11	K14
      0x88340F09,  //  0035  GETMBR	R13	R7	K9
      0x88380F0A,  //  0036  GETMBR	R14	R7	K10
      0x7C2C0600,  //  0037  CALL	R11	3
      0xB8321E00,  //  0038  GETNGBL	R12	K15
      0x8C301910,  //  0039  GETMET	R12	R12	K16
      0x60380018,  //  003A  GETGBL	R14	G24
      0x583C0011,  //  003B  LDCONST	R15	K17
      0x88400312,  //  003C  GETMBR	R16	R1	K18
      0x60440008,  //  003D  GETGBL	R17	G8
      0x5C480E00,  //  003E  MOVE	R18	R7
      0x7C440200,  //  003F  CALL	R17	1
      0x782E0002,  //  0040  JMPF	R11	#0044
      0x004A260B,  //  0041  ADD	R18	K19	R11
      0x00482514,  //  0042  ADD	R18	R18	K20
      0x70020000,  //  0043  JMP		#0045
      0x58480015,  //  0044  LDCONST	R18	K21
      0x00442212,  //  0045  ADD	R17	R17	R18
      0x7C380600,  //  0046  CALL	R14	3
      0x583C0016,  //  0047  LDCONST	R15	K22
      0x7C300600,  //  0048  CALL	R12	3
      0x7002000A,  //  0049  JMP		#0055
      0xB82E1E00,  //  004A  GETNGBL	R11	K15
      0x8C2C1710,  //  004B  GETMET	R11	R11	K16
      0x60340018,  //  004C  GETGBL	R13	G24
      0x58380011,  //  004D  LDCONST	R14	K17
      0x883C0312,  //  004E  GETMBR	R15	R1	K18
      0x60400008,  //  004F  GETGBL	R16	G8
      0x5C440E00,  //  0050  MOVE	R17	R7
      0x7C400200,  //  0051  CALL	R16	1
      0x7C340600,  //  0052  CALL	R13	3
      0x58380016,  //  0053  LDCONST	R14	K22
      0x7C2C0600,  //  0054  CALL	R11	3
      0x882C0100,  //  0055  GETMBR	R11	R0	K0
      0x8C2C1717,  //  0056  GETMET	R11	R11	K23
      0x5C340E00,  //  0057  MOVE	R13	R7
      0x84380001,  //  0058  CLOSURE	R14	P1
      0x7C2C0600,  //  0059  CALL	R11	3
      0x7001FFB6,  //  005A  JMP		#0012
      0x58240018,  //  005B  LDCONST	R9	K24
      0xAC240200,  //  005C  CATCH	R9	1	0
      0xB0080000,  //  005D  RAISE	2	R0	R0
      0xA0000000,  //  005E  CLOSE	R0
      0x80041000,  //  005F  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: report_data
********************************************************************/
be_local_closure(Matter_IM_report_data,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(ReportDataMessage),
    /* K2   */  be_nested_str_weak(from_TLV),
    }),
    be_str_weak(report_data),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x50100000,  //  0006  LDBOOL	R4	0	0
      0x80040800,  //  0007  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: attributestatus2raw
********************************************************************/
be_local_closure(Matter_IM_attributestatus2raw,   /* name */
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
    /* K0   */  be_nested_str_weak(add),
    /* K1   */  be_const_int(1),
    /* K2   */  be_nested_str_weak(path2raw),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(status),
    /* K5   */  be_const_int(2),
    }),
    be_str_weak(attributestatus2raw),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0x8C100300,  //  0000  GETMET	R4	R1	K0
      0x541A0014,  //  0001  LDINT	R6	21
      0x581C0001,  //  0002  LDCONST	R7	K1
      0x7C100600,  //  0003  CALL	R4	3
      0x8C100300,  //  0004  GETMET	R4	R1	K0
      0x541A34FF,  //  0005  LDINT	R6	13568
      0x541DFFFD,  //  0006  LDINT	R7	-2
      0x7C100600,  //  0007  CALL	R4	3
      0x8C100102,  //  0008  GETMET	R4	R0	K2
      0x5C180200,  //  0009  MOVE	R6	R1
      0x5C1C0400,  //  000A  MOVE	R7	R2
      0x58200003,  //  000B  LDCONST	R8	K3
      0x7C100800,  //  000C  CALL	R4	4
      0x8C100300,  //  000D  GETMET	R4	R1	K0
      0x541A3500,  //  000E  LDINT	R6	13569
      0x541DFFFD,  //  000F  LDINT	R7	-2
      0x7C100600,  //  0010  CALL	R4	3
      0x88100504,  //  0011  GETMBR	R4	R2	K4
      0x541600FE,  //  0012  LDINT	R5	255
      0x18100805,  //  0013  LE	R4	R4	R5
      0x78120008,  //  0014  JMPF	R4	#001E
      0x8C100300,  //  0015  GETMET	R4	R1	K0
      0x541A23FF,  //  0016  LDINT	R6	9216
      0x541DFFFD,  //  0017  LDINT	R7	-2
      0x7C100600,  //  0018  CALL	R4	3
      0x8C100300,  //  0019  GETMET	R4	R1	K0
      0x88180504,  //  001A  GETMBR	R6	R2	K4
      0x581C0001,  //  001B  LDCONST	R7	K1
      0x7C100600,  //  001C  CALL	R4	3
      0x70020007,  //  001D  JMP		#0026
      0x8C100300,  //  001E  GETMET	R4	R1	K0
      0x541A24FF,  //  001F  LDINT	R6	9472
      0x541DFFFD,  //  0020  LDINT	R7	-2
      0x7C100600,  //  0021  CALL	R4	3
      0x8C100300,  //  0022  GETMET	R4	R1	K0
      0x88180504,  //  0023  GETMBR	R6	R2	K4
      0x581C0005,  //  0024  LDCONST	R7	K5
      0x7C100600,  //  0025  CALL	R4	3
      0x8C100300,  //  0026  GETMET	R4	R1	K0
      0x541A1817,  //  0027  LDINT	R6	6168
      0x541DFFFD,  //  0028  LDINT	R7	-2
      0x7C100600,  //  0029  CALL	R4	3
      0x8C100300,  //  002A  GETMET	R4	R1	K0
      0x541A0017,  //  002B  LDINT	R6	24
      0x581C0001,  //  002C  LDCONST	R7	K1
      0x7C100600,  //  002D  CALL	R4	3
      0x80000000,  //  002E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_subscribe_response
********************************************************************/
be_local_closure(Matter_IM_send_subscribe_response,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(send_queue),
    /* K1   */  be_nested_str_weak(push),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(IM_SubscribeResponse),
    }),
    be_str_weak(send_subscribe_response),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x8C100901,  //  0001  GETMET	R4	R4	K1
      0xB81A0400,  //  0002  GETNGBL	R6	K2
      0x8C180D03,  //  0003  GETMET	R6	R6	K3
      0x5C200200,  //  0004  MOVE	R8	R1
      0x5C240400,  //  0005  MOVE	R9	R2
      0x5C280600,  //  0006  MOVE	R10	R3
      0x7C180800,  //  0007  CALL	R6	4
      0x7C100400,  //  0008  CALL	R4	2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_report_data
********************************************************************/
be_local_closure(Matter_IM_send_report_data,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(send_queue),
    /* K1   */  be_nested_str_weak(push),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(IM_ReportData),
    }),
    be_str_weak(send_report_data),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0xB8160400,  //  0002  GETNGBL	R5	K2
      0x8C140B03,  //  0003  GETMET	R5	R5	K3
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C140600,  //  0006  CALL	R5	3
      0x7C0C0400,  //  0007  CALL	R3	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_sendqueue_by_exchangeid
********************************************************************/
be_local_closure(Matter_IM_find_sendqueue_by_exchangeid,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(send_queue),
    /* K2   */  be_nested_str_weak(get_exchangeid),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(find_sendqueue_by_exchangeid),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x58080000,  //  0005  LDCONST	R2	K0
      0x600C000C,  //  0006  GETGBL	R3	G12
      0x88100101,  //  0007  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0008  CALL	R3	1
      0x140C0403,  //  0009  LT	R3	R2	R3
      0x780E0008,  //  000A  JMPF	R3	#0014
      0x880C0101,  //  000B  GETMBR	R3	R0	K1
      0x940C0602,  //  000C  GETIDX	R3	R3	R2
      0x8C100702,  //  000D  GETMET	R4	R3	K2
      0x7C100200,  //  000E  CALL	R4	1
      0x1C100801,  //  000F  EQ	R4	R4	R1
      0x78120000,  //  0010  JMPF	R4	#0012
      0x80040600,  //  0011  RET	1	R3
      0x00080503,  //  0012  ADD	R2	R2	K3
      0x7001FFF1,  //  0013  JMP		#0006
      0x4C0C0000,  //  0014  LDNIL	R3
      0x80040600,  //  0015  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_read_request_solo
********************************************************************/
be_local_closure(Matter_IM_process_read_request_solo,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[55]) {     /* constants */
    /* K0   */  be_nested_str_weak(status),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(INVALID_ACTION),
    /* K3   */  be_nested_str_weak(msg),
    /* K4   */  be_nested_str_weak(device),
    /* K5   */  be_nested_str_weak(process_attribute_read_solo),
    /* K6   */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K7   */  be_nested_str_weak(read_attribute),
    /* K8   */  be_nested_str_weak(session),
    /* K9   */  be_nested_str_weak(tlv_solo),
    /* K10  */  be_nested_str_weak(profiler),
    /* K11  */  be_nested_str_weak(log),
    /* K12  */  be_nested_str_weak(read_request_solo_X20read_X20done),
    /* K13  */  be_nested_str_weak(is_list),
    /* K14  */  be_nested_str_weak(is_array),
    /* K15  */  be_nested_str_weak(encode_len),
    /* K16  */  be_nested_str_weak(IM_ReportData),
    /* K17  */  be_nested_str_weak(MAX_MESSAGE),
    /* K18  */  be_nested_str_weak(tasmota),
    /* K19  */  be_nested_str_weak(MTR_X3A_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20Response_X20to_X20big_X2C_X20revert_X20to_X20non_X2Dsolo),
    /* K20  */  be_const_int(3),
    /* K21  */  be_nested_str_weak(TLV),
    /* K22  */  be_nested_str_weak(parse),
    /* K23  */  be_nested_str_weak(raw),
    /* K24  */  be_nested_str_weak(app_payload_idx),
    /* K25  */  be_nested_str_weak(process_read_request),
    /* K26  */  be_nested_str_weak(add),
    /* K27  */  be_const_int(1),
    /* K28  */  be_nested_str_weak(attributedata2raw),
    /* K29  */  be_const_int(405077761),
    /* K30  */  be_nested_str_weak(attributestatus2raw),
    /* K31  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X20_X2D_X20IGNORED),
    /* K32  */  be_nested_str_weak(local_session_id),
    /* K33  */  be_nested_str_weak(build_response),
    /* K34  */  be_nested_str_weak(message_handler),
    /* K35  */  be_nested_str_weak(clear),
    /* K36  */  be_nested_str_weak(encode_frame),
    /* K37  */  be_nested_str_weak(encrypt),
    /* K38  */  be_nested_str_weak(loglevel),
    /* K39  */  be_nested_str_weak(MTR_X3A_X20_X3Csnd_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20id_X3D_X25i_X20exch_X3D_X25i_X20rack_X3D_X25s),
    /* K40  */  be_nested_str_weak(message_counter),
    /* K41  */  be_nested_str_weak(exchange_id),
    /* K42  */  be_nested_str_weak(ack_message_counter),
    /* K43  */  be_nested_str_weak(send_response_frame),
    /* K44  */  be_nested_str_weak(RESPONSE_X20SENT),
    /* K45  */  be_nested_str_weak(get_attribute_name),
    /* K46  */  be_nested_str_weak(cluster),
    /* K47  */  be_nested_str_weak(attribute),
    /* K48  */  be_nested_str_weak(_X20_X28),
    /* K49  */  be_nested_str_weak(_X29),
    /* K50  */  be_nested_str_weak(),
    /* K51  */  be_nested_str_weak(to_str_val),
    /* K52  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr1_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20_X25s),
    /* K53  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr1_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20STATUS_X3A_X200x_X2502X_X20_X25s),
    /* K54  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr1_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20IGNORED),
    }),
    be_str_weak(process_read_request_solo),
    &be_const_str_solidified,
    ( &(const binstruction[251]) {  /* code */
      0xB80E0200,  //  0000  GETNGBL	R3	K1
      0x880C0702,  //  0001  GETMBR	R3	R3	K2
      0x900A0003,  //  0002  SETMBR	R2	K0	R3
      0x900A0601,  //  0003  SETMBR	R2	K3	R1
      0x880C0104,  //  0004  GETMBR	R3	R0	K4
      0x8C0C0705,  //  0005  GETMET	R3	R3	K5
      0x5C140400,  //  0006  MOVE	R5	R2
      0x7C0C0400,  //  0007  CALL	R3	2
      0x4C100000,  //  0008  LDNIL	R4
      0x4C140000,  //  0009  LDNIL	R5
      0x4C180000,  //  000A  LDNIL	R6
      0x20180606,  //  000B  NE	R6	R3	R6
      0x781A0008,  //  000C  JMPF	R6	#0016
      0xB81A0200,  //  000D  GETNGBL	R6	K1
      0x88180D06,  //  000E  GETMBR	R6	R6	K6
      0x900A0006,  //  000F  SETMBR	R2	K0	R6
      0x8C180707,  //  0010  GETMET	R6	R3	K7
      0x88200308,  //  0011  GETMBR	R8	R1	K8
      0x5C240400,  //  0012  MOVE	R9	R2
      0x88280109,  //  0013  GETMBR	R10	R0	K9
      0x7C180800,  //  0014  CALL	R6	4
      0x5C100C00,  //  0015  MOVE	R4	R6
      0xB81A0200,  //  0016  GETNGBL	R6	K1
      0x88180D0A,  //  0017  GETMBR	R6	R6	K10
      0x8C180D0B,  //  0018  GETMET	R6	R6	K11
      0x5820000C,  //  0019  LDCONST	R8	K12
      0x7C180400,  //  001A  CALL	R6	2
      0x4C180000,  //  001B  LDNIL	R6
      0x20180806,  //  001C  NE	R6	R4	R6
      0x781A0037,  //  001D  JMPF	R6	#0056
      0x8818090D,  //  001E  GETMBR	R6	R4	K13
      0x741A0001,  //  001F  JMPT	R6	#0022
      0x8818090E,  //  0020  GETMBR	R6	R4	K14
      0x781A0019,  //  0021  JMPF	R6	#003C
      0x8C18090F,  //  0022  GETMET	R6	R4	K15
      0x7C180200,  //  0023  CALL	R6	1
      0xB81E0200,  //  0024  GETNGBL	R7	K1
      0x881C0F10,  //  0025  GETMBR	R7	R7	K16
      0x881C0F11,  //  0026  GETMBR	R7	R7	K17
      0x24180C07,  //  0027  GT	R6	R6	R7
      0x781A0012,  //  0028  JMPF	R6	#003C
      0x4C100000,  //  0029  LDNIL	R4
      0xB81A2400,  //  002A  GETNGBL	R6	K18
      0x8C180D0B,  //  002B  GETMET	R6	R6	K11
      0x60200018,  //  002C  GETGBL	R8	G24
      0x58240013,  //  002D  LDCONST	R9	K19
      0x7C200200,  //  002E  CALL	R8	1
      0x58240014,  //  002F  LDCONST	R9	K20
      0x7C180600,  //  0030  CALL	R6	3
      0xB81A0200,  //  0031  GETNGBL	R6	K1
      0x88180D15,  //  0032  GETMBR	R6	R6	K21
      0x8C180D16,  //  0033  GETMET	R6	R6	K22
      0x88200317,  //  0034  GETMBR	R8	R1	K23
      0x88240318,  //  0035  GETMBR	R9	R1	K24
      0x7C180600,  //  0036  CALL	R6	3
      0x8C1C0119,  //  0037  GETMET	R7	R0	K25
      0x5C240200,  //  0038  MOVE	R9	R1
      0x5C280C00,  //  0039  MOVE	R10	R6
      0x7C1C0600,  //  003A  CALL	R7	3
      0x80040E00,  //  003B  RET	1	R7
      0x60180015,  //  003C  GETGBL	R6	G21
      0x541E002F,  //  003D  LDINT	R7	48
      0x7C180200,  //  003E  CALL	R6	1
      0x5C140C00,  //  003F  MOVE	R5	R6
      0x8C180B1A,  //  0040  GETMET	R6	R5	K26
      0x54220014,  //  0041  LDINT	R8	21
      0x5824001B,  //  0042  LDCONST	R9	K27
      0x7C180600,  //  0043  CALL	R6	3
      0x8C180B1A,  //  0044  GETMET	R6	R5	K26
      0x54223600,  //  0045  LDINT	R8	13825
      0x5425FFFD,  //  0046  LDINT	R9	-2
      0x7C180600,  //  0047  CALL	R6	3
      0x8C18011C,  //  0048  GETMET	R6	R0	K28
      0x5C200A00,  //  0049  MOVE	R8	R5
      0x5C240400,  //  004A  MOVE	R9	R2
      0x5C280800,  //  004B  MOVE	R10	R4
      0x7C180800,  //  004C  CALL	R6	4
      0x8C180B1A,  //  004D  GETMET	R6	R5	K26
      0x5820001D,  //  004E  LDCONST	R8	K29
      0x5425FFFB,  //  004F  LDINT	R9	-4
      0x7C180600,  //  0050  CALL	R6	3
      0x8C180B1A,  //  0051  GETMET	R6	R5	K26
      0x54220017,  //  0052  LDINT	R8	24
      0x5824001B,  //  0053  LDCONST	R9	K27
      0x7C180600,  //  0054  CALL	R6	3
      0x70020029,  //  0055  JMP		#0080
      0x88180500,  //  0056  GETMBR	R6	R2	K0
      0x4C1C0000,  //  0057  LDNIL	R7
      0x20180C07,  //  0058  NE	R6	R6	R7
      0x781A0019,  //  0059  JMPF	R6	#0074
      0x60180015,  //  005A  GETGBL	R6	G21
      0x541E002F,  //  005B  LDINT	R7	48
      0x7C180200,  //  005C  CALL	R6	1
      0x5C140C00,  //  005D  MOVE	R5	R6
      0x8C180B1A,  //  005E  GETMET	R6	R5	K26
      0x54220014,  //  005F  LDINT	R8	21
      0x5824001B,  //  0060  LDCONST	R9	K27
      0x7C180600,  //  0061  CALL	R6	3
      0x8C180B1A,  //  0062  GETMET	R6	R5	K26
      0x54223600,  //  0063  LDINT	R8	13825
      0x5425FFFD,  //  0064  LDINT	R9	-2
      0x7C180600,  //  0065  CALL	R6	3
      0x8C18011E,  //  0066  GETMET	R6	R0	K30
      0x5C200A00,  //  0067  MOVE	R8	R5
      0x5C240400,  //  0068  MOVE	R9	R2
      0x88280500,  //  0069  GETMBR	R10	R2	K0
      0x7C180800,  //  006A  CALL	R6	4
      0x8C180B1A,  //  006B  GETMET	R6	R5	K26
      0x5820001D,  //  006C  LDCONST	R8	K29
      0x5425FFFB,  //  006D  LDINT	R9	-4
      0x7C180600,  //  006E  CALL	R6	3
      0x8C180B1A,  //  006F  GETMET	R6	R5	K26
      0x54220017,  //  0070  LDINT	R8	24
      0x5824001B,  //  0071  LDCONST	R9	K27
      0x7C180600,  //  0072  CALL	R6	3
      0x7002000B,  //  0073  JMP		#0080
      0xB81A2400,  //  0074  GETNGBL	R6	K18
      0x8C180D0B,  //  0075  GETMET	R6	R6	K11
      0x60200018,  //  0076  GETGBL	R8	G24
      0x5824001F,  //  0077  LDCONST	R9	K31
      0x88280308,  //  0078  GETMBR	R10	R1	K8
      0x88281520,  //  0079  GETMBR	R10	R10	K32
      0x5C2C0400,  //  007A  MOVE	R11	R2
      0x7C200600,  //  007B  CALL	R8	3
      0x58240014,  //  007C  LDCONST	R9	K20
      0x7C180600,  //  007D  CALL	R6	3
      0x50180000,  //  007E  LDBOOL	R6	0	0
      0x80040C00,  //  007F  RET	1	R6
      0x8C180321,  //  0080  GETMET	R6	R1	K33
      0x54220004,  //  0081  LDINT	R8	5
      0x50240200,  //  0082  LDBOOL	R9	1	0
      0x7C180600,  //  0083  CALL	R6	3
      0x881C0104,  //  0084  GETMBR	R7	R0	K4
      0x881C0F22,  //  0085  GETMBR	R7	R7	K34
      0x88200317,  //  0086  GETMBR	R8	R1	K23
      0x8C241123,  //  0087  GETMET	R9	R8	K35
      0x7C240200,  //  0088  CALL	R9	1
      0x8C240D24,  //  0089  GETMET	R9	R6	K36
      0x5C2C0A00,  //  008A  MOVE	R11	R5
      0x5C301000,  //  008B  MOVE	R12	R8
      0x7C240600,  //  008C  CALL	R9	3
      0x8C240D25,  //  008D  GETMET	R9	R6	K37
      0x7C240200,  //  008E  CALL	R9	1
      0xB8262400,  //  008F  GETNGBL	R9	K18
      0x8C241326,  //  0090  GETMET	R9	R9	K38
      0x542E0003,  //  0091  LDINT	R11	4
      0x7C240400,  //  0092  CALL	R9	2
      0x7826000B,  //  0093  JMPF	R9	#00A0
      0xB8262400,  //  0094  GETNGBL	R9	K18
      0x8C24130B,  //  0095  GETMET	R9	R9	K11
      0x602C0018,  //  0096  GETGBL	R11	G24
      0x58300027,  //  0097  LDCONST	R12	K39
      0x88340D08,  //  0098  GETMBR	R13	R6	K8
      0x88341B20,  //  0099  GETMBR	R13	R13	K32
      0x88380D28,  //  009A  GETMBR	R14	R6	K40
      0x883C0D29,  //  009B  GETMBR	R15	R6	K41
      0x88400D2A,  //  009C  GETMBR	R16	R6	K42
      0x7C2C0A00,  //  009D  CALL	R11	5
      0x54320003,  //  009E  LDINT	R12	4
      0x7C240600,  //  009F  CALL	R9	3
      0x8C240F2B,  //  00A0  GETMET	R9	R7	K43
      0x5C2C0C00,  //  00A1  MOVE	R11	R6
      0x7C240400,  //  00A2  CALL	R9	2
      0xB8260200,  //  00A3  GETNGBL	R9	K1
      0x8824130A,  //  00A4  GETMBR	R9	R9	K10
      0x8C24130B,  //  00A5  GETMET	R9	R9	K11
      0x582C002C,  //  00A6  LDCONST	R11	K44
      0x7C240400,  //  00A7  CALL	R9	2
      0xB8260200,  //  00A8  GETNGBL	R9	K1
      0x8C24132D,  //  00A9  GETMET	R9	R9	K45
      0x882C052E,  //  00AA  GETMBR	R11	R2	K46
      0x8830052F,  //  00AB  GETMBR	R12	R2	K47
      0x7C240600,  //  00AC  CALL	R9	3
      0x78260002,  //  00AD  JMPF	R9	#00B1
      0x002A6009,  //  00AE  ADD	R10	K48	R9
      0x00281531,  //  00AF  ADD	R10	R10	K49
      0x70020000,  //  00B0  JMP		#00B2
      0x58280032,  //  00B1  LDCONST	R10	K50
      0x5C241400,  //  00B2  MOVE	R9	R10
      0x4C280000,  //  00B3  LDNIL	R10
      0x2028080A,  //  00B4  NE	R10	R4	R10
      0x782A0013,  //  00B5  JMPF	R10	#00CA
      0x8C280933,  //  00B6  GETMET	R10	R4	K51
      0x7C280200,  //  00B7  CALL	R10	1
      0xB82E2400,  //  00B8  GETNGBL	R11	K18
      0x8C2C1726,  //  00B9  GETMET	R11	R11	K38
      0x58340014,  //  00BA  LDCONST	R13	K20
      0x7C2C0400,  //  00BB  CALL	R11	2
      0x782E000B,  //  00BC  JMPF	R11	#00C9
      0xB82E2400,  //  00BD  GETNGBL	R11	K18
      0x8C2C170B,  //  00BE  GETMET	R11	R11	K11
      0x60340018,  //  00BF  GETGBL	R13	G24
      0x58380034,  //  00C0  LDCONST	R14	K52
      0x883C0308,  //  00C1  GETMBR	R15	R1	K8
      0x883C1F20,  //  00C2  GETMBR	R15	R15	K32
      0x5C400400,  //  00C3  MOVE	R16	R2
      0x5C441200,  //  00C4  MOVE	R17	R9
      0x5C481400,  //  00C5  MOVE	R18	R10
      0x7C340A00,  //  00C6  CALL	R13	5
      0x58380014,  //  00C7  LDCONST	R14	K20
      0x7C2C0600,  //  00C8  CALL	R11	3
      0x7002002E,  //  00C9  JMP		#00F9
      0x88280500,  //  00CA  GETMBR	R10	R2	K0
      0x4C2C0000,  //  00CB  LDNIL	R11
      0x2028140B,  //  00CC  NE	R10	R10	R11
      0x782A001A,  //  00CD  JMPF	R10	#00E9
      0x88280500,  //  00CE  GETMBR	R10	R2	K0
      0xB82E0200,  //  00CF  GETNGBL	R11	K1
      0x882C1706,  //  00D0  GETMBR	R11	R11	K6
      0x1C28140B,  //  00D1  EQ	R10	R10	R11
      0x782A0001,  //  00D2  JMPF	R10	#00D5
      0x58280006,  //  00D3  LDCONST	R10	K6
      0x70020000,  //  00D4  JMP		#00D6
      0x58280032,  //  00D5  LDCONST	R10	K50
      0xB82E2400,  //  00D6  GETNGBL	R11	K18
      0x8C2C1726,  //  00D7  GETMET	R11	R11	K38
      0x58340014,  //  00D8  LDCONST	R13	K20
      0x7C2C0400,  //  00D9  CALL	R11	2
      0x782E000C,  //  00DA  JMPF	R11	#00E8
      0xB82E2400,  //  00DB  GETNGBL	R11	K18
      0x8C2C170B,  //  00DC  GETMET	R11	R11	K11
      0x60340018,  //  00DD  GETGBL	R13	G24
      0x58380035,  //  00DE  LDCONST	R14	K53
      0x883C0308,  //  00DF  GETMBR	R15	R1	K8
      0x883C1F20,  //  00E0  GETMBR	R15	R15	K32
      0x5C400400,  //  00E1  MOVE	R16	R2
      0x5C441200,  //  00E2  MOVE	R17	R9
      0x88480500,  //  00E3  GETMBR	R18	R2	K0
      0x5C4C1400,  //  00E4  MOVE	R19	R10
      0x7C340C00,  //  00E5  CALL	R13	6
      0x58380014,  //  00E6  LDCONST	R14	K20
      0x7C2C0600,  //  00E7  CALL	R11	3
      0x7002000F,  //  00E8  JMP		#00F9
      0xB82A2400,  //  00E9  GETNGBL	R10	K18
      0x8C281526,  //  00EA  GETMET	R10	R10	K38
      0x58300014,  //  00EB  LDCONST	R12	K20
      0x7C280400,  //  00EC  CALL	R10	2
      0x782A000A,  //  00ED  JMPF	R10	#00F9
      0xB82A2400,  //  00EE  GETNGBL	R10	K18
      0x8C28150B,  //  00EF  GETMET	R10	R10	K11
      0x60300018,  //  00F0  GETGBL	R12	G24
      0x58340036,  //  00F1  LDCONST	R13	K54
      0x88380308,  //  00F2  GETMBR	R14	R1	K8
      0x88381D20,  //  00F3  GETMBR	R14	R14	K32
      0x5C3C0400,  //  00F4  MOVE	R15	R2
      0x5C401200,  //  00F5  MOVE	R16	R9
      0x7C300800,  //  00F6  CALL	R12	4
      0x58340014,  //  00F7  LDCONST	R13	K20
      0x7C280600,  //  00F8  CALL	R10	3
      0x50280200,  //  00F9  LDBOOL	R10	1	0
      0x80041400,  //  00FA  RET	1	R10
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_enqueued
********************************************************************/
be_local_closure(Matter_IM_send_enqueued,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(send_queue),
    /* K2   */  be_nested_str_weak(finish),
    /* K3   */  be_nested_str_weak(ready),
    /* K4   */  be_nested_str_weak(send_im),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20remove_X20IM_X20message_X20exch_X3D),
    /* K8   */  be_nested_str_weak(resp),
    /* K9   */  be_nested_str_weak(exchange_id),
    /* K10  */  be_nested_str_weak(remove),
    /* K11  */  be_const_int(1),
    }),
    be_str_weak(send_enqueued),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E001A,  //  0005  JMPF	R3	#0021
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x88100702,  //  0008  GETMBR	R4	R3	K2
      0x74120004,  //  0009  JMPT	R4	#000F
      0x88100703,  //  000A  GETMBR	R4	R3	K3
      0x78120002,  //  000B  JMPF	R4	#000F
      0x8C100704,  //  000C  GETMET	R4	R3	K4
      0x5C180200,  //  000D  MOVE	R6	R1
      0x7C100400,  //  000E  CALL	R4	2
      0x88100702,  //  000F  GETMBR	R4	R3	K2
      0x7812000D,  //  0010  JMPF	R4	#001F
      0xB8120A00,  //  0011  GETNGBL	R4	K5
      0x8C100906,  //  0012  GETMET	R4	R4	K6
      0x60180008,  //  0013  GETGBL	R6	G8
      0x881C0708,  //  0014  GETMBR	R7	R3	K8
      0x881C0F09,  //  0015  GETMBR	R7	R7	K9
      0x7C180200,  //  0016  CALL	R6	1
      0x001A0E06,  //  0017  ADD	R6	K7	R6
      0x541E0003,  //  0018  LDINT	R7	4
      0x7C100600,  //  0019  CALL	R4	3
      0x88100101,  //  001A  GETMBR	R4	R0	K1
      0x8C10090A,  //  001B  GETMET	R4	R4	K10
      0x5C180400,  //  001C  MOVE	R6	R2
      0x7C100400,  //  001D  CALL	R4	2
      0x70020000,  //  001E  JMP		#0020
      0x0008050B,  //  001F  ADD	R2	R2	K11
      0x7001FFDF,  //  0020  JMP		#0001
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_IM_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(send_queue),
    /* K2   */  be_nested_str_weak(subs_shop),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(IM_Subscription_Shop),
    /* K5   */  be_nested_str_weak(read_request_solo),
    /* K6   */  be_nested_str_weak(ReadRequestMessage_solo),
    /* K7   */  be_nested_str_weak(invoke_request_solo),
    /* K8   */  be_nested_str_weak(InvokeRequestMessage_solo),
    /* K9   */  be_nested_str_weak(tlv_solo),
    /* K10  */  be_nested_str_weak(TLV),
    /* K11  */  be_nested_str_weak(Matter_TLV_item),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x90020202,  //  0003  SETMBR	R0	K1	R2
      0xB80A0600,  //  0004  GETNGBL	R2	K3
      0x8C080504,  //  0005  GETMET	R2	R2	K4
      0x5C100000,  //  0006  MOVE	R4	R0
      0x7C080400,  //  0007  CALL	R2	2
      0x90020402,  //  0008  SETMBR	R0	K2	R2
      0xB80A0600,  //  0009  GETNGBL	R2	K3
      0x8C080506,  //  000A  GETMET	R2	R2	K6
      0x7C080200,  //  000B  CALL	R2	1
      0x90020A02,  //  000C  SETMBR	R0	K5	R2
      0xB80A0600,  //  000D  GETNGBL	R2	K3
      0x8C080508,  //  000E  GETMET	R2	R2	K8
      0x7C080200,  //  000F  CALL	R2	1
      0x90020E02,  //  0010  SETMBR	R0	K7	R2
      0xB80A0600,  //  0011  GETNGBL	R2	K3
      0x8808050A,  //  0012  GETMBR	R2	R2	K10
      0x8C08050B,  //  0013  GETMET	R2	R2	K11
      0x7C080200,  //  0014  CALL	R2	1
      0x90021202,  //  0015  SETMBR	R0	K9	R2
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: expire_sendqueue
********************************************************************/
be_local_closure(Matter_IM_expire_sendqueue,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(send_queue),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(time_reached),
    /* K4   */  be_nested_str_weak(expiration),
    /* K5   */  be_nested_str_weak(reached_timeout),
    /* K6   */  be_nested_str_weak(remove),
    /* K7   */  be_const_int(1),
    }),
    be_str_weak(expire_sendqueue),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x14080202,  //  0004  LT	R2	R1	R2
      0x780A000F,  //  0005  JMPF	R2	#0016
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x94080401,  //  0007  GETIDX	R2	R2	R1
      0xB80E0400,  //  0008  GETNGBL	R3	K2
      0x8C0C0703,  //  0009  GETMET	R3	R3	K3
      0x88140504,  //  000A  GETMBR	R5	R2	K4
      0x7C0C0400,  //  000B  CALL	R3	2
      0x780E0006,  //  000C  JMPF	R3	#0014
      0x8C0C0505,  //  000D  GETMET	R3	R2	K5
      0x7C0C0200,  //  000E  CALL	R3	1
      0x880C0101,  //  000F  GETMBR	R3	R0	K1
      0x8C0C0706,  //  0010  GETMET	R3	R3	K6
      0x5C140200,  //  0011  MOVE	R5	R1
      0x7C0C0400,  //  0012  CALL	R3	2
      0x70020000,  //  0013  JMP		#0015
      0x00040307,  //  0014  ADD	R1	R1	K7
      0x7001FFEA,  //  0015  JMP		#0001
      0x4C080000,  //  0016  LDNIL	R2
      0x80040400,  //  0017  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: subscribe_request
********************************************************************/
be_local_closure(Matter_IM_subscribe_request,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[33]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(SubscribeRequestMessage),
    /* K2   */  be_nested_str_weak(from_TLV),
    /* K3   */  be_nested_str_weak(keep_subscriptions),
    /* K4   */  be_nested_str_weak(subs_shop),
    /* K5   */  be_nested_str_weak(remove_by_session),
    /* K6   */  be_nested_str_weak(session),
    /* K7   */  be_nested_str_weak(new_subscription),
    /* K8   */  be_nested_str_weak(Path),
    /* K9   */  be_nested_str_weak(msg),
    /* K10  */  be_nested_str_weak(attributes_requests),
    /* K11  */  be_nested_str_weak(endpoint),
    /* K12  */  be_nested_str_weak(cluster),
    /* K13  */  be_nested_str_weak(attribute),
    /* K14  */  be_nested_str_weak(push),
    /* K15  */  be_nested_str_weak(stop_iteration),
    /* K16  */  be_nested_str_weak(tasmota),
    /* K17  */  be_nested_str_weak(log),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20_X3ESubscribe_X20_X28_X256i_X29_X20_X25s_X20_X28min_X3D_X25i_X2C_X20max_X3D_X25i_X2C_X20keep_X3D_X25i_X29_X20sub_X3D_X25i_X20fabric_filtered_X3D_X25s),
    /* K19  */  be_nested_str_weak(local_session_id),
    /* K20  */  be_nested_str_weak(concat),
    /* K21  */  be_nested_str_weak(_X20),
    /* K22  */  be_nested_str_weak(min_interval),
    /* K23  */  be_nested_str_weak(max_interval),
    /* K24  */  be_const_int(1),
    /* K25  */  be_const_int(0),
    /* K26  */  be_nested_str_weak(subscription_id),
    /* K27  */  be_nested_str_weak(fabric_filtered),
    /* K28  */  be_const_int(3),
    /* K29  */  be_nested_str_weak(event_requests),
    /* K30  */  be_nested_str_weak(MTR_X3A_X20_X3ESubscribe_X20_X28_X25_X256i_X29_X20event_requests_size_X3D_X25s),
    /* K31  */  be_nested_str_weak(_inner_process_read_request),
    /* K32  */  be_nested_str_weak(send_subscribe_response),
    }),
    be_str_weak(subscribe_request),
    &be_const_str_solidified,
    ( &(const binstruction[99]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x88100703,  //  0006  GETMBR	R4	R3	K3
      0x74120003,  //  0007  JMPT	R4	#000C
      0x88100104,  //  0008  GETMBR	R4	R0	K4
      0x8C100905,  //  0009  GETMET	R4	R4	K5
      0x88180306,  //  000A  GETMBR	R6	R1	K6
      0x7C100400,  //  000B  CALL	R4	2
      0x88100104,  //  000C  GETMBR	R4	R0	K4
      0x8C100907,  //  000D  GETMET	R4	R4	K7
      0x88180306,  //  000E  GETMBR	R6	R1	K6
      0x5C1C0600,  //  000F  MOVE	R7	R3
      0x7C100600,  //  0010  CALL	R4	3
      0x60140012,  //  0011  GETGBL	R5	G18
      0x7C140000,  //  0012  CALL	R5	0
      0xB81A0000,  //  0013  GETNGBL	R6	K0
      0x8C180D08,  //  0014  GETMET	R6	R6	K8
      0x7C180200,  //  0015  CALL	R6	1
      0x901A1201,  //  0016  SETMBR	R6	K9	R1
      0x601C0010,  //  0017  GETGBL	R7	G16
      0x8820070A,  //  0018  GETMBR	R8	R3	K10
      0x7C1C0200,  //  0019  CALL	R7	1
      0xA802000D,  //  001A  EXBLK	0	#0029
      0x5C200E00,  //  001B  MOVE	R8	R7
      0x7C200000,  //  001C  CALL	R8	0
      0x8824110B,  //  001D  GETMBR	R9	R8	K11
      0x901A1609,  //  001E  SETMBR	R6	K11	R9
      0x8824110C,  //  001F  GETMBR	R9	R8	K12
      0x901A1809,  //  0020  SETMBR	R6	K12	R9
      0x8824110D,  //  0021  GETMBR	R9	R8	K13
      0x901A1A09,  //  0022  SETMBR	R6	K13	R9
      0x8C240B0E,  //  0023  GETMET	R9	R5	K14
      0x602C0008,  //  0024  GETGBL	R11	G8
      0x5C300C00,  //  0025  MOVE	R12	R6
      0x7C2C0200,  //  0026  CALL	R11	1
      0x7C240400,  //  0027  CALL	R9	2
      0x7001FFF1,  //  0028  JMP		#001B
      0x581C000F,  //  0029  LDCONST	R7	K15
      0xAC1C0200,  //  002A  CATCH	R7	1	0
      0xB0080000,  //  002B  RAISE	2	R0	R0
      0xB81E2000,  //  002C  GETNGBL	R7	K16
      0x8C1C0F11,  //  002D  GETMET	R7	R7	K17
      0x60240018,  //  002E  GETGBL	R9	G24
      0x58280012,  //  002F  LDCONST	R10	K18
      0x882C0306,  //  0030  GETMBR	R11	R1	K6
      0x882C1713,  //  0031  GETMBR	R11	R11	K19
      0x8C300B14,  //  0032  GETMET	R12	R5	K20
      0x58380015,  //  0033  LDCONST	R14	K21
      0x7C300400,  //  0034  CALL	R12	2
      0x88340916,  //  0035  GETMBR	R13	R4	K22
      0x88380917,  //  0036  GETMBR	R14	R4	K23
      0x883C0703,  //  0037  GETMBR	R15	R3	K3
      0x783E0001,  //  0038  JMPF	R15	#003B
      0x583C0018,  //  0039  LDCONST	R15	K24
      0x70020000,  //  003A  JMP		#003C
      0x583C0019,  //  003B  LDCONST	R15	K25
      0x8840091A,  //  003C  GETMBR	R16	R4	K26
      0x8844071B,  //  003D  GETMBR	R17	R3	K27
      0x7C241000,  //  003E  CALL	R9	8
      0x5828001C,  //  003F  LDCONST	R10	K28
      0x7C1C0600,  //  0040  CALL	R7	3
      0x881C071D,  //  0041  GETMBR	R7	R3	K29
      0x4C200000,  //  0042  LDNIL	R8
      0x201C0E08,  //  0043  NE	R7	R7	R8
      0x781E000E,  //  0044  JMPF	R7	#0054
      0x601C000C,  //  0045  GETGBL	R7	G12
      0x8820071D,  //  0046  GETMBR	R8	R3	K29
      0x7C1C0200,  //  0047  CALL	R7	1
      0x241C0F19,  //  0048  GT	R7	R7	K25
      0x781E0009,  //  0049  JMPF	R7	#0054
      0xB81E2000,  //  004A  GETNGBL	R7	K16
      0x8C1C0F11,  //  004B  GETMET	R7	R7	K17
      0x60240018,  //  004C  GETGBL	R9	G24
      0x5828001E,  //  004D  LDCONST	R10	K30
      0x602C000C,  //  004E  GETGBL	R11	G12
      0x8830071D,  //  004F  GETMBR	R12	R3	K29
      0x7C2C0200,  //  0050  CALL	R11	1
      0x7C240400,  //  0051  CALL	R9	2
      0x5828001C,  //  0052  LDCONST	R10	K28
      0x7C1C0600,  //  0053  CALL	R7	3
      0x8C1C011F,  //  0054  GETMET	R7	R0	K31
      0x88240306,  //  0055  GETMBR	R9	R1	K6
      0x5C280600,  //  0056  MOVE	R10	R3
      0x5C2C0200,  //  0057  MOVE	R11	R1
      0x50300200,  //  0058  LDBOOL	R12	1	0
      0x7C1C0A00,  //  0059  CALL	R7	5
      0x8820091A,  //  005A  GETMBR	R8	R4	K26
      0x901E3408,  //  005B  SETMBR	R7	K26	R8
      0x8C200120,  //  005C  GETMET	R8	R0	K32
      0x5C280200,  //  005D  MOVE	R10	R1
      0x5C2C0E00,  //  005E  MOVE	R11	R7
      0x5C300800,  //  005F  MOVE	R12	R4
      0x7C200800,  //  0060  CALL	R8	4
      0x50200200,  //  0061  LDBOOL	R8	1	0
      0x80041000,  //  0062  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_subscribe_heartbeat
********************************************************************/
be_local_closure(Matter_IM_send_subscribe_heartbeat,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(session),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20_X3CSub_Alive_X20_X28_X256i_X29_X20sub_X3D_X25i),
    /* K4   */  be_nested_str_weak(local_session_id),
    /* K5   */  be_nested_str_weak(subscription_id),
    /* K6   */  be_const_int(3),
    /* K7   */  be_nested_str_weak(is_keep_alive),
    /* K8   */  be_nested_str_weak(matter),
    /* K9   */  be_nested_str_weak(ReportDataMessage),
    /* K10  */  be_nested_str_weak(suppress_response),
    /* K11  */  be_nested_str_weak(IM_SubscribedHeartbeat),
    /* K12  */  be_nested_str_weak(_message_handler),
    /* K13  */  be_nested_str_weak(send_queue),
    /* K14  */  be_nested_str_weak(push),
    /* K15  */  be_nested_str_weak(send_enqueued),
    }),
    be_str_weak(send_subscribe_heartbeat),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x60140018,  //  0003  GETGBL	R5	G24
      0x58180003,  //  0004  LDCONST	R6	K3
      0x881C0504,  //  0005  GETMBR	R7	R2	K4
      0x88200305,  //  0006  GETMBR	R8	R1	K5
      0x7C140600,  //  0007  CALL	R5	3
      0x58180006,  //  0008  LDCONST	R6	K6
      0x7C0C0600,  //  0009  CALL	R3	3
      0x500C0200,  //  000A  LDBOOL	R3	1	0
      0x90060E03,  //  000B  SETMBR	R1	K7	R3
      0xB80E1000,  //  000C  GETNGBL	R3	K8
      0x8C0C0709,  //  000D  GETMET	R3	R3	K9
      0x7C0C0200,  //  000E  CALL	R3	1
      0x50100200,  //  000F  LDBOOL	R4	1	0
      0x900E1404,  //  0010  SETMBR	R3	K10	R4
      0x88100305,  //  0011  GETMBR	R4	R1	K5
      0x900E0A04,  //  0012  SETMBR	R3	K5	R4
      0xB8121000,  //  0013  GETNGBL	R4	K8
      0x8C10090B,  //  0014  GETMET	R4	R4	K11
      0x8818050C,  //  0015  GETMBR	R6	R2	K12
      0x5C1C0400,  //  0016  MOVE	R7	R2
      0x5C200600,  //  0017  MOVE	R8	R3
      0x5C240200,  //  0018  MOVE	R9	R1
      0x7C100A00,  //  0019  CALL	R4	5
      0x8814010D,  //  001A  GETMBR	R5	R0	K13
      0x8C140B0E,  //  001B  GETMET	R5	R5	K14
      0x5C1C0800,  //  001C  MOVE	R7	R4
      0x7C140400,  //  001D  CALL	R5	2
      0x8C14010F,  //  001E  GETMET	R5	R0	K15
      0x881C050C,  //  001F  GETMBR	R7	R2	K12
      0x7C140400,  //  0020  CALL	R5	2
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM
********************************************************************/
be_local_class(Matter_IM,
    6,
    NULL,
    be_nested_map(40,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(every_250ms, -1), be_const_closure(Matter_IM_every_250ms_closure) },
        { be_const_key_weak(process_incoming_ack, -1), be_const_closure(Matter_IM_process_incoming_ack_closure) },
        { be_const_key_weak(send_ack_now, -1), be_const_closure(Matter_IM_send_ack_now_closure) },
        { be_const_key_weak(subscribe_response, -1), be_const_closure(Matter_IM_subscribe_response_closure) },
        { be_const_key_weak(process_write_response, -1), be_const_closure(Matter_IM_process_write_response_closure) },
        { be_const_key_weak(process_status_response, -1), be_const_closure(Matter_IM_process_status_response_closure) },
        { be_const_key_weak(send_subscribe_heartbeat, -1), be_const_closure(Matter_IM_send_subscribe_heartbeat_closure) },
        { be_const_key_weak(attributedata2raw, 12), be_const_closure(Matter_IM_attributedata2raw_closure) },
        { be_const_key_weak(subscribe_request, 37), be_const_closure(Matter_IM_subscribe_request_closure) },
        { be_const_key_weak(send_subscribe_update, -1), be_const_closure(Matter_IM_send_subscribe_update_closure) },
        { be_const_key_weak(invokeresponse2raw, 2), be_const_closure(Matter_IM_invokeresponse2raw_closure) },
        { be_const_key_weak(read_request_solo, 38), be_const_var(3) },
        { be_const_key_weak(process_incoming, -1), be_const_closure(Matter_IM_process_incoming_closure) },
        { be_const_key_weak(process_invoke_request_solo, 8), be_const_closure(Matter_IM_process_invoke_request_solo_closure) },
        { be_const_key_weak(send_invoke_response, -1), be_const_closure(Matter_IM_send_invoke_response_closure) },
        { be_const_key_weak(send_write_response, 6), be_const_closure(Matter_IM_send_write_response_closure) },
        { be_const_key_weak(process_write_request, -1), be_const_closure(Matter_IM_process_write_request_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_IM_every_second_closure) },
        { be_const_key_weak(path2raw, -1), be_const_closure(Matter_IM_path2raw_closure) },
        { be_const_key_weak(invoke_request_solo, -1), be_const_var(4) },
        { be_const_key_weak(_inner_process_read_request, -1), be_const_closure(Matter_IM__inner_process_read_request_closure) },
        { be_const_key_weak(tlv_solo, -1), be_const_var(5) },
        { be_const_key_weak(remove_sendqueue_by_exchangeid, -1), be_const_closure(Matter_IM_remove_sendqueue_by_exchangeid_closure) },
        { be_const_key_weak(find_sendqueue_by_exchangeid, -1), be_const_closure(Matter_IM_find_sendqueue_by_exchangeid_closure) },
        { be_const_key_weak(send_report_data, -1), be_const_closure(Matter_IM_send_report_data_closure) },
        { be_const_key_weak(process_invoke_request, 17), be_const_closure(Matter_IM_process_invoke_request_closure) },
        { be_const_key_weak(send_status, -1), be_const_closure(Matter_IM_send_status_closure) },
        { be_const_key_weak(subs_shop, 20), be_const_var(1) },
        { be_const_key_weak(process_timed_request, 32), be_const_closure(Matter_IM_process_timed_request_closure) },
        { be_const_key_weak(attributestatus2raw, 23), be_const_closure(Matter_IM_attributestatus2raw_closure) },
        { be_const_key_weak(send_subscribe_response, -1), be_const_closure(Matter_IM_send_subscribe_response_closure) },
        { be_const_key_weak(process_invoke_response, 24), be_const_closure(Matter_IM_process_invoke_response_closure) },
        { be_const_key_weak(report_data, 9), be_const_closure(Matter_IM_report_data_closure) },
        { be_const_key_weak(process_read_request_solo, -1), be_const_closure(Matter_IM_process_read_request_solo_closure) },
        { be_const_key_weak(send_enqueued, -1), be_const_closure(Matter_IM_send_enqueued_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_IM_init_closure) },
        { be_const_key_weak(expire_sendqueue, -1), be_const_closure(Matter_IM_expire_sendqueue_closure) },
        { be_const_key_weak(process_read_request, -1), be_const_closure(Matter_IM_process_read_request_closure) },
        { be_const_key_weak(device, -1), be_const_var(0) },
        { be_const_key_weak(send_queue, -1), be_const_var(2) },
    })),
    be_str_weak(Matter_IM)
);
/*******************************************************************/

void be_load_Matter_IM_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_IM);
    be_setglobal(vm, "Matter_IM");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
