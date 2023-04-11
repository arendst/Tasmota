/* Solidification of Matter_IM.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_IM;

/********************************************************************
** Solidified function: subscribe_request
********************************************************************/
be_local_closure(Matter_IM_subscribe_request,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[33]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(SubscribeRequestMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(keep_subscriptions),
    /* K5   */  be_nested_str_weak(subs_shop),
    /* K6   */  be_nested_str_weak(remove_by_session),
    /* K7   */  be_nested_str_weak(session),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20received_X20SubscribeRequestMessage_X3D),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str_weak(new_subscription),
    /* K13  */  be_nested_str_weak(Path),
    /* K14  */  be_nested_str_weak(attributes_requests),
    /* K15  */  be_nested_str_weak(endpoint),
    /* K16  */  be_nested_str_weak(cluster),
    /* K17  */  be_nested_str_weak(attribute),
    /* K18  */  be_nested_str_weak(push),
    /* K19  */  be_nested_str_weak(stop_iteration),
    /* K20  */  be_nested_str_weak(format),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20_X3ESubscribe_X20_X28_X256i_X29_X20_X25s_X20_X28min_X3D_X25i_X2C_X20max_X3D_X25i_X2C_X20keep_X3D_X25i_X29_X20sub_X3D_X25i),
    /* K22  */  be_nested_str_weak(local_session_id),
    /* K23  */  be_nested_str_weak(concat),
    /* K24  */  be_nested_str_weak(_X20),
    /* K25  */  be_nested_str_weak(min_interval),
    /* K26  */  be_nested_str_weak(max_interval),
    /* K27  */  be_const_int(1),
    /* K28  */  be_const_int(0),
    /* K29  */  be_nested_str_weak(subscription_id),
    /* K30  */  be_const_int(2),
    /* K31  */  be_nested_str_weak(_inner_process_read_request),
    /* K32  */  be_nested_str_weak(send_subscribe_response),
    }),
    be_str_weak(subscribe_request),
    &be_const_str_solidified,
    ( &(const binstruction[86]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0x88140904,  //  0007  GETMBR	R5	R4	K4
      0x74160003,  //  0008  JMPT	R5	#000D
      0x88140105,  //  0009  GETMBR	R5	R0	K5
      0x8C140B06,  //  000A  GETMET	R5	R5	K6
      0x881C0307,  //  000B  GETMBR	R7	R1	K7
      0x7C140400,  //  000C  CALL	R5	2
      0xB8161000,  //  000D  GETNGBL	R5	K8
      0x8C140B09,  //  000E  GETMET	R5	R5	K9
      0x601C0008,  //  000F  GETGBL	R7	G8
      0x5C200800,  //  0010  MOVE	R8	R4
      0x7C1C0200,  //  0011  CALL	R7	1
      0x001E1407,  //  0012  ADD	R7	K10	R7
      0x5820000B,  //  0013  LDCONST	R8	K11
      0x7C140600,  //  0014  CALL	R5	3
      0x88140105,  //  0015  GETMBR	R5	R0	K5
      0x8C140B0C,  //  0016  GETMET	R5	R5	K12
      0x881C0307,  //  0017  GETMBR	R7	R1	K7
      0x5C200800,  //  0018  MOVE	R8	R4
      0x7C140600,  //  0019  CALL	R5	3
      0x60180012,  //  001A  GETGBL	R6	G18
      0x7C180000,  //  001B  CALL	R6	0
      0xB81E0200,  //  001C  GETNGBL	R7	K1
      0x8C1C0F0D,  //  001D  GETMET	R7	R7	K13
      0x7C1C0200,  //  001E  CALL	R7	1
      0x60200010,  //  001F  GETGBL	R8	G16
      0x8824090E,  //  0020  GETMBR	R9	R4	K14
      0x7C200200,  //  0021  CALL	R8	1
      0xA802000D,  //  0022  EXBLK	0	#0031
      0x5C241000,  //  0023  MOVE	R9	R8
      0x7C240000,  //  0024  CALL	R9	0
      0x8828130F,  //  0025  GETMBR	R10	R9	K15
      0x901E1E0A,  //  0026  SETMBR	R7	K15	R10
      0x88281310,  //  0027  GETMBR	R10	R9	K16
      0x901E200A,  //  0028  SETMBR	R7	K16	R10
      0x88281311,  //  0029  GETMBR	R10	R9	K17
      0x901E220A,  //  002A  SETMBR	R7	K17	R10
      0x8C280D12,  //  002B  GETMET	R10	R6	K18
      0x60300008,  //  002C  GETGBL	R12	G8
      0x5C340E00,  //  002D  MOVE	R13	R7
      0x7C300200,  //  002E  CALL	R12	1
      0x7C280400,  //  002F  CALL	R10	2
      0x7001FFF1,  //  0030  JMP		#0023
      0x58200013,  //  0031  LDCONST	R8	K19
      0xAC200200,  //  0032  CATCH	R8	1	0
      0xB0080000,  //  0033  RAISE	2	R0	R0
      0xB8221000,  //  0034  GETNGBL	R8	K8
      0x8C201109,  //  0035  GETMET	R8	R8	K9
      0x8C280714,  //  0036  GETMET	R10	R3	K20
      0x58300015,  //  0037  LDCONST	R12	K21
      0x88340307,  //  0038  GETMBR	R13	R1	K7
      0x88341B16,  //  0039  GETMBR	R13	R13	K22
      0x8C380D17,  //  003A  GETMET	R14	R6	K23
      0x58400018,  //  003B  LDCONST	R16	K24
      0x7C380400,  //  003C  CALL	R14	2
      0x883C0B19,  //  003D  GETMBR	R15	R5	K25
      0x88400B1A,  //  003E  GETMBR	R16	R5	K26
      0x88440904,  //  003F  GETMBR	R17	R4	K4
      0x78460001,  //  0040  JMPF	R17	#0043
      0x5844001B,  //  0041  LDCONST	R17	K27
      0x70020000,  //  0042  JMP		#0044
      0x5844001C,  //  0043  LDCONST	R17	K28
      0x88480B1D,  //  0044  GETMBR	R18	R5	K29
      0x7C281000,  //  0045  CALL	R10	8
      0x582C001E,  //  0046  LDCONST	R11	K30
      0x7C200600,  //  0047  CALL	R8	3
      0x8C20011F,  //  0048  GETMET	R8	R0	K31
      0x88280307,  //  0049  GETMBR	R10	R1	K7
      0x5C2C0800,  //  004A  MOVE	R11	R4
      0x50300200,  //  004B  LDBOOL	R12	1	0
      0x7C200800,  //  004C  CALL	R8	4
      0x88240B1D,  //  004D  GETMBR	R9	R5	K29
      0x90223A09,  //  004E  SETMBR	R8	K29	R9
      0x8C240120,  //  004F  GETMET	R9	R0	K32
      0x5C2C0200,  //  0050  MOVE	R11	R1
      0x5C301000,  //  0051  MOVE	R12	R8
      0x5C340A00,  //  0052  MOVE	R13	R5
      0x7C240800,  //  0053  CALL	R9	4
      0x50240200,  //  0054  LDBOOL	R9	1	0
      0x80041200,  //  0055  RET	1	R9
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
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(ReadRequestMessage),
    /* K2   */  be_nested_str_weak(from_TLV),
    /* K3   */  be_nested_str_weak(attributes_requests),
    /* K4   */  be_nested_str_weak(_inner_process_read_request),
    /* K5   */  be_nested_str_weak(session),
    /* K6   */  be_nested_str_weak(send_report_data),
    }),
    be_str_weak(process_read_request),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x88100703,  //  0006  GETMBR	R4	R3	K3
      0x4C140000,  //  0007  LDNIL	R5
      0x20100805,  //  0008  NE	R4	R4	R5
      0x78120007,  //  0009  JMPF	R4	#0012
      0x8C100104,  //  000A  GETMET	R4	R0	K4
      0x88180305,  //  000B  GETMBR	R6	R1	K5
      0x5C1C0600,  //  000C  MOVE	R7	R3
      0x7C100600,  //  000D  CALL	R4	3
      0x8C140106,  //  000E  GETMET	R5	R0	K6
      0x5C1C0200,  //  000F  MOVE	R7	R1
      0x5C200800,  //  0010  MOVE	R8	R4
      0x7C140600,  //  0011  CALL	R5	3
      0x50100200,  //  0012  LDBOOL	R4	1	0
      0x80040800,  //  0013  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_write_response
********************************************************************/
be_local_closure(Matter_IM_process_write_response,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(WriteResponseMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20received_X20WriteResponseMessage_X3D),
    /* K7   */  be_const_int(2),
    }),
    be_str_weak(process_write_response),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0xB8160800,  //  0007  GETNGBL	R5	K4
      0x8C140B05,  //  0008  GETMET	R5	R5	K5
      0x601C0008,  //  0009  GETGBL	R7	G8
      0x5C200800,  //  000A  MOVE	R8	R4
      0x7C1C0200,  //  000B  CALL	R7	1
      0x001E0C07,  //  000C  ADD	R7	K6	R7
      0x58200007,  //  000D  LDCONST	R8	K7
      0x7C140600,  //  000E  CALL	R5	3
      0x50140000,  //  000F  LDBOOL	R5	0	0
      0x80040A00,  //  0010  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_status_response
