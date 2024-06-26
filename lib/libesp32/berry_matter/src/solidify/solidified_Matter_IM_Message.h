/* Solidification of Matter_IM_Message.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_IM_Message;

/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_IM_Message;
be_local_closure(class_Matter_IM_Message_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_Message, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(reset),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C100100,  //  0000  GETMET	R4	R0	K0
      0x5C180200,  //  0001  MOVE	R6	R1
      0x5C1C0400,  //  0002  MOVE	R7	R2
      0x5C200600,  //  0003  MOVE	R8	R3
      0x7C100800,  //  0004  CALL	R4	4
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_exchangeid
********************************************************************/
extern const bclass be_class_Matter_IM_Message;
be_local_closure(class_Matter_IM_Message_get_exchangeid,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_Message, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(resp),
    /* K1   */  be_nested_str_weak(exchange_id),
    }),
    be_str_weak(get_exchangeid),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_im
********************************************************************/
extern const bclass be_class_Matter_IM_Message;
be_local_closure(class_Matter_IM_Message_send_im,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_Message, 
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(ready),
    /* K1   */  be_nested_str_weak(resp),
    /* K2   */  be_nested_str_weak(data),
    /* K3   */  be_nested_str_weak(to_TLV),
    /* K4   */  be_nested_str_weak(tlv2raw),
    /* K5   */  be_nested_str_weak(encode_frame),
    /* K6   */  be_nested_str_weak(encrypt),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(loglevel),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20_X3Csnd_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20id_X3D_X25i_X20exch_X3D_X25i_X20rack_X3D_X25s),
    /* K11  */  be_nested_str_weak(session),
    /* K12  */  be_nested_str_weak(local_session_id),
    /* K13  */  be_nested_str_weak(message_counter),
    /* K14  */  be_nested_str_weak(exchange_id),
    /* K15  */  be_nested_str_weak(ack_message_counter),
    /* K16  */  be_nested_str_weak(send_response_frame),
    /* K17  */  be_nested_str_weak(last_counter),
    /* K18  */  be_nested_str_weak(finish),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0001,  //  0001  JMPT	R2	#0004
      0x50080000,  //  0002  LDBOOL	R2	0	0
      0x80040400,  //  0003  RET	1	R2
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x880C0102,  //  0005  GETMBR	R3	R0	K2
      0x8C0C0703,  //  0006  GETMET	R3	R3	K3
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C100704,  //  0008  GETMET	R4	R3	K4
      0x7C100200,  //  0009  CALL	R4	1
      0x8C140505,  //  000A  GETMET	R5	R2	K5
      0x5C1C0800,  //  000B  MOVE	R7	R4
      0x7C140400,  //  000C  CALL	R5	2
      0x8C140506,  //  000D  GETMET	R5	R2	K6
      0x7C140200,  //  000E  CALL	R5	1
      0xB8160E00,  //  000F  GETNGBL	R5	K7
      0x8C140B08,  //  0010  GETMET	R5	R5	K8
      0x541E0003,  //  0011  LDINT	R7	4
      0x7C140400,  //  0012  CALL	R5	2
      0x7816000A,  //  0013  JMPF	R5	#001F
      0xB8161200,  //  0014  GETNGBL	R5	K9
      0x60180018,  //  0015  GETGBL	R6	G24
      0x581C000A,  //  0016  LDCONST	R7	K10
      0x8820050B,  //  0017  GETMBR	R8	R2	K11
      0x8820110C,  //  0018  GETMBR	R8	R8	K12
      0x8824050D,  //  0019  GETMBR	R9	R2	K13
      0x8828050E,  //  001A  GETMBR	R10	R2	K14
      0x882C050F,  //  001B  GETMBR	R11	R2	K15
      0x7C180A00,  //  001C  CALL	R6	5
      0x541E0003,  //  001D  LDINT	R7	4
      0x7C140400,  //  001E  CALL	R5	2
      0x8C140310,  //  001F  GETMET	R5	R1	K16
      0x5C1C0400,  //  0020  MOVE	R7	R2
      0x7C140400,  //  0021  CALL	R5	2
      0x8814050D,  //  0022  GETMBR	R5	R2	K13
      0x90022205,  //  0023  SETMBR	R0	K17	R5
      0x50140200,  //  0024  LDBOOL	R5	1	0
      0x90022405,  //  0025  SETMBR	R0	K18	R5
      0x80000000,  //  0026  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: status_error_received
********************************************************************/
extern const bclass be_class_Matter_IM_Message;
be_local_closure(class_Matter_IM_Message_status_error_received,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_Message, 
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(status_error_received),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reset
********************************************************************/
extern const bclass be_class_Matter_IM_Message;
be_local_closure(class_Matter_IM_Message_reset,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_Message, 
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(resp),
    /* K1   */  be_nested_str_weak(build_response),
    /* K2   */  be_nested_str_weak(ready),
    /* K3   */  be_nested_str_weak(expiration),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(millis),
    /* K6   */  be_nested_str_weak(MSG_TIMEOUT),
    /* K7   */  be_nested_str_weak(last_counter),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(finish),
    /* K10  */  be_nested_str_weak(data),
    }),
    be_str_weak(reset),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x20100204,  //  0001  NE	R4	R1	R4
      0x78120004,  //  0002  JMPF	R4	#0008
      0x8C100301,  //  0003  GETMET	R4	R1	K1
      0x5C180400,  //  0004  MOVE	R6	R2
      0x5C1C0600,  //  0005  MOVE	R7	R3
      0x7C100600,  //  0006  CALL	R4	3
      0x70020000,  //  0007  JMP		#0009
      0x4C100000,  //  0008  LDNIL	R4
      0x90020004,  //  0009  SETMBR	R0	K0	R4
      0x50100200,  //  000A  LDBOOL	R4	1	0
      0x90020404,  //  000B  SETMBR	R0	K2	R4
      0xB8120800,  //  000C  GETNGBL	R4	K4
      0x8C100905,  //  000D  GETMET	R4	R4	K5
      0x7C100200,  //  000E  CALL	R4	1
      0x88140106,  //  000F  GETMBR	R5	R0	K6
      0x00100805,  //  0010  ADD	R4	R4	R5
      0x90020604,  //  0011  SETMBR	R0	K3	R4
      0x90020F08,  //  0012  SETMBR	R0	K7	K8
      0x50100000,  //  0013  LDBOOL	R4	0	0
      0x90021204,  //  0014  SETMBR	R0	K9	R4
      0x4C100000,  //  0015  LDNIL	R4
      0x90021404,  //  0016  SETMBR	R0	K10	R4
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reached_timeout
********************************************************************/
extern const bclass be_class_Matter_IM_Message;
be_local_closure(class_Matter_IM_Message_reached_timeout,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_Message, 
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(reached_timeout),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: status_ok_received
********************************************************************/
extern const bclass be_class_Matter_IM_Message;
be_local_closure(class_Matter_IM_Message_status_ok_received,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_Message, 
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(expiration),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(millis),
    /* K3   */  be_nested_str_weak(MSG_TIMEOUT),
    /* K4   */  be_nested_str_weak(resp),
    /* K5   */  be_nested_str_weak(build_response),
    /* K6   */  be_nested_str_weak(opcode),
    /* K7   */  be_nested_str_weak(x_flag_r),
    /* K8   */  be_nested_str_weak(ready),
    }),
    be_str_weak(status_ok_received),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0xB80A0200,  //  0000  GETNGBL	R2	K1
      0x8C080502,  //  0001  GETMET	R2	R2	K2
      0x7C080200,  //  0002  CALL	R2	1
      0x880C0103,  //  0003  GETMBR	R3	R0	K3
      0x00080403,  //  0004  ADD	R2	R2	R3
      0x90020002,  //  0005  SETMBR	R0	K0	R2
      0x78060007,  //  0006  JMPF	R1	#000F
      0x8C080305,  //  0007  GETMET	R2	R1	K5
      0x88100104,  //  0008  GETMBR	R4	R0	K4
      0x88100906,  //  0009  GETMBR	R4	R4	K6
      0x88140104,  //  000A  GETMBR	R5	R0	K4
      0x88140B07,  //  000B  GETMBR	R5	R5	K7
      0x88180104,  //  000C  GETMBR	R6	R0	K4
      0x7C080800,  //  000D  CALL	R2	4
      0x90020802,  //  000E  SETMBR	R0	K4	R2
      0x50080200,  //  000F  LDBOOL	R2	1	0
      0x90021002,  //  0010  SETMBR	R0	K8	R2
      0x50080200,  //  0011  LDBOOL	R2	1	0
      0x80040400,  //  0012  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ack_received
