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
    9,                          /* nstack */
    4,                          /* argc */
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
    ( &(const binstruction[22]) {  /* code */
      0x8C100300,  //  0000  GETMET	R4	R1	K0
      0x58180001,  //  0001  LDCONST	R6	K1
      0x541DFFFB,  //  0002  LDINT	R7	-4
      0x7C100600,  //  0003  CALL	R4	3
      0x8C100300,  //  0004  GETMET	R4	R1	K0
      0x58180002,  //  0005  LDCONST	R6	K2
      0x541DFFFD,  //  0006  LDINT	R7	-2
      0x7C100600,  //  0007  CALL	R4	3
      0x8C100103,  //  0008  GETMET	R4	R0	K3
      0x5C180200,  //  0009  MOVE	R6	R1
      0x5C1C0400,  //  000A  MOVE	R7	R2
      0x58200002,  //  000B  LDCONST	R8	K2
      0x7C100800,  //  000C  CALL	R4	4
      0x900E0905,  //  000D  SETMBR	R3	K4	K5
      0x8C100706,  //  000E  GETMET	R4	R3	K6
      0x5C180200,  //  000F  MOVE	R6	R1
      0x7C100400,  //  0010  CALL	R4	2
      0x8C100300,  //  0011  GETMET	R4	R1	K0
      0x541A1817,  //  0012  LDINT	R6	6168
      0x541DFFFD,  //  0013  LDINT	R7	-2
      0x7C100600,  //  0014  CALL	R4	3
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_invoke_request_solo
********************************************************************/
be_local_closure(Matter_IM_process_invoke_request_solo,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[45]) {     /* constants */
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
    /* K18  */  be_nested_str_weak(endpoint),
    /* K19  */  be_const_int(0),
    /* K20  */  be_const_int(2),
    /* K21  */  be_const_int(3),
    /* K22  */  be_nested_str_weak(tasmota),
    /* K23  */  be_nested_str_weak(loglevel),
    /* K24  */  be_nested_str_weak(MTR_X3A_X20_X3ECommand1_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s_X20_X25s),
    /* K25  */  be_nested_str_weak(local_session_id),
    /* K26  */  be_nested_str_weak(add),
    /* K27  */  be_const_int(354943030),
    /* K28  */  be_const_int(1),
    /* K29  */  be_nested_str_weak(SUCCESS),
    /* K30  */  be_nested_str_weak(invokeresponse2raw),
    /* K31  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20OK_X20exch_X3D_X25i),
    /* K32  */  be_nested_str_weak(exchange_id),
    /* K33  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s),
    /* K34  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20Status_X3D0x_X2502X_X20exch_X3D_X25i),
    /* K35  */  be_nested_str_weak(MTR_X3A_X20_Ignore_X20_X20_X20_X20_X28_X256i_X29_X20exch_X3D_X25i),
    /* K36  */  be_const_int(405077761),
    /* K37  */  be_nested_str_weak(build_response),
    /* K38  */  be_nested_str_weak(message_handler),
    /* K39  */  be_nested_str_weak(raw),
    /* K40  */  be_nested_str_weak(clear),
    /* K41  */  be_nested_str_weak(encode_frame),
    /* K42  */  be_nested_str_weak(encrypt),
    /* K43  */  be_nested_str_weak(send_response_frame),
    /* K44  */  be_nested_str_weak(RESPONSE_X20SENT),
    }),
    be_str_weak(process_invoke_request_solo),
    &be_const_str_solidified,
    ( &(const binstruction[214]) {  /* code */
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
      0x881C0512,  //  0027  GETMBR	R7	R2	K18
      0x201C0F13,  //  0028  NE	R7	R7	K19
      0x781E0001,  //  0029  JMPF	R7	#002C
      0x581C0014,  //  002A  LDCONST	R7	K20
      0x70020000,  //  002B  JMP		#002D
      0x581C0015,  //  002C  LDCONST	R7	K21
      0xB8222C00,  //  002D  GETNGBL	R8	K22
      0x8C201117,  //  002E  GETMET	R8	R8	K23
      0x5C280E00,  //  002F  MOVE	R10	R7
      0x7C200400,  //  0030  CALL	R8	2
      0x7822000E,  //  0031  JMPF	R8	#0041
      0xB8222C00,  //  0032  GETNGBL	R8	K22
      0x8C201102,  //  0033  GETMET	R8	R8	K2
      0x60280018,  //  0034  GETGBL	R10	G24
      0x582C0018,  //  0035  LDCONST	R11	K24
      0x8830030C,  //  0036  GETMBR	R12	R1	K12
      0x88301919,  //  0037  GETMBR	R12	R12	K25
      0x5C340800,  //  0038  MOVE	R13	R4
      0x780E0001,  //  0039  JMPF	R3	#003C
      0x5C380600,  //  003A  MOVE	R14	R3
      0x70020000,  //  003B  JMP		#003D
      0x58380011,  //  003C  LDCONST	R14	K17
      0x5C3C0C00,  //  003D  MOVE	R15	R6
      0x7C280A00,  //  003E  CALL	R10	5
      0x5C2C0E00,  //  003F  MOVE	R11	R7
      0x7C200600,  //  0040  CALL	R8	3
      0x4C200000,  //  0041  LDNIL	R8
      0x900A0408,  //  0042  SETMBR	R2	K2	R8
      0x60200015,  //  0043  GETGBL	R8	G21
      0x5426002F,  //  0044  LDINT	R9	48
      0x7C200200,  //  0045  CALL	R8	1
      0x8C24111A,  //  0046  GETMET	R9	R8	K26
      0x582C001B,  //  0047  LDCONST	R11	K27
      0x5431FFFB,  //  0048  LDINT	R12	-4
      0x7C240600,  //  0049  CALL	R9	3
      0x8C24111A,  //  004A  GETMET	R9	R8	K26
      0x582C001C,  //  004B  LDCONST	R11	K28
      0x5830001C,  //  004C  LDCONST	R12	K28
      0x7C240600,  //  004D  CALL	R9	3
      0x50240200,  //  004E  LDBOOL	R9	1	0
      0x1C240A09,  //  004F  EQ	R9	R5	R9
      0x74260004,  //  0050  JMPT	R9	#0056
      0x88240505,  //  0051  GETMBR	R9	R2	K5
      0xB82A0000,  //  0052  GETNGBL	R10	K0
      0x8828151D,  //  0053  GETMBR	R10	R10	K29
      0x1C24120A,  //  0054  EQ	R9	R9	R10
      0x78260017,  //  0055  JMPF	R9	#006E
      0xB8260000,  //  0056  GETNGBL	R9	K0
      0x8824131D,  //  0057  GETMBR	R9	R9	K29
      0x900A0A09,  //  0058  SETMBR	R2	K5	R9
      0x8C24011E,  //  0059  GETMET	R9	R0	K30
      0x5C2C1000,  //  005A  MOVE	R11	R8
      0x5C300400,  //  005B  MOVE	R12	R2
      0x4C340000,  //  005C  LDNIL	R13
      0x7C240800,  //  005D  CALL	R9	4
      0xB8262C00,  //  005E  GETNGBL	R9	K22
      0x8C241317,  //  005F  GETMET	R9	R9	K23
      0x582C0015,  //  0060  LDCONST	R11	K21
      0x7C240400,  //  0061  CALL	R9	2
      0x78260009,  //  0062  JMPF	R9	#006D
      0xB8262C00,  //  0063  GETNGBL	R9	K22
      0x8C241302,  //  0064  GETMET	R9	R9	K2
      0x602C0018,  //  0065  GETGBL	R11	G24
      0x5830001F,  //  0066  LDCONST	R12	K31
      0x8834030C,  //  0067  GETMBR	R13	R1	K12
      0x88341B19,  //  0068  GETMBR	R13	R13	K25
      0x88380320,  //  0069  GETMBR	R14	R1	K32
      0x7C2C0600,  //  006A  CALL	R11	3
      0x58300015,  //  006B  LDCONST	R12	K21
      0x7C240600,  //  006C  CALL	R9	3
      0x70020046,  //  006D  JMP		#00B5
      0x4C240000,  //  006E  LDNIL	R9
      0x20240A09,  //  006F  NE	R9	R5	R9
      0x78260018,  //  0070  JMPF	R9	#008A
      0x8C24011E,  //  0071  GETMET	R9	R0	K30
      0x5C2C1000,  //  0072  MOVE	R11	R8
      0x5C300400,  //  0073  MOVE	R12	R2
      0x5C340A00,  //  0074  MOVE	R13	R5
      0x7C240800,  //  0075  CALL	R9	4
      0x5C240600,  //  0076  MOVE	R9	R3
      0x74260000,  //  0077  JMPT	R9	#0079
      0x580C0011,  //  0078  LDCONST	R3	K17
      0xB8262C00,  //  0079  GETNGBL	R9	K22
      0x8C241317,  //  007A  GETMET	R9	R9	K23
      0x582C0015,  //  007B  LDCONST	R11	K21
      0x7C240400,  //  007C  CALL	R9	2
      0x7826000A,  //  007D  JMPF	R9	#0089
      0xB8262C00,  //  007E  GETNGBL	R9	K22
      0x8C241302,  //  007F  GETMET	R9	R9	K2
      0x602C0018,  //  0080  GETGBL	R11	G24
      0x58300021,  //  0081  LDCONST	R12	K33
      0x8834030C,  //  0082  GETMBR	R13	R1	K12
      0x88341B19,  //  0083  GETMBR	R13	R13	K25
      0x5C380400,  //  0084  MOVE	R14	R2
      0x5C3C0600,  //  0085  MOVE	R15	R3
      0x7C2C0800,  //  0086  CALL	R11	4
      0x58300015,  //  0087  LDCONST	R12	K21
      0x7C240600,  //  0088  CALL	R9	3
      0x7002002A,  //  0089  JMP		#00B5
      0x88240505,  //  008A  GETMBR	R9	R2	K5
      0x4C280000,  //  008B  LDNIL	R10
      0x2024120A,  //  008C  NE	R9	R9	R10
      0x78260015,  //  008D  JMPF	R9	#00A4
      0x8C24011E,  //  008E  GETMET	R9	R0	K30
      0x5C2C1000,  //  008F  MOVE	R11	R8
      0x5C300400,  //  0090  MOVE	R12	R2
      0x4C340000,  //  0091  LDNIL	R13
      0x7C240800,  //  0092  CALL	R9	4
      0xB8262C00,  //  0093  GETNGBL	R9	K22
      0x8C241317,  //  0094  GETMET	R9	R9	K23
      0x582C0015,  //  0095  LDCONST	R11	K21
      0x7C240400,  //  0096  CALL	R9	2
      0x7826000A,  //  0097  JMPF	R9	#00A3
      0xB8262C00,  //  0098  GETNGBL	R9	K22
      0x8C241302,  //  0099  GETMET	R9	R9	K2
      0x602C0018,  //  009A  GETGBL	R11	G24
      0x58300022,  //  009B  LDCONST	R12	K34
      0x8834030C,  //  009C  GETMBR	R13	R1	K12
      0x88341B19,  //  009D  GETMBR	R13	R13	K25
      0x88380505,  //  009E  GETMBR	R14	R2	K5
      0x883C0320,  //  009F  GETMBR	R15	R1	K32
      0x7C2C0800,  //  00A0  CALL	R11	4
      0x58300015,  //  00A1  LDCONST	R12	K21
      0x7C240600,  //  00A2  CALL	R9	3
      0x70020010,  //  00A3  JMP		#00B5
      0xB8262C00,  //  00A4  GETNGBL	R9	K22
      0x8C241317,  //  00A5  GETMET	R9	R9	K23
      0x582C0015,  //  00A6  LDCONST	R11	K21
      0x7C240400,  //  00A7  CALL	R9	2
      0x78260009,  //  00A8  JMPF	R9	#00B3
      0xB8262C00,  //  00A9  GETNGBL	R9	K22
      0x8C241302,  //  00AA  GETMET	R9	R9	K2
      0x602C0018,  //  00AB  GETGBL	R11	G24
      0x58300023,  //  00AC  LDCONST	R12	K35
      0x8834030C,  //  00AD  GETMBR	R13	R1	K12
      0x88341B19,  //  00AE  GETMBR	R13	R13	K25
      0x88380320,  //  00AF  GETMBR	R14	R1	K32
      0x7C2C0600,  //  00B0  CALL	R11	3
      0x58300015,  //  00B1  LDCONST	R12	K21
      0x7C240600,  //  00B2  CALL	R9	3
      0x50240000,  //  00B3  LDBOOL	R9	0	0
      0x80041200,  //  00B4  RET	1	R9
      0x8C24111A,  //  00B5  GETMET	R9	R8	K26
      0x582C0024,  //  00B6  LDCONST	R11	K36
      0x5431FFFB,  //  00B7  LDINT	R12	-4
      0x7C240600,  //  00B8  CALL	R9	3
      0x8C24111A,  //  00B9  GETMET	R9	R8	K26
      0x542E0017,  //  00BA  LDINT	R11	24
      0x5830001C,  //  00BB  LDCONST	R12	K28
      0x7C240600,  //  00BC  CALL	R9	3
      0x8C240325,  //  00BD  GETMET	R9	R1	K37
      0x542E0008,  //  00BE  LDINT	R11	9
      0x50300200,  //  00BF  LDBOOL	R12	1	0
      0x7C240600,  //  00C0  CALL	R9	3
      0x8828010A,  //  00C1  GETMBR	R10	R0	K10
      0x88281526,  //  00C2  GETMBR	R10	R10	K38
      0x882C0327,  //  00C3  GETMBR	R11	R1	K39
      0x8C301728,  //  00C4  GETMET	R12	R11	K40
      0x7C300200,  //  00C5  CALL	R12	1
      0x8C301329,  //  00C6  GETMET	R12	R9	K41
      0x5C381000,  //  00C7  MOVE	R14	R8
      0x5C3C1600,  //  00C8  MOVE	R15	R11
      0x7C300600,  //  00C9  CALL	R12	3
      0x8C30132A,  //  00CA  GETMET	R12	R9	K42
      0x7C300200,  //  00CB  CALL	R12	1
      0x8C30152B,  //  00CC  GETMET	R12	R10	K43
      0x5C381200,  //  00CD  MOVE	R14	R9
      0x7C300400,  //  00CE  CALL	R12	2
      0xB8320000,  //  00CF  GETNGBL	R12	K0
      0x88301901,  //  00D0  GETMBR	R12	R12	K1
      0x8C301902,  //  00D1  GETMET	R12	R12	K2
      0x5838002C,  //  00D2  LDCONST	R14	K44
      0x7C300400,  //  00D3  CALL	R12	2
      0x50300200,  //  00D4  LDBOOL	R12	1	0
      0x80041800,  //  00D5  RET	1	R12
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
    ( &(const bvalue[44]) {     /* constants */
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
    /* K30  */  be_const_int(0),
    /* K31  */  be_const_int(2),
    /* K32  */  be_const_int(3),
    /* K33  */  be_nested_str_weak(SUCCESS),
    /* K34  */  be_nested_str_weak(invokeresponse2raw),
    /* K35  */  be_nested_str_weak(push),
    /* K36  */  be_nested_str_weak(loglevel),
    /* K37  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20OK_X20exch_X3D_X25i),
    /* K38  */  be_nested_str_weak(exchange_id),
    /* K39  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s),
    /* K40  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20Status_X3D0x_X2502X_X20exch_X3D_X25i),
    /* K41  */  be_nested_str_weak(MTR_X3A_X20_Ignore_X20_X20_X20_X20_X28_X256i_X29_X20exch_X3D_X25i),
    /* K42  */  be_nested_str_weak(stop_iteration),
    /* K43  */  be_nested_str_weak(send_invoke_response),
    }),
    be_str_weak(process_invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[238]) {  /* code */
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
      0x781600D9,  //  0012  JMPF	R5	#00ED
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
      0xA80200BC,  //  001E  EXBLK	0	#00DC
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
      0x883C070C,  //  0058  GETMBR	R15	R3	K12
      0x203C1F1E,  //  0059  NE	R15	R15	K30
      0x783E0001,  //  005A  JMPF	R15	#005D
      0x583C001F,  //  005B  LDCONST	R15	K31
      0x70020000,  //  005C  JMP		#005E
      0x583C0020,  //  005D  LDCONST	R15	K32
      0x7C300600,  //  005E  CALL	R12	3
      0x4C300000,  //  005F  LDNIL	R12
      0x900E040C,  //  0060  SETMBR	R3	K2	R12
      0x60300015,  //  0061  GETGBL	R12	G21
      0x5436001F,  //  0062  LDINT	R13	32
      0x7C300200,  //  0063  CALL	R12	1
      0x50340200,  //  0064  LDBOOL	R13	1	0
      0x1C34140D,  //  0065  EQ	R13	R10	R13
      0x74360004,  //  0066  JMPT	R13	#006C
      0x88340710,  //  0067  GETMBR	R13	R3	K16
      0xB83A0000,  //  0068  GETNGBL	R14	K0
      0x88381D21,  //  0069  GETMBR	R14	R14	K33
      0x1C341A0E,  //  006A  EQ	R13	R13	R14
      0x7836001B,  //  006B  JMPF	R13	#0088
      0xB8360000,  //  006C  GETNGBL	R13	K0
      0x88341B21,  //  006D  GETMBR	R13	R13	K33
      0x900E200D,  //  006E  SETMBR	R3	K16	R13
      0x8C340122,  //  006F  GETMET	R13	R0	K34
      0x5C3C1800,  //  0070  MOVE	R15	R12
      0x5C400600,  //  0071  MOVE	R16	R3
      0x4C440000,  //  0072  LDNIL	R17
      0x7C340800,  //  0073  CALL	R13	4
      0x88340B0B,  //  0074  GETMBR	R13	R5	K11
      0x8C341B23,  //  0075  GETMET	R13	R13	K35
      0x5C3C1800,  //  0076  MOVE	R15	R12
      0x7C340400,  //  0077  CALL	R13	2
      0xB8363600,  //  0078  GETNGBL	R13	K27
      0x8C341B24,  //  0079  GETMET	R13	R13	K36
      0x583C0020,  //  007A  LDCONST	R15	K32
      0x7C340400,  //  007B  CALL	R13	2
      0x78360009,  //  007C  JMPF	R13	#0087
      0xB8363600,  //  007D  GETNGBL	R13	K27
      0x8C341B02,  //  007E  GETMET	R13	R13	K2
      0x603C0018,  //  007F  GETGBL	R15	G24
      0x58400025,  //  0080  LDCONST	R16	K37
      0x88440315,  //  0081  GETMBR	R17	R1	K21
      0x8844231D,  //  0082  GETMBR	R17	R17	K29
      0x88480326,  //  0083  GETMBR	R18	R1	K38
      0x7C3C0600,  //  0084  CALL	R15	3
      0x58400020,  //  0085  LDCONST	R16	K32
      0x7C340600,  //  0086  CALL	R13	3
      0x70020052,  //  0087  JMP		#00DB
      0x4C340000,  //  0088  LDNIL	R13
      0x2034140D,  //  0089  NE	R13	R10	R13
      0x78360022,  //  008A  JMPF	R13	#00AE
      0x8C340122,  //  008B  GETMET	R13	R0	K34
      0x5C3C1800,  //  008C  MOVE	R15	R12
      0x5C400600,  //  008D  MOVE	R16	R3
      0x5C441400,  //  008E  MOVE	R17	R10
      0x7C340800,  //  008F  CALL	R13	4
      0x88340B0B,  //  0090  GETMBR	R13	R5	K11
      0x8C341B23,  //  0091  GETMET	R13	R13	K35
      0x5C3C1800,  //  0092  MOVE	R15	R12
      0x7C340400,  //  0093  CALL	R13	2
      0xB8360000,  //  0094  GETNGBL	R13	K0
      0x8C341B12,  //  0095  GETMET	R13	R13	K18
      0x883C070E,  //  0096  GETMBR	R15	R3	K14
      0x8840070F,  //  0097  GETMBR	R16	R3	K15
      0x7C340600,  //  0098  CALL	R13	3
      0x5C201A00,  //  0099  MOVE	R8	R13
      0x5C341000,  //  009A  MOVE	R13	R8
      0x74360000,  //  009B  JMPT	R13	#009D
      0x5820001A,  //  009C  LDCONST	R8	K26
      0xB8363600,  //  009D  GETNGBL	R13	K27
      0x8C341B24,  //  009E  GETMET	R13	R13	K36
      0x583C0020,  //  009F  LDCONST	R15	K32
      0x7C340400,  //  00A0  CALL	R13	2
      0x7836000A,  //  00A1  JMPF	R13	#00AD
      0xB8363600,  //  00A2  GETNGBL	R13	K27
      0x8C341B02,  //  00A3  GETMET	R13	R13	K2
      0x603C0018,  //  00A4  GETGBL	R15	G24
      0x58400027,  //  00A5  LDCONST	R16	K39
      0x88440315,  //  00A6  GETMBR	R17	R1	K21
      0x8844231D,  //  00A7  GETMBR	R17	R17	K29
      0x5C480600,  //  00A8  MOVE	R18	R3
      0x5C4C1000,  //  00A9  MOVE	R19	R8
      0x7C3C0800,  //  00AA  CALL	R15	4
      0x58400020,  //  00AB  LDCONST	R16	K32
      0x7C340600,  //  00AC  CALL	R13	3
      0x7002002C,  //  00AD  JMP		#00DB
      0x88340710,  //  00AE  GETMBR	R13	R3	K16
      0x4C380000,  //  00AF  LDNIL	R14
      0x20341A0E,  //  00B0  NE	R13	R13	R14
      0x78360019,  //  00B1  JMPF	R13	#00CC
      0x8C340122,  //  00B2  GETMET	R13	R0	K34
      0x5C3C1800,  //  00B3  MOVE	R15	R12
      0x5C400600,  //  00B4  MOVE	R16	R3
      0x4C440000,  //  00B5  LDNIL	R17
      0x7C340800,  //  00B6  CALL	R13	4
      0x88340B0B,  //  00B7  GETMBR	R13	R5	K11
      0x8C341B23,  //  00B8  GETMET	R13	R13	K35
      0x5C3C1800,  //  00B9  MOVE	R15	R12
      0x7C340400,  //  00BA  CALL	R13	2
      0xB8363600,  //  00BB  GETNGBL	R13	K27
      0x8C341B24,  //  00BC  GETMET	R13	R13	K36
      0x583C0020,  //  00BD  LDCONST	R15	K32
      0x7C340400,  //  00BE  CALL	R13	2
      0x7836000A,  //  00BF  JMPF	R13	#00CB
      0xB8363600,  //  00C0  GETNGBL	R13	K27
      0x8C341B02,  //  00C1  GETMET	R13	R13	K2
      0x603C0018,  //  00C2  GETGBL	R15	G24
      0x58400028,  //  00C3  LDCONST	R16	K40
      0x88440315,  //  00C4  GETMBR	R17	R1	K21
      0x8844231D,  //  00C5  GETMBR	R17	R17	K29
      0x88480710,  //  00C6  GETMBR	R18	R3	K16
      0x884C0326,  //  00C7  GETMBR	R19	R1	K38
      0x7C3C0800,  //  00C8  CALL	R15	4
      0x58400020,  //  00C9  LDCONST	R16	K32
      0x7C340600,  //  00CA  CALL	R13	3
      0x7002000E,  //  00CB  JMP		#00DB
      0xB8363600,  //  00CC  GETNGBL	R13	K27
      0x8C341B24,  //  00CD  GETMET	R13	R13	K36
      0x583C0020,  //  00CE  LDCONST	R15	K32
      0x7C340400,  //  00CF  CALL	R13	2
      0x78360009,  //  00D0  JMPF	R13	#00DB
      0xB8363600,  //  00D1  GETNGBL	R13	K27
      0x8C341B02,  //  00D2  GETMET	R13	R13	K2
      0x603C0018,  //  00D3  GETGBL	R15	G24
      0x58400029,  //  00D4  LDCONST	R16	K41
      0x88440315,  //  00D5  GETMBR	R17	R1	K21
      0x8844231D,  //  00D6  GETMBR	R17	R17	K29
      0x88480326,  //  00D7  GETMBR	R18	R1	K38
      0x7C3C0600,  //  00D8  CALL	R15	3
      0x58400020,  //  00D9  LDCONST	R16	K32
      0x7C340600,  //  00DA  CALL	R13	3
      0x7001FF42,  //  00DB  JMP		#001F
      0x5818002A,  //  00DC  LDCONST	R6	K42
      0xAC180200,  //  00DD  CATCH	R6	1	0
      0xB0080000,  //  00DE  RAISE	2	R0	R0
      0x6018000C,  //  00DF  GETGBL	R6	G12
      0x881C0B0B,  //  00E0  GETMBR	R7	R5	K11
      0x7C180200,  //  00E1  CALL	R6	1
      0x24180D1E,  //  00E2  GT	R6	R6	K30
      0x781A0004,  //  00E3  JMPF	R6	#00E9
      0x8C18012B,  //  00E4  GETMET	R6	R0	K43
      0x5C200200,  //  00E5  MOVE	R8	R1
      0x5C240A00,  //  00E6  MOVE	R9	R5
      0x7C180600,  //  00E7  CALL	R6	3
      0x70020001,  //  00E8  JMP		#00EB
      0x50180000,  //  00E9  LDBOOL	R6	0	0
      0x80040C00,  //  00EA  RET	1	R6
      0x50180200,  //  00EB  LDBOOL	R6	1	0
      0x80040C00,  //  00EC  RET	1	R6
      0x80000000,  //  00ED  RET	0
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
    ( &(const binstruction[25]) {  /* code */
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
      0x78120007,  //  000E  JMPF	R4	#0017
      0x8C100107,  //  000F  GETMET	R4	R0	K7
      0x88180308,  //  0010  GETMBR	R6	R1	K8
      0x5C1C0600,  //  0011  MOVE	R7	R3
      0x7C100600,  //  0012  CALL	R4	3
      0x8C140109,  //  0013  GETMET	R5	R0	K9
      0x5C1C0200,  //  0014  MOVE	R7	R1
      0x5C200800,  //  0015  MOVE	R8	R4
      0x7C140600,  //  0016  CALL	R5	3
      0x50100200,  //  0017  LDBOOL	R4	1	0
      0x80040800,  //  0018  RET	1	R4
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
    ( &(const bvalue[29]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(WriteRequestMessage),
    /* K2   */  be_nested_str_weak(from_TLV),
    /* K3   */  be_nested_str_weak(suppress_response),
    /* K4   */  be_nested_str_weak(device),
    /* K5   */  be_nested_str_weak(get_active_endpoints),
    /* K6   */  be_nested_str_weak(Path),
    /* K7   */  be_nested_str_weak(write_requests),
    /* K8   */  be_nested_str_weak(WriteResponseMessage),
    /* K9   */  be_nested_str_weak(write_responses),
    /* K10  */  be_nested_str_weak(path),
    /* K11  */  be_nested_str_weak(data),
    /* K12  */  be_nested_str_weak(endpoint),
    /* K13  */  be_nested_str_weak(cluster),
    /* K14  */  be_nested_str_weak(attribute),
    /* K15  */  be_nested_str_weak(status),
    /* K16  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K17  */  be_nested_str_weak(INVALID_ACTION),
    /* K18  */  be_nested_str_weak(get_attribute_name),
    /* K19  */  be_nested_str_weak(tasmota),
    /* K20  */  be_nested_str_weak(log),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20Write_Attr_X20),
    /* K22  */  be_nested_str_weak(_X20_X28),
    /* K23  */  be_nested_str_weak(_X29),
    /* K24  */  be_nested_str_weak(),
    /* K25  */  be_const_int(3),
    /* K26  */  be_nested_str_weak(process_attribute_expansion),
    /* K27  */  be_nested_str_weak(stop_iteration),
    /* K28  */  be_nested_str_weak(send_write_response),
    }),
    be_str_weak(process_write_request),
    &be_const_str_solidified,
    ( &(const binstruction[103]) {  /* code */
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
      0x88200707,  //  000E  GETMBR	R8	R3	K7
      0x4C240000,  //  000F  LDNIL	R9
      0x20201009,  //  0010  NE	R8	R8	R9
      0x78220051,  //  0011  JMPF	R8	#0064
      0xB8220000,  //  0012  GETNGBL	R8	K0
      0x8C201108,  //  0013  GETMET	R8	R8	K8
      0x7C200200,  //  0014  CALL	R8	1
      0x60240012,  //  0015  GETGBL	R9	G18
      0x7C240000,  //  0016  CALL	R9	0
      0x90221209,  //  0017  SETMBR	R8	K9	R9
      0x60240010,  //  0018  GETGBL	R9	G16
      0x88280707,  //  0019  GETMBR	R10	R3	K7
      0x7C240200,  //  001A  CALL	R9	1
      0xA802003D,  //  001B  EXBLK	0	#005A
      0x5C281200,  //  001C  MOVE	R10	R9
      0x7C280000,  //  001D  CALL	R10	0
      0x882C150A,  //  001E  GETMBR	R11	R10	K10
      0x8830150B,  //  001F  GETMBR	R12	R10	K11
      0x8834170C,  //  0020  GETMBR	R13	R11	K12
      0x901E180D,  //  0021  SETMBR	R7	K12	R13
      0x8834170D,  //  0022  GETMBR	R13	R11	K13
      0x901E1A0D,  //  0023  SETMBR	R7	K13	R13
      0x8834170E,  //  0024  GETMBR	R13	R11	K14
      0x901E1C0D,  //  0025  SETMBR	R7	K14	R13
      0xB8360000,  //  0026  GETNGBL	R13	K0
      0x88341B10,  //  0027  GETMBR	R13	R13	K16
      0x901E1E0D,  //  0028  SETMBR	R7	K15	R13
      0x88340F0D,  //  0029  GETMBR	R13	R7	K13
      0x4C380000,  //  002A  LDNIL	R14
      0x1C341A0E,  //  002B  EQ	R13	R13	R14
      0x74360003,  //  002C  JMPT	R13	#0031
      0x88340F0E,  //  002D  GETMBR	R13	R7	K14
      0x4C380000,  //  002E  LDNIL	R14
      0x1C341A0E,  //  002F  EQ	R13	R13	R14
      0x7836000A,  //  0030  JMPF	R13	#003C
      0xB8360000,  //  0031  GETNGBL	R13	K0
      0x88341B11,  //  0032  GETMBR	R13	R13	K17
      0x901E1E0D,  //  0033  SETMBR	R7	K15	R13
      0x5C340C00,  //  0034  MOVE	R13	R6
      0x5C381000,  //  0035  MOVE	R14	R8
      0x4C3C0000,  //  0036  LDNIL	R15
      0x5C400E00,  //  0037  MOVE	R16	R7
      0x4C440000,  //  0038  LDNIL	R17
      0x50480200,  //  0039  LDBOOL	R18	1	0
      0x7C340A00,  //  003A  CALL	R13	5
      0x7001FFDF,  //  003B  JMP		#001C
      0x88340F0C,  //  003C  GETMBR	R13	R7	K12
      0x4C380000,  //  003D  LDNIL	R14
      0x1C341A0E,  //  003E  EQ	R13	R13	R14
      0x78360012,  //  003F  JMPF	R13	#0053
      0xB8360000,  //  0040  GETNGBL	R13	K0
      0x8C341B12,  //  0041  GETMET	R13	R13	K18
      0x883C0F0D,  //  0042  GETMBR	R15	R7	K13
      0x88400F0E,  //  0043  GETMBR	R16	R7	K14
      0x7C340600,  //  0044  CALL	R13	3
      0xB83A2600,  //  0045  GETNGBL	R14	K19
      0x8C381D14,  //  0046  GETMET	R14	R14	K20
      0x60400008,  //  0047  GETGBL	R16	G8
      0x5C440E00,  //  0048  MOVE	R17	R7
      0x7C400200,  //  0049  CALL	R16	1
      0x00422A10,  //  004A  ADD	R16	K21	R16
      0x78360002,  //  004B  JMPF	R13	#004F
      0x00462C0D,  //  004C  ADD	R17	K22	R13
      0x00442317,  //  004D  ADD	R17	R17	K23
      0x70020000,  //  004E  JMP		#0050
      0x58440018,  //  004F  LDCONST	R17	K24
      0x00402011,  //  0050  ADD	R16	R16	R17
      0x58440019,  //  0051  LDCONST	R17	K25
      0x7C380600,  //  0052  CALL	R14	3
      0x88340104,  //  0053  GETMBR	R13	R0	K4
      0x8C341B1A,  //  0054  GETMET	R13	R13	K26
      0x5C3C0E00,  //  0055  MOVE	R15	R7
      0x84400001,  //  0056  CLOSURE	R16	P1
      0x7C340600,  //  0057  CALL	R13	3
      0xA0240000,  //  0058  CLOSE	R9
      0x7001FFC1,  //  0059  JMP		#001C
      0x5824001B,  //  005A  LDCONST	R9	K27
      0xAC240200,  //  005B  CATCH	R9	1	0
      0xB0080000,  //  005C  RAISE	2	R0	R0
      0x5C240800,  //  005D  MOVE	R9	R4
      0x74260003,  //  005E  JMPT	R9	#0063
      0x8C24011C,  //  005F  GETMET	R9	R0	K28
      0x5C2C0200,  //  0060  MOVE	R11	R1
      0x5C301000,  //  0061  MOVE	R12	R8
      0x7C240600,  //  0062  CALL	R9	3
      0xA0200000,  //  0063  CLOSE	R8
      0x50200200,  //  0064  LDBOOL	R8	1	0
      0xA0000000,  //  0065  CLOSE	R0
      0x80041000,  //  0066  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: path2raw
********************************************************************/
be_local_closure(Matter_IM_path2raw,   /* name */
  be_nested_proto(
    8,                          /* nstack */
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
    /* K2   */  be_nested_str_weak(endpoint),
    /* K3   */  be_const_int(2),
    /* K4   */  be_nested_str_weak(cluster),
    /* K5   */  be_nested_str_weak(attribute),
    }),
    be_str_weak(path2raw),
    &be_const_str_solidified,
    ( &(const binstruction[102]) {  /* code */
      0x8C100300,  //  0000  GETMET	R4	R1	K0
      0x541A0036,  //  0001  LDINT	R6	55
      0x581C0001,  //  0002  LDCONST	R7	K1
      0x7C100600,  //  0003  CALL	R4	3
      0x8C100300,  //  0004  GETMET	R4	R1	K0
      0x5C180600,  //  0005  MOVE	R6	R3
      0x581C0001,  //  0006  LDCONST	R7	K1
      0x7C100600,  //  0007  CALL	R4	3
      0x88100502,  //  0008  GETMBR	R4	R2	K2
      0x541600FE,  //  0009  LDINT	R5	255
      0x18100805,  //  000A  LE	R4	R4	R5
      0x78120008,  //  000B  JMPF	R4	#0015
      0x8C100300,  //  000C  GETMET	R4	R1	K0
      0x541A2401,  //  000D  LDINT	R6	9218
      0x541DFFFD,  //  000E  LDINT	R7	-2
      0x7C100600,  //  000F  CALL	R4	3
      0x8C100300,  //  0010  GETMET	R4	R1	K0
      0x88180502,  //  0011  GETMBR	R6	R2	K2
      0x581C0001,  //  0012  LDCONST	R7	K1
      0x7C100600,  //  0013  CALL	R4	3
      0x70020007,  //  0014  JMP		#001D
      0x8C100300,  //  0015  GETMET	R4	R1	K0
      0x541A2501,  //  0016  LDINT	R6	9474
      0x541DFFFD,  //  0017  LDINT	R7	-2
      0x7C100600,  //  0018  CALL	R4	3
      0x8C100300,  //  0019  GETMET	R4	R1	K0
      0x88180502,  //  001A  GETMBR	R6	R2	K2
      0x581C0003,  //  001B  LDCONST	R7	K3
      0x7C100600,  //  001C  CALL	R4	3
      0x88100504,  //  001D  GETMBR	R4	R2	K4
      0x541600FE,  //  001E  LDINT	R5	255
      0x18100805,  //  001F  LE	R4	R4	R5
      0x78120008,  //  0020  JMPF	R4	#002A
      0x8C100300,  //  0021  GETMET	R4	R1	K0
      0x541A2402,  //  0022  LDINT	R6	9219
      0x541DFFFD,  //  0023  LDINT	R7	-2
      0x7C100600,  //  0024  CALL	R4	3
      0x8C100300,  //  0025  GETMET	R4	R1	K0
      0x88180504,  //  0026  GETMBR	R6	R2	K4
      0x581C0001,  //  0027  LDCONST	R7	K1
      0x7C100600,  //  0028  CALL	R4	3
      0x70020014,  //  0029  JMP		#003F
      0x88100504,  //  002A  GETMBR	R4	R2	K4
      0x5416FFFE,  //  002B  LDINT	R5	65535
      0x18100805,  //  002C  LE	R4	R4	R5
      0x78120008,  //  002D  JMPF	R4	#0037
      0x8C100300,  //  002E  GETMET	R4	R1	K0
      0x541A2502,  //  002F  LDINT	R6	9475
      0x541DFFFD,  //  0030  LDINT	R7	-2
      0x7C100600,  //  0031  CALL	R4	3
      0x8C100300,  //  0032  GETMET	R4	R1	K0
      0x88180504,  //  0033  GETMBR	R6	R2	K4
      0x581C0003,  //  0034  LDCONST	R7	K3
      0x7C100600,  //  0035  CALL	R4	3
      0x70020007,  //  0036  JMP		#003F
      0x8C100300,  //  0037  GETMET	R4	R1	K0
      0x541A2602,  //  0038  LDINT	R6	9731
      0x541DFFFD,  //  0039  LDINT	R7	-2
      0x7C100600,  //  003A  CALL	R4	3
      0x8C100300,  //  003B  GETMET	R4	R1	K0
      0x88180504,  //  003C  GETMBR	R6	R2	K4
      0x541E0003,  //  003D  LDINT	R7	4
      0x7C100600,  //  003E  CALL	R4	3
      0x88100505,  //  003F  GETMBR	R4	R2	K5
      0x541600FE,  //  0040  LDINT	R5	255
      0x18100805,  //  0041  LE	R4	R4	R5
      0x78120008,  //  0042  JMPF	R4	#004C
      0x8C100300,  //  0043  GETMET	R4	R1	K0
      0x541A2403,  //  0044  LDINT	R6	9220
      0x541DFFFD,  //  0045  LDINT	R7	-2
      0x7C100600,  //  0046  CALL	R4	3
      0x8C100300,  //  0047  GETMET	R4	R1	K0
      0x88180505,  //  0048  GETMBR	R6	R2	K5
      0x581C0001,  //  0049  LDCONST	R7	K1
      0x7C100600,  //  004A  CALL	R4	3
      0x70020014,  //  004B  JMP		#0061
      0x88100505,  //  004C  GETMBR	R4	R2	K5
      0x5416FFFE,  //  004D  LDINT	R5	65535
      0x18100805,  //  004E  LE	R4	R4	R5
      0x78120008,  //  004F  JMPF	R4	#0059
      0x8C100300,  //  0050  GETMET	R4	R1	K0
      0x541A2503,  //  0051  LDINT	R6	9476
      0x541DFFFD,  //  0052  LDINT	R7	-2
      0x7C100600,  //  0053  CALL	R4	3
      0x8C100300,  //  0054  GETMET	R4	R1	K0
      0x88180505,  //  0055  GETMBR	R6	R2	K5
      0x581C0003,  //  0056  LDCONST	R7	K3
      0x7C100600,  //  0057  CALL	R4	3
      0x70020007,  //  0058  JMP		#0061
      0x8C100300,  //  0059  GETMET	R4	R1	K0
      0x541A2603,  //  005A  LDINT	R6	9732
      0x541DFFFD,  //  005B  LDINT	R7	-2
      0x7C100600,  //  005C  CALL	R4	3
      0x8C100300,  //  005D  GETMET	R4	R1	K0
      0x88180505,  //  005E  GETMBR	R6	R2	K5
      0x541E0003,  //  005F  LDINT	R7	4
      0x7C100600,  //  0060  CALL	R4	3
      0x8C100300,  //  0061  GETMET	R4	R1	K0
      0x541A0017,  //  0062  LDINT	R6	24
      0x581C0001,  //  0063  LDCONST	R7	K1
      0x7C100600,  //  0064  CALL	R4	3
      0x80000000,  //  0065  RET	0
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
    ( &(const binstruction[66]) {  /* code */
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
      0x7C100600,  //  002E  CALL	R4	3
      0x50140000,  //  002F  LDBOOL	R5	0	0
      0x90122805,  //  0030  SETMBR	R4	K20	R5
      0x88140310,  //  0031  GETMBR	R5	R1	K16
      0x90122005,  //  0032  SETMBR	R4	K16	R5
      0xB8160200,  //  0033  GETNGBL	R5	K1
      0x8C140B15,  //  0034  GETMET	R5	R5	K21
      0x881C0516,  //  0035  GETMBR	R7	R2	K22
      0x5C200400,  //  0036  MOVE	R8	R2
      0x5C240800,  //  0037  MOVE	R9	R4
      0x5C280200,  //  0038  MOVE	R10	R1
      0x7C140A00,  //  0039  CALL	R5	5
      0x88180117,  //  003A  GETMBR	R6	R0	K23
      0x8C180D0A,  //  003B  GETMET	R6	R6	K10
      0x5C200A00,  //  003C  MOVE	R8	R5
      0x7C180400,  //  003D  CALL	R6	2
      0x8C180118,  //  003E  GETMET	R6	R0	K24
      0x88200516,  //  003F  GETMBR	R8	R2	K22
      0x7C180400,  //  0040  CALL	R6	2
      0x80000000,  //  0041  RET	0
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
    18,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        19,                          /* nstack */
        4,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 3]) {  /* upvals */
          be_local_const_upval(1, 1),
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 3),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[28]) {     /* constants */
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
        /* K11  */  be_nested_str_weak(attributedata2raw),
        /* K12  */  be_nested_str_weak(tasmota),
        /* K13  */  be_nested_str_weak(log),
        /* K14  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20_X25s),
        /* K15  */  be_nested_str_weak(local_session_id),
        /* K16  */  be_const_int(3),
        /* K17  */  be_nested_str_weak(status),
        /* K18  */  be_nested_str_weak(attributestatus2raw),
        /* K19  */  be_nested_str_weak(loglevel),
        /* K20  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20STATUS_X3A_X200x_X2502X_X20_X25s),
        /* K21  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
        /* K22  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20IGNORED),
        /* K23  */  be_nested_str_weak(attribute_reports),
        /* K24  */  be_const_int(0),
        /* K25  */  be_nested_str_weak(push),
        /* K26  */  be_nested_str_weak(IM_ReportData),
        /* K27  */  be_nested_str_weak(MAX_MESSAGE),
        }),
        be_str_weak(read_single_attribute),
        &be_const_str_solidified,
        ( &(const binstruction[148]) {  /* code */
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
          0x7826001C,  //  001C  JMPF	R9	#003A
          0x8C240D0A,  //  001D  GETMET	R9	R6	K10
          0x7C240200,  //  001E  CALL	R9	1
          0x60280015,  //  001F  GETGBL	R10	G21
          0x542E002F,  //  0020  LDINT	R11	48
          0x7C280200,  //  0021  CALL	R10	1
          0x5C201400,  //  0022  MOVE	R8	R10
          0x68280001,  //  0023  GETUPV	R10	U1
          0x8C28150B,  //  0024  GETMET	R10	R10	K11
          0x5C301000,  //  0025  MOVE	R12	R8
          0x5C340400,  //  0026  MOVE	R13	R2
          0x5C380C00,  //  0027  MOVE	R14	R6
          0x7C280800,  //  0028  CALL	R10	4
          0x68280002,  //  0029  GETUPV	R10	U2
          0x742A000D,  //  002A  JMPT	R10	#0039
          0xB82A1800,  //  002B  GETNGBL	R10	K12
          0x8C28150D,  //  002C  GETMET	R10	R10	K13
          0x60300018,  //  002D  GETGBL	R12	G24
          0x5834000E,  //  002E  LDCONST	R13	K14
          0x68380000,  //  002F  GETUPV	R14	U0
          0x88381D0F,  //  0030  GETMBR	R14	R14	K15
          0x603C0008,  //  0031  GETGBL	R15	G8
          0x5C400400,  //  0032  MOVE	R16	R2
          0x7C3C0200,  //  0033  CALL	R15	1
          0x5C400A00,  //  0034  MOVE	R16	R5
          0x5C441200,  //  0035  MOVE	R17	R9
          0x7C300A00,  //  0036  CALL	R12	5
          0x58340010,  //  0037  LDCONST	R13	K16
          0x7C280600,  //  0038  CALL	R10	3
          0x70020038,  //  0039  JMP		#0073
          0x88240511,  //  003A  GETMBR	R9	R2	K17
          0x4C280000,  //  003B  LDNIL	R10
          0x2024120A,  //  003C  NE	R9	R9	R10
          0x78260026,  //  003D  JMPF	R9	#0065
          0x780E0024,  //  003E  JMPF	R3	#0064
          0x60240015,  //  003F  GETGBL	R9	G21
          0x542A002F,  //  0040  LDINT	R10	48
          0x7C240200,  //  0041  CALL	R9	1
          0x5C201200,  //  0042  MOVE	R8	R9
          0x68240001,  //  0043  GETUPV	R9	U1
          0x8C241312,  //  0044  GETMET	R9	R9	K18
          0x5C2C1000,  //  0045  MOVE	R11	R8
          0x5C300400,  //  0046  MOVE	R12	R2
          0x88340511,  //  0047  GETMBR	R13	R2	K17
          0x7C240800,  //  0048  CALL	R9	4
          0xB8261800,  //  0049  GETNGBL	R9	K12
          0x8C241313,  //  004A  GETMET	R9	R9	K19
          0x582C0010,  //  004B  LDCONST	R11	K16
          0x7C240400,  //  004C  CALL	R9	2
          0x78260015,  //  004D  JMPF	R9	#0064
          0xB8261800,  //  004E  GETNGBL	R9	K12
          0x8C24130D,  //  004F  GETMET	R9	R9	K13
          0x602C0018,  //  0050  GETGBL	R11	G24
          0x58300014,  //  0051  LDCONST	R12	K20
          0x68340000,  //  0052  GETUPV	R13	U0
          0x88341B0F,  //  0053  GETMBR	R13	R13	K15
          0x60380008,  //  0054  GETGBL	R14	G8
          0x5C3C0400,  //  0055  MOVE	R15	R2
          0x7C380200,  //  0056  CALL	R14	1
          0x5C3C0A00,  //  0057  MOVE	R15	R5
          0x88400511,  //  0058  GETMBR	R16	R2	K17
          0x88440511,  //  0059  GETMBR	R17	R2	K17
          0xB84A0000,  //  005A  GETNGBL	R18	K0
          0x88482515,  //  005B  GETMBR	R18	R18	K21
          0x1C442212,  //  005C  EQ	R17	R17	R18
          0x78460001,  //  005D  JMPF	R17	#0060
          0x58440015,  //  005E  LDCONST	R17	K21
          0x70020000,  //  005F  JMP		#0061
          0x58440007,  //  0060  LDCONST	R17	K7
          0x7C2C0C00,  //  0061  CALL	R11	6
          0x58300010,  //  0062  LDCONST	R12	K16
          0x7C240600,  //  0063  CALL	R9	3
          0x7002000D,  //  0064  JMP		#0073
          0xB8261800,  //  0065  GETNGBL	R9	K12
          0x8C24130D,  //  0066  GETMET	R9	R9	K13
          0x602C0018,  //  0067  GETGBL	R11	G24
          0x58300016,  //  0068  LDCONST	R12	K22
          0x68340000,  //  0069  GETUPV	R13	U0
          0x88341B0F,  //  006A  GETMBR	R13	R13	K15
          0x60380008,  //  006B  GETGBL	R14	G8
          0x5C3C0400,  //  006C  MOVE	R15	R2
          0x7C380200,  //  006D  CALL	R14	1
          0x5C3C0A00,  //  006E  MOVE	R15	R5
          0x7C2C0800,  //  006F  CALL	R11	4
          0x58300010,  //  0070  LDCONST	R12	K16
          0x7C240600,  //  0071  CALL	R9	3
          0x501C0000,  //  0072  LDBOOL	R7	0	0
          0x7822001E,  //  0073  JMPF	R8	#0093
          0x6024000C,  //  0074  GETGBL	R9	G12
          0x88280117,  //  0075  GETMBR	R10	R0	K23
          0x7C240200,  //  0076  CALL	R9	1
          0x1C241318,  //  0077  EQ	R9	R9	K24
          0x78260004,  //  0078  JMPF	R9	#007E
          0x88240117,  //  0079  GETMBR	R9	R0	K23
          0x8C241319,  //  007A  GETMET	R9	R9	K25
          0x5C2C1000,  //  007B  MOVE	R11	R8
          0x7C240400,  //  007C  CALL	R9	2
          0x70020014,  //  007D  JMP		#0093
          0x5425FFFE,  //  007E  LDINT	R9	-1
          0x88280117,  //  007F  GETMBR	R10	R0	K23
          0x94241409,  //  0080  GETIDX	R9	R10	R9
          0x602C000C,  //  0081  GETGBL	R11	G12
          0x5C301200,  //  0082  MOVE	R12	R9
          0x7C2C0200,  //  0083  CALL	R11	1
          0x6030000C,  //  0084  GETGBL	R12	G12
          0x5C341000,  //  0085  MOVE	R13	R8
          0x7C300200,  //  0086  CALL	R12	1
          0x002C160C,  //  0087  ADD	R11	R11	R12
          0xB8320000,  //  0088  GETNGBL	R12	K0
          0x8830191A,  //  0089  GETMBR	R12	R12	K26
          0x8830191B,  //  008A  GETMBR	R12	R12	K27
          0x182C160C,  //  008B  LE	R11	R11	R12
          0x782E0001,  //  008C  JMPF	R11	#008F
          0x402C1208,  //  008D  CONNECT	R11	R9	R8
          0x70020003,  //  008E  JMP		#0093
          0x88280117,  //  008F  GETMBR	R10	R0	K23
          0x8C281519,  //  0090  GETMET	R10	R10	K25
          0x5C301000,  //  0091  MOVE	R12	R8
          0x7C280400,  //  0092  CALL	R10	2
          0x80040E00,  //  0093  RET	1	R7
        })
      ),
      be_nested_proto(
        8,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 4),
          be_local_const_upval(1, 7),
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
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(get_active_endpoints),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(Path),
    /* K4   */  be_nested_str_weak(ReportDataMessage),
    /* K5   */  be_nested_str_weak(attribute_reports),
    /* K6   */  be_nested_str_weak(attributes_requests),
    /* K7   */  be_nested_str_weak(endpoint),
    /* K8   */  be_nested_str_weak(cluster),
    /* K9   */  be_nested_str_weak(attribute),
    /* K10  */  be_nested_str_weak(status),
    /* K11  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K12  */  be_nested_str_weak(get_attribute_name),
    /* K13  */  be_nested_str_weak(tasmota),
    /* K14  */  be_nested_str_weak(log),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s),
    /* K16  */  be_nested_str_weak(local_session_id),
    /* K17  */  be_nested_str_weak(_X20_X28),
    /* K18  */  be_nested_str_weak(_X29),
    /* K19  */  be_nested_str_weak(),
    /* K20  */  be_const_int(3),
    /* K21  */  be_nested_str_weak(process_attribute_expansion),
    /* K22  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(_inner_process_read_request),
    &be_const_str_solidified,
    ( &(const binstruction[93]) {  /* code */
      0x84100000,  //  0000  CLOSURE	R4	P0
      0x88140100,  //  0001  GETMBR	R5	R0	K0
      0x8C140B01,  //  0002  GETMET	R5	R5	K1
      0x7C140200,  //  0003  CALL	R5	1
      0xB81A0400,  //  0004  GETNGBL	R6	K2
      0x8C180D03,  //  0005  GETMET	R6	R6	K3
      0x7C180200,  //  0006  CALL	R6	1
      0xB81E0400,  //  0007  GETNGBL	R7	K2
      0x8C1C0F04,  //  0008  GETMET	R7	R7	K4
      0x7C1C0200,  //  0009  CALL	R7	1
      0x60200012,  //  000A  GETGBL	R8	G18
      0x7C200000,  //  000B  CALL	R8	0
      0x901E0A08,  //  000C  SETMBR	R7	K5	R8
      0x60200010,  //  000D  GETGBL	R8	G16
      0x88240506,  //  000E  GETMBR	R9	R2	K6
      0x7C200200,  //  000F  CALL	R8	1
      0xA8020046,  //  0010  EXBLK	0	#0058
      0x5C241000,  //  0011  MOVE	R9	R8
      0x7C240000,  //  0012  CALL	R9	0
      0x88281307,  //  0013  GETMBR	R10	R9	K7
      0x901A0E0A,  //  0014  SETMBR	R6	K7	R10
      0x88281308,  //  0015  GETMBR	R10	R9	K8
      0x901A100A,  //  0016  SETMBR	R6	K8	R10
      0x88281309,  //  0017  GETMBR	R10	R9	K9
      0x901A120A,  //  0018  SETMBR	R6	K9	R10
      0xB82A0400,  //  0019  GETNGBL	R10	K2
      0x8828150B,  //  001A  GETMBR	R10	R10	K11
      0x901A140A,  //  001B  SETMBR	R6	K10	R10
      0x88280D07,  //  001C  GETMBR	R10	R6	K7
      0x4C2C0000,  //  001D  LDNIL	R11
      0x1C28140B,  //  001E  EQ	R10	R10	R11
      0x742A0007,  //  001F  JMPT	R10	#0028
      0x88280D08,  //  0020  GETMBR	R10	R6	K8
      0x4C2C0000,  //  0021  LDNIL	R11
      0x1C28140B,  //  0022  EQ	R10	R10	R11
      0x742A0003,  //  0023  JMPT	R10	#0028
      0x88280D09,  //  0024  GETMBR	R10	R6	K9
      0x4C2C0000,  //  0025  LDNIL	R11
      0x1C28140B,  //  0026  EQ	R10	R10	R11
      0x782A0029,  //  0027  JMPF	R10	#0052
      0x88280D08,  //  0028  GETMBR	R10	R6	K8
      0x4C2C0000,  //  0029  LDNIL	R11
      0x2028140B,  //  002A  NE	R10	R10	R11
      0x782A001A,  //  002B  JMPF	R10	#0047
      0x88280D09,  //  002C  GETMBR	R10	R6	K9
      0x4C2C0000,  //  002D  LDNIL	R11
      0x2028140B,  //  002E  NE	R10	R10	R11
      0x782A0016,  //  002F  JMPF	R10	#0047
      0xB82A0400,  //  0030  GETNGBL	R10	K2
      0x8C28150C,  //  0031  GETMET	R10	R10	K12
      0x88300D08,  //  0032  GETMBR	R12	R6	K8
      0x88340D09,  //  0033  GETMBR	R13	R6	K9
      0x7C280600,  //  0034  CALL	R10	3
      0xB82E1A00,  //  0035  GETNGBL	R11	K13
      0x8C2C170E,  //  0036  GETMET	R11	R11	K14
      0x60340018,  //  0037  GETGBL	R13	G24
      0x5838000F,  //  0038  LDCONST	R14	K15
      0x883C0310,  //  0039  GETMBR	R15	R1	K16
      0x60400008,  //  003A  GETGBL	R16	G8
      0x5C440C00,  //  003B  MOVE	R17	R6
      0x7C400200,  //  003C  CALL	R16	1
      0x782A0002,  //  003D  JMPF	R10	#0041
      0x0046220A,  //  003E  ADD	R17	K17	R10
      0x00442312,  //  003F  ADD	R17	R17	K18
      0x70020000,  //  0040  JMP		#0042
      0x58440013,  //  0041  LDCONST	R17	K19
      0x00402011,  //  0042  ADD	R16	R16	R17
      0x7C340600,  //  0043  CALL	R13	3
      0x58380014,  //  0044  LDCONST	R14	K20
      0x7C2C0600,  //  0045  CALL	R11	3
      0x7002000A,  //  0046  JMP		#0052
      0xB82A1A00,  //  0047  GETNGBL	R10	K13
      0x8C28150E,  //  0048  GETMET	R10	R10	K14
      0x60300018,  //  0049  GETGBL	R12	G24
      0x5834000F,  //  004A  LDCONST	R13	K15
      0x88380310,  //  004B  GETMBR	R14	R1	K16
      0x603C0008,  //  004C  GETGBL	R15	G8
      0x5C400C00,  //  004D  MOVE	R16	R6
      0x7C3C0200,  //  004E  CALL	R15	1
      0x7C300600,  //  004F  CALL	R12	3
      0x58340014,  //  0050  LDCONST	R13	K20
      0x7C280600,  //  0051  CALL	R10	3
      0x88280100,  //  0052  GETMBR	R10	R0	K0
      0x8C281515,  //  0053  GETMET	R10	R10	K21
      0x5C300C00,  //  0054  MOVE	R12	R6
      0x84340001,  //  0055  CLOSURE	R13	P1
      0x7C280600,  //  0056  CALL	R10	3
      0x7001FFB8,  //  0057  JMP		#0011
      0x58200016,  //  0058  LDCONST	R8	K22
      0xAC200200,  //  0059  CATCH	R8	1	0
      0xB0080000,  //  005A  RAISE	2	R0	R0
      0xA0000000,  //  005B  CLOSE	R0
      0x80040E00,  //  005C  RET	1	R7
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
    ( &(const bvalue[44]) {     /* constants */
    /* K0   */  be_nested_str_weak(status),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(INVALID_ACTION),
    /* K3   */  be_nested_str_weak(device),
    /* K4   */  be_nested_str_weak(process_attribute_read_solo),
    /* K5   */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K6   */  be_nested_str_weak(read_attribute),
    /* K7   */  be_nested_str_weak(session),
    /* K8   */  be_nested_str_weak(tlv_solo),
    /* K9   */  be_nested_str_weak(profiler),
    /* K10  */  be_nested_str_weak(log),
    /* K11  */  be_nested_str_weak(read_request_solo_X20read_X20done),
    /* K12  */  be_nested_str_weak(add),
    /* K13  */  be_const_int(1),
    /* K14  */  be_nested_str_weak(attributedata2raw),
    /* K15  */  be_const_int(405077761),
    /* K16  */  be_nested_str_weak(attributestatus2raw),
    /* K17  */  be_nested_str_weak(tasmota),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X20_X2D_X20IGNORED),
    /* K19  */  be_nested_str_weak(local_session_id),
    /* K20  */  be_const_int(3),
    /* K21  */  be_nested_str_weak(build_response),
    /* K22  */  be_nested_str_weak(message_handler),
    /* K23  */  be_nested_str_weak(raw),
    /* K24  */  be_nested_str_weak(clear),
    /* K25  */  be_nested_str_weak(encode_frame),
    /* K26  */  be_nested_str_weak(encrypt),
    /* K27  */  be_nested_str_weak(loglevel),
    /* K28  */  be_nested_str_weak(MTR_X3A_X20_X3Csnd_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20id_X3D_X25i_X20exch_X3D_X25i_X20rack_X3D_X25s),
    /* K29  */  be_nested_str_weak(message_counter),
    /* K30  */  be_nested_str_weak(exchange_id),
    /* K31  */  be_nested_str_weak(ack_message_counter),
    /* K32  */  be_nested_str_weak(send_response_frame),
    /* K33  */  be_nested_str_weak(RESPONSE_X20SENT),
    /* K34  */  be_nested_str_weak(get_attribute_name),
    /* K35  */  be_nested_str_weak(cluster),
    /* K36  */  be_nested_str_weak(attribute),
    /* K37  */  be_nested_str_weak(_X20_X28),
    /* K38  */  be_nested_str_weak(_X29),
    /* K39  */  be_nested_str_weak(),
    /* K40  */  be_nested_str_weak(to_str_val),
    /* K41  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr1_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20_X25s),
    /* K42  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr1_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20STATUS_X3A_X200x_X2502X_X20_X25s),
    /* K43  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr1_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20IGNORED),
    }),
    be_str_weak(process_read_request_solo),
    &be_const_str_solidified,
    ( &(const binstruction[220]) {  /* code */
      0xB80E0200,  //  0000  GETNGBL	R3	K1
      0x880C0702,  //  0001  GETMBR	R3	R3	K2
      0x900A0003,  //  0002  SETMBR	R2	K0	R3
      0x880C0103,  //  0003  GETMBR	R3	R0	K3
      0x8C0C0704,  //  0004  GETMET	R3	R3	K4
      0x5C140400,  //  0005  MOVE	R5	R2
      0x7C0C0400,  //  0006  CALL	R3	2
      0x4C100000,  //  0007  LDNIL	R4
      0x4C140000,  //  0008  LDNIL	R5
      0x4C180000,  //  0009  LDNIL	R6
      0x20180606,  //  000A  NE	R6	R3	R6
      0x781A0008,  //  000B  JMPF	R6	#0015
      0xB81A0200,  //  000C  GETNGBL	R6	K1
      0x88180D05,  //  000D  GETMBR	R6	R6	K5
      0x900A0006,  //  000E  SETMBR	R2	K0	R6
      0x8C180706,  //  000F  GETMET	R6	R3	K6
      0x88200307,  //  0010  GETMBR	R8	R1	K7
      0x5C240400,  //  0011  MOVE	R9	R2
      0x88280108,  //  0012  GETMBR	R10	R0	K8
      0x7C180800,  //  0013  CALL	R6	4
      0x5C100C00,  //  0014  MOVE	R4	R6
      0xB81A0200,  //  0015  GETNGBL	R6	K1
      0x88180D09,  //  0016  GETMBR	R6	R6	K9
      0x8C180D0A,  //  0017  GETMET	R6	R6	K10
      0x5820000B,  //  0018  LDCONST	R8	K11
      0x7C180400,  //  0019  CALL	R6	2
      0x4C180000,  //  001A  LDNIL	R6
      0x20180806,  //  001B  NE	R6	R4	R6
      0x781A0019,  //  001C  JMPF	R6	#0037
      0x60180015,  //  001D  GETGBL	R6	G21
      0x541E002F,  //  001E  LDINT	R7	48
      0x7C180200,  //  001F  CALL	R6	1
      0x5C140C00,  //  0020  MOVE	R5	R6
      0x8C180B0C,  //  0021  GETMET	R6	R5	K12
      0x54220014,  //  0022  LDINT	R8	21
      0x5824000D,  //  0023  LDCONST	R9	K13
      0x7C180600,  //  0024  CALL	R6	3
      0x8C180B0C,  //  0025  GETMET	R6	R5	K12
      0x54223600,  //  0026  LDINT	R8	13825
      0x5425FFFD,  //  0027  LDINT	R9	-2
      0x7C180600,  //  0028  CALL	R6	3
      0x8C18010E,  //  0029  GETMET	R6	R0	K14
      0x5C200A00,  //  002A  MOVE	R8	R5
      0x5C240400,  //  002B  MOVE	R9	R2
      0x5C280800,  //  002C  MOVE	R10	R4
      0x7C180800,  //  002D  CALL	R6	4
      0x8C180B0C,  //  002E  GETMET	R6	R5	K12
      0x5820000F,  //  002F  LDCONST	R8	K15
      0x5425FFFB,  //  0030  LDINT	R9	-4
      0x7C180600,  //  0031  CALL	R6	3
      0x8C180B0C,  //  0032  GETMET	R6	R5	K12
      0x54220017,  //  0033  LDINT	R8	24
      0x5824000D,  //  0034  LDCONST	R9	K13
      0x7C180600,  //  0035  CALL	R6	3
      0x70020029,  //  0036  JMP		#0061
      0x88180500,  //  0037  GETMBR	R6	R2	K0
      0x4C1C0000,  //  0038  LDNIL	R7
      0x20180C07,  //  0039  NE	R6	R6	R7
      0x781A0019,  //  003A  JMPF	R6	#0055
      0x60180015,  //  003B  GETGBL	R6	G21
      0x541E002F,  //  003C  LDINT	R7	48
      0x7C180200,  //  003D  CALL	R6	1
      0x5C140C00,  //  003E  MOVE	R5	R6
      0x8C180B0C,  //  003F  GETMET	R6	R5	K12
      0x54220014,  //  0040  LDINT	R8	21
      0x5824000D,  //  0041  LDCONST	R9	K13
      0x7C180600,  //  0042  CALL	R6	3
      0x8C180B0C,  //  0043  GETMET	R6	R5	K12
      0x54223600,  //  0044  LDINT	R8	13825
      0x5425FFFD,  //  0045  LDINT	R9	-2
      0x7C180600,  //  0046  CALL	R6	3
      0x8C180110,  //  0047  GETMET	R6	R0	K16
      0x5C200A00,  //  0048  MOVE	R8	R5
      0x5C240400,  //  0049  MOVE	R9	R2
      0x88280500,  //  004A  GETMBR	R10	R2	K0
      0x7C180800,  //  004B  CALL	R6	4
      0x8C180B0C,  //  004C  GETMET	R6	R5	K12
      0x5820000F,  //  004D  LDCONST	R8	K15
      0x5425FFFB,  //  004E  LDINT	R9	-4
      0x7C180600,  //  004F  CALL	R6	3
      0x8C180B0C,  //  0050  GETMET	R6	R5	K12
      0x54220017,  //  0051  LDINT	R8	24
      0x5824000D,  //  0052  LDCONST	R9	K13
      0x7C180600,  //  0053  CALL	R6	3
      0x7002000B,  //  0054  JMP		#0061
      0xB81A2200,  //  0055  GETNGBL	R6	K17
      0x8C180D0A,  //  0056  GETMET	R6	R6	K10
      0x60200018,  //  0057  GETGBL	R8	G24
      0x58240012,  //  0058  LDCONST	R9	K18
      0x88280307,  //  0059  GETMBR	R10	R1	K7
      0x88281513,  //  005A  GETMBR	R10	R10	K19
      0x5C2C0400,  //  005B  MOVE	R11	R2
      0x7C200600,  //  005C  CALL	R8	3
      0x58240014,  //  005D  LDCONST	R9	K20
      0x7C180600,  //  005E  CALL	R6	3
      0x50180000,  //  005F  LDBOOL	R6	0	0
      0x80040C00,  //  0060  RET	1	R6
      0x8C180315,  //  0061  GETMET	R6	R1	K21
      0x54220004,  //  0062  LDINT	R8	5
      0x50240200,  //  0063  LDBOOL	R9	1	0
      0x7C180600,  //  0064  CALL	R6	3
      0x881C0103,  //  0065  GETMBR	R7	R0	K3
      0x881C0F16,  //  0066  GETMBR	R7	R7	K22
      0x88200317,  //  0067  GETMBR	R8	R1	K23
      0x8C241118,  //  0068  GETMET	R9	R8	K24
      0x7C240200,  //  0069  CALL	R9	1
      0x8C240D19,  //  006A  GETMET	R9	R6	K25
      0x5C2C0A00,  //  006B  MOVE	R11	R5
      0x5C301000,  //  006C  MOVE	R12	R8
      0x7C240600,  //  006D  CALL	R9	3
      0x8C240D1A,  //  006E  GETMET	R9	R6	K26
      0x7C240200,  //  006F  CALL	R9	1
      0xB8262200,  //  0070  GETNGBL	R9	K17
      0x8C24131B,  //  0071  GETMET	R9	R9	K27
      0x542E0003,  //  0072  LDINT	R11	4
      0x7C240400,  //  0073  CALL	R9	2
      0x7826000B,  //  0074  JMPF	R9	#0081
      0xB8262200,  //  0075  GETNGBL	R9	K17
      0x8C24130A,  //  0076  GETMET	R9	R9	K10
      0x602C0018,  //  0077  GETGBL	R11	G24
      0x5830001C,  //  0078  LDCONST	R12	K28
      0x88340D07,  //  0079  GETMBR	R13	R6	K7
      0x88341B13,  //  007A  GETMBR	R13	R13	K19
      0x88380D1D,  //  007B  GETMBR	R14	R6	K29
      0x883C0D1E,  //  007C  GETMBR	R15	R6	K30
      0x88400D1F,  //  007D  GETMBR	R16	R6	K31
      0x7C2C0A00,  //  007E  CALL	R11	5
      0x54320003,  //  007F  LDINT	R12	4
      0x7C240600,  //  0080  CALL	R9	3
      0x8C240F20,  //  0081  GETMET	R9	R7	K32
      0x5C2C0C00,  //  0082  MOVE	R11	R6
      0x7C240400,  //  0083  CALL	R9	2
      0xB8260200,  //  0084  GETNGBL	R9	K1
      0x88241309,  //  0085  GETMBR	R9	R9	K9
      0x8C24130A,  //  0086  GETMET	R9	R9	K10
      0x582C0021,  //  0087  LDCONST	R11	K33
      0x7C240400,  //  0088  CALL	R9	2
      0xB8260200,  //  0089  GETNGBL	R9	K1
      0x8C241322,  //  008A  GETMET	R9	R9	K34
      0x882C0523,  //  008B  GETMBR	R11	R2	K35
      0x88300524,  //  008C  GETMBR	R12	R2	K36
      0x7C240600,  //  008D  CALL	R9	3
      0x78260002,  //  008E  JMPF	R9	#0092
      0x002A4A09,  //  008F  ADD	R10	K37	R9
      0x00281526,  //  0090  ADD	R10	R10	K38
      0x70020000,  //  0091  JMP		#0093
      0x58280027,  //  0092  LDCONST	R10	K39
      0x5C241400,  //  0093  MOVE	R9	R10
      0x4C280000,  //  0094  LDNIL	R10
      0x2028080A,  //  0095  NE	R10	R4	R10
      0x782A0013,  //  0096  JMPF	R10	#00AB
      0x8C280928,  //  0097  GETMET	R10	R4	K40
      0x7C280200,  //  0098  CALL	R10	1
      0xB82E2200,  //  0099  GETNGBL	R11	K17
      0x8C2C171B,  //  009A  GETMET	R11	R11	K27
      0x58340014,  //  009B  LDCONST	R13	K20
      0x7C2C0400,  //  009C  CALL	R11	2
      0x782E000B,  //  009D  JMPF	R11	#00AA
      0xB82E2200,  //  009E  GETNGBL	R11	K17
      0x8C2C170A,  //  009F  GETMET	R11	R11	K10
      0x60340018,  //  00A0  GETGBL	R13	G24
      0x58380029,  //  00A1  LDCONST	R14	K41
      0x883C0307,  //  00A2  GETMBR	R15	R1	K7
      0x883C1F13,  //  00A3  GETMBR	R15	R15	K19
      0x5C400400,  //  00A4  MOVE	R16	R2
      0x5C441200,  //  00A5  MOVE	R17	R9
      0x5C481400,  //  00A6  MOVE	R18	R10
      0x7C340A00,  //  00A7  CALL	R13	5
      0x58380014,  //  00A8  LDCONST	R14	K20
      0x7C2C0600,  //  00A9  CALL	R11	3
      0x7002002E,  //  00AA  JMP		#00DA
      0x88280500,  //  00AB  GETMBR	R10	R2	K0
      0x4C2C0000,  //  00AC  LDNIL	R11
      0x2028140B,  //  00AD  NE	R10	R10	R11
      0x782A001A,  //  00AE  JMPF	R10	#00CA
      0x88280500,  //  00AF  GETMBR	R10	R2	K0
      0xB82E0200,  //  00B0  GETNGBL	R11	K1
      0x882C1705,  //  00B1  GETMBR	R11	R11	K5
      0x1C28140B,  //  00B2  EQ	R10	R10	R11
      0x782A0001,  //  00B3  JMPF	R10	#00B6
      0x58280005,  //  00B4  LDCONST	R10	K5
      0x70020000,  //  00B5  JMP		#00B7
      0x58280027,  //  00B6  LDCONST	R10	K39
      0xB82E2200,  //  00B7  GETNGBL	R11	K17
      0x8C2C171B,  //  00B8  GETMET	R11	R11	K27
      0x58340014,  //  00B9  LDCONST	R13	K20
      0x7C2C0400,  //  00BA  CALL	R11	2
      0x782E000C,  //  00BB  JMPF	R11	#00C9
      0xB82E2200,  //  00BC  GETNGBL	R11	K17
      0x8C2C170A,  //  00BD  GETMET	R11	R11	K10
      0x60340018,  //  00BE  GETGBL	R13	G24
      0x5838002A,  //  00BF  LDCONST	R14	K42
      0x883C0307,  //  00C0  GETMBR	R15	R1	K7
      0x883C1F13,  //  00C1  GETMBR	R15	R15	K19
      0x5C400400,  //  00C2  MOVE	R16	R2
      0x5C441200,  //  00C3  MOVE	R17	R9
      0x88480500,  //  00C4  GETMBR	R18	R2	K0
      0x5C4C1400,  //  00C5  MOVE	R19	R10
      0x7C340C00,  //  00C6  CALL	R13	6
      0x58380014,  //  00C7  LDCONST	R14	K20
      0x7C2C0600,  //  00C8  CALL	R11	3
      0x7002000F,  //  00C9  JMP		#00DA
      0xB82A2200,  //  00CA  GETNGBL	R10	K17
      0x8C28151B,  //  00CB  GETMET	R10	R10	K27
      0x58300014,  //  00CC  LDCONST	R12	K20
      0x7C280400,  //  00CD  CALL	R10	2
      0x782A000A,  //  00CE  JMPF	R10	#00DA
      0xB82A2200,  //  00CF  GETNGBL	R10	K17
      0x8C28150A,  //  00D0  GETMET	R10	R10	K10
      0x60300018,  //  00D1  GETGBL	R12	G24
      0x5834002B,  //  00D2  LDCONST	R13	K43
      0x88380307,  //  00D3  GETMBR	R14	R1	K7
      0x88381D13,  //  00D4  GETMBR	R14	R14	K19
      0x5C3C0400,  //  00D5  MOVE	R15	R2
      0x5C401200,  //  00D6  MOVE	R16	R9
      0x7C300800,  //  00D7  CALL	R12	4
      0x58340014,  //  00D8  LDCONST	R13	K20
      0x7C280600,  //  00D9  CALL	R10	3
      0x50280200,  //  00DA  LDBOOL	R10	1	0
      0x80041400,  //  00DB  RET	1	R10
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
    17,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
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
    /* K18  */  be_nested_str_weak(MTR_X3A_X20_X3ESubscribe_X20_X28_X256i_X29_X20_X25s_X20_X28min_X3D_X25i_X2C_X20max_X3D_X25i_X2C_X20keep_X3D_X25i_X29_X20sub_X3D_X25i),
    /* K19  */  be_nested_str_weak(local_session_id),
    /* K20  */  be_nested_str_weak(concat),
    /* K21  */  be_nested_str_weak(_X20),
    /* K22  */  be_nested_str_weak(min_interval),
    /* K23  */  be_nested_str_weak(max_interval),
    /* K24  */  be_const_int(1),
    /* K25  */  be_const_int(0),
    /* K26  */  be_nested_str_weak(subscription_id),
    /* K27  */  be_const_int(3),
    /* K28  */  be_nested_str_weak(_inner_process_read_request),
    /* K29  */  be_nested_str_weak(send_subscribe_response),
    }),
    be_str_weak(subscribe_request),
    &be_const_str_solidified,
    ( &(const binstruction[78]) {  /* code */
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
      0x7C240E00,  //  003D  CALL	R9	7
      0x5828001B,  //  003E  LDCONST	R10	K27
      0x7C1C0600,  //  003F  CALL	R7	3
      0x8C1C011C,  //  0040  GETMET	R7	R0	K28
      0x88240306,  //  0041  GETMBR	R9	R1	K6
      0x5C280600,  //  0042  MOVE	R10	R3
      0x502C0200,  //  0043  LDBOOL	R11	1	0
      0x7C1C0800,  //  0044  CALL	R7	4
      0x8820091A,  //  0045  GETMBR	R8	R4	K26
      0x901E3408,  //  0046  SETMBR	R7	K26	R8
      0x8C20011D,  //  0047  GETMET	R8	R0	K29
      0x5C280200,  //  0048  MOVE	R10	R1
      0x5C2C0E00,  //  0049  MOVE	R11	R7
      0x5C300800,  //  004A  MOVE	R12	R4
      0x7C200800,  //  004B  CALL	R8	4
      0x50200200,  //  004C  LDBOOL	R8	1	0
      0x80041000,  //  004D  RET	1	R8
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
