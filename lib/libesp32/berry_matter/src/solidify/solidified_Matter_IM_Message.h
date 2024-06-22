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
** Solidified function: send_im
********************************************************************/
extern const bclass be_class_Matter_IM_ReportData_Pull;
be_local_closure(class_Matter_IM_ReportData_Pull_send_im,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_ReportData_Pull, 
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(ready),
    /* K1   */  be_nested_str_weak(resp),
    /* K2   */  be_nested_str_weak(data),
    /* K3   */  be_nested_str_weak(generator_or_arr),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(next),
    /* K6   */  be_nested_str_weak(device),
    /* K7   */  be_nested_str_weak(debug),
    /* K8   */  be_nested_str_weak(is_direct),
    /* K9   */  be_nested_str_weak(im),
    /* K10  */  be_nested_str_weak(read_single_attribute_to_bytes),
    /* K11  */  be_nested_str_weak(get_pi),
    /* K12  */  be_nested_str_weak(session),
    /* K13  */  be_nested_str_weak(MAX_MESSAGE),
    /* K14  */  be_nested_str_weak(append),
    /* K15  */  be_nested_str_weak(remove),
    /* K16  */  be_nested_str_weak(matter),
    /* K17  */  be_nested_str_weak(ReportDataMessage),
    /* K18  */  be_nested_str_weak(subscription_id),
    /* K19  */  be_nested_str_weak(suppress_response),
    /* K20  */  be_nested_str_weak(attribute_reports),
    /* K21  */  be_nested_str_weak(more_chunked_messages),
    /* K22  */  be_nested_str_weak(to_TLV),
    /* K23  */  be_nested_str_weak(tlv2raw),
    /* K24  */  be_nested_str_weak(encode_frame),
    /* K25  */  be_nested_str_weak(encrypt),
    /* K26  */  be_nested_str_weak(send_response_frame),
    /* K27  */  be_nested_str_weak(last_counter),
    /* K28  */  be_nested_str_weak(message_counter),
    /* K29  */  be_nested_str_weak(finish),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[134]) {  /* code */
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
      0x7812004C,  //  0010  JMPF	R4	#005E
      0x88140103,  //  0011  GETMBR	R5	R0	K3
      0x4C180000,  //  0012  LDNIL	R6
      0x20140A06,  //  0013  NE	R5	R5	R6
      0x78160048,  //  0014  JMPF	R5	#005E
      0x6014000F,  //  0015  GETGBL	R5	G15
      0x88180103,  //  0016  GETMBR	R6	R0	K3
      0x601C0012,  //  0017  GETGBL	R7	G18
      0x7C140400,  //  0018  CALL	R5	2
      0x78160002,  //  0019  JMPF	R5	#001D
      0x88140103,  //  001A  GETMBR	R5	R0	K3
      0x94140B04,  //  001B  GETIDX	R5	R5	K4
      0x70020000,  //  001C  JMP		#001E
      0x88140103,  //  001D  GETMBR	R5	R0	K3
      0x4C180000,  //  001E  LDNIL	R6
      0x78120028,  //  001F  JMPF	R4	#0049
      0x8C1C0B05,  //  0020  GETMET	R7	R5	K5
      0x7C1C0200,  //  0021  CALL	R7	1
      0x5C180E00,  //  0022  MOVE	R6	R7
      0x781E0024,  //  0023  JMPF	R7	#0049
      0x881C0306,  //  0024  GETMBR	R7	R1	K6
      0x881C0F07,  //  0025  GETMBR	R7	R7	K7
      0x8C200B08,  //  0026  GETMET	R8	R5	K8
      0x7C200200,  //  0027  CALL	R8	1
      0x74220001,  //  0028  JMPT	R8	#002B
      0x741E0000,  //  0029  JMPT	R7	#002B
      0x50200001,  //  002A  LDBOOL	R8	0	1
      0x50200200,  //  002B  LDBOOL	R8	1	0
      0x88240309,  //  002C  GETMBR	R9	R1	K9
      0x8C24130A,  //  002D  GETMET	R9	R9	K10
      0x8C2C0B0B,  //  002E  GETMET	R11	R5	K11
      0x7C2C0200,  //  002F  CALL	R11	1
      0x5C300C00,  //  0030  MOVE	R12	R6
      0x8834050C,  //  0031  GETMBR	R13	R2	K12
      0x5C381000,  //  0032  MOVE	R14	R8
      0x7C240A00,  //  0033  CALL	R9	5
      0x4C280000,  //  0034  LDNIL	R10
      0x1C28120A,  //  0035  EQ	R10	R9	R10
      0x782A0000,  //  0036  JMPF	R10	#0038
      0x7001FFE6,  //  0037  JMP		#001F
      0x6028000C,  //  0038  GETGBL	R10	G12
      0x5C2C0600,  //  0039  MOVE	R11	R3
      0x7C280200,  //  003A  CALL	R10	1
      0x602C000C,  //  003B  GETGBL	R11	G12
      0x5C301200,  //  003C  MOVE	R12	R9
      0x7C2C0200,  //  003D  CALL	R11	1
      0x0028140B,  //  003E  ADD	R10	R10	R11
      0x882C010D,  //  003F  GETMBR	R11	R0	K13
      0x2428140B,  //  0040  GT	R10	R10	R11
      0x782A0002,  //  0041  JMPF	R10	#0045
      0x90020409,  //  0042  SETMBR	R0	K2	R9
      0x50100000,  //  0043  LDBOOL	R4	0	0
      0x70020002,  //  0044  JMP		#0048
      0x8C28070E,  //  0045  GETMET	R10	R3	K14
      0x5C301200,  //  0046  MOVE	R12	R9
      0x7C280400,  //  0047  CALL	R10	2
      0x7001FFD5,  //  0048  JMP		#001F
      0x78120012,  //  0049  JMPF	R4	#005D
      0x601C000F,  //  004A  GETGBL	R7	G15
      0x88200103,  //  004B  GETMBR	R8	R0	K3
      0x60240012,  //  004C  GETGBL	R9	G18
      0x7C1C0400,  //  004D  CALL	R7	2
      0x781E000B,  //  004E  JMPF	R7	#005B
      0x881C0103,  //  004F  GETMBR	R7	R0	K3
      0x8C1C0F0F,  //  0050  GETMET	R7	R7	K15
      0x58240004,  //  0051  LDCONST	R9	K4
      0x7C1C0400,  //  0052  CALL	R7	2
      0x601C000C,  //  0053  GETGBL	R7	G12
      0x88200103,  //  0054  GETMBR	R8	R0	K3
      0x7C1C0200,  //  0055  CALL	R7	1
      0x1C1C0F04,  //  0056  EQ	R7	R7	K4
      0x781E0001,  //  0057  JMPF	R7	#005A
      0x4C1C0000,  //  0058  LDNIL	R7
      0x90020607,  //  0059  SETMBR	R0	K3	R7
      0x70020001,  //  005A  JMP		#005D
      0x4C1C0000,  //  005B  LDNIL	R7
      0x90020607,  //  005C  SETMBR	R0	K3	R7
      0x7001FFB1,  //  005D  JMP		#0010
      0xB8162000,  //  005E  GETNGBL	R5	K16
      0x8C140B11,  //  005F  GETMET	R5	R5	K17
      0x7C140200,  //  0060  CALL	R5	1
      0x88180112,  //  0061  GETMBR	R6	R0	K18
      0x90162406,  //  0062  SETMBR	R5	K18	R6
      0x88180113,  //  0063  GETMBR	R6	R0	K19
      0x90162606,  //  0064  SETMBR	R5	K19	R6
      0x60180012,  //  0065  GETGBL	R6	G18
      0x7C180000,  //  0066  CALL	R6	0
      0x401C0C03,  //  0067  CONNECT	R7	R6	R3
      0x90162806,  //  0068  SETMBR	R5	K20	R6
      0x88180102,  //  0069  GETMBR	R6	R0	K2
      0x4C1C0000,  //  006A  LDNIL	R7
      0x20180C07,  //  006B  NE	R6	R6	R7
      0x90162A06,  //  006C  SETMBR	R5	K21	R6
      0x8C180B16,  //  006D  GETMET	R6	R5	K22
      0x7C180200,  //  006E  CALL	R6	1
      0x8C1C0D17,  //  006F  GETMET	R7	R6	K23
      0x60240015,  //  0070  GETGBL	R9	G21
      0x8828010D,  //  0071  GETMBR	R10	R0	K13
      0x7C240200,  //  0072  CALL	R9	1
      0x7C1C0400,  //  0073  CALL	R7	2
      0x8C200518,  //  0074  GETMET	R8	R2	K24
      0x5C280E00,  //  0075  MOVE	R10	R7
      0x7C200400,  //  0076  CALL	R8	2
      0x8C200519,  //  0077  GETMET	R8	R2	K25
      0x7C200200,  //  0078  CALL	R8	1
      0x8C20031A,  //  0079  GETMET	R8	R1	K26
      0x5C280400,  //  007A  MOVE	R10	R2
      0x7C200400,  //  007B  CALL	R8	2
      0x8820051C,  //  007C  GETMBR	R8	R2	K28
      0x90023608,  //  007D  SETMBR	R0	K27	R8
      0x88200B15,  //  007E  GETMBR	R8	R5	K21
      0x78220002,  //  007F  JMPF	R8	#0083
      0x50200000,  //  0080  LDBOOL	R8	0	0
      0x90020008,  //  0081  SETMBR	R0	K0	R8
      0x70020001,  //  0082  JMP		#0085
      0x50200200,  //  0083  LDBOOL	R8	1	0
      0x90023A08,  //  0084  SETMBR	R0	K29	R8
      0x80000000,  //  0085  RET	0
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
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    &be_class_Matter_IM_ReportData_Pull, 
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(generator_or_arr),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x541A0004,  //  0005  LDINT	R6	5
      0x501C0200,  //  0006  LDBOOL	R7	1	0
      0x7C0C0800,  //  0007  CALL	R3	4
      0x90020202,  //  0008  SETMBR	R0	K1	R2
      0x80000000,  //  0009  RET	0
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
    3,
    &be_class_Matter_IM_Message,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(suppress_response, -1), be_const_var(2) },
        { be_const_key_weak(MAX_MESSAGE, -1), be_const_int(1200) },
        { be_const_key_weak(generator_or_arr, 5), be_const_var(0) },
        { be_const_key_weak(set_subscription_id, 6), be_const_closure(class_Matter_IM_ReportData_Pull_set_subscription_id_closure) },
        { be_const_key_weak(subscription_id, -1), be_const_var(1) },
        { be_const_key_weak(send_im, -1), be_const_closure(class_Matter_IM_ReportData_Pull_send_im_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_IM_ReportData_Pull_init_closure) },
        { be_const_key_weak(set_suppress_response, -1), be_const_closure(class_Matter_IM_ReportData_Pull_set_suppress_response_closure) },
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
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(ready),
    /* K1   */  be_nested_str_weak(generator_or_arr),
    /* K2   */  be_nested_str_weak(report_data_phase),
    /* K3   */  be_nested_str_weak(send_im),
    /* K4   */  be_nested_str_weak(finish),
    /* K5   */  be_nested_str_weak(resp),
    /* K6   */  be_nested_str_weak(build_standalone_ack),
    /* K7   */  be_nested_str_weak(encode_frame),
    /* K8   */  be_nested_str_weak(encrypt),
    /* K9   */  be_nested_str_weak(tasmota),
    /* K10  */  be_nested_str_weak(loglevel),
    /* K11  */  be_nested_str_weak(log),
    /* K12  */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i),
    /* K13  */  be_nested_str_weak(session),
    /* K14  */  be_nested_str_weak(local_session_id),
    /* K15  */  be_nested_str_weak(ack_message_counter),
    /* K16  */  be_nested_str_weak(message_counter),
    /* K17  */  be_nested_str_weak(send_response_frame),
    /* K18  */  be_nested_str_weak(last_counter),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[71]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0001,  //  0001  JMPT	R2	#0004
      0x50080000,  //  0002  LDBOOL	R2	0	0
      0x80040400,  //  0003  RET	1	R2
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x4C0C0000,  //  0005  LDNIL	R3
      0x20080403,  //  0006  NE	R2	R2	R3
      0x780A0030,  //  0007  JMPF	R2	#0039
      0x88080102,  //  0008  GETMBR	R2	R0	K2
      0x780A000F,  //  0009  JMPF	R2	#001A
      0x60080003,  //  000A  GETGBL	R2	G3
      0x5C0C0000,  //  000B  MOVE	R3	R0
      0x7C080200,  //  000C  CALL	R2	1
      0x8C080503,  //  000D  GETMET	R2	R2	K3
      0x5C100200,  //  000E  MOVE	R4	R1
      0x7C080400,  //  000F  CALL	R2	2
      0x88080104,  //  0010  GETMBR	R2	R0	K4
      0x740A0000,  //  0011  JMPT	R2	#0013
      0x80000400,  //  0012  RET	0
      0x50080000,  //  0013  LDBOOL	R2	0	0
      0x90020402,  //  0014  SETMBR	R0	K2	R2
      0x50080000,  //  0015  LDBOOL	R2	0	0
      0x90020002,  //  0016  SETMBR	R0	K0	R2
      0x50080000,  //  0017  LDBOOL	R2	0	0
      0x90020802,  //  0018  SETMBR	R0	K4	R2
      0x7002001D,  //  0019  JMP		#0038
      0x88080105,  //  001A  GETMBR	R2	R0	K5
      0x8C080506,  //  001B  GETMET	R2	R2	K6
      0x50100000,  //  001C  LDBOOL	R4	0	0
      0x7C080400,  //  001D  CALL	R2	2
      0x8C0C0507,  //  001E  GETMET	R3	R2	K7
      0x7C0C0200,  //  001F  CALL	R3	1
      0x8C0C0508,  //  0020  GETMET	R3	R2	K8
      0x7C0C0200,  //  0021  CALL	R3	1
      0xB80E1200,  //  0022  GETNGBL	R3	K9
      0x8C0C070A,  //  0023  GETMET	R3	R3	K10
      0x54160003,  //  0024  LDINT	R5	4
      0x7C0C0400,  //  0025  CALL	R3	2
      0x780E0009,  //  0026  JMPF	R3	#0031
      0xB80E1600,  //  0027  GETNGBL	R3	K11
      0x60100018,  //  0028  GETGBL	R4	G24
      0x5814000C,  //  0029  LDCONST	R5	K12
      0x8818050D,  //  002A  GETMBR	R6	R2	K13
      0x88180D0E,  //  002B  GETMBR	R6	R6	K14
      0x881C050F,  //  002C  GETMBR	R7	R2	K15
      0x88200510,  //  002D  GETMBR	R8	R2	K16
      0x7C100800,  //  002E  CALL	R4	4
      0x54160003,  //  002F  LDINT	R5	4
      0x7C0C0400,  //  0030  CALL	R3	2
      0x8C0C0311,  //  0031  GETMET	R3	R1	K17
      0x5C140400,  //  0032  MOVE	R5	R2
      0x7C0C0400,  //  0033  CALL	R3	2
      0x880C0510,  //  0034  GETMBR	R3	R2	K16
      0x90022403,  //  0035  SETMBR	R0	K18	R3
      0x500C0200,  //  0036  LDBOOL	R3	1	0
      0x90020803,  //  0037  SETMBR	R0	K4	R3
      0x7002000C,  //  0038  JMP		#0046
      0x88080102,  //  0039  GETMBR	R2	R0	K2
      0x780A0008,  //  003A  JMPF	R2	#0044
      0x60080003,  //  003B  GETGBL	R2	G3
      0x5C0C0000,  //  003C  MOVE	R3	R0
      0x7C080200,  //  003D  CALL	R2	1
      0x8C080503,  //  003E  GETMET	R2	R2	K3
      0x5C100200,  //  003F  MOVE	R4	R1
      0x7C080400,  //  0040  CALL	R2	2
      0x50080000,  //  0041  LDBOOL	R2	0	0
      0x90020402,  //  0042  SETMBR	R0	K2	R2
      0x70020001,  //  0043  JMP		#0046
      0x50080200,  //  0044  LDBOOL	R2	1	0
      0x90020802,  //  0045  SETMBR	R0	K4	R2
      0x80000000,  //  0046  RET	0
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
    11,                          /* nstack */
    5,                          /* argc */
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
    ( &(const binstruction[26]) {  /* code */
      0x60140003,  //  0000  GETGBL	R5	G3
      0x5C180000,  //  0001  MOVE	R6	R0
      0x7C140200,  //  0002  CALL	R5	1
      0x8C140B00,  //  0003  GETMET	R5	R5	K0
      0x4C1C0000,  //  0004  LDNIL	R7
      0x5C200600,  //  0005  MOVE	R8	R3
      0x7C140600,  //  0006  CALL	R5	3
      0xB8160400,  //  0007  GETNGBL	R5	K2
      0x88140B03,  //  0008  GETMBR	R5	R5	K3
      0x8C140B04,  //  0009  GETMET	R5	R5	K4
      0x5C1C0200,  //  000A  MOVE	R7	R1
      0x5C200400,  //  000B  MOVE	R8	R2
      0x54260004,  //  000C  LDINT	R9	5
      0x50280200,  //  000D  LDBOOL	R10	1	0
      0x7C140A00,  //  000E  CALL	R5	5
      0x90020205,  //  000F  SETMBR	R0	K1	R5
      0x90020A04,  //  0010  SETMBR	R0	K5	R4
      0x50140200,  //  0011  LDBOOL	R5	1	0
      0x90020C05,  //  0012  SETMBR	R0	K6	R5
      0x8C140107,  //  0013  GETMET	R5	R0	K7
      0x881C0908,  //  0014  GETMBR	R7	R4	K8
      0x7C140400,  //  0015  CALL	R5	2
      0x8C140109,  //  0016  GETMET	R5	R0	K9
      0x501C0000,  //  0017  LDBOOL	R7	0	0
      0x7C140400,  //  0018  CALL	R5	2
      0x80000000,  //  0019  RET	0
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
    ( &(const binstruction[24]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x4C180000,  //  0004  LDNIL	R6
      0x4C1C0000,  //  0005  LDNIL	R7
      0x7C100600,  //  0006  CALL	R4	3
      0xB8120400,  //  0007  GETNGBL	R4	K2
      0x88100903,  //  0008  GETMBR	R4	R4	K3
      0x8C100904,  //  0009  GETMET	R4	R4	K4
      0x5C180200,  //  000A  MOVE	R6	R1
      0x5C1C0400,  //  000B  MOVE	R7	R2
      0x54220004,  //  000C  LDINT	R8	5
      0x50240200,  //  000D  LDBOOL	R9	1	0
      0x7C100A00,  //  000E  CALL	R4	5
      0x90020204,  //  000F  SETMBR	R0	K1	R4
      0x90020A03,  //  0010  SETMBR	R0	K5	R3
      0x8C100106,  //  0011  GETMET	R4	R0	K6
      0x88180707,  //  0012  GETMBR	R6	R3	K7
      0x7C100400,  //  0013  CALL	R4	2
      0x8C100108,  //  0014  GETMET	R4	R0	K8
      0x50180200,  //  0015  LDBOOL	R6	1	0
      0x7C100400,  //  0016  CALL	R4	2
      0x80000000,  //  0017  RET	0
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
    8,                          /* nstack */
    4,                          /* argc */
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
    ( &(const binstruction[14]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100900,  //  0003  GETMET	R4	R4	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x7C100600,  //  0006  CALL	R4	3
      0x90020203,  //  0007  SETMBR	R0	K1	R3
      0x50100200,  //  0008  LDBOOL	R4	1	0
      0x90020404,  //  0009  SETMBR	R0	K2	R4
      0x8C100103,  //  000A  GETMET	R4	R0	K3
      0x88180704,  //  000B  GETMBR	R6	R3	K4
      0x7C100400,  //  000C  CALL	R4	2
      0x80000000,  //  000D  RET	0
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