********************************************************************/
extern const bclass be_class_Matter_IM_Message;
be_local_closure(class_Matter_IM_Message_ack_received,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_Message, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(expiration),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(millis),
    /* K3   */  be_nested_str_weak(MSG_TIMEOUT),
    }),
    be_str_weak(ack_received),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xB80A0200,  //  0000  GETNGBL	R2	K1
      0x8C080502,  //  0001  GETMET	R2	R2	K2
      0x7C080200,  //  0002  CALL	R2	1
      0x880C0103,  //  0003  GETMBR	R3	R0	K3
      0x00080403,  //  0004  ADD	R2	R2	R3
      0x90020002,  //  0005  SETMBR	R0	K0	R2
      0x50080000,  //  0006  LDBOOL	R2	0	0
      0x80040400,  //  0007  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM_Message
********************************************************************/
be_local_class(Matter_IM_Message,
    6,
    NULL,
    be_nested_map(15,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, 11), be_const_closure(class_Matter_IM_Message_init_closure) },
        { be_const_key_weak(get_exchangeid, 10), be_const_closure(class_Matter_IM_Message_get_exchangeid_closure) },
        { be_const_key_weak(reset, -1), be_const_closure(class_Matter_IM_Message_reset_closure) },
        { be_const_key_weak(send_im, -1), be_const_closure(class_Matter_IM_Message_send_im_closure) },
        { be_const_key_weak(status_error_received, -1), be_const_closure(class_Matter_IM_Message_status_error_received_closure) },
        { be_const_key_weak(finish, -1), be_const_var(3) },
        { be_const_key_weak(status_ok_received, 2), be_const_closure(class_Matter_IM_Message_status_ok_received_closure) },
        { be_const_key_weak(last_counter, -1), be_const_var(5) },
        { be_const_key_weak(reached_timeout, -1), be_const_closure(class_Matter_IM_Message_reached_timeout_closure) },
        { be_const_key_weak(resp, -1), be_const_var(1) },
        { be_const_key_weak(data, -1), be_const_var(4) },
        { be_const_key_weak(expiration, -1), be_const_var(0) },
        { be_const_key_weak(ready, 6), be_const_var(2) },
        { be_const_key_weak(MSG_TIMEOUT, 5), be_const_int(5000) },
        { be_const_key_weak(ack_received, -1), be_const_closure(class_Matter_IM_Message_ack_received_closure) },
    })),
    be_str_weak(Matter_IM_Message)
);

extern const bclass be_class_Matter_IM_Status;

/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_IM_Status;
be_local_closure(class_Matter_IM_Status_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_Status, 
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_const_int(1),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(StatusResponseMessage),
    /* K4   */  be_nested_str_weak(status),
    /* K5   */  be_nested_str_weak(data),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x58180001,  //  0005  LDCONST	R6	K1
      0x501C0200,  //  0006  LDBOOL	R7	1	0
      0x7C0C0800,  //  0007  CALL	R3	4
      0xB80E0400,  //  0008  GETNGBL	R3	K2
      0x8C0C0703,  //  0009  GETMET	R3	R3	K3
      0x7C0C0200,  //  000A  CALL	R3	1
      0x900E0802,  //  000B  SETMBR	R3	K4	R2
      0x90020A03,  //  000C  SETMBR	R0	K5	R3
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM_Status
********************************************************************/
extern const bclass be_class_Matter_IM_Message;
be_local_class(Matter_IM_Status,
    0,
    &be_class_Matter_IM_Message,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_IM_Status_init_closure) },
    })),
    be_str_weak(Matter_IM_Status)
);

extern const bclass be_class_Matter_IM_InvokeResponse;

/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_IM_InvokeResponse;
be_local_closure(class_Matter_IM_InvokeResponse_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_InvokeResponse, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(data),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x541A0008,  //  0005  LDINT	R6	9
      0x501C0200,  //  0006  LDBOOL	R7	1	0
      0x7C0C0800,  //  0007  CALL	R3	4
      0x90020202,  //  0008  SETMBR	R0	K1	R2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM_InvokeResponse
********************************************************************/
extern const bclass be_class_Matter_IM_Message;
be_local_class(Matter_IM_InvokeResponse,
    0,
    &be_class_Matter_IM_Message,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_IM_InvokeResponse_init_closure) },
    })),
    be_str_weak(Matter_IM_InvokeResponse)
);

extern const bclass be_class_Matter_IM_WriteResponse;

/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_IM_WriteResponse;
be_local_closure(class_Matter_IM_WriteResponse_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_WriteResponse, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(data),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x541A0006,  //  0005  LDINT	R6	7
      0x501C0200,  //  0006  LDBOOL	R7	1	0
      0x7C0C0800,  //  0007  CALL	R3	4
      0x90020202,  //  0008  SETMBR	R0	K1	R2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM_WriteResponse
********************************************************************/
extern const bclass be_class_Matter_IM_Message;
be_local_class(Matter_IM_WriteResponse,
    0,
    &be_class_Matter_IM_Message,
    be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_IM_WriteResponse_init_closure) },
    })),
    be_str_weak(Matter_IM_WriteResponse)
);

extern const bclass be_class_Matter_IM_ReportData_Pull;

/********************************************************************
** Solidified function: set_subscription_id
********************************************************************/
extern const bclass be_class_Matter_IM_ReportData_Pull;
be_local_closure(class_Matter_IM_ReportData_Pull_set_subscription_id,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_ReportData_Pull, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(subscription_id),
    }),
    be_str_weak(set_subscription_id),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_IM_ReportData_Pull;