********************************************************************/
be_local_closure(Matter_IM_process_status_response,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(findsubval),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(find_sendqueue_by_exchangeid),
    /* K4   */  be_nested_str_weak(exchange_id),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(SUCCESS),
    /* K7   */  be_nested_str_weak(status_ok_received),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(format),
    /* K11  */  be_nested_str_weak(MTR_X3A_X20_X3EOK_X20_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20exch_X3D_X25i_X20not_X20found),
    /* K12  */  be_nested_str_weak(session),
    /* K13  */  be_nested_str_weak(local_session_id),
    /* K14  */  be_const_int(3),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20_X3EStatus_X20_X20_X20_X20ERROR_X20_X3D_X200x_X2502X),
    /* K16  */  be_const_int(2),
    /* K17  */  be_nested_str_weak(status_error_received),
    /* K18  */  be_nested_str_weak(remove_sendqueue_by_exchangeid),
    }),
    be_str_weak(process_status_response),
    &be_const_str_solidified,
    ( &(const binstruction[46]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x8C100501,  //  0001  GETMET	R4	R2	K1
      0x58180002,  //  0002  LDCONST	R6	K2
      0x541E00FE,  //  0003  LDINT	R7	255
      0x7C100600,  //  0004  CALL	R4	3
      0x8C140103,  //  0005  GETMET	R5	R0	K3
      0x881C0304,  //  0006  GETMBR	R7	R1	K4
      0x7C140400,  //  0007  CALL	R5	2
      0xB81A0A00,  //  0008  GETNGBL	R6	K5
      0x88180D06,  //  0009  GETMBR	R6	R6	K6
      0x1C180806,  //  000A  EQ	R6	R4	R6
      0x781A0010,  //  000B  JMPF	R6	#001D
      0x78160004,  //  000C  JMPF	R5	#0012
      0x8C180B07,  //  000D  GETMET	R6	R5	K7
      0x5C200200,  //  000E  MOVE	R8	R1
      0x7C180400,  //  000F  CALL	R6	2
      0x80040C00,  //  0010  RET	1	R6
      0x70020009,  //  0011  JMP		#001C
      0xB81A1000,  //  0012  GETNGBL	R6	K8
      0x8C180D09,  //  0013  GETMET	R6	R6	K9
      0x8C20070A,  //  0014  GETMET	R8	R3	K10
      0x5828000B,  //  0015  LDCONST	R10	K11
      0x882C030C,  //  0016  GETMBR	R11	R1	K12
      0x882C170D,  //  0017  GETMBR	R11	R11	K13
      0x88300304,  //  0018  GETMBR	R12	R1	K4
      0x7C200800,  //  0019  CALL	R8	4
      0x5824000E,  //  001A  LDCONST	R9	K14
      0x7C180600,  //  001B  CALL	R6	3
      0x7002000E,  //  001C  JMP		#002C
      0xB81A1000,  //  001D  GETNGBL	R6	K8
      0x8C180D09,  //  001E  GETMET	R6	R6	K9
      0x8C20070A,  //  001F  GETMET	R8	R3	K10
      0x5828000F,  //  0020  LDCONST	R10	K15
      0x5C2C0800,  //  0021  MOVE	R11	R4
      0x7C200600,  //  0022  CALL	R8	3
      0x58240010,  //  0023  LDCONST	R9	K16
      0x7C180600,  //  0024  CALL	R6	3
      0x78160005,  //  0025  JMPF	R5	#002C
      0x8C180B11,  //  0026  GETMET	R6	R5	K17
      0x5C200200,  //  0027  MOVE	R8	R1
      0x7C180400,  //  0028  CALL	R6	2
      0x8C180112,  //  0029  GETMET	R6	R0	K18
      0x88200304,  //  002A  GETMBR	R8	R1	K4
      0x7C180400,  //  002B  CALL	R6	2
      0x50180000,  //  002C  LDBOOL	R6	0	0
      0x80040C00,  //  002D  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_write_request
********************************************************************/
be_local_closure(Matter_IM_process_write_request,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        19,                          /* nstack */
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
        /* K0   */  be_nested_str_weak(string),
        /* K1   */  be_nested_str_weak(matter),
        /* K2   */  be_nested_str_weak(get_attribute_name),
        /* K3   */  be_nested_str_weak(cluster),
        /* K4   */  be_nested_str_weak(attribute),
        /* K5   */  be_nested_str_weak(_X20_X28),
        /* K6   */  be_nested_str_weak(_X29),
        /* K7   */  be_nested_str_weak(),
        /* K8   */  be_nested_str_weak(status),
        /* K9   */  be_nested_str_weak(UNSUPPORTED_WRITE),
        /* K10  */  be_nested_str_weak(write_attribute),
        /* K11  */  be_nested_str_weak(session),
        /* K12  */  be_nested_str_weak(SUCCESS),
        /* K13  */  be_nested_str_weak(AttributeStatusIB),
        /* K14  */  be_nested_str_weak(path),
        /* K15  */  be_nested_str_weak(AttributePathIB),
        /* K16  */  be_nested_str_weak(StatusIB),
        /* K17  */  be_nested_str_weak(endpoint),
        /* K18  */  be_nested_str_weak(write_responses),
        /* K19  */  be_nested_str_weak(push),
        /* K20  */  be_nested_str_weak(tasmota),
        /* K21  */  be_nested_str_weak(log),
        /* K22  */  be_nested_str_weak(format),
        /* K23  */  be_nested_str_weak(MTR_X3A_X20Write_Attr_X20_X25s_X25s_X20_X2D_X20STATUS_X3A_X200x_X2502X_X20_X25s),
        /* K24  */  be_const_int(2),
        /* K25  */  be_nested_str_weak(MTR_X3A_X20Write_Attr_X20_X25s_X25s_X20_X2D_X20IGNORED),
        }),
        be_str_weak(write_single_attribute),
        &be_const_str_solidified,
        ( &(const binstruction[97]) {  /* code */
          0xA4160000,  //  0000  IMPORT	R5	K0
          0xB81A0200,  //  0001  GETNGBL	R6	K1
          0x8C180D02,  //  0002  GETMET	R6	R6	K2
          0x88200503,  //  0003  GETMBR	R8	R2	K3
          0x88240504,  //  0004  GETMBR	R9	R2	K4
          0x7C180600,  //  0005  CALL	R6	3
          0x781A0002,  //  0006  JMPF	R6	#000A
          0x001E0A06,  //  0007  ADD	R7	K5	R6
          0x001C0F06,  //  0008  ADD	R7	R7	K6
          0x70020000,  //  0009  JMP		#000B
          0x581C0007,  //  000A  LDCONST	R7	K7
          0x5C180E00,  //  000B  MOVE	R6	R7
          0xB81E0200,  //  000C  GETNGBL	R7	K1
          0x881C0F09,  //  000D  GETMBR	R7	R7	K9
          0x900A1007,  //  000E  SETMBR	R2	K8	R7
          0x4C1C0000,  //  000F  LDNIL	R7
          0x201C0207,  //  0010  NE	R7	R1	R7
          0x781E0006,  //  0011  JMPF	R7	#0019
          0x8C1C030A,  //  0012  GETMET	R7	R1	K10
          0x68240000,  //  0013  GETUPV	R9	U0
          0x8824130B,  //  0014  GETMBR	R9	R9	K11
          0x5C280400,  //  0015  MOVE	R10	R2
          0x5C2C0600,  //  0016  MOVE	R11	R3
          0x7C1C0800,  //  0017  CALL	R7	4
          0x70020000,  //  0018  JMP		#001A
          0x4C1C0000,  //  0019  LDNIL	R7
          0x781E0002,  //  001A  JMPF	R7	#001E
          0xB8220200,  //  001B  GETNGBL	R8	K1
          0x8820110C,  //  001C  GETMBR	R8	R8	K12
          0x900A1008,  //  001D  SETMBR	R2	K8	R8
          0x88200508,  //  001E  GETMBR	R8	R2	K8
          0x4C240000,  //  001F  LDNIL	R9
          0x20201009,  //  0020  NE	R8	R8	R9
          0x78220032,  //  0021  JMPF	R8	#0055
          0x78120030,  //  0022  JMPF	R4	#0054
          0xB8220200,  //  0023  GETNGBL	R8	K1
          0x8C20110D,  //  0024  GETMET	R8	R8	K13
          0x7C200200,  //  0025  CALL	R8	1
          0xB8260200,  //  0026  GETNGBL	R9	K1
          0x8C24130F,  //  0027  GETMET	R9	R9	K15
          0x7C240200,  //  0028  CALL	R9	1
          0x90221C09,  //  0029  SETMBR	R8	K14	R9
          0xB8260200,  //  002A  GETNGBL	R9	K1
          0x8C241310,  //  002B  GETMET	R9	R9	K16
          0x7C240200,  //  002C  CALL	R9	1
          0x90221009,  //  002D  SETMBR	R8	K8	R9
          0x8824110E,  //  002E  GETMBR	R9	R8	K14
          0x88280511,  //  002F  GETMBR	R10	R2	K17
          0x9026220A,  //  0030  SETMBR	R9	K17	R10
          0x8824110E,  //  0031  GETMBR	R9	R8	K14
          0x88280503,  //  0032  GETMBR	R10	R2	K3
          0x9026060A,  //  0033  SETMBR	R9	K3	R10
          0x8824110E,  //  0034  GETMBR	R9	R8	K14
          0x88280504,  //  0035  GETMBR	R10	R2	K4
          0x9026080A,  //  0036  SETMBR	R9	K4	R10
          0x88241108,  //  0037  GETMBR	R9	R8	K8
          0x88280508,  //  0038  GETMBR	R10	R2	K8
          0x9026100A,  //  0039  SETMBR	R9	K8	R10
          0x88240112,  //  003A  GETMBR	R9	R0	K18
          0x8C241313,  //  003B  GETMET	R9	R9	K19
          0x5C2C1000,  //  003C  MOVE	R11	R8
          0x7C240400,  //  003D  CALL	R9	2
          0xB8262800,  //  003E  GETNGBL	R9	K20
          0x8C241315,  //  003F  GETMET	R9	R9	K21
          0x8C2C0B16,  //  0040  GETMET	R11	R5	K22
          0x58340017,  //  0041  LDCONST	R13	K23
          0x60380008,  //  0042  GETGBL	R14	G8
          0x5C3C0400,  //  0043  MOVE	R15	R2
          0x7C380200,  //  0044  CALL	R14	1
          0x5C3C0C00,  //  0045  MOVE	R15	R6
          0x88400508,  //  0046  GETMBR	R16	R2	K8
          0x88440508,  //  0047  GETMBR	R17	R2	K8
          0xB84A0200,  //  0048  GETNGBL	R18	K1
          0x8848250C,  //  0049  GETMBR	R18	R18	K12
          0x1C442212,  //  004A  EQ	R17	R17	R18
          0x78460001,  //  004B  JMPF	R17	#004E
          0x5844000C,  //  004C  LDCONST	R17	K12
          0x70020000,  //  004D  JMP		#004F
          0x58440007,  //  004E  LDCONST	R17	K7
          0x7C2C0C00,  //  004F  CALL	R11	6
          0x58300018,  //  0050  LDCONST	R12	K24
          0x7C240600,  //  0051  CALL	R9	3
          0x50240200,  //  0052  LDBOOL	R9	1	0
          0x80041200,  //  0053  RET	1	R9
          0x7002000A,  //  0054  JMP		#0060
          0xB8222800,  //  0055  GETNGBL	R8	K20
          0x8C201115,  //  0056  GETMET	R8	R8	K21
          0x8C280B16,  //  0057  GETMET	R10	R5	K22
          0x58300019,  //  0058  LDCONST	R12	K25
          0x60340008,  //  0059  GETGBL	R13	G8
          0x5C380400,  //  005A  MOVE	R14	R2
          0x7C340200,  //  005B  CALL	R13	1
          0x5C380C00,  //  005C  MOVE	R14	R6
          0x7C280800,  //  005D  CALL	R10	4
          0x582C0018,  //  005E  LDCONST	R11	K24
          0x7C200600,  //  005F  CALL	R8	3
          0x80000000,  //  0060  RET	0
        })
      ),
      be_nested_proto(
        9,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 3]) {  /* upvals */
          be_local_const_upval(1, 7),
          be_local_const_upval(1, 9),
          be_local_const_upval(1, 13),
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
    ( &(const bvalue[36]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(WriteRequestMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20received_X20WriteRequestMessage_X3D),
    /* K7   */  be_const_int(3),
    /* K8   */  be_nested_str_weak(suppress_response),
    /* K9   */  be_nested_str_weak(device),
    /* K10  */  be_nested_str_weak(get_active_endpoints),
    /* K11  */  be_nested_str_weak(MTR_X3A_X20IM_X3Awrite_request_X20processing_X20start),
    /* K12  */  be_nested_str_weak(Path),
    /* K13  */  be_nested_str_weak(write_requests),
    /* K14  */  be_nested_str_weak(WriteResponseMessage),
    /* K15  */  be_nested_str_weak(write_responses),
    /* K16  */  be_nested_str_weak(path),
    /* K17  */  be_nested_str_weak(data),
    /* K18  */  be_nested_str_weak(endpoint),
    /* K19  */  be_nested_str_weak(cluster),
    /* K20  */  be_nested_str_weak(attribute),
    /* K21  */  be_nested_str_weak(status),
    /* K22  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K23  */  be_nested_str_weak(INVALID_ACTION),
    /* K24  */  be_nested_str_weak(get_attribute_name),
    /* K25  */  be_nested_str_weak(MTR_X3A_X20Write_Attr_X20),
    /* K26  */  be_nested_str_weak(_X20_X28),
    /* K27  */  be_nested_str_weak(_X29),
    /* K28  */  be_nested_str_weak(),
    /* K29  */  be_const_int(2),
    /* K30  */  be_nested_str_weak(process_attribute_expansion),
    /* K31  */  be_nested_str_weak(stop_iteration),
    /* K32  */  be_nested_str_weak(MTR_X3A_X20ReportWriteMessage_X3D),
    /* K33  */  be_nested_str_weak(MTR_X3A_X20ReportWriteMessageTLV_X3D),
    /* K34  */  be_nested_str_weak(to_TLV),
    /* K35  */  be_nested_str_weak(send_write_response),
    }),
    be_str_weak(process_write_request),
    &be_const_str_solidified,
    ( &(const binstruction[134]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0xB8160800,  //  0007  GETNGBL	R5	K4
      0x8C140B05,  //  0008  GETMET	R5	R5	K5
      0x601C0008,  //  0009  GETGBL	R7	G8
      0x5C200800,  //  000A  MOVE	R8	R4
      0x7C1C0200,  //  000B  CALL	R7	1
      0x001E0C07,  //  000C  ADD	R7	K6	R7
      0x58200007,  //  000D  LDCONST	R8	K7
      0x7C140600,  //  000E  CALL	R5	3
      0x88140908,  //  000F  GETMBR	R5	R4	K8
      0x88180109,  //  0010  GETMBR	R6	R0	K9
      0x8C180D0A,  //  0011  GETMET	R6	R6	K10
      0x7C180200,  //  0012  CALL	R6	1
      0x841C0000,  //  0013  CLOSURE	R7	P0
      0xB8220800,  //  0014  GETNGBL	R8	K4
      0x8C201105,  //  0015  GETMET	R8	R8	K5
      0x5828000B,  //  0016  LDCONST	R10	K11
      0x542E0003,  //  0017  LDINT	R11	4
      0x7C200600,  //  0018  CALL	R8	3
      0xB8220200,  //  0019  GETNGBL	R8	K1
      0x8C20110C,  //  001A  GETMET	R8	R8	K12
      0x7C200200,  //  001B  CALL	R8	1
      0x8824090D,  //  001C  GETMBR	R9	R4	K13
      0x4C280000,  //  001D  LDNIL	R10
      0x2024120A,  //  001E  NE	R9	R9	R10
      0x78260062,  //  001F  JMPF	R9	#0083
      0xB8260200,  //  0020  GETNGBL	R9	K1
      0x8C24130E,  //  0021  GETMET	R9	R9	K14
      0x7C240200,  //  0022  CALL	R9	1
      0x60280012,  //  0023  GETGBL	R10	G18
      0x7C280000,  //  0024  CALL	R10	0
      0x90261E0A,  //  0025  SETMBR	R9	K15	R10
      0x60280010,  //  0026  GETGBL	R10	G16
      0x882C090D,  //  0027  GETMBR	R11	R4	K13
      0x7C280200,  //  0028  CALL	R10	1
      0xA802003D,  //  0029  EXBLK	0	#0068
      0x5C2C1400,  //  002A  MOVE	R11	R10
      0x7C2C0000,  //  002B  CALL	R11	0
      0x88301710,  //  002C  GETMBR	R12	R11	K16
      0x88341711,  //  002D  GETMBR	R13	R11	K17
      0x88381912,  //  002E  GETMBR	R14	R12	K18
      0x9022240E,  //  002F  SETMBR	R8	K18	R14
      0x88381913,  //  0030  GETMBR	R14	R12	K19
      0x9022260E,  //  0031  SETMBR	R8	K19	R14
      0x88381914,  //  0032  GETMBR	R14	R12	K20
      0x9022280E,  //  0033  SETMBR	R8	K20	R14
      0xB83A0200,  //  0034  GETNGBL	R14	K1
      0x88381D16,  //  0035  GETMBR	R14	R14	K22
      0x90222A0E,  //  0036  SETMBR	R8	K21	R14
      0x88381113,  //  0037  GETMBR	R14	R8	K19
      0x4C3C0000,  //  0038  LDNIL	R15
      0x1C381C0F,  //  0039  EQ	R14	R14	R15
      0x743A0003,  //  003A  JMPT	R14	#003F
      0x88381114,  //  003B  GETMBR	R14	R8	K20
      0x4C3C0000,  //  003C  LDNIL	R15
      0x1C381C0F,  //  003D  EQ	R14	R14	R15
      0x783A000A,  //  003E  JMPF	R14	#004A
      0xB83A0200,  //  003F  GETNGBL	R14	K1
      0x88381D17,  //  0040  GETMBR	R14	R14	K23
      0x90222A0E,  //  0041  SETMBR	R8	K21	R14
      0x5C380E00,  //  0042  MOVE	R14	R7
      0x5C3C1200,  //  0043  MOVE	R15	R9
      0x4C400000,  //  0044  LDNIL	R16
      0x5C441000,  //  0045  MOVE	R17	R8
      0x4C480000,  //  0046  LDNIL	R18
      0x504C0200,  //  0047  LDBOOL	R19	1	0
      0x7C380A00,  //  0048  CALL	R14	5
      0x7001FFDF,  //  0049  JMP		#002A
      0x88381112,  //  004A  GETMBR	R14	R8	K18
      0x4C3C0000,  //  004B  LDNIL	R15
      0x1C381C0F,  //  004C  EQ	R14	R14	R15
      0x783A0012,  //  004D  JMPF	R14	#0061
      0xB83A0200,  //  004E  GETNGBL	R14	K1
      0x8C381D18,  //  004F  GETMET	R14	R14	K24
      0x88401113,  //  0050  GETMBR	R16	R8	K19
      0x88441114,  //  0051  GETMBR	R17	R8	K20
      0x7C380600,  //  0052  CALL	R14	3
      0xB83E0800,  //  0053  GETNGBL	R15	K4
      0x8C3C1F05,  //  0054  GETMET	R15	R15	K5
      0x60440008,  //  0055  GETGBL	R17	G8
      0x5C481000,  //  0056  MOVE	R18	R8
      0x7C440200,  //  0057  CALL	R17	1
      0x00463211,  //  0058  ADD	R17	K25	R17
      0x783A0002,  //  0059  JMPF	R14	#005D
      0x004A340E,  //  005A  ADD	R18	K26	R14
      0x0048251B,  //  005B  ADD	R18	R18	K27
      0x70020000,  //  005C  JMP		#005E
      0x5848001C,  //  005D  LDCONST	R18	K28
      0x00442212,  //  005E  ADD	R17	R17	R18
      0x5848001D,  //  005F  LDCONST	R18	K29
      0x7C3C0600,  //  0060  CALL	R15	3
      0x88380109,  //  0061  GETMBR	R14	R0	K9
      0x8C381D1E,  //  0062  GETMET	R14	R14	K30
      0x5C401000,  //  0063  MOVE	R16	R8
      0x84440001,  //  0064  CLOSURE	R17	P1
      0x7C380600,  //  0065  CALL	R14	3
      0xA0280000,  //  0066  CLOSE	R10
      0x7001FFC1,  //  0067  JMP		#002A
      0x5828001F,  //  0068  LDCONST	R10	K31
      0xAC280200,  //  0069  CATCH	R10	1	0
      0xB0080000,  //  006A  RAISE	2	R0	R0
      0xB82A0800,  //  006B  GETNGBL	R10	K4
      0x8C281505,  //  006C  GETMET	R10	R10	K5
      0x60300008,  //  006D  GETGBL	R12	G8
      0x5C341200,  //  006E  MOVE	R13	R9
      0x7C300200,  //  006F  CALL	R12	1
      0x0032400C,  //  0070  ADD	R12	K32	R12
      0x54360003,  //  0071  LDINT	R13	4
      0x7C280600,  //  0072  CALL	R10	3
      0xB82A0800,  //  0073  GETNGBL	R10	K4
      0x8C281505,  //  0074  GETMET	R10	R10	K5
      0x60300008,  //  0075  GETGBL	R12	G8
      0x8C341322,  //  0076  GETMET	R13	R9	K34
      0x7C340200,  //  0077  CALL	R13	1
      0x7C300200,  //  0078  CALL	R12	1
      0x0032420C,  //  0079  ADD	R12	K33	R12
      0x58340007,  //  007A  LDCONST	R13	K7
      0x7C280600,  //  007B  CALL	R10	3
      0x5C280A00,  //  007C  MOVE	R10	R5
      0x742A0003,  //  007D  JMPT	R10	#0082
      0x8C280123,  //  007E  GETMET	R10	R0	K35
      0x5C300200,  //  007F  MOVE	R12	R1
      0x5C341200,  //  0080  MOVE	R13	R9
      0x7C280600,  //  0081  CALL	R10	3
      0xA0240000,  //  0082  CLOSE	R9
      0x50240200,  //  0083  LDBOOL	R9	1	0
      0xA0000000,  //  0084  CLOSE	R0
      0x80041200,  //  0085  RET	1	R9
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
** Solidified function: process_invoke_response
********************************************************************/
be_local_closure(Matter_IM_process_invoke_response,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(InvokeResponseMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20received_X20InvokeResponseMessage_X3D),
    /* K7   */  be_const_int(2),
    }),
    be_str_weak(process_invoke_response),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0xB8160800,  //  0007  GETNGBL	R5	K4
      0x8C140B05,  //  0008  GETMET	R5	R5	K5
      0x601C0008,  //  0009  GETGBL	R7	G8
      0x5C200800,  //  000A  MOVE	R8	R4
      0x7C1C0200,  //  000B  CALL	R7	1
      0x001E0C07,  //  000C  ADD	R7	K6	R7
      0x58200007,  //  000D  LDCONST	R8	K7
      0x7C140600,  //  000E  CALL	R5	3
      0x50140000,  //  000F  LDBOOL	R5	0	0
      0x80040A00,  //  0010  RET	1	R5
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
** Solidified function: subscribe_response
********************************************************************/
be_local_closure(Matter_IM_subscribe_response,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(SubscribeResponseMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20received_X20SubscribeResponsetMessage_X3D),
    /* K7   */  be_const_int(2),
    }),
    be_str_weak(subscribe_response),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0xB8160800,  //  0007  GETNGBL	R5	K4
      0x8C140B05,  //  0008  GETMET	R5	R5	K5
      0x601C0008,  //  0009  GETGBL	R7	G8
      0x5C200800,  //  000A  MOVE	R8	R4
      0x7C1C0200,  //  000B  CALL	R7	1
      0x001E0C07,  //  000C  ADD	R7	K6	R7
      0x58200007,  //  000D  LDCONST	R8	K7
      0x7C140600,  //  000E  CALL	R5	3
      0x50140000,  //  000F  LDBOOL	R5	0	0
      0x80040A00,  //  0010  RET	1	R5
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
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(send_queue),
    /* K2   */  be_nested_str_weak(send_im),
    /* K3   */  be_nested_str_weak(remove),
    /* K4   */  be_const_int(1),
    }),
    be_str_weak(send_enqueued),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E000C,  //  0005  JMPF	R3	#0013
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x8C100702,  //  0008  GETMET	R4	R3	K2
      0x5C180200,  //  0009  MOVE	R6	R1
      0x7C100400,  //  000A  CALL	R4	2
      0x78120004,  //  000B  JMPF	R4	#0011
      0x88140101,  //  000C  GETMBR	R5	R0	K1
      0x8C140B03,  //  000D  GETMET	R5	R5	K3
      0x5C1C0400,  //  000E  MOVE	R7	R2
      0x7C140400,  //  000F  CALL	R5	2
      0x04080504,  //  0010  SUB	R2	R2	K4
      0x00080504,  //  0011  ADD	R2	R2	K4
      0x7001FFED,  //  0012  JMP		#0001
      0x80000000,  //  0013  RET	0
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
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(send_queue),
    /* K2   */  be_nested_str_weak(subs_shop),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(IM_Subscription_Shop),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x90020202,  //  0003  SETMBR	R0	K1	R2
      0xB80A0600,  //  0004  GETNGBL	R2	K3
      0x8C080504,  //  0005  GETMET	R2	R2	K4
      0x5C100000,  //  0006  MOVE	R4	R0
      0x7C080400,  //  0007  CALL	R2	2
      0x90020402,  //  0008  SETMBR	R0	K2	R2
      0x80000000,  //  0009  RET	0
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
** Solidified function: process_incoming
********************************************************************/
be_local_closure(Matter_IM_process_incoming,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[25]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(MTR_X3A_X20received_X20IM_X20message_X20),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(inspect),
    /* K5   */  be_const_int(3),
    /* K6   */  be_nested_str_weak(TLV),
    /* K7   */  be_nested_str_weak(parse),
    /* K8   */  be_nested_str_weak(raw),
    /* K9   */  be_nested_str_weak(app_payload_idx),
    /* K10  */  be_nested_str_weak(findsubval),
    /* K11  */  be_nested_str_weak(opcode),
    /* K12  */  be_const_int(1),
    /* K13  */  be_nested_str_weak(process_status_response),
    /* K14  */  be_const_int(2),
    /* K15  */  be_nested_str_weak(send_ack_now),
    /* K16  */  be_nested_str_weak(process_read_request),
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
    ( &(const binstruction[119]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0xB8120600,  //  0002  GETNGBL	R4	K3
      0x8C100904,  //  0003  GETMET	R4	R4	K4
      0x5C180200,  //  0004  MOVE	R6	R1
      0x7C100400,  //  0005  CALL	R4	2
      0x00120404,  //  0006  ADD	R4	K2	R4
      0x58140005,  //  0007  LDCONST	R5	K5
      0x7C080600,  //  0008  CALL	R2	3
      0xB80A0600,  //  0009  GETNGBL	R2	K3
      0x88080506,  //  000A  GETMBR	R2	R2	K6
      0x8C080507,  //  000B  GETMET	R2	R2	K7
      0x88100308,  //  000C  GETMBR	R4	R1	K8
      0x88140309,  //  000D  GETMBR	R5	R1	K9
      0x7C080600,  //  000E  CALL	R2	3
      0x8C0C050A,  //  000F  GETMET	R3	R2	K10
      0x541600FE,  //  0010  LDINT	R5	255
      0x7C0C0400,  //  0011  CALL	R3	2
      0x8810030B,  //  0012  GETMBR	R4	R1	K11
      0x1C14090C,  //  0013  EQ	R5	R4	K12
      0x78160005,  //  0014  JMPF	R5	#001B
      0x8C14010D,  //  0015  GETMET	R5	R0	K13
      0x5C1C0200,  //  0016  MOVE	R7	R1
      0x5C200400,  //  0017  MOVE	R8	R2
      0x7C140600,  //  0018  CALL	R5	3
      0x80040A00,  //  0019  RET	1	R5
      0x70020059,  //  001A  JMP		#0075
      0x1C14090E,  //  001B  EQ	R5	R4	K14
      0x78160008,  //  001C  JMPF	R5	#0026
      0x8C14010F,  //  001D  GETMET	R5	R0	K15
      0x5C1C0200,  //  001E  MOVE	R7	R1
      0x7C140400,  //  001F  CALL	R5	2
      0x8C140110,  //  0020  GETMET	R5	R0	K16
      0x5C1C0200,  //  0021  MOVE	R7	R1
      0x5C200400,  //  0022  MOVE	R8	R2
      0x7C140600,  //  0023  CALL	R5	3
      0x80040A00,  //  0024  RET	1	R5
      0x7002004E,  //  0025  JMP		#0075
      0x1C140905,  //  0026  EQ	R5	R4	K5
      0x78160008,  //  0027  JMPF	R5	#0031
      0x8C14010F,  //  0028  GETMET	R5	R0	K15
      0x5C1C0200,  //  0029  MOVE	R7	R1
      0x7C140400,  //  002A  CALL	R5	2
      0x8C140111,  //  002B  GETMET	R5	R0	K17
      0x5C1C0200,  //  002C  MOVE	R7	R1
      0x5C200400,  //  002D  MOVE	R8	R2
      0x7C140600,  //  002E  CALL	R5	3
      0x80040A00,  //  002F  RET	1	R5
      0x70020043,  //  0030  JMP		#0075
      0x54160003,  //  0031  LDINT	R5	4
      0x1C140805,  //  0032  EQ	R5	R4	R5
      0x78160005,  //  0033  JMPF	R5	#003A
      0x8C140112,  //  0034  GETMET	R5	R0	K18
      0x5C1C0200,  //  0035  MOVE	R7	R1
      0x5C200400,  //  0036  MOVE	R8	R2
      0x7C140600,  //  0037  CALL	R5	3
      0x80040A00,  //  0038  RET	1	R5
      0x7002003A,  //  0039  JMP		#0075
      0x54160004,  //  003A  LDINT	R5	5
      0x1C140805,  //  003B  EQ	R5	R4	R5
      0x78160005,  //  003C  JMPF	R5	#0043
      0x8C140113,  //  003D  GETMET	R5	R0	K19
      0x5C1C0200,  //  003E  MOVE	R7	R1
      0x5C200400,  //  003F  MOVE	R8	R2
      0x7C140600,  //  0040  CALL	R5	3
      0x80040A00,  //  0041  RET	1	R5
      0x70020031,  //  0042  JMP		#0075
      0x54160005,  //  0043  LDINT	R5	6
      0x1C140805,  //  0044  EQ	R5	R4	R5
      0x78160008,  //  0045  JMPF	R5	#004F
      0x8C14010F,  //  0046  GETMET	R5	R0	K15
      0x5C1C0200,  //  0047  MOVE	R7	R1
      0x7C140400,  //  0048  CALL	R5	2
      0x8C140114,  //  0049  GETMET	R5	R0	K20
      0x5C1C0200,  //  004A  MOVE	R7	R1
      0x5C200400,  //  004B  MOVE	R8	R2
      0x7C140600,  //  004C  CALL	R5	3
      0x80040A00,  //  004D  RET	1	R5
      0x70020025,  //  004E  JMP		#0075
      0x54160006,  //  004F  LDINT	R5	7
      0x1C140805,  //  0050  EQ	R5	R4	R5
      0x78160005,  //  0051  JMPF	R5	#0058
      0x8C140115,  //  0052  GETMET	R5	R0	K21
      0x5C1C0200,  //  0053  MOVE	R7	R1
      0x5C200400,  //  0054  MOVE	R8	R2
      0x7C140600,  //  0055  CALL	R5	3
      0x80040A00,  //  0056  RET	1	R5
      0x7002001C,  //  0057  JMP		#0075
      0x54160007,  //  0058  LDINT	R5	8
      0x1C140805,  //  0059  EQ	R5	R4	R5
      0x78160008,  //  005A  JMPF	R5	#0064
      0x8C14010F,  //  005B  GETMET	R5	R0	K15
      0x5C1C0200,  //  005C  MOVE	R7	R1
      0x7C140400,  //  005D  CALL	R5	2
      0x8C140116,  //  005E  GETMET	R5	R0	K22
      0x5C1C0200,  //  005F  MOVE	R7	R1
      0x5C200400,  //  0060  MOVE	R8	R2
      0x7C140600,  //  0061  CALL	R5	3
      0x80040A00,  //  0062  RET	1	R5
      0x70020010,  //  0063  JMP		#0075
      0x54160008,  //  0064  LDINT	R5	9
      0x1C140805,  //  0065  EQ	R5	R4	R5
      0x78160005,  //  0066  JMPF	R5	#006D
      0x8C140117,  //  0067  GETMET	R5	R0	K23
      0x5C1C0200,  //  0068  MOVE	R7	R1
      0x5C200400,  //  0069  MOVE	R8	R2
      0x7C140600,  //  006A  CALL	R5	3
      0x80040A00,  //  006B  RET	1	R5
      0x70020007,  //  006C  JMP		#0075
      0x54160009,  //  006D  LDINT	R5	10
      0x1C140805,  //  006E  EQ	R5	R4	R5
      0x78160004,  //  006F  JMPF	R5	#0075
      0x8C140118,  //  0070  GETMET	R5	R0	K24
      0x5C1C0200,  //  0071  MOVE	R7	R1
      0x5C200400,  //  0072  MOVE	R8	R2
      0x7C140600,  //  0073  CALL	R5	3
      0x80040A00,  //  0074  RET	1	R5
      0x50140000,  //  0075  LDBOOL	R5	0	0
      0x80040A00,  //  0076  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_timed_request
********************************************************************/
be_local_closure(Matter_IM_process_timed_request,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TimedRequestMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20received_X20TimedRequestMessage_X3D),
    /* K7   */  be_const_int(3),
    /* K8   */  be_nested_str_weak(format),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20_X3ECommand_X20_X20_X20_X28_X256i_X29_X20TimedRequest_X3D_X25i),
    /* K10  */  be_nested_str_weak(session),
    /* K11  */  be_nested_str_weak(local_session_id),
    /* K12  */  be_nested_str_weak(timeout),
    /* K13  */  be_const_int(2),
    /* K14  */  be_nested_str_weak(send_status),
    /* K15  */  be_nested_str_weak(SUCCESS),
    }),
    be_str_weak(process_timed_request),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0xB8160800,  //  0007  GETNGBL	R5	K4
      0x8C140B05,  //  0008  GETMET	R5	R5	K5
      0x601C0008,  //  0009  GETGBL	R7	G8
      0x5C200800,  //  000A  MOVE	R8	R4
      0x7C1C0200,  //  000B  CALL	R7	1
      0x001E0C07,  //  000C  ADD	R7	K6	R7
      0x58200007,  //  000D  LDCONST	R8	K7
      0x7C140600,  //  000E  CALL	R5	3
      0xB8160800,  //  000F  GETNGBL	R5	K4
      0x8C140B05,  //  0010  GETMET	R5	R5	K5
      0x8C1C0708,  //  0011  GETMET	R7	R3	K8
      0x58240009,  //  0012  LDCONST	R9	K9
      0x8828030A,  //  0013  GETMBR	R10	R1	K10
      0x8828150B,  //  0014  GETMBR	R10	R10	K11
      0x882C090C,  //  0015  GETMBR	R11	R4	K12
      0x7C1C0800,  //  0016  CALL	R7	4
      0x5820000D,  //  0017  LDCONST	R8	K13
      0x7C140600,  //  0018  CALL	R5	3
      0x8C14010E,  //  0019  GETMET	R5	R0	K14
      0x5C1C0200,  //  001A  MOVE	R7	R1
      0xB8220200,  //  001B  GETNGBL	R8	K1
      0x8820110F,  //  001C  GETMBR	R8	R8	K15
      0x7C140600,  //  001D  CALL	R5	3
      0x50140200,  //  001E  LDBOOL	R5	1	0
      0x80040A00,  //  001F  RET	1	R5
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
** Solidified function: process_invoke_request
********************************************************************/
be_local_closure(Matter_IM_process_invoke_request,   /* name */
  be_nested_proto(
    21,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[50]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20IM_X3Ainvoke_request_X20processing_X20start),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(Path),
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
    /* K23  */  be_nested_str_weak(_X28),
    /* K24  */  be_nested_str_weak(_X29_X20),
    /* K25  */  be_nested_str_weak(),
    /* K26  */  be_nested_str_weak(format),
    /* K27  */  be_nested_str_weak(MTR_X3A_X20_X3ECommand_X20_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s_X20_X25s),
    /* K28  */  be_nested_str_weak(local_session_id),
    /* K29  */  be_const_int(2),
    /* K30  */  be_nested_str_weak(InvokeResponseIB),
    /* K31  */  be_nested_str_weak(SUCCESS),
    /* K32  */  be_nested_str_weak(CommandStatusIB),
    /* K33  */  be_nested_str_weak(CommandPathIB),
    /* K34  */  be_nested_str_weak(StatusIB),
    /* K35  */  be_nested_str_weak(push),
    /* K36  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20OK_X20exch_X3D_X25i),
    /* K37  */  be_nested_str_weak(exchange_id),
    /* K38  */  be_nested_str_weak(CommandDataIB),
    /* K39  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20_X25s_X20_X25s),
    /* K40  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20Status_X3D0x_X2502X_X20exch_X3D_X25i),
    /* K41  */  be_nested_str_weak(MTR_X3A_X20_Ignore_X20_X20_X20_X20_X28_X256i_X29_X20exch_X3D_X25i),
    /* K42  */  be_nested_str_weak(stop_iteration),
    /* K43  */  be_nested_str_weak(MTR_X3A_X20invoke_responses_X3D),
    /* K44  */  be_const_int(0),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20InvokeResponse_X3D),
    /* K46  */  be_nested_str_weak(MTR_X3A_X20InvokeResponseTLV_X3D),
    /* K47  */  be_nested_str_weak(to_TLV),
    /* K48  */  be_const_int(3),
    /* K49  */  be_nested_str_weak(send_invoke_response),
    }),
    be_str_weak(process_invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[300]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x58180003,  //  0003  LDCONST	R6	K3
      0x541E0003,  //  0004  LDINT	R7	4
      0x7C100600,  //  0005  CALL	R4	3
      0xB8120800,  //  0006  GETNGBL	R4	K4
      0x8C100905,  //  0007  GETMET	R4	R4	K5
      0x7C100200,  //  0008  CALL	R4	1
      0xB8160800,  //  0009  GETNGBL	R5	K4
      0x8C140B06,  //  000A  GETMET	R5	R5	K6
      0x7C140200,  //  000B  CALL	R5	1
      0x8C140B07,  //  000C  GETMET	R5	R5	K7
      0x5C1C0400,  //  000D  MOVE	R7	R2
      0x7C140400,  //  000E  CALL	R5	2
      0x88180B08,  //  000F  GETMBR	R6	R5	K8
      0x4C1C0000,  //  0010  LDNIL	R7
      0x20180C07,  //  0011  NE	R6	R6	R7
      0x781A0117,  //  0012  JMPF	R6	#012B
      0xB81A0800,  //  0013  GETNGBL	R6	K4
      0x8C180D09,  //  0014  GETMET	R6	R6	K9
      0x7C180200,  //  0015  CALL	R6	1
      0x501C0000,  //  0016  LDBOOL	R7	0	0
      0x901A1407,  //  0017  SETMBR	R6	K10	R7
      0x601C0012,  //  0018  GETGBL	R7	G18
      0x7C1C0000,  //  0019  CALL	R7	0
      0x901A1607,  //  001A  SETMBR	R6	K11	R7
      0x601C0010,  //  001B  GETGBL	R7	G16
      0x88200B08,  //  001C  GETMBR	R8	R5	K8
      0x7C1C0200,  //  001D  CALL	R7	1
      0xA80200E1,  //  001E  EXBLK	0	#0101
      0x5C200E00,  //  001F  MOVE	R8	R7
      0x7C200000,  //  0020  CALL	R8	0
      0x8824110D,  //  0021  GETMBR	R9	R8	K13
      0x8824130C,  //  0022  GETMBR	R9	R9	K12
      0x90121809,  //  0023  SETMBR	R4	K12	R9
      0x8824110D,  //  0024  GETMBR	R9	R8	K13
      0x8824130E,  //  0025  GETMBR	R9	R9	K14
      0x90121C09,  //  0026  SETMBR	R4	K14	R9
      0x8824110D,  //  0027  GETMBR	R9	R8	K13
      0x8824130F,  //  0028  GETMBR	R9	R9	K15
      0x90121E09,  //  0029  SETMBR	R4	K15	R9
      0xB8260800,  //  002A  GETNGBL	R9	K4
      0x88241311,  //  002B  GETMBR	R9	R9	K17
      0x90122009,  //  002C  SETMBR	R4	K16	R9
      0xB8260800,  //  002D  GETNGBL	R9	K4
      0x8C241312,  //  002E  GETMET	R9	R9	K18
      0x882C090E,  //  002F  GETMBR	R11	R4	K14
      0x8830090F,  //  0030  GETMBR	R12	R4	K15
      0x7C240600,  //  0031  CALL	R9	3
      0x88280113,  //  0032  GETMBR	R10	R0	K19
      0x8C281514,  //  0033  GETMET	R10	R10	K20
      0x88300315,  //  0034  GETMBR	R12	R1	K21
      0x88341116,  //  0035  GETMBR	R13	R8	K22
      0x5C380800,  //  0036  MOVE	R14	R4
      0x7C280800,  //  0037  CALL	R10	4
      0x882C0902,  //  0038  GETMBR	R11	R4	K2
      0x4C300000,  //  0039  LDNIL	R12
      0x202C160C,  //  003A  NE	R11	R11	R12
      0x782E0005,  //  003B  JMPF	R11	#0042
      0x602C0008,  //  003C  GETGBL	R11	G8
      0x88300902,  //  003D  GETMBR	R12	R4	K2
      0x7C2C0200,  //  003E  CALL	R11	1
      0x002E2E0B,  //  003F  ADD	R11	K23	R11
      0x002C1718,  //  0040  ADD	R11	R11	K24
      0x70020000,  //  0041  JMP		#0043
      0x582C0019,  //  0042  LDCONST	R11	K25
      0xB8320200,  //  0043  GETNGBL	R12	K1
      0x8C301902,  //  0044  GETMET	R12	R12	K2
      0x8C38071A,  //  0045  GETMET	R14	R3	K26
      0x5840001B,  //  0046  LDCONST	R16	K27
      0x88440315,  //  0047  GETMBR	R17	R1	K21
      0x8844231C,  //  0048  GETMBR	R17	R17	K28
      0x60480008,  //  0049  GETGBL	R18	G8
      0x5C4C0800,  //  004A  MOVE	R19	R4
      0x7C480200,  //  004B  CALL	R18	1
      0x78260001,  //  004C  JMPF	R9	#004F
      0x5C4C1200,  //  004D  MOVE	R19	R9
      0x70020000,  //  004E  JMP		#0050
      0x584C0019,  //  004F  LDCONST	R19	K25
      0x5C501600,  //  0050  MOVE	R20	R11
      0x7C380C00,  //  0051  CALL	R14	6
      0x583C001D,  //  0052  LDCONST	R15	K29
      0x7C300600,  //  0053  CALL	R12	3
      0x4C300000,  //  0054  LDNIL	R12
      0x9012040C,  //  0055  SETMBR	R4	K2	R12
      0xB8320800,  //  0056  GETNGBL	R12	K4
      0x8C30191E,  //  0057  GETMET	R12	R12	K30
      0x7C300200,  //  0058  CALL	R12	1
      0x50340200,  //  0059  LDBOOL	R13	1	0
      0x1C34140D,  //  005A  EQ	R13	R10	R13
      0x74360004,  //  005B  JMPT	R13	#0061
      0x88340910,  //  005C  GETMBR	R13	R4	K16
      0xB83A0800,  //  005D  GETNGBL	R14	K4
      0x88381D1F,  //  005E  GETMBR	R14	R14	K31
      0x1C341A0E,  //  005F  EQ	R13	R13	R14
      0x7836002D,  //  0060  JMPF	R13	#008F
      0xB8360800,  //  0061  GETNGBL	R13	K4
      0x8C341B20,  //  0062  GETMET	R13	R13	K32
      0x7C340200,  //  0063  CALL	R13	1
      0x9032200D,  //  0064  SETMBR	R12	K16	R13
      0x88341910,  //  0065  GETMBR	R13	R12	K16
      0xB83A0800,  //  0066  GETNGBL	R14	K4
      0x8C381D21,  //  0067  GETMET	R14	R14	K33
      0x7C380200,  //  0068  CALL	R14	1
      0x90361A0E,  //  0069  SETMBR	R13	K13	R14
      0x88341910,  //  006A  GETMBR	R13	R12	K16
      0x88341B0D,  //  006B  GETMBR	R13	R13	K13
      0x8838090C,  //  006C  GETMBR	R14	R4	K12
      0x9036180E,  //  006D  SETMBR	R13	K12	R14
      0x88341910,  //  006E  GETMBR	R13	R12	K16
      0x88341B0D,  //  006F  GETMBR	R13	R13	K13
      0x8838090E,  //  0070  GETMBR	R14	R4	K14
      0x90361C0E,  //  0071  SETMBR	R13	K14	R14
      0x88341910,  //  0072  GETMBR	R13	R12	K16
      0x88341B0D,  //  0073  GETMBR	R13	R13	K13
      0x8838090F,  //  0074  GETMBR	R14	R4	K15
      0x90361E0E,  //  0075  SETMBR	R13	K15	R14
      0x88341910,  //  0076  GETMBR	R13	R12	K16
      0xB83A0800,  //  0077  GETNGBL	R14	K4
      0x8C381D22,  //  0078  GETMET	R14	R14	K34
      0x7C380200,  //  0079  CALL	R14	1
      0x9036200E,  //  007A  SETMBR	R13	K16	R14
      0x88341910,  //  007B  GETMBR	R13	R12	K16
      0x88341B10,  //  007C  GETMBR	R13	R13	K16
      0xB83A0800,  //  007D  GETNGBL	R14	K4
      0x88381D1F,  //  007E  GETMBR	R14	R14	K31
      0x9036200E,  //  007F  SETMBR	R13	K16	R14
      0x88340D0B,  //  0080  GETMBR	R13	R6	K11
      0x8C341B23,  //  0081  GETMET	R13	R13	K35
      0x5C3C1800,  //  0082  MOVE	R15	R12
      0x7C340400,  //  0083  CALL	R13	2
      0xB8360200,  //  0084  GETNGBL	R13	K1
      0x8C341B02,  //  0085  GETMET	R13	R13	K2
      0x8C3C071A,  //  0086  GETMET	R15	R3	K26
      0x58440024,  //  0087  LDCONST	R17	K36
      0x88480315,  //  0088  GETMBR	R18	R1	K21
      0x8848251C,  //  0089  GETMBR	R18	R18	K28
      0x884C0325,  //  008A  GETMBR	R19	R1	K37
      0x7C3C0800,  //  008B  CALL	R15	4
      0x5840001D,  //  008C  LDCONST	R16	K29
      0x7C340600,  //  008D  CALL	R13	3
      0x70020070,  //  008E  JMP		#0100
      0x4C340000,  //  008F  LDNIL	R13
      0x2034140D,  //  0090  NE	R13	R10	R13
      0x78360031,  //  0091  JMPF	R13	#00C4
      0xB8360800,  //  0092  GETNGBL	R13	K4
      0x8C341B26,  //  0093  GETMET	R13	R13	K38
      0x7C340200,  //  0094  CALL	R13	1
      0x90321E0D,  //  0095  SETMBR	R12	K15	R13
      0x8834190F,  //  0096  GETMBR	R13	R12	K15
      0xB83A0800,  //  0097  GETNGBL	R14	K4
      0x8C381D21,  //  0098  GETMET	R14	R14	K33
      0x7C380200,  //  0099  CALL	R14	1
      0x90361A0E,  //  009A  SETMBR	R13	K13	R14
      0x8834190F,  //  009B  GETMBR	R13	R12	K15
      0x88341B0D,  //  009C  GETMBR	R13	R13	K13
      0x8838090C,  //  009D  GETMBR	R14	R4	K12
      0x9036180E,  //  009E  SETMBR	R13	K12	R14
      0x8834190F,  //  009F  GETMBR	R13	R12	K15
      0x88341B0D,  //  00A0  GETMBR	R13	R13	K13
      0x8838090E,  //  00A1  GETMBR	R14	R4	K14
      0x90361C0E,  //  00A2  SETMBR	R13	K14	R14
      0x8834190F,  //  00A3  GETMBR	R13	R12	K15
      0x88341B0D,  //  00A4  GETMBR	R13	R13	K13
      0x8838090F,  //  00A5  GETMBR	R14	R4	K15
      0x90361E0E,  //  00A6  SETMBR	R13	K15	R14
      0x8834190F,  //  00A7  GETMBR	R13	R12	K15
      0x90362C0A,  //  00A8  SETMBR	R13	K22	R10
      0x88340D0B,  //  00A9  GETMBR	R13	R6	K11
      0x8C341B23,  //  00AA  GETMET	R13	R13	K35
      0x5C3C1800,  //  00AB  MOVE	R15	R12
      0x7C340400,  //  00AC  CALL	R13	2
      0xB8360800,  //  00AD  GETNGBL	R13	K4
      0x8C341B12,  //  00AE  GETMET	R13	R13	K18
      0x883C090E,  //  00AF  GETMBR	R15	R4	K14
      0x8840090F,  //  00B0  GETMBR	R16	R4	K15
      0x7C340600,  //  00B1  CALL	R13	3
      0x5C241A00,  //  00B2  MOVE	R9	R13
      0xB8360200,  //  00B3  GETNGBL	R13	K1
      0x8C341B02,  //  00B4  GETMET	R13	R13	K2
      0x8C3C071A,  //  00B5  GETMET	R15	R3	K26
      0x58440027,  //  00B6  LDCONST	R17	K39
      0x88480315,  //  00B7  GETMBR	R18	R1	K21
      0x8848251C,  //  00B8  GETMBR	R18	R18	K28
      0x604C0008,  //  00B9  GETGBL	R19	G8
      0x5C500800,  //  00BA  MOVE	R20	R4
      0x7C4C0200,  //  00BB  CALL	R19	1
      0x78260001,  //  00BC  JMPF	R9	#00BF
      0x5C501200,  //  00BD  MOVE	R20	R9
      0x70020000,  //  00BE  JMP		#00C0
      0x58500019,  //  00BF  LDCONST	R20	K25
      0x7C3C0A00,  //  00C0  CALL	R15	5
      0x5840001D,  //  00C1  LDCONST	R16	K29
      0x7C340600,  //  00C2  CALL	R13	3
      0x7002003B,  //  00C3  JMP		#0100
      0x88340910,  //  00C4  GETMBR	R13	R4	K16
      0x4C380000,  //  00C5  LDNIL	R14
      0x20341A0E,  //  00C6  NE	R13	R13	R14
      0x7836002D,  //  00C7  JMPF	R13	#00F6
      0xB8360800,  //  00C8  GETNGBL	R13	K4
      0x8C341B20,  //  00C9  GETMET	R13	R13	K32
      0x7C340200,  //  00CA  CALL	R13	1
      0x9032200D,  //  00CB  SETMBR	R12	K16	R13
      0x88341910,  //  00CC  GETMBR	R13	R12	K16
      0xB83A0800,  //  00CD  GETNGBL	R14	K4
      0x8C381D21,  //  00CE  GETMET	R14	R14	K33
      0x7C380200,  //  00CF  CALL	R14	1
      0x90361A0E,  //  00D0  SETMBR	R13	K13	R14
      0x88341910,  //  00D1  GETMBR	R13	R12	K16
      0x88341B0D,  //  00D2  GETMBR	R13	R13	K13
      0x8838090C,  //  00D3  GETMBR	R14	R4	K12
      0x9036180E,  //  00D4  SETMBR	R13	K12	R14
      0x88341910,  //  00D5  GETMBR	R13	R12	K16
      0x88341B0D,  //  00D6  GETMBR	R13	R13	K13
      0x8838090E,  //  00D7  GETMBR	R14	R4	K14
      0x90361C0E,  //  00D8  SETMBR	R13	K14	R14
      0x88341910,  //  00D9  GETMBR	R13	R12	K16
      0x88341B0D,  //  00DA  GETMBR	R13	R13	K13
      0x8838090F,  //  00DB  GETMBR	R14	R4	K15
      0x90361E0E,  //  00DC  SETMBR	R13	K15	R14
      0x88341910,  //  00DD  GETMBR	R13	R12	K16
      0xB83A0800,  //  00DE  GETNGBL	R14	K4
      0x8C381D22,  //  00DF  GETMET	R14	R14	K34
      0x7C380200,  //  00E0  CALL	R14	1
      0x9036200E,  //  00E1  SETMBR	R13	K16	R14
      0x88341910,  //  00E2  GETMBR	R13	R12	K16
      0x88341B10,  //  00E3  GETMBR	R13	R13	K16
      0x88380910,  //  00E4  GETMBR	R14	R4	K16
      0x9036200E,  //  00E5  SETMBR	R13	K16	R14
      0x88340D0B,  //  00E6  GETMBR	R13	R6	K11
      0x8C341B23,  //  00E7  GETMET	R13	R13	K35
      0x5C3C1800,  //  00E8  MOVE	R15	R12
      0x7C340400,  //  00E9  CALL	R13	2
      0xB8360200,  //  00EA  GETNGBL	R13	K1
      0x8C341B02,  //  00EB  GETMET	R13	R13	K2
      0x8C3C071A,  //  00EC  GETMET	R15	R3	K26
      0x58440028,  //  00ED  LDCONST	R17	K40
      0x88480315,  //  00EE  GETMBR	R18	R1	K21
      0x8848251C,  //  00EF  GETMBR	R18	R18	K28
      0x884C0910,  //  00F0  GETMBR	R19	R4	K16
      0x88500325,  //  00F1  GETMBR	R20	R1	K37
      0x7C3C0A00,  //  00F2  CALL	R15	5
      0x5840001D,  //  00F3  LDCONST	R16	K29
      0x7C340600,  //  00F4  CALL	R13	3
      0x70020009,  //  00F5  JMP		#0100
      0xB8360200,  //  00F6  GETNGBL	R13	K1
      0x8C341B02,  //  00F7  GETMET	R13	R13	K2
      0x8C3C071A,  //  00F8  GETMET	R15	R3	K26
      0x58440029,  //  00F9  LDCONST	R17	K41
      0x88480315,  //  00FA  GETMBR	R18	R1	K21
      0x8848251C,  //  00FB  GETMBR	R18	R18	K28
      0x884C0325,  //  00FC  GETMBR	R19	R1	K37
      0x7C3C0800,  //  00FD  CALL	R15	4
      0x5840001D,  //  00FE  LDCONST	R16	K29
      0x7C340600,  //  00FF  CALL	R13	3
      0x7001FF1D,  //  0100  JMP		#001F
      0x581C002A,  //  0101  LDCONST	R7	K42
      0xAC1C0200,  //  0102  CATCH	R7	1	0
      0xB0080000,  //  0103  RAISE	2	R0	R0
      0xB81E0200,  //  0104  GETNGBL	R7	K1
      0x8C1C0F02,  //  0105  GETMET	R7	R7	K2
      0x60240008,  //  0106  GETGBL	R9	G8
      0x88280D0B,  //  0107  GETMBR	R10	R6	K11
      0x7C240200,  //  0108  CALL	R9	1
      0x00265609,  //  0109  ADD	R9	K43	R9
      0x542A0003,  //  010A  LDINT	R10	4
      0x7C1C0600,  //  010B  CALL	R7	3
      0x601C000C,  //  010C  GETGBL	R7	G12
      0x88200D0B,  //  010D  GETMBR	R8	R6	K11
      0x7C1C0200,  //  010E  CALL	R7	1
      0x241C0F2C,  //  010F  GT	R7	R7	K44
      0x781E0015,  //  0110  JMPF	R7	#0127
      0xB81E0200,  //  0111  GETNGBL	R7	K1
      0x8C1C0F02,  //  0112  GETMET	R7	R7	K2
      0x60240008,  //  0113  GETGBL	R9	G8
      0x5C280C00,  //  0114  MOVE	R10	R6
      0x7C240200,  //  0115  CALL	R9	1
      0x00265A09,  //  0116  ADD	R9	K45	R9
      0x542A0003,  //  0117  LDINT	R10	4
      0x7C1C0600,  //  0118  CALL	R7	3
      0xB81E0200,  //  0119  GETNGBL	R7	K1
      0x8C1C0F02,  //  011A  GETMET	R7	R7	K2
      0x60240008,  //  011B  GETGBL	R9	G8
      0x8C280D2F,  //  011C  GETMET	R10	R6	K47
      0x7C280200,  //  011D  CALL	R10	1
      0x7C240200,  //  011E  CALL	R9	1
      0x00265C09,  //  011F  ADD	R9	K46	R9
      0x58280030,  //  0120  LDCONST	R10	K48
      0x7C1C0600,  //  0121  CALL	R7	3
      0x8C1C0131,  //  0122  GETMET	R7	R0	K49
      0x5C240200,  //  0123  MOVE	R9	R1
      0x5C280C00,  //  0124  MOVE	R10	R6
      0x7C1C0600,  //  0125  CALL	R7	3
      0x70020001,  //  0126  JMP		#0129
      0x501C0000,  //  0127  LDBOOL	R7	0	0
      0x80040E00,  //  0128  RET	1	R7
      0x501C0200,  //  0129  LDBOOL	R7	1	0
      0x80040E00,  //  012A  RET	1	R7
      0x80000000,  //  012B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_subscribe_update
********************************************************************/
be_local_closure(Matter_IM_send_subscribe_update,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[29]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(ReadRequestMessage),
    /* K4   */  be_nested_str_weak(fabric_filtered),
    /* K5   */  be_nested_str_weak(attributes_requests),
    /* K6   */  be_nested_str_weak(updates),
    /* K7   */  be_nested_str_weak(AttributePathIB),
    /* K8   */  be_nested_str_weak(endpoint),
    /* K9   */  be_nested_str_weak(cluster),
    /* K10  */  be_nested_str_weak(attribute),
    /* K11  */  be_nested_str_weak(push),
    /* K12  */  be_nested_str_weak(stop_iteration),
    /* K13  */  be_const_int(0),
    /* K14  */  be_nested_str_weak(tasmota),
    /* K15  */  be_nested_str_weak(log),
    /* K16  */  be_nested_str_weak(format),
    /* K17  */  be_nested_str_weak(MTR_X3A_X20_X3CSub_Data_X20_X20_X28_X256i_X29_X20sub_X3D_X25i),
    /* K18  */  be_nested_str_weak(local_session_id),
    /* K19  */  be_nested_str_weak(subscription_id),
    /* K20  */  be_const_int(2),
    /* K21  */  be_nested_str_weak(is_keep_alive),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20_X3CSub_Alive_X20_X28_X256i_X29_X20sub_X3D_X25i),
    /* K23  */  be_nested_str_weak(_inner_process_read_request),
    /* K24  */  be_nested_str_weak(suppress_response),
    /* K25  */  be_nested_str_weak(IM_ReportDataSubscribed),
    /* K26  */  be_nested_str_weak(_message_handler),
    /* K27  */  be_nested_str_weak(send_queue),
    /* K28  */  be_nested_str_weak(send_enqueued),
    }),
    be_str_weak(send_subscribe_update),
    &be_const_str_solidified,
    ( &(const binstruction[87]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0301,  //  0001  GETMBR	R3	R1	K1
      0xB8120400,  //  0002  GETNGBL	R4	K2
      0x8C100903,  //  0003  GETMET	R4	R4	K3
      0x7C100200,  //  0004  CALL	R4	1
      0x50140000,  //  0005  LDBOOL	R5	0	0
      0x90120805,  //  0006  SETMBR	R4	K4	R5
      0x60140012,  //  0007  GETGBL	R5	G18
      0x7C140000,  //  0008  CALL	R5	0
      0x90120A05,  //  0009  SETMBR	R4	K5	R5
      0x60140010,  //  000A  GETGBL	R5	G16
      0x88180306,  //  000B  GETMBR	R6	R1	K6
      0x7C140200,  //  000C  CALL	R5	1
      0xA802000F,  //  000D  EXBLK	0	#001E
      0x5C180A00,  //  000E  MOVE	R6	R5
      0x7C180000,  //  000F  CALL	R6	0
      0xB81E0400,  //  0010  GETNGBL	R7	K2
      0x8C1C0F07,  //  0011  GETMET	R7	R7	K7
      0x7C1C0200,  //  0012  CALL	R7	1
      0x88200D08,  //  0013  GETMBR	R8	R6	K8
      0x901E1008,  //  0014  SETMBR	R7	K8	R8
      0x88200D09,  //  0015  GETMBR	R8	R6	K9
      0x901E1208,  //  0016  SETMBR	R7	K9	R8
      0x88200D0A,  //  0017  GETMBR	R8	R6	K10
      0x901E1408,  //  0018  SETMBR	R7	K10	R8
      0x88200905,  //  0019  GETMBR	R8	R4	K5
      0x8C20110B,  //  001A  GETMET	R8	R8	K11
      0x5C280E00,  //  001B  MOVE	R10	R7
      0x7C200400,  //  001C  CALL	R8	2
      0x7001FFEF,  //  001D  JMP		#000E
      0x5814000C,  //  001E  LDCONST	R5	K12
      0xAC140200,  //  001F  CATCH	R5	1	0
      0xB0080000,  //  0020  RAISE	2	R0	R0
      0x6014000C,  //  0021  GETGBL	R5	G12
      0x88180905,  //  0022  GETMBR	R6	R4	K5
      0x7C140200,  //  0023  CALL	R5	1
      0x24140B0D,  //  0024  GT	R5	R5	K13
      0x7816000B,  //  0025  JMPF	R5	#0032
      0xB8161C00,  //  0026  GETNGBL	R5	K14
      0x8C140B0F,  //  0027  GETMET	R5	R5	K15
      0x8C1C0510,  //  0028  GETMET	R7	R2	K16
      0x58240011,  //  0029  LDCONST	R9	K17
      0x88280712,  //  002A  GETMBR	R10	R3	K18
      0x882C0313,  //  002B  GETMBR	R11	R1	K19
      0x7C1C0800,  //  002C  CALL	R7	4
      0x58200014,  //  002D  LDCONST	R8	K20
      0x7C140600,  //  002E  CALL	R5	3
      0x50140000,  //  002F  LDBOOL	R5	0	0
      0x90062A05,  //  0030  SETMBR	R1	K21	R5
      0x7002000A,  //  0031  JMP		#003D
      0xB8161C00,  //  0032  GETNGBL	R5	K14
      0x8C140B0F,  //  0033  GETMET	R5	R5	K15
      0x8C1C0510,  //  0034  GETMET	R7	R2	K16
      0x58240016,  //  0035  LDCONST	R9	K22
      0x88280712,  //  0036  GETMBR	R10	R3	K18
      0x882C0313,  //  0037  GETMBR	R11	R1	K19
      0x7C1C0800,  //  0038  CALL	R7	4
      0x58200014,  //  0039  LDCONST	R8	K20
      0x7C140600,  //  003A  CALL	R5	3
      0x50140200,  //  003B  LDBOOL	R5	1	0
      0x90062A05,  //  003C  SETMBR	R1	K21	R5
      0x8C140117,  //  003D  GETMET	R5	R0	K23
      0x5C1C0600,  //  003E  MOVE	R7	R3
      0x5C200800,  //  003F  MOVE	R8	R4
      0x7C140600,  //  0040  CALL	R5	3
      0x6018000C,  //  0041  GETGBL	R6	G12
      0x881C0905,  //  0042  GETMBR	R7	R4	K5
      0x7C180200,  //  0043  CALL	R6	1
      0x1C180D0D,  //  0044  EQ	R6	R6	K13
      0x90163006,  //  0045  SETMBR	R5	K24	R6
      0x88180313,  //  0046  GETMBR	R6	R1	K19
      0x90162606,  //  0047  SETMBR	R5	K19	R6
      0xB81A0400,  //  0048  GETNGBL	R6	K2
      0x8C180D19,  //  0049  GETMET	R6	R6	K25
      0x8820071A,  //  004A  GETMBR	R8	R3	K26
      0x5C240600,  //  004B  MOVE	R9	R3
      0x5C280A00,  //  004C  MOVE	R10	R5
      0x5C2C0200,  //  004D  MOVE	R11	R1
      0x7C180A00,  //  004E  CALL	R6	5
      0x881C011B,  //  004F  GETMBR	R7	R0	K27
      0x8C1C0F0B,  //  0050  GETMET	R7	R7	K11
      0x5C240C00,  //  0051  MOVE	R9	R6
      0x7C1C0400,  //  0052  CALL	R7	2
      0x8C1C011C,  //  0053  GETMET	R7	R0	K28
      0x8824071A,  //  0054  GETMBR	R9	R3	K26
      0x7C1C0400,  //  0055  CALL	R7	2
      0x80000000,  //  0056  RET	0
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
    ( &(const binstruction[ 7]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0x88080501,  //  0001  GETMBR	R2	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x5C100200,  //  0003  MOVE	R4	R1
      0x50140000,  //  0004  LDBOOL	R5	0	0
      0x7C080600,  //  0005  CALL	R2	3
      0x80000000,  //  0006  RET	0
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
** Solidified function: _inner_process_read_request
********************************************************************/
be_local_closure(Matter_IM__inner_process_read_request,   /* name */
  be_nested_proto(
    20,                          /* nstack */
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
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 1),
          be_local_const_upval(1, 3),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[33]) {     /* constants */
        /* K0   */  be_nested_str_weak(string),
        /* K1   */  be_nested_str_weak(matter),
        /* K2   */  be_nested_str_weak(get_attribute_name),
        /* K3   */  be_nested_str_weak(cluster),
        /* K4   */  be_nested_str_weak(attribute),
        /* K5   */  be_nested_str_weak(_X20_X28),
        /* K6   */  be_nested_str_weak(_X29),
        /* K7   */  be_nested_str_weak(),
        /* K8   */  be_nested_str_weak(read_attribute),
        /* K9   */  be_nested_str_weak(AttributeReportIB),
        /* K10  */  be_nested_str_weak(attribute_data),
        /* K11  */  be_nested_str_weak(AttributeDataIB),
        /* K12  */  be_nested_str_weak(data_version),
        /* K13  */  be_const_int(1),
        /* K14  */  be_nested_str_weak(path),
        /* K15  */  be_nested_str_weak(AttributePathIB),
        /* K16  */  be_nested_str_weak(endpoint),
        /* K17  */  be_nested_str_weak(data),
        /* K18  */  be_nested_str_weak(attribute_reports),
        /* K19  */  be_nested_str_weak(push),
        /* K20  */  be_nested_str_weak(tasmota),
        /* K21  */  be_nested_str_weak(log),
        /* K22  */  be_nested_str_weak(format),
        /* K23  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20_X25s),
        /* K24  */  be_nested_str_weak(local_session_id),
        /* K25  */  be_const_int(2),
        /* K26  */  be_nested_str_weak(status),
        /* K27  */  be_nested_str_weak(attribute_status),
        /* K28  */  be_nested_str_weak(AttributeStatusIB),
        /* K29  */  be_nested_str_weak(StatusIB),
        /* K30  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20STATUS_X3A_X200x_X2502X_X20_X25s),
        /* K31  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
        /* K32  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s_X25s_X20_X2D_X20IGNORED),
        }),
        be_str_weak(read_single_attribute),
        &be_const_str_solidified,
        ( &(const binstruction[160]) {  /* code */
          0xA4120000,  //  0000  IMPORT	R4	K0
          0xB8160200,  //  0001  GETNGBL	R5	K1
          0x8C140B02,  //  0002  GETMET	R5	R5	K2
          0x881C0503,  //  0003  GETMBR	R7	R2	K3
          0x88200504,  //  0004  GETMBR	R8	R2	K4
          0x7C140600,  //  0005  CALL	R5	3
          0x78160002,  //  0006  JMPF	R5	#000A
          0x001A0A05,  //  0007  ADD	R6	K5	R5
          0x00180D06,  //  0008  ADD	R6	R6	K6
          0x70020000,  //  0009  JMP		#000B
          0x58180007,  //  000A  LDCONST	R6	K7
          0x5C140C00,  //  000B  MOVE	R5	R6
          0x4C180000,  //  000C  LDNIL	R6
          0x20180206,  //  000D  NE	R6	R1	R6
          0x781A0004,  //  000E  JMPF	R6	#0014
          0x8C180308,  //  000F  GETMET	R6	R1	K8
          0x68200000,  //  0010  GETUPV	R8	U0
          0x5C240400,  //  0011  MOVE	R9	R2
          0x7C180600,  //  0012  CALL	R6	3
          0x70020000,  //  0013  JMP		#0015
          0x4C180000,  //  0014  LDNIL	R6
          0x4C1C0000,  //  0015  LDNIL	R7
          0x201C0C07,  //  0016  NE	R7	R6	R7
          0x781E0034,  //  0017  JMPF	R7	#004D
          0xB81E0200,  //  0018  GETNGBL	R7	K1
          0x8C1C0F09,  //  0019  GETMET	R7	R7	K9
          0x7C1C0200,  //  001A  CALL	R7	1
          0xB8220200,  //  001B  GETNGBL	R8	K1
          0x8C20110B,  //  001C  GETMET	R8	R8	K11
          0x7C200200,  //  001D  CALL	R8	1
          0x901E1408,  //  001E  SETMBR	R7	K10	R8
          0x88200F0A,  //  001F  GETMBR	R8	R7	K10
          0x9022190D,  //  0020  SETMBR	R8	K12	K13
          0x88200F0A,  //  0021  GETMBR	R8	R7	K10
          0xB8260200,  //  0022  GETNGBL	R9	K1
          0x8C24130F,  //  0023  GETMET	R9	R9	K15
          0x7C240200,  //  0024  CALL	R9	1
          0x90221C09,  //  0025  SETMBR	R8	K14	R9
          0x88200F0A,  //  0026  GETMBR	R8	R7	K10
          0x8820110E,  //  0027  GETMBR	R8	R8	K14
          0x88240510,  //  0028  GETMBR	R9	R2	K16
          0x90222009,  //  0029  SETMBR	R8	K16	R9
          0x88200F0A,  //  002A  GETMBR	R8	R7	K10
          0x8820110E,  //  002B  GETMBR	R8	R8	K14
          0x88240503,  //  002C  GETMBR	R9	R2	K3
          0x90220609,  //  002D  SETMBR	R8	K3	R9
          0x88200F0A,  //  002E  GETMBR	R8	R7	K10
          0x8820110E,  //  002F  GETMBR	R8	R8	K14
          0x88240504,  //  0030  GETMBR	R9	R2	K4
          0x90220809,  //  0031  SETMBR	R8	K4	R9
          0x88200F0A,  //  0032  GETMBR	R8	R7	K10
          0x90222206,  //  0033  SETMBR	R8	K17	R6
          0x88200112,  //  0034  GETMBR	R8	R0	K18
          0x8C201113,  //  0035  GETMET	R8	R8	K19
          0x5C280E00,  //  0036  MOVE	R10	R7
          0x7C200400,  //  0037  CALL	R8	2
          0x68200001,  //  0038  GETUPV	R8	U1
          0x7422000F,  //  0039  JMPT	R8	#004A
          0xB8222800,  //  003A  GETNGBL	R8	K20
          0x8C201115,  //  003B  GETMET	R8	R8	K21
          0x8C280916,  //  003C  GETMET	R10	R4	K22
          0x58300017,  //  003D  LDCONST	R12	K23
          0x68340000,  //  003E  GETUPV	R13	U0
          0x88341B18,  //  003F  GETMBR	R13	R13	K24
          0x60380008,  //  0040  GETGBL	R14	G8
          0x5C3C0400,  //  0041  MOVE	R15	R2
          0x7C380200,  //  0042  CALL	R14	1
          0x5C3C0A00,  //  0043  MOVE	R15	R5
          0x60400008,  //  0044  GETGBL	R16	G8
          0x5C440C00,  //  0045  MOVE	R17	R6
          0x7C400200,  //  0046  CALL	R16	1
          0x7C280C00,  //  0047  CALL	R10	6
          0x582C0019,  //  0048  LDCONST	R11	K25
          0x7C200600,  //  0049  CALL	R8	3
          0x50200200,  //  004A  LDBOOL	R8	1	0
          0x80041000,  //  004B  RET	1	R8
          0x70020051,  //  004C  JMP		#009F
          0x881C051A,  //  004D  GETMBR	R7	R2	K26
          0x4C200000,  //  004E  LDNIL	R8
          0x201C0E08,  //  004F  NE	R7	R7	R8
          0x781E003E,  //  0050  JMPF	R7	#0090
          0x780E003C,  //  0051  JMPF	R3	#008F
          0xB81E0200,  //  0052  GETNGBL	R7	K1
          0x8C1C0F09,  //  0053  GETMET	R7	R7	K9
          0x7C1C0200,  //  0054  CALL	R7	1
          0xB8220200,  //  0055  GETNGBL	R8	K1
          0x8C20111C,  //  0056  GETMET	R8	R8	K28
          0x7C200200,  //  0057  CALL	R8	1
          0x901E3608,  //  0058  SETMBR	R7	K27	R8
          0x88200F1B,  //  0059  GETMBR	R8	R7	K27
          0xB8260200,  //  005A  GETNGBL	R9	K1
          0x8C24130F,  //  005B  GETMET	R9	R9	K15
          0x7C240200,  //  005C  CALL	R9	1
          0x90221C09,  //  005D  SETMBR	R8	K14	R9
          0x88200F1B,  //  005E  GETMBR	R8	R7	K27
          0xB8260200,  //  005F  GETNGBL	R9	K1
          0x8C24131D,  //  0060  GETMET	R9	R9	K29
          0x7C240200,  //  0061  CALL	R9	1
          0x90223409,  //  0062  SETMBR	R8	K26	R9
          0x88200F1B,  //  0063  GETMBR	R8	R7	K27
          0x8820110E,  //  0064  GETMBR	R8	R8	K14
          0x88240510,  //  0065  GETMBR	R9	R2	K16
          0x90222009,  //  0066  SETMBR	R8	K16	R9
          0x88200F1B,  //  0067  GETMBR	R8	R7	K27
          0x8820110E,  //  0068  GETMBR	R8	R8	K14
          0x88240503,  //  0069  GETMBR	R9	R2	K3
          0x90220609,  //  006A  SETMBR	R8	K3	R9
          0x88200F1B,  //  006B  GETMBR	R8	R7	K27
          0x8820110E,  //  006C  GETMBR	R8	R8	K14
          0x88240504,  //  006D  GETMBR	R9	R2	K4
          0x90220809,  //  006E  SETMBR	R8	K4	R9
          0x88200F1B,  //  006F  GETMBR	R8	R7	K27
          0x8820111A,  //  0070  GETMBR	R8	R8	K26
          0x8824051A,  //  0071  GETMBR	R9	R2	K26
          0x90223409,  //  0072  SETMBR	R8	K26	R9
          0x88200112,  //  0073  GETMBR	R8	R0	K18
          0x8C201113,  //  0074  GETMET	R8	R8	K19
          0x5C280E00,  //  0075  MOVE	R10	R7
          0x7C200400,  //  0076  CALL	R8	2
          0xB8222800,  //  0077  GETNGBL	R8	K20
          0x8C201115,  //  0078  GETMET	R8	R8	K21
          0x8C280916,  //  0079  GETMET	R10	R4	K22
          0x5830001E,  //  007A  LDCONST	R12	K30
          0x68340000,  //  007B  GETUPV	R13	U0
          0x88341B18,  //  007C  GETMBR	R13	R13	K24
          0x60380008,  //  007D  GETGBL	R14	G8
          0x5C3C0400,  //  007E  MOVE	R15	R2
          0x7C380200,  //  007F  CALL	R14	1
          0x5C3C0A00,  //  0080  MOVE	R15	R5
          0x8840051A,  //  0081  GETMBR	R16	R2	K26
          0x8844051A,  //  0082  GETMBR	R17	R2	K26
          0xB84A0200,  //  0083  GETNGBL	R18	K1
          0x8848251F,  //  0084  GETMBR	R18	R18	K31
          0x1C442212,  //  0085  EQ	R17	R17	R18
          0x78460001,  //  0086  JMPF	R17	#0089
          0x5844001F,  //  0087  LDCONST	R17	K31
          0x70020000,  //  0088  JMP		#008A
          0x58440007,  //  0089  LDCONST	R17	K7
          0x7C280E00,  //  008A  CALL	R10	7
          0x582C0019,  //  008B  LDCONST	R11	K25
          0x7C200600,  //  008C  CALL	R8	3
          0x50200200,  //  008D  LDBOOL	R8	1	0
          0x80041000,  //  008E  RET	1	R8
          0x7002000E,  //  008F  JMP		#009F
          0xB81E2800,  //  0090  GETNGBL	R7	K20
          0x8C1C0F15,  //  0091  GETMET	R7	R7	K21
          0x8C240916,  //  0092  GETMET	R9	R4	K22
          0x582C0020,  //  0093  LDCONST	R11	K32
          0x68300000,  //  0094  GETUPV	R12	U0
          0x88301918,  //  0095  GETMBR	R12	R12	K24
          0x60340008,  //  0096  GETGBL	R13	G8
          0x5C380400,  //  0097  MOVE	R14	R2
          0x7C340200,  //  0098  CALL	R13	1
          0x5C380A00,  //  0099  MOVE	R14	R5
          0x7C240A00,  //  009A  CALL	R9	5
          0x58280019,  //  009B  LDCONST	R10	K25
          0x7C1C0600,  //  009C  CALL	R7	3
          0x501C0000,  //  009D  LDBOOL	R7	0	0
          0x80040E00,  //  009E  RET	1	R7
          0x80000000,  //  009F  RET	0
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
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(device),
    /* K2   */  be_nested_str_weak(get_active_endpoints),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(Path),
    /* K5   */  be_nested_str_weak(ReportDataMessage),
    /* K6   */  be_nested_str_weak(attribute_reports),
    /* K7   */  be_nested_str_weak(attributes_requests),
    /* K8   */  be_nested_str_weak(endpoint),
    /* K9   */  be_nested_str_weak(cluster),
    /* K10  */  be_nested_str_weak(attribute),
    /* K11  */  be_nested_str_weak(status),
    /* K12  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K13  */  be_nested_str_weak(get_attribute_name),
    /* K14  */  be_nested_str_weak(tasmota),
    /* K15  */  be_nested_str_weak(log),
    /* K16  */  be_nested_str_weak(format),
    /* K17  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Attr_X20_X28_X256i_X29_X20_X25s),
    /* K18  */  be_nested_str_weak(local_session_id),
    /* K19  */  be_nested_str_weak(_X20_X28),
    /* K20  */  be_nested_str_weak(_X29),
    /* K21  */  be_nested_str_weak(),
    /* K22  */  be_const_int(2),
    /* K23  */  be_nested_str_weak(process_attribute_expansion),
    /* K24  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(_inner_process_read_request),
    &be_const_str_solidified,
    ( &(const binstruction[94]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x84140000,  //  0001  CLOSURE	R5	P0
      0x88180101,  //  0002  GETMBR	R6	R0	K1
      0x8C180D02,  //  0003  GETMET	R6	R6	K2
      0x7C180200,  //  0004  CALL	R6	1
      0xB81E0600,  //  0005  GETNGBL	R7	K3
      0x8C1C0F04,  //  0006  GETMET	R7	R7	K4
      0x7C1C0200,  //  0007  CALL	R7	1
      0xB8220600,  //  0008  GETNGBL	R8	K3
      0x8C201105,  //  0009  GETMET	R8	R8	K5
      0x7C200200,  //  000A  CALL	R8	1
      0x60240012,  //  000B  GETGBL	R9	G18
      0x7C240000,  //  000C  CALL	R9	0
      0x90220C09,  //  000D  SETMBR	R8	K6	R9
      0x60240010,  //  000E  GETGBL	R9	G16
      0x88280507,  //  000F  GETMBR	R10	R2	K7
      0x7C240200,  //  0010  CALL	R9	1
      0xA8020046,  //  0011  EXBLK	0	#0059
      0x5C281200,  //  0012  MOVE	R10	R9
      0x7C280000,  //  0013  CALL	R10	0
      0x882C1508,  //  0014  GETMBR	R11	R10	K8
      0x901E100B,  //  0015  SETMBR	R7	K8	R11
      0x882C1509,  //  0016  GETMBR	R11	R10	K9
      0x901E120B,  //  0017  SETMBR	R7	K9	R11
      0x882C150A,  //  0018  GETMBR	R11	R10	K10
      0x901E140B,  //  0019  SETMBR	R7	K10	R11
      0xB82E0600,  //  001A  GETNGBL	R11	K3
      0x882C170C,  //  001B  GETMBR	R11	R11	K12
      0x901E160B,  //  001C  SETMBR	R7	K11	R11
      0x882C0F08,  //  001D  GETMBR	R11	R7	K8
      0x4C300000,  //  001E  LDNIL	R12
      0x1C2C160C,  //  001F  EQ	R11	R11	R12
      0x742E0007,  //  0020  JMPT	R11	#0029
      0x882C0F09,  //  0021  GETMBR	R11	R7	K9
      0x4C300000,  //  0022  LDNIL	R12
      0x1C2C160C,  //  0023  EQ	R11	R11	R12
      0x742E0003,  //  0024  JMPT	R11	#0029
      0x882C0F0A,  //  0025  GETMBR	R11	R7	K10
      0x4C300000,  //  0026  LDNIL	R12
      0x1C2C160C,  //  0027  EQ	R11	R11	R12
      0x782E0029,  //  0028  JMPF	R11	#0053
      0x882C0F09,  //  0029  GETMBR	R11	R7	K9
      0x4C300000,  //  002A  LDNIL	R12
      0x202C160C,  //  002B  NE	R11	R11	R12
      0x782E001A,  //  002C  JMPF	R11	#0048
      0x882C0F0A,  //  002D  GETMBR	R11	R7	K10
      0x4C300000,  //  002E  LDNIL	R12
      0x202C160C,  //  002F  NE	R11	R11	R12
      0x782E0016,  //  0030  JMPF	R11	#0048
      0xB82E0600,  //  0031  GETNGBL	R11	K3
      0x8C2C170D,  //  0032  GETMET	R11	R11	K13
      0x88340F09,  //  0033  GETMBR	R13	R7	K9
      0x88380F0A,  //  0034  GETMBR	R14	R7	K10
      0x7C2C0600,  //  0035  CALL	R11	3
      0xB8321C00,  //  0036  GETNGBL	R12	K14
      0x8C30190F,  //  0037  GETMET	R12	R12	K15
      0x8C380910,  //  0038  GETMET	R14	R4	K16
      0x58400011,  //  0039  LDCONST	R16	K17
      0x88440312,  //  003A  GETMBR	R17	R1	K18
      0x60480008,  //  003B  GETGBL	R18	G8
      0x5C4C0E00,  //  003C  MOVE	R19	R7
      0x7C480200,  //  003D  CALL	R18	1
      0x782E0002,  //  003E  JMPF	R11	#0042
      0x004E260B,  //  003F  ADD	R19	K19	R11
      0x004C2714,  //  0040  ADD	R19	R19	K20
      0x70020000,  //  0041  JMP		#0043
      0x584C0015,  //  0042  LDCONST	R19	K21
      0x00482413,  //  0043  ADD	R18	R18	R19
      0x7C380800,  //  0044  CALL	R14	4
      0x583C0016,  //  0045  LDCONST	R15	K22
      0x7C300600,  //  0046  CALL	R12	3
      0x7002000A,  //  0047  JMP		#0053
      0xB82E1C00,  //  0048  GETNGBL	R11	K14
      0x8C2C170F,  //  0049  GETMET	R11	R11	K15
      0x8C340910,  //  004A  GETMET	R13	R4	K16
      0x583C0011,  //  004B  LDCONST	R15	K17
      0x88400312,  //  004C  GETMBR	R16	R1	K18
      0x60440008,  //  004D  GETGBL	R17	G8
      0x5C480E00,  //  004E  MOVE	R18	R7
      0x7C440200,  //  004F  CALL	R17	1
      0x7C340800,  //  0050  CALL	R13	4
      0x58380016,  //  0051  LDCONST	R14	K22
      0x7C2C0600,  //  0052  CALL	R11	3
      0x882C0101,  //  0053  GETMBR	R11	R0	K1
      0x8C2C1717,  //  0054  GETMET	R11	R11	K23
      0x5C340E00,  //  0055  MOVE	R13	R7
      0x84380001,  //  0056  CLOSURE	R14	P1
      0x7C2C0600,  //  0057  CALL	R11	3
      0x7001FFB8,  //  0058  JMP		#0012
      0x58240018,  //  0059  LDCONST	R9	K24
      0xAC240200,  //  005A  CATCH	R9	1	0
      0xB0080000,  //  005B  RAISE	2	R0	R0
      0xA0000000,  //  005C  CLOSE	R0
      0x80041000,  //  005D  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: report_data
********************************************************************/
be_local_closure(Matter_IM_report_data,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(ReportDataMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20received_X20ReportDataMessage_X3D),
    /* K7   */  be_const_int(2),
    }),
    be_str_weak(report_data),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0xB8160800,  //  0007  GETNGBL	R5	K4
      0x8C140B05,  //  0008  GETMET	R5	R5	K5
      0x601C0008,  //  0009  GETGBL	R7	G8
      0x5C200800,  //  000A  MOVE	R8	R4
      0x7C1C0200,  //  000B  CALL	R7	1
      0x001E0C07,  //  000C  ADD	R7	K6	R7
      0x58200007,  //  000D  LDCONST	R8	K7
      0x7C140600,  //  000E  CALL	R5	3
      0x50140000,  //  000F  LDBOOL	R5	0	0
      0x80040A00,  //  0010  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM
********************************************************************/
be_local_class(Matter_IM,
    3,
    NULL,
    be_nested_map(30,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(process_status_response, -1), be_const_closure(Matter_IM_process_status_response_closure) },
        { be_const_key_weak(report_data, -1), be_const_closure(Matter_IM_report_data_closure) },
        { be_const_key_weak(_inner_process_read_request, -1), be_const_closure(Matter_IM__inner_process_read_request_closure) },
        { be_const_key_weak(process_read_request, -1), be_const_closure(Matter_IM_process_read_request_closure) },
        { be_const_key_weak(process_write_response, -1), be_const_closure(Matter_IM_process_write_response_closure) },
        { be_const_key_weak(send_write_response, 0), be_const_closure(Matter_IM_send_write_response_closure) },
        { be_const_key_weak(process_write_request, -1), be_const_closure(Matter_IM_process_write_request_closure) },
        { be_const_key_weak(every_second, 22), be_const_closure(Matter_IM_every_second_closure) },
        { be_const_key_weak(send_ack_now, 23), be_const_closure(Matter_IM_send_ack_now_closure) },
        { be_const_key_weak(find_sendqueue_by_exchangeid, -1), be_const_closure(Matter_IM_find_sendqueue_by_exchangeid_closure) },
        { be_const_key_weak(subscribe_response, -1), be_const_closure(Matter_IM_subscribe_response_closure) },
        { be_const_key_weak(process_incoming_ack, 24), be_const_closure(Matter_IM_process_incoming_ack_closure) },
        { be_const_key_weak(remove_sendqueue_by_exchangeid, -1), be_const_closure(Matter_IM_remove_sendqueue_by_exchangeid_closure) },
        { be_const_key_weak(subscribe_request, 10), be_const_closure(Matter_IM_subscribe_request_closure) },
        { be_const_key_weak(send_invoke_response, 26), be_const_closure(Matter_IM_send_invoke_response_closure) },
        { be_const_key_weak(init, 7), be_const_closure(Matter_IM_init_closure) },
        { be_const_key_weak(expire_sendqueue, 25), be_const_closure(Matter_IM_expire_sendqueue_closure) },
        { be_const_key_weak(process_incoming, -1), be_const_closure(Matter_IM_process_incoming_closure) },
        { be_const_key_weak(process_timed_request, -1), be_const_closure(Matter_IM_process_timed_request_closure) },
        { be_const_key_weak(process_invoke_response, -1), be_const_closure(Matter_IM_process_invoke_response_closure) },
        { be_const_key_weak(send_subscribe_response, 8), be_const_closure(Matter_IM_send_subscribe_response_closure) },
        { be_const_key_weak(device, -1), be_const_var(0) },
        { be_const_key_weak(process_invoke_request, -1), be_const_closure(Matter_IM_process_invoke_request_closure) },
        { be_const_key_weak(every_250ms, -1), be_const_closure(Matter_IM_every_250ms_closure) },
        { be_const_key_weak(send_report_data, 19), be_const_closure(Matter_IM_send_report_data_closure) },
        { be_const_key_weak(send_status, -1), be_const_closure(Matter_IM_send_status_closure) },
        { be_const_key_weak(send_enqueued, -1), be_const_closure(Matter_IM_send_enqueued_closure) },
        { be_const_key_weak(subs_shop, 2), be_const_var(1) },
        { be_const_key_weak(send_subscribe_update, 1), be_const_closure(Matter_IM_send_subscribe_update_closure) },
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