be_local_closure(class_Matter_IM_ReportData_Pull_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_ReportData_Pull, 
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(generator_or_arr),
    /* K2   */  be_nested_str_weak(event_generator_or_arr),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x541E0004,  //  0005  LDINT	R7	5
      0x50200200,  //  0006  LDBOOL	R8	1	0
      0x7C100800,  //  0007  CALL	R4	4
      0x90020202,  //  0008  SETMBR	R0	K1	R2
      0x90020403,  //  0009  SETMBR	R0	K2	R3
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_im
********************************************************************/
extern const bclass be_class_Matter_IM_ReportData_Pull;
be_local_closure(class_Matter_IM_ReportData_Pull_send_im,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_ReportData_Pull, 
    1,                          /* has constants */
    ( &(const bvalue[51]) {     /* constants */
    /* K0   */  be_nested_str_weak(ready),
    /* K1   */  be_nested_str_weak(resp),
    /* K2   */  be_nested_str_weak(data),
    /* K3   */  be_nested_str_weak(device),
    /* K4   */  be_nested_str_weak(debug),
    /* K5   */  be_nested_str_weak(data_ev),
    /* K6   */  be_nested_str_weak(event_generator_or_arr),
    /* K7   */  be_nested_str_weak(generator_or_arr),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(next_attribute),
    /* K10  */  be_nested_str_weak(is_direct),
    /* K11  */  be_nested_str_weak(im),
    /* K12  */  be_nested_str_weak(read_single_attribute_to_bytes),
    /* K13  */  be_nested_str_weak(get_pi),
    /* K14  */  be_nested_str_weak(session),
    /* K15  */  be_nested_str_weak(MAX_MESSAGE),
    /* K16  */  be_nested_str_weak(append),
    /* K17  */  be_nested_str_weak(remove),
    /* K18  */  be_nested_str_weak(next_event),
    /* K19  */  be_nested_str_weak(tasmota),
    /* K20  */  be_nested_str_weak(loglevel),
    /* K21  */  be_const_int(3),
    /* K22  */  be_nested_str_weak(),
    /* K23  */  be_nested_str_weak(data0),
    /* K24  */  be_nested_str_weak(_X20_X2D_X20),
    /* K25  */  be_nested_str_weak(data1),
    /* K26  */  be_nested_str_weak(_X2C_X20),
    /* K27  */  be_nested_str_weak(data2),
    /* K28  */  be_nested_str_weak(log),
    /* K29  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Event_X28_X256i_X7C_X258s_X29_X20_X5B_X2502X_X5D_X2504X_X2F_X2502X_X25s),
    /* K30  */  be_nested_str_weak(local_session_id),
    /* K31  */  be_nested_str_weak(event_no),
    /* K32  */  be_nested_str_weak(endpoint),
    /* K33  */  be_nested_str_weak(cluster),
    /* K34  */  be_nested_str_weak(event_id),
    /* K35  */  be_nested_str_weak(to_raw_bytes),
    /* K36  */  be_nested_str_weak(matter),
    /* K37  */  be_nested_str_weak(ReportDataMessage),
    /* K38  */  be_nested_str_weak(subscription_id),
    /* K39  */  be_nested_str_weak(suppress_response),
    /* K40  */  be_nested_str_weak(attribute_reports),
    /* K41  */  be_nested_str_weak(event_reports),
    /* K42  */  be_nested_str_weak(more_chunked_messages),
    /* K43  */  be_nested_str_weak(to_TLV),
    /* K44  */  be_nested_str_weak(tlv2raw),
    /* K45  */  be_nested_str_weak(encode_frame),
    /* K46  */  be_nested_str_weak(encrypt),
    /* K47  */  be_nested_str_weak(send_response_frame),
    /* K48  */  be_nested_str_weak(last_counter),
    /* K49  */  be_nested_str_weak(message_counter),
    /* K50  */  be_nested_str_weak(finish),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[311]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0001,  //  0001  JMPT	R2	#0004
      0x50080000,  //  0002  LDBOOL	R2	0	0
      0x80040400,  //  0003  RET	1	R2
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x880C0102,  //  0005  GETMBR	R3	R0	K2
      0x4C100000,  //  0006  LDNIL	R4
      0x200C0604,  //  0007  NE	R3	R3	R4
      0x780E0001,  //  0008  JMPF	R3	#000B
      0x880C0102,  //  0009  GETMBR	R3	R0	K2
      0x70020001,  //  000A  JMP		#000D
      0x600C0015,  //  000B  GETGBL	R3	G21
      0x7C0C0000,  //  000C  CALL	R3	0
      0x4C100000,  //  000D  LDNIL	R4
      0x90020404,  //  000E  SETMBR	R0	K2	R4
      0x50100200,  //  000F  LDBOOL	R4	1	0
      0x88140303,  //  0010  GETMBR	R5	R1	K3
      0x88140B04,  //  0011  GETMBR	R5	R5	K4
      0x88180105,  //  0012  GETMBR	R6	R0	K5
      0x4C1C0000,  //  0013  LDNIL	R7
      0x20180C07,  //  0014  NE	R6	R6	R7
      0x781A0001,  //  0015  JMPF	R6	#0018
      0x88180105,  //  0016  GETMBR	R6	R0	K5
      0x70020007,  //  0017  JMP		#0020
      0x88180106,  //  0018  GETMBR	R6	R0	K6
      0x4C1C0000,  //  0019  LDNIL	R7
      0x20180C07,  //  001A  NE	R6	R6	R7
      0x781A0002,  //  001B  JMPF	R6	#001F
      0x60180015,  //  001C  GETGBL	R6	G21
      0x7C180000,  //  001D  CALL	R6	0
      0x70020000,  //  001E  JMP		#0020
      0x4C180000,  //  001F  LDNIL	R6
      0x7812004A,  //  0020  JMPF	R4	#006C
      0x881C0107,  //  0021  GETMBR	R7	R0	K7
      0x4C200000,  //  0022  LDNIL	R8
      0x201C0E08,  //  0023  NE	R7	R7	R8
      0x781E0046,  //  0024  JMPF	R7	#006C
      0x601C000F,  //  0025  GETGBL	R7	G15
      0x88200107,  //  0026  GETMBR	R8	R0	K7
      0x60240012,  //  0027  GETGBL	R9	G18
      0x7C1C0400,  //  0028  CALL	R7	2
      0x781E0002,  //  0029  JMPF	R7	#002D
      0x881C0107,  //  002A  GETMBR	R7	R0	K7
      0x941C0F08,  //  002B  GETIDX	R7	R7	K8
      0x70020000,  //  002C  JMP		#002E
      0x881C0107,  //  002D  GETMBR	R7	R0	K7
      0x4C200000,  //  002E  LDNIL	R8
      0x78120026,  //  002F  JMPF	R4	#0057
      0x8C240F09,  //  0030  GETMET	R9	R7	K9
      0x7C240200,  //  0031  CALL	R9	1
      0x5C201200,  //  0032  MOVE	R8	R9
      0x78260022,  //  0033  JMPF	R9	#0057
      0x8C240F0A,  //  0034  GETMET	R9	R7	K10
      0x7C240200,  //  0035  CALL	R9	1
      0x74260001,  //  0036  JMPT	R9	#0039
      0x74160000,  //  0037  JMPT	R5	#0039
      0x50240001,  //  0038  LDBOOL	R9	0	1
      0x50240200,  //  0039  LDBOOL	R9	1	0
      0x8828030B,  //  003A  GETMBR	R10	R1	K11
      0x8C28150C,  //  003B  GETMET	R10	R10	K12
      0x8C300F0D,  //  003C  GETMET	R12	R7	K13
      0x7C300200,  //  003D  CALL	R12	1
      0x5C341000,  //  003E  MOVE	R13	R8
      0x8838050E,  //  003F  GETMBR	R14	R2	K14
      0x5C3C1200,  //  0040  MOVE	R15	R9
      0x7C280A00,  //  0041  CALL	R10	5
      0x4C2C0000,  //  0042  LDNIL	R11
      0x1C2C140B,  //  0043  EQ	R11	R10	R11
      0x782E0000,  //  0044  JMPF	R11	#0046
      0x7001FFE8,  //  0045  JMP		#002F
      0x602C000C,  //  0046  GETGBL	R11	G12
      0x5C300600,  //  0047  MOVE	R12	R3
      0x7C2C0200,  //  0048  CALL	R11	1
      0x6030000C,  //  0049  GETGBL	R12	G12
      0x5C341400,  //  004A  MOVE	R13	R10
      0x7C300200,  //  004B  CALL	R12	1
      0x002C160C,  //  004C  ADD	R11	R11	R12
      0x8830010F,  //  004D  GETMBR	R12	R0	K15
      0x242C160C,  //  004E  GT	R11	R11	R12
      0x782E0002,  //  004F  JMPF	R11	#0053
      0x9002040A,  //  0050  SETMBR	R0	K2	R10
      0x50100000,  //  0051  LDBOOL	R4	0	0
      0x70020002,  //  0052  JMP		#0056
      0x8C2C0710,  //  0053  GETMET	R11	R3	K16
      0x5C341400,  //  0054  MOVE	R13	R10
      0x7C2C0400,  //  0055  CALL	R11	2
      0x7001FFD7,  //  0056  JMP		#002F
      0x78120012,  //  0057  JMPF	R4	#006B
      0x6024000F,  //  0058  GETGBL	R9	G15
      0x88280107,  //  0059  GETMBR	R10	R0	K7
      0x602C0012,  //  005A  GETGBL	R11	G18
      0x7C240400,  //  005B  CALL	R9	2
      0x7826000B,  //  005C  JMPF	R9	#0069
      0x88240107,  //  005D  GETMBR	R9	R0	K7
      0x8C241311,  //  005E  GETMET	R9	R9	K17
      0x582C0008,  //  005F  LDCONST	R11	K8
      0x7C240400,  //  0060  CALL	R9	2
      0x6024000C,  //  0061  GETGBL	R9	G12
      0x88280107,  //  0062  GETMBR	R10	R0	K7
      0x7C240200,  //  0063  CALL	R9	1
      0x1C241308,  //  0064  EQ	R9	R9	K8
      0x78260001,  //  0065  JMPF	R9	#0068
      0x4C240000,  //  0066  LDNIL	R9
      0x90020E09,  //  0067  SETMBR	R0	K7	R9
      0x70020001,  //  0068  JMP		#006B
      0x4C240000,  //  0069  LDNIL	R9
      0x90020E09,  //  006A  SETMBR	R0	K7	R9
      0x7001FFB3,  //  006B  JMP		#0020
      0x78120017,  //  006C  JMPF	R4	#0085
      0x881C0105,  //  006D  GETMBR	R7	R0	K5
      0x4C200000,  //  006E  LDNIL	R8
      0x201C0E08,  //  006F  NE	R7	R7	R8
      0x781E0013,  //  0070  JMPF	R7	#0085
      0x601C000C,  //  0071  GETGBL	R7	G12
      0x88200105,  //  0072  GETMBR	R8	R0	K5
      0x7C1C0200,  //  0073  CALL	R7	1
      0x241C0F08,  //  0074  GT	R7	R7	K8
      0x781E000E,  //  0075  JMPF	R7	#0085
      0x601C000C,  //  0076  GETGBL	R7	G12
      0x5C200600,  //  0077  MOVE	R8	R3
      0x7C1C0200,  //  0078  CALL	R7	1
      0x6020000C,  //  0079  GETGBL	R8	G12
      0x88240105,  //  007A  GETMBR	R9	R0	K5
      0x7C200200,  //  007B  CALL	R8	1
      0x001C0E08,  //  007C  ADD	R7	R7	R8
      0x8820010F,  //  007D  GETMBR	R8	R0	K15
      0x241C0E08,  //  007E  GT	R7	R7	R8
      0x781E0002,  //  007F  JMPF	R7	#0083
      0x50100000,  //  0080  LDBOOL	R4	0	0
      0x4C180000,  //  0081  LDNIL	R6
      0x70020001,  //  0082  JMP		#0085
      0x4C1C0000,  //  0083  LDNIL	R7
      0x90020A07,  //  0084  SETMBR	R0	K5	R7
      0x7812006D,  //  0085  JMPF	R4	#00F4
      0x881C0106,  //  0086  GETMBR	R7	R0	K6
      0x4C200000,  //  0087  LDNIL	R8
      0x201C0E08,  //  0088  NE	R7	R7	R8
      0x781E0069,  //  0089  JMPF	R7	#00F4
      0x601C000F,  //  008A  GETGBL	R7	G15
      0x88200106,  //  008B  GETMBR	R8	R0	K6
      0x60240012,  //  008C  GETGBL	R9	G18
      0x7C1C0400,  //  008D  CALL	R7	2
      0x781E0002,  //  008E  JMPF	R7	#0092
      0x881C0106,  //  008F  GETMBR	R7	R0	K6
      0x941C0F08,  //  0090  GETIDX	R7	R7	K8
      0x70020000,  //  0091  JMP		#0093
      0x881C0106,  //  0092  GETMBR	R7	R0	K6
      0x4C200000,  //  0093  LDNIL	R8
      0x78120049,  //  0094  JMPF	R4	#00DF
      0x8C240F12,  //  0095  GETMET	R9	R7	K18
      0x7C240200,  //  0096  CALL	R9	1
      0x5C201200,  //  0097  MOVE	R8	R9
      0x78260045,  //  0098  JMPF	R9	#00DF
      0x7816002D,  //  0099  JMPF	R5	#00C8
      0xB8262600,  //  009A  GETNGBL	R9	K19
      0x8C241314,  //  009B  GETMET	R9	R9	K20
      0x582C0015,  //  009C  LDCONST	R11	K21
      0x7C240400,  //  009D  CALL	R9	2
      0x78260028,  //  009E  JMPF	R9	#00C8
      0x58240016,  //  009F  LDCONST	R9	K22
      0x88281117,  //  00A0  GETMBR	R10	R8	K23
      0x4C2C0000,  //  00A1  LDNIL	R11
      0x2028140B,  //  00A2  NE	R10	R10	R11
      0x782A0004,  //  00A3  JMPF	R10	#00A9
      0x60280008,  //  00A4  GETGBL	R10	G8
      0x882C1117,  //  00A5  GETMBR	R11	R8	K23
      0x7C280200,  //  00A6  CALL	R10	1
      0x002A300A,  //  00A7  ADD	R10	K24	R10
      0x5C241400,  //  00A8  MOVE	R9	R10
      0x88281119,  //  00A9  GETMBR	R10	R8	K25
      0x4C2C0000,  //  00AA  LDNIL	R11
      0x2028140B,  //  00AB  NE	R10	R10	R11
      0x782A0004,  //  00AC  JMPF	R10	#00B2
      0x60280008,  //  00AD  GETGBL	R10	G8
      0x882C1119,  //  00AE  GETMBR	R11	R8	K25
      0x7C280200,  //  00AF  CALL	R10	1
      0x002A340A,  //  00B0  ADD	R10	K26	R10
      0x0024120A,  //  00B1  ADD	R9	R9	R10
      0x8828111B,  //  00B2  GETMBR	R10	R8	K27
      0x4C2C0000,  //  00B3  LDNIL	R11
      0x2028140B,  //  00B4  NE	R10	R10	R11
      0x782A0004,  //  00B5  JMPF	R10	#00BB
      0x60280008,  //  00B6  GETGBL	R10	G8
      0x882C111B,  //  00B7  GETMBR	R11	R8	K27
      0x7C280200,  //  00B8  CALL	R10	1
      0x002A340A,  //  00B9  ADD	R10	K26	R10
      0x0024120A,  //  00BA  ADD	R9	R9	R10
      0xB82A3800,  //  00BB  GETNGBL	R10	K28
      0x602C0018,  //  00BC  GETGBL	R11	G24
      0x5830001D,  //  00BD  LDCONST	R12	K29
      0x8834050E,  //  00BE  GETMBR	R13	R2	K14
      0x88341B1E,  //  00BF  GETMBR	R13	R13	K30
      0x8838111F,  //  00C0  GETMBR	R14	R8	K31
      0x883C1120,  //  00C1  GETMBR	R15	R8	K32
      0x88401121,  //  00C2  GETMBR	R16	R8	K33
      0x88441122,  //  00C3  GETMBR	R17	R8	K34
      0x5C481200,  //  00C4  MOVE	R18	R9
      0x7C2C0E00,  //  00C5  CALL	R11	7
      0x58300015,  //  00C6  LDCONST	R12	K21
      0x7C280400,  //  00C7  CALL	R10	2
      0x8C241123,  //  00C8  GETMET	R9	R8	K35
      0x7C240200,  //  00C9  CALL	R9	1
      0x6028000C,  //  00CA  GETGBL	R10	G12
      0x5C2C0600,  //  00CB  MOVE	R11	R3
      0x7C280200,  //  00CC  CALL	R10	1
      0x602C000C,  //  00CD  GETGBL	R11	G12
      0x5C300C00,  //  00CE  MOVE	R12	R6
      0x7C2C0200,  //  00CF  CALL	R11	1
      0x0028140B,  //  00D0  ADD	R10	R10	R11
      0x602C000C,  //  00D1  GETGBL	R11	G12
      0x5C301200,  //  00D2  MOVE	R12	R9
      0x7C2C0200,  //  00D3  CALL	R11	1
      0x0028140B,  //  00D4  ADD	R10	R10	R11
      0x882C010F,  //  00D5  GETMBR	R11	R0	K15
      0x2428140B,  //  00D6  GT	R10	R10	R11
      0x782A0002,  //  00D7  JMPF	R10	#00DB
      0x90020A09,  //  00D8  SETMBR	R0	K5	R9
      0x50100000,  //  00D9  LDBOOL	R4	0	0
      0x70020002,  //  00DA  JMP		#00DE
      0x8C280D10,  //  00DB  GETMET	R10	R6	K16
      0x5C301200,  //  00DC  MOVE	R12	R9
      0x7C280400,  //  00DD  CALL	R10	2
      0x7001FFB4,  //  00DE  JMP		#0094
      0x78120012,  //  00DF  JMPF	R4	#00F3
      0x6024000F,  //  00E0  GETGBL	R9	G15
      0x88280106,  //  00E1  GETMBR	R10	R0	K6
      0x602C0012,  //  00E2  GETGBL	R11	G18
      0x7C240400,  //  00E3  CALL	R9	2
      0x7826000B,  //  00E4  JMPF	R9	#00F1
      0x88240106,  //  00E5  GETMBR	R9	R0	K6
      0x8C241311,  //  00E6  GETMET	R9	R9	K17
      0x582C0008,  //  00E7  LDCONST	R11	K8
      0x7C240400,  //  00E8  CALL	R9	2
      0x6024000C,  //  00E9  GETGBL	R9	G12
      0x88280106,  //  00EA  GETMBR	R10	R0	K6
      0x7C240200,  //  00EB  CALL	R9	1
      0x1C241308,  //  00EC  EQ	R9	R9	K8
      0x78260001,  //  00ED  JMPF	R9	#00F0
      0x4C240000,  //  00EE  LDNIL	R9
      0x90020C09,  //  00EF  SETMBR	R0	K6	R9
      0x70020001,  //  00F0  JMP		#00F3
      0x4C240000,  //  00F1  LDNIL	R9
      0x90020C09,  //  00F2  SETMBR	R0	K6	R9
      0x7001FF90,  //  00F3  JMP		#0085
      0xB81E4800,  //  00F4  GETNGBL	R7	K36
      0x8C1C0F25,  //  00F5  GETMET	R7	R7	K37
      0x7C1C0200,  //  00F6  CALL	R7	1
      0x88200126,  //  00F7  GETMBR	R8	R0	K38
      0x901E4C08,  //  00F8  SETMBR	R7	K38	R8
      0x88200127,  //  00F9  GETMBR	R8	R0	K39
      0x901E4E08,  //  00FA  SETMBR	R7	K39	R8
      0x4C200000,  //  00FB  LDNIL	R8
      0x20200608,  //  00FC  NE	R8	R3	R8
      0x78220008,  //  00FD  JMPF	R8	#0107
      0x6020000C,  //  00FE  GETGBL	R8	G12
      0x5C240600,  //  00FF  MOVE	R9	R3
      0x7C200200,  //  0100  CALL	R8	1
      0x24201108,  //  0101  GT	R8	R8	K8
      0x78220003,  //  0102  JMPF	R8	#0107
      0x60200012,  //  0103  GETGBL	R8	G18
      0x7C200000,  //  0104  CALL	R8	0
      0x40241003,  //  0105  CONNECT	R9	R8	R3
      0x901E5008,  //  0106  SETMBR	R7	K40	R8
      0x4C200000,  //  0107  LDNIL	R8
      0x20200C08,  //  0108  NE	R8	R6	R8
      0x78220008,  //  0109  JMPF	R8	#0113
      0x6020000C,  //  010A  GETGBL	R8	G12
      0x5C240C00,  //  010B  MOVE	R9	R6
      0x7C200200,  //  010C  CALL	R8	1
      0x24201108,  //  010D  GT	R8	R8	K8
      0x78220003,  //  010E  JMPF	R8	#0113
      0x60200012,  //  010F  GETGBL	R8	G18
      0x7C200000,  //  0110  CALL	R8	0
      0x40241006,  //  0111  CONNECT	R9	R8	R6
      0x901E5208,  //  0112  SETMBR	R7	K41	R8
      0x88200102,  //  0113  GETMBR	R8	R0	K2
      0x4C240000,  //  0114  LDNIL	R9
      0x20201009,  //  0115  NE	R8	R8	R9
      0x74220004,  //  0116  JMPT	R8	#011C
      0x88200105,  //  0117  GETMBR	R8	R0	K5
      0x4C240000,  //  0118  LDNIL	R9
      0x20201009,  //  0119  NE	R8	R8	R9
      0x74220000,  //  011A  JMPT	R8	#011C
      0x50200001,  //  011B  LDBOOL	R8	0	1
      0x50200200,  //  011C  LDBOOL	R8	1	0
      0x901E5408,  //  011D  SETMBR	R7	K42	R8
      0x8C200F2B,  //  011E  GETMET	R8	R7	K43
      0x7C200200,  //  011F  CALL	R8	1
      0x8C24112C,  //  0120  GETMET	R9	R8	K44
      0x602C0015,  //  0121  GETGBL	R11	G21
      0x8830010F,  //  0122  GETMBR	R12	R0	K15
      0x7C2C0200,  //  0123  CALL	R11	1
      0x7C240400,  //  0124  CALL	R9	2
      0x8C28052D,  //  0125  GETMET	R10	R2	K45
      0x5C301200,  //  0126  MOVE	R12	R9
      0x7C280400,  //  0127  CALL	R10	2
      0x8C28052E,  //  0128  GETMET	R10	R2	K46
      0x7C280200,  //  0129  CALL	R10	1
      0x8C28032F,  //  012A  GETMET	R10	R1	K47
      0x5C300400,  //  012B  MOVE	R12	R2
      0x7C280400,  //  012C  CALL	R10	2
      0x88280531,  //  012D  GETMBR	R10	R2	K49
      0x9002600A,  //  012E  SETMBR	R0	K48	R10
      0x88280F2A,  //  012F  GETMBR	R10	R7	K42
      0x782A0002,  //  0130  JMPF	R10	#0134
      0x50280000,  //  0131  LDBOOL	R10	0	0
      0x9002000A,  //  0132  SETMBR	R0	K0	R10
      0x70020001,  //  0133  JMP		#0136
      0x50280200,  //  0134  LDBOOL	R10	1	0
      0x9002640A,  //  0135  SETMBR	R0	K50	R10
      0x80000000,  //  0136  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_suppress_response
********************************************************************/
extern const bclass be_class_Matter_IM_ReportData_Pull;
be_local_closure(class_Matter_IM_ReportData_Pull_set_suppress_response,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_ReportData_Pull, 
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(suppress_response),
    }),
    be_str_weak(set_suppress_response),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM_ReportData_Pull
********************************************************************/
extern const bclass be_class_Matter_IM_Message;
be_local_class(Matter_IM_ReportData_Pull,
    5,
    &be_class_Matter_IM_Message,
    be_nested_map(10,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(set_suppress_response, 2), be_const_closure(class_Matter_IM_ReportData_Pull_set_suppress_response_closure) },
        { be_const_key_weak(data_ev, -1), be_const_var(4) },
        { be_const_key_weak(event_generator_or_arr, -1), be_const_var(1) },
        { be_const_key_weak(MAX_MESSAGE, -1), be_const_int(1200) },
        { be_const_key_weak(subscription_id, -1), be_const_var(2) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_IM_ReportData_Pull_init_closure) },
        { be_const_key_weak(generator_or_arr, 9), be_const_var(0) },
        { be_const_key_weak(set_subscription_id, 0), be_const_closure(class_Matter_IM_ReportData_Pull_set_subscription_id_closure) },
        { be_const_key_weak(send_im, -1), be_const_closure(class_Matter_IM_ReportData_Pull_send_im_closure) },
        { be_const_key_weak(suppress_response, -1), be_const_var(3) },
    })),
    be_str_weak(Matter_IM_ReportData_Pull)
);

extern const bclass be_class_Matter_IM_ReportDataSubscribed_Pull;

/********************************************************************
** Solidified function: ack_received
********************************************************************/
extern const bclass be_class_Matter_IM_ReportDataSubscribed_Pull;
be_local_closure(class_Matter_IM_ReportDataSubscribed_Pull_ack_received,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_ReportDataSubscribed_Pull, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(ack_received),
    /* K1   */  be_nested_str_weak(report_data_phase),
    /* K2   */  be_nested_str_weak(sub),
    /* K3   */  be_nested_str_weak(is_keep_alive),
    /* K4   */  be_nested_str_weak(re_arm),
    }),
    be_str_weak(ack_received),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x740A0008,  //  0007  JMPT	R2	#0011
      0x88080102,  //  0008  GETMBR	R2	R0	K2
      0x88080503,  //  0009  GETMBR	R2	R2	K3
      0x780A0002,  //  000A  JMPF	R2	#000E
      0x88080102,  //  000B  GETMBR	R2	R0	K2
      0x8C080504,  //  000C  GETMET	R2	R2	K4
      0x7C080200,  //  000D  CALL	R2	1
      0x50080200,  //  000E  LDBOOL	R2	1	0
      0x80040400,  //  000F  RET	1	R2
      0x70020001,  //  0010  JMP		#0013
      0x50080000,  //  0011  LDBOOL	R2	0	0
      0x80040400,  //  0012  RET	1	R2
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_im
********************************************************************/
extern const bclass be_class_Matter_IM_ReportDataSubscribed_Pull;
be_local_closure(class_Matter_IM_ReportDataSubscribed_Pull_send_im,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_ReportDataSubscribed_Pull, 
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(ready),
    /* K1   */  be_nested_str_weak(generator_or_arr),
    /* K2   */  be_nested_str_weak(event_generator_or_arr),
    /* K3   */  be_nested_str_weak(report_data_phase),
    /* K4   */  be_nested_str_weak(send_im),
    /* K5   */  be_nested_str_weak(finish),
    /* K6   */  be_nested_str_weak(resp),
    /* K7   */  be_nested_str_weak(build_standalone_ack),
    /* K8   */  be_nested_str_weak(encode_frame),
    /* K9   */  be_nested_str_weak(encrypt),
    /* K10  */  be_nested_str_weak(tasmota),
    /* K11  */  be_nested_str_weak(loglevel),
    /* K12  */  be_nested_str_weak(log),
    /* K13  */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i),
    /* K14  */  be_nested_str_weak(session),
    /* K15  */  be_nested_str_weak(local_session_id),
    /* K16  */  be_nested_str_weak(ack_message_counter),
    /* K17  */  be_nested_str_weak(message_counter),
    /* K18  */  be_nested_str_weak(send_response_frame),
    /* K19  */  be_nested_str_weak(last_counter),
    /* K20  */  be_nested_str_weak(sub),
    /* K21  */  be_nested_str_weak(re_arm),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[81]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0001,  //  0001  JMPT	R2	#0004
      0x50080000,  //  0002  LDBOOL	R2	0	0
      0x80040400,  //  0003  RET	1	R2
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0005  LDNIL	R3
      0x20080403,  //  0006  NE	R2	R2	R3
      0x740A0003,  //  0007  JMPT	R2	#000C
      0x88080102,  //  0008  GETMBR	R2	R0	K2
      0x4C0C0000,  //  0009  LDNIL	R3
      0x20080403,  //  000A  NE	R2	R2	R3
      0x780A0033,  //  000B  JMPF	R2	#0040
      0x88080103,  //  000C  GETMBR	R2	R0	K3
      0x780A000F,  //  000D  JMPF	R2	#001E
      0x60080003,  //  000E  GETGBL	R2	G3
      0x5C0C0000,  //  000F  MOVE	R3	R0
      0x7C080200,  //  0010  CALL	R2	1
      0x8C080504,  //  0011  GETMET	R2	R2	K4
      0x5C100200,  //  0012  MOVE	R4	R1
      0x7C080400,  //  0013  CALL	R2	2
      0x88080105,  //  0014  GETMBR	R2	R0	K5
      0x740A0000,  //  0015  JMPT	R2	#0017
      0x80000400,  //  0016  RET	0
      0x50080000,  //  0017  LDBOOL	R2	0	0
      0x90020602,  //  0018  SETMBR	R0	K3	R2
      0x50080000,  //  0019  LDBOOL	R2	0	0
      0x90020002,  //  001A  SETMBR	R0	K0	R2
      0x50080000,  //  001B  LDBOOL	R2	0	0
      0x90020A02,  //  001C  SETMBR	R0	K5	R2
      0x70020020,  //  001D  JMP		#003F
      0x88080106,  //  001E  GETMBR	R2	R0	K6
      0x8C080507,  //  001F  GETMET	R2	R2	K7
      0x50100000,  //  0020  LDBOOL	R4	0	0
      0x7C080400,  //  0021  CALL	R2	2
      0x8C0C0508,  //  0022  GETMET	R3	R2	K8
      0x7C0C0200,  //  0023  CALL	R3	1
      0x8C0C0509,  //  0024  GETMET	R3	R2	K9
      0x7C0C0200,  //  0025  CALL	R3	1
      0xB80E1400,  //  0026  GETNGBL	R3	K10
      0x8C0C070B,  //  0027  GETMET	R3	R3	K11
      0x54160003,  //  0028  LDINT	R5	4
      0x7C0C0400,  //  0029  CALL	R3	2
      0x780E0009,  //  002A  JMPF	R3	#0035
      0xB80E1800,  //  002B  GETNGBL	R3	K12
      0x60100018,  //  002C  GETGBL	R4	G24
      0x5814000D,  //  002D  LDCONST	R5	K13
      0x8818050E,  //  002E  GETMBR	R6	R2	K14
      0x88180D0F,  //  002F  GETMBR	R6	R6	K15
      0x881C0510,  //  0030  GETMBR	R7	R2	K16
      0x88200511,  //  0031  GETMBR	R8	R2	K17
      0x7C100800,  //  0032  CALL	R4	4
      0x54160003,  //  0033  LDINT	R5	4
      0x7C0C0400,  //  0034  CALL	R3	2
      0x8C0C0312,  //  0035  GETMET	R3	R1	K18
      0x5C140400,  //  0036  MOVE	R5	R2
      0x7C0C0400,  //  0037  CALL	R3	2
      0x880C0511,  //  0038  GETMBR	R3	R2	K17
      0x90022603,  //  0039  SETMBR	R0	K19	R3
      0x500C0200,  //  003A  LDBOOL	R3	1	0
      0x90020A03,  //  003B  SETMBR	R0	K5	R3
      0x880C0114,  //  003C  GETMBR	R3	R0	K20
      0x8C0C0715,  //  003D  GETMET	R3	R3	K21
      0x7C0C0200,  //  003E  CALL	R3	1
      0x7002000F,  //  003F  JMP		#0050
      0x88080103,  //  0040  GETMBR	R2	R0	K3
      0x780A0008,  //  0041  JMPF	R2	#004B
      0x60080003,  //  0042  GETGBL	R2	G3
      0x5C0C0000,  //  0043  MOVE	R3	R0
      0x7C080200,  //  0044  CALL	R2	1
      0x8C080504,  //  0045  GETMET	R2	R2	K4
      0x5C100200,  //  0046  MOVE	R4	R1
      0x7C080400,  //  0047  CALL	R2	2
      0x50080000,  //  0048  LDBOOL	R2	0	0
      0x90020602,  //  0049  SETMBR	R0	K3	R2
      0x70020004,  //  004A  JMP		#0050
      0x50080200,  //  004B  LDBOOL	R2	1	0
      0x90020A02,  //  004C  SETMBR	R0	K5	R2
      0x88080114,  //  004D  GETMBR	R2	R0	K20
      0x8C080515,  //  004E  GETMET	R2	R2	K21
      0x7C080200,  //  004F  CALL	R2	1
      0x80000000,  //  0050  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_IM_ReportDataSubscribed_Pull;
be_local_closure(class_Matter_IM_ReportDataSubscribed_Pull_init,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    6,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_ReportDataSubscribed_Pull, 
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(resp),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(Frame),
    /* K4   */  be_nested_str_weak(initiate_response),
    /* K5   */  be_nested_str_weak(sub),
    /* K6   */  be_nested_str_weak(report_data_phase),
    /* K7   */  be_nested_str_weak(set_subscription_id),
    /* K8   */  be_nested_str_weak(subscription_id),
    /* K9   */  be_nested_str_weak(set_suppress_response),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x60180003,  //  0000  GETGBL	R6	G3
      0x5C1C0000,  //  0001  MOVE	R7	R0
      0x7C180200,  //  0002  CALL	R6	1
      0x8C180D00,  //  0003  GETMET	R6	R6	K0
      0x4C200000,  //  0004  LDNIL	R8
      0x5C240600,  //  0005  MOVE	R9	R3
      0x5C280800,  //  0006  MOVE	R10	R4
      0x7C180800,  //  0007  CALL	R6	4
      0xB81A0400,  //  0008  GETNGBL	R6	K2
      0x88180D03,  //  0009  GETMBR	R6	R6	K3
      0x8C180D04,  //  000A  GETMET	R6	R6	K4
      0x5C200200,  //  000B  MOVE	R8	R1
      0x5C240400,  //  000C  MOVE	R9	R2
      0x542A0004,  //  000D  LDINT	R10	5
      0x502C0200,  //  000E  LDBOOL	R11	1	0
      0x7C180A00,  //  000F  CALL	R6	5
      0x90020206,  //  0010  SETMBR	R0	K1	R6
      0x90020A05,  //  0011  SETMBR	R0	K5	R5
      0x50180200,  //  0012  LDBOOL	R6	1	0
      0x90020C06,  //  0013  SETMBR	R0	K6	R6
      0x8C180107,  //  0014  GETMET	R6	R0	K7
      0x88200B08,  //  0015  GETMBR	R8	R5	K8
      0x7C180400,  //  0016  CALL	R6	2
      0x8C180109,  //  0017  GETMET	R6	R0	K9
      0x50200000,  //  0018  LDBOOL	R8	0	0
      0x7C180400,  //  0019  CALL	R6	2
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: status_error_received
********************************************************************/
extern const bclass be_class_Matter_IM_ReportDataSubscribed_Pull;
be_local_closure(class_Matter_IM_ReportDataSubscribed_Pull_status_error_received,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_ReportDataSubscribed_Pull, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(sub),
    /* K1   */  be_nested_str_weak(remove_self),
    }),
    be_str_weak(status_error_received),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reached_timeout
********************************************************************/
extern const bclass be_class_Matter_IM_ReportDataSubscribed_Pull;
be_local_closure(class_Matter_IM_ReportDataSubscribed_Pull_reached_timeout,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_ReportDataSubscribed_Pull, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(sub),
    /* K1   */  be_nested_str_weak(remove_self),
    }),
    be_str_weak(reached_timeout),
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
** Solidified function: status_ok_received
********************************************************************/
extern const bclass be_class_Matter_IM_ReportDataSubscribed_Pull;
be_local_closure(class_Matter_IM_ReportDataSubscribed_Pull_status_ok_received,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_ReportDataSubscribed_Pull, 
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(report_data_phase),
    /* K1   */  be_nested_str_weak(status_ok_received),
    /* K2   */  be_nested_str_weak(sub),
    /* K3   */  be_nested_str_weak(re_arm),
    }),
    be_str_weak(status_ok_received),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A0007,  //  0001  JMPF	R2	#000A
      0x60080003,  //  0002  GETGBL	R2	G3
      0x5C0C0000,  //  0003  MOVE	R3	R0
      0x7C080200,  //  0004  CALL	R2	1
      0x8C080501,  //  0005  GETMET	R2	R2	K1
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x80040400,  //  0008  RET	1	R2
      0x7002000A,  //  0009  JMP		#0015
      0x88080102,  //  000A  GETMBR	R2	R0	K2
      0x8C080503,  //  000B  GETMET	R2	R2	K3
      0x7C080200,  //  000C  CALL	R2	1
      0x60080003,  //  000D  GETGBL	R2	G3
      0x5C0C0000,  //  000E  MOVE	R3	R0
      0x7C080200,  //  000F  CALL	R2	1
      0x8C080501,  //  0010  GETMET	R2	R2	K1
      0x4C100000,  //  0011  LDNIL	R4
      0x7C080400,  //  0012  CALL	R2	2
      0x50080000,  //  0013  LDBOOL	R2	0	0
      0x80040400,  //  0014  RET	1	R2
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM_ReportDataSubscribed_Pull
********************************************************************/
extern const bclass be_class_Matter_IM_ReportData_Pull;
be_local_class(Matter_IM_ReportDataSubscribed_Pull,
    2,
    &be_class_Matter_IM_ReportData_Pull,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ack_received, 1), be_const_closure(class_Matter_IM_ReportDataSubscribed_Pull_ack_received_closure) },
        { be_const_key_weak(status_ok_received, -1), be_const_closure(class_Matter_IM_ReportDataSubscribed_Pull_status_ok_received_closure) },
        { be_const_key_weak(send_im, -1), be_const_closure(class_Matter_IM_ReportDataSubscribed_Pull_send_im_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_IM_ReportDataSubscribed_Pull_init_closure) },
        { be_const_key_weak(report_data_phase, 7), be_const_var(1) },
        { be_const_key_weak(sub, 6), be_const_var(0) },
        { be_const_key_weak(reached_timeout, -1), be_const_closure(class_Matter_IM_ReportDataSubscribed_Pull_reached_timeout_closure) },
        { be_const_key_weak(status_error_received, -1), be_const_closure(class_Matter_IM_ReportDataSubscribed_Pull_status_error_received_closure) },
    })),
    be_str_weak(Matter_IM_ReportDataSubscribed_Pull)
);

extern const bclass be_class_Matter_IM_SubscribedHeartbeat;

/********************************************************************
** Solidified function: status_error_received
********************************************************************/
extern const bclass be_class_Matter_IM_SubscribedHeartbeat;
be_local_closure(class_Matter_IM_SubscribedHeartbeat_status_error_received,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_SubscribedHeartbeat, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(sub),
    /* K1   */  be_nested_str_weak(remove_self),
    }),
    be_str_weak(status_error_received),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x50080000,  //  0003  LDBOOL	R2	0	0
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_im
********************************************************************/
extern const bclass be_class_Matter_IM_SubscribedHeartbeat;
be_local_closure(class_Matter_IM_SubscribedHeartbeat_send_im,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_SubscribedHeartbeat, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(ready),
    /* K1   */  be_nested_str_weak(send_im),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0001,  //  0001  JMPT	R2	#0004
      0x50080000,  //  0002  LDBOOL	R2	0	0
      0x80040400,  //  0003  RET	1	R2
      0x60080003,  //  0004  GETGBL	R2	G3
      0x5C0C0000,  //  0005  MOVE	R3	R0
      0x7C080200,  //  0006  CALL	R2	1
      0x8C080501,  //  0007  GETMET	R2	R2	K1
      0x5C100200,  //  0008  MOVE	R4	R1
      0x7C080400,  //  0009  CALL	R2	2
      0x50080000,  //  000A  LDBOOL	R2	0	0
      0x90020002,  //  000B  SETMBR	R0	K0	R2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ack_received
********************************************************************/
extern const bclass be_class_Matter_IM_SubscribedHeartbeat;
be_local_closure(class_Matter_IM_SubscribedHeartbeat_ack_received,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_SubscribedHeartbeat, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(ack_received),
    /* K1   */  be_nested_str_weak(finish),
    }),
    be_str_weak(ack_received),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x50080200,  //  0006  LDBOOL	R2	1	0
      0x90020202,  //  0007  SETMBR	R0	K1	R2
      0x50080200,  //  0008  LDBOOL	R2	1	0
      0x80040400,  //  0009  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reached_timeout
********************************************************************/
extern const bclass be_class_Matter_IM_SubscribedHeartbeat;
be_local_closure(class_Matter_IM_SubscribedHeartbeat_reached_timeout,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_SubscribedHeartbeat, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(sub),
    /* K1   */  be_nested_str_weak(remove_self),
    }),
    be_str_weak(reached_timeout),
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
** Solidified function: status_ok_received
********************************************************************/
extern const bclass be_class_Matter_IM_SubscribedHeartbeat;
be_local_closure(class_Matter_IM_SubscribedHeartbeat_status_ok_received,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_SubscribedHeartbeat, 
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(status_ok_received),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x50080000,  //  0000  LDBOOL	R2	0	0
      0x80040400,  //  0001  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_IM_SubscribedHeartbeat;
be_local_closure(class_Matter_IM_SubscribedHeartbeat_init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_SubscribedHeartbeat, 
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(resp),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(Frame),
    /* K4   */  be_nested_str_weak(initiate_response),
    /* K5   */  be_nested_str_weak(sub),
    /* K6   */  be_nested_str_weak(set_subscription_id),
    /* K7   */  be_nested_str_weak(subscription_id),
    /* K8   */  be_nested_str_weak(set_suppress_response),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x4C180000,  //  0004  LDNIL	R6
      0x4C1C0000,  //  0005  LDNIL	R7
      0x4C200000,  //  0006  LDNIL	R8
      0x7C100800,  //  0007  CALL	R4	4
      0xB8120400,  //  0008  GETNGBL	R4	K2
      0x88100903,  //  0009  GETMBR	R4	R4	K3
      0x8C100904,  //  000A  GETMET	R4	R4	K4
      0x5C180200,  //  000B  MOVE	R6	R1
      0x5C1C0400,  //  000C  MOVE	R7	R2
      0x54220004,  //  000D  LDINT	R8	5
      0x50240200,  //  000E  LDBOOL	R9	1	0
      0x7C100A00,  //  000F  CALL	R4	5
      0x90020204,  //  0010  SETMBR	R0	K1	R4
      0x90020A03,  //  0011  SETMBR	R0	K5	R3
      0x8C100106,  //  0012  GETMET	R4	R0	K6
      0x88180707,  //  0013  GETMBR	R6	R3	K7
      0x7C100400,  //  0014  CALL	R4	2
      0x8C100108,  //  0015  GETMET	R4	R0	K8
      0x50180200,  //  0016  LDBOOL	R6	1	0
      0x7C100400,  //  0017  CALL	R4	2
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM_SubscribedHeartbeat
********************************************************************/
extern const bclass be_class_Matter_IM_ReportData_Pull;
be_local_class(Matter_IM_SubscribedHeartbeat,
    1,
    &be_class_Matter_IM_ReportData_Pull,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, 1), be_const_closure(class_Matter_IM_SubscribedHeartbeat_init_closure) },
        { be_const_key_weak(status_ok_received, -1), be_const_closure(class_Matter_IM_SubscribedHeartbeat_status_ok_received_closure) },
        { be_const_key_weak(sub, 6), be_const_var(0) },
        { be_const_key_weak(ack_received, -1), be_const_closure(class_Matter_IM_SubscribedHeartbeat_ack_received_closure) },
        { be_const_key_weak(reached_timeout, -1), be_const_closure(class_Matter_IM_SubscribedHeartbeat_reached_timeout_closure) },
        { be_const_key_weak(status_error_received, 0), be_const_closure(class_Matter_IM_SubscribedHeartbeat_status_error_received_closure) },
        { be_const_key_weak(send_im, -1), be_const_closure(class_Matter_IM_SubscribedHeartbeat_send_im_closure) },
    })),
    be_str_weak(Matter_IM_SubscribedHeartbeat)
);

extern const bclass be_class_Matter_IM_SubscribeResponse_Pull;

/********************************************************************
** Solidified function: init
********************************************************************/
extern const bclass be_class_Matter_IM_SubscribeResponse_Pull;
be_local_closure(class_Matter_IM_SubscribeResponse_Pull_init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_SubscribeResponse_Pull, 
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(sub),
    /* K2   */  be_nested_str_weak(report_data_phase),
    /* K3   */  be_nested_str_weak(set_subscription_id),
    /* K4   */  be_nested_str_weak(subscription_id),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x60140003,  //  0000  GETGBL	R5	G3
      0x5C180000,  //  0001  MOVE	R6	R0
      0x7C140200,  //  0002  CALL	R5	1
      0x8C140B00,  //  0003  GETMET	R5	R5	K0
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x5C240600,  //  0006  MOVE	R9	R3
      0x7C140800,  //  0007  CALL	R5	4
      0x90020204,  //  0008  SETMBR	R0	K1	R4
      0x50140200,  //  0009  LDBOOL	R5	1	0
      0x90020405,  //  000A  SETMBR	R0	K2	R5
      0x8C140103,  //  000B  GETMET	R5	R0	K3
      0x881C0904,  //  000C  GETMBR	R7	R4	K4
      0x7C140400,  //  000D  CALL	R5	2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: status_ok_received
********************************************************************/
extern const bclass be_class_Matter_IM_SubscribeResponse_Pull;
be_local_closure(class_Matter_IM_SubscribeResponse_Pull_status_ok_received,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_SubscribeResponse_Pull, 
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(loglevel),
    /* K2   */  be_const_int(3),
    /* K3   */  be_nested_str_weak(log),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20_X3ESub_OK_X20_X20_X20_X20_X28_X256i_X29_X20sub_X3D_X25i),
    /* K5   */  be_nested_str_weak(session),
    /* K6   */  be_nested_str_weak(local_session_id),
    /* K7   */  be_nested_str_weak(sub),
    /* K8   */  be_nested_str_weak(subscription_id),
    /* K9   */  be_nested_str_weak(status_ok_received),
    }),
    be_str_weak(status_ok_received),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0009,  //  0004  JMPF	R2	#000F
      0xB80A0600,  //  0005  GETNGBL	R2	K3
      0x600C0018,  //  0006  GETGBL	R3	G24
      0x58100004,  //  0007  LDCONST	R4	K4
      0x88140305,  //  0008  GETMBR	R5	R1	K5
      0x88140B06,  //  0009  GETMBR	R5	R5	K6
      0x88180107,  //  000A  GETMBR	R6	R0	K7
      0x88180D08,  //  000B  GETMBR	R6	R6	K8
      0x7C0C0600,  //  000C  CALL	R3	3
      0x58100002,  //  000D  LDCONST	R4	K2
      0x7C080400,  //  000E  CALL	R2	2
      0x60080003,  //  000F  GETGBL	R2	G3
      0x5C0C0000,  //  0010  MOVE	R3	R0
      0x7C080200,  //  0011  CALL	R2	1
      0x8C080509,  //  0012  GETMET	R2	R2	K9
      0x5C100200,  //  0013  MOVE	R4	R1
      0x7C080400,  //  0014  CALL	R2	2
      0x80040400,  //  0015  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_im
********************************************************************/
extern const bclass be_class_Matter_IM_SubscribeResponse_Pull;
be_local_closure(class_Matter_IM_SubscribeResponse_Pull_send_im,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_SubscribeResponse_Pull, 
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(ready),
    /* K1   */  be_nested_str_weak(report_data_phase),
    /* K2   */  be_nested_str_weak(send_im),
    /* K3   */  be_nested_str_weak(finish),
    /* K4   */  be_nested_str_weak(resp),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(SubscribeResponseMessage),
    /* K7   */  be_nested_str_weak(subscription_id),
    /* K8   */  be_nested_str_weak(sub),
    /* K9   */  be_nested_str_weak(max_interval),
    /* K10  */  be_nested_str_weak(opcode),
    /* K11  */  be_nested_str_weak(encode_frame),
    /* K12  */  be_nested_str_weak(to_TLV),
    /* K13  */  be_nested_str_weak(tlv2raw),
    /* K14  */  be_nested_str_weak(encrypt),
    /* K15  */  be_nested_str_weak(send_response_frame),
    /* K16  */  be_nested_str_weak(last_counter),
    /* K17  */  be_nested_str_weak(message_counter),
    /* K18  */  be_nested_str_weak(re_arm),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0001,  //  0001  JMPT	R2	#0004
      0x50080000,  //  0002  LDBOOL	R2	0	0
      0x80040400,  //  0003  RET	1	R2
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x780A000E,  //  0005  JMPF	R2	#0015
      0x60080003,  //  0006  GETGBL	R2	G3
      0x5C0C0000,  //  0007  MOVE	R3	R0
      0x7C080200,  //  0008  CALL	R2	1
      0x8C080502,  //  0009  GETMET	R2	R2	K2
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x88080103,  //  000C  GETMBR	R2	R0	K3
      0x780A0003,  //  000D  JMPF	R2	#0012
      0x50080000,  //  000E  LDBOOL	R2	0	0
      0x90020202,  //  000F  SETMBR	R0	K1	R2
      0x50080000,  //  0010  LDBOOL	R2	0	0
      0x90020602,  //  0011  SETMBR	R0	K3	R2
      0x50080000,  //  0012  LDBOOL	R2	0	0
      0x90020002,  //  0013  SETMBR	R0	K0	R2
      0x7002001E,  //  0014  JMP		#0034
      0x88080104,  //  0015  GETMBR	R2	R0	K4
      0xB80E0A00,  //  0016  GETNGBL	R3	K5
      0x8C0C0706,  //  0017  GETMET	R3	R3	K6
      0x7C0C0200,  //  0018  CALL	R3	1
      0x88100108,  //  0019  GETMBR	R4	R0	K8
      0x88100907,  //  001A  GETMBR	R4	R4	K7
      0x900E0E04,  //  001B  SETMBR	R3	K7	R4
      0x88100108,  //  001C  GETMBR	R4	R0	K8
      0x88100909,  //  001D  GETMBR	R4	R4	K9
      0x900E1204,  //  001E  SETMBR	R3	K9	R4
      0x88100104,  //  001F  GETMBR	R4	R0	K4
      0x54160003,  //  0020  LDINT	R5	4
      0x90121405,  //  0021  SETMBR	R4	K10	R5
      0x8C10050B,  //  0022  GETMET	R4	R2	K11
      0x8C18070C,  //  0023  GETMET	R6	R3	K12
      0x7C180200,  //  0024  CALL	R6	1
      0x8C180D0D,  //  0025  GETMET	R6	R6	K13
      0x7C180200,  //  0026  CALL	R6	1
      0x7C100400,  //  0027  CALL	R4	2
      0x8C10050E,  //  0028  GETMET	R4	R2	K14
      0x7C100200,  //  0029  CALL	R4	1
      0x8C10030F,  //  002A  GETMET	R4	R1	K15
      0x5C180400,  //  002B  MOVE	R6	R2
      0x7C100400,  //  002C  CALL	R4	2
      0x88100511,  //  002D  GETMBR	R4	R2	K17
      0x90022004,  //  002E  SETMBR	R0	K16	R4
      0x88100108,  //  002F  GETMBR	R4	R0	K8
      0x8C100912,  //  0030  GETMET	R4	R4	K18
      0x7C100200,  //  0031  CALL	R4	1
      0x50100200,  //  0032  LDBOOL	R4	1	0
      0x90020604,  //  0033  SETMBR	R0	K3	R4
      0x80000000,  //  0034  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM_SubscribeResponse_Pull
********************************************************************/
extern const bclass be_class_Matter_IM_ReportData_Pull;
be_local_class(Matter_IM_SubscribeResponse_Pull,
    2,
    &be_class_Matter_IM_ReportData_Pull,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, 4), be_const_closure(class_Matter_IM_SubscribeResponse_Pull_init_closure) },
        { be_const_key_weak(sub, -1), be_const_var(0) },
        { be_const_key_weak(status_ok_received, -1), be_const_closure(class_Matter_IM_SubscribeResponse_Pull_status_ok_received_closure) },
        { be_const_key_weak(send_im, -1), be_const_closure(class_Matter_IM_SubscribeResponse_Pull_send_im_closure) },
        { be_const_key_weak(report_data_phase, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_IM_SubscribeResponse_Pull)
);
/********************************************************************/
/* End of solidification */
