/* Solidification of Matter_IM_Message.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_IM_Message;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_IM_Message_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
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
be_local_closure(Matter_IM_Message_get_exchangeid,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
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
be_local_closure(Matter_IM_Message_send_im,   /* name */
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
    ( &(const binstruction[40]) {  /* code */
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
      0x7816000B,  //  0013  JMPF	R5	#0020
      0xB8160E00,  //  0014  GETNGBL	R5	K7
      0x8C140B09,  //  0015  GETMET	R5	R5	K9
      0x601C0018,  //  0016  GETGBL	R7	G24
      0x5820000A,  //  0017  LDCONST	R8	K10
      0x8824050B,  //  0018  GETMBR	R9	R2	K11
      0x8824130C,  //  0019  GETMBR	R9	R9	K12
      0x8828050D,  //  001A  GETMBR	R10	R2	K13
      0x882C050E,  //  001B  GETMBR	R11	R2	K14
      0x8830050F,  //  001C  GETMBR	R12	R2	K15
      0x7C1C0A00,  //  001D  CALL	R7	5
      0x54220003,  //  001E  LDINT	R8	4
      0x7C140600,  //  001F  CALL	R5	3
      0x8C140310,  //  0020  GETMET	R5	R1	K16
      0x5C1C0400,  //  0021  MOVE	R7	R2
      0x7C140400,  //  0022  CALL	R5	2
      0x8814050D,  //  0023  GETMBR	R5	R2	K13
      0x90022205,  //  0024  SETMBR	R0	K17	R5
      0x50140200,  //  0025  LDBOOL	R5	1	0
      0x90022405,  //  0026  SETMBR	R0	K18	R5
      0x80000000,  //  0027  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: status_error_received
********************************************************************/
be_local_closure(Matter_IM_Message_status_error_received,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
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
be_local_closure(Matter_IM_Message_reset,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
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
    ( &(const binstruction[19]) {  /* code */
      0x8C100301,  //  0000  GETMET	R4	R1	K1
      0x5C180400,  //  0001  MOVE	R6	R2
      0x5C1C0600,  //  0002  MOVE	R7	R3
      0x7C100600,  //  0003  CALL	R4	3
      0x90020004,  //  0004  SETMBR	R0	K0	R4
      0x50100200,  //  0005  LDBOOL	R4	1	0
      0x90020404,  //  0006  SETMBR	R0	K2	R4
      0xB8120800,  //  0007  GETNGBL	R4	K4
      0x8C100905,  //  0008  GETMET	R4	R4	K5
      0x7C100200,  //  0009  CALL	R4	1
      0x88140106,  //  000A  GETMBR	R5	R0	K6
      0x00100805,  //  000B  ADD	R4	R4	R5
      0x90020604,  //  000C  SETMBR	R0	K3	R4
      0x90020F08,  //  000D  SETMBR	R0	K7	K8
      0x50100000,  //  000E  LDBOOL	R4	0	0
      0x90021204,  //  000F  SETMBR	R0	K9	R4
      0x4C100000,  //  0010  LDNIL	R4
      0x90021404,  //  0011  SETMBR	R0	K10	R4
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reached_timeout
********************************************************************/
be_local_closure(Matter_IM_Message_reached_timeout,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
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
be_local_closure(Matter_IM_Message_status_ok_received,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
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
be_local_closure(Matter_IM_Message_ack_received,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
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
        { be_const_key_weak(init, 11), be_const_closure(Matter_IM_Message_init_closure) },
        { be_const_key_weak(get_exchangeid, 10), be_const_closure(Matter_IM_Message_get_exchangeid_closure) },
        { be_const_key_weak(reset, -1), be_const_closure(Matter_IM_Message_reset_closure) },
        { be_const_key_weak(send_im, -1), be_const_closure(Matter_IM_Message_send_im_closure) },
        { be_const_key_weak(status_error_received, -1), be_const_closure(Matter_IM_Message_status_error_received_closure) },
        { be_const_key_weak(finish, -1), be_const_var(3) },
        { be_const_key_weak(status_ok_received, 2), be_const_closure(Matter_IM_Message_status_ok_received_closure) },
        { be_const_key_weak(last_counter, -1), be_const_var(5) },
        { be_const_key_weak(reached_timeout, -1), be_const_closure(Matter_IM_Message_reached_timeout_closure) },
        { be_const_key_weak(resp, -1), be_const_var(1) },
        { be_const_key_weak(data, -1), be_const_var(4) },
        { be_const_key_weak(expiration, -1), be_const_var(0) },
        { be_const_key_weak(ready, 6), be_const_var(2) },
        { be_const_key_weak(MSG_TIMEOUT, 5), be_const_int(5000) },
        { be_const_key_weak(ack_received, -1), be_const_closure(Matter_IM_Message_ack_received_closure) },
    })),
    be_str_weak(Matter_IM_Message)
);
/*******************************************************************/

void be_load_Matter_IM_Message_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_IM_Message);
    be_setglobal(vm, "Matter_IM_Message");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_IM_Status;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_IM_Status_init,   /* name */
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
        { be_const_key_weak(init, -1), be_const_closure(Matter_IM_Status_init_closure) },
    })),
    be_str_weak(Matter_IM_Status)
);
/*******************************************************************/

void be_load_Matter_IM_Status_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_IM_Status);
    be_setglobal(vm, "Matter_IM_Status");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_IM_InvokeResponse;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_IM_InvokeResponse_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
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
        { be_const_key_weak(init, -1), be_const_closure(Matter_IM_InvokeResponse_init_closure) },
    })),
    be_str_weak(Matter_IM_InvokeResponse)
);
/*******************************************************************/

void be_load_Matter_IM_InvokeResponse_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_IM_InvokeResponse);
    be_setglobal(vm, "Matter_IM_InvokeResponse");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_IM_WriteResponse;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_IM_WriteResponse_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
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
        { be_const_key_weak(init, -1), be_const_closure(Matter_IM_WriteResponse_init_closure) },
    })),
    be_str_weak(Matter_IM_WriteResponse)
);
/*******************************************************************/

void be_load_Matter_IM_WriteResponse_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_IM_WriteResponse);
    be_setglobal(vm, "Matter_IM_WriteResponse");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_IM_ReportData;

/********************************************************************
** Solidified function: send_im
********************************************************************/
be_local_closure(Matter_IM_ReportData_send_im,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(ready),
    /* K1   */  be_nested_str_weak(resp),
    /* K2   */  be_nested_str_weak(data),
    /* K3   */  be_nested_str_weak(more_chunked_messages),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(attribute_reports),
    /* K6   */  be_const_int(2147483647),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(to_TLV),
    /* K9   */  be_nested_str_weak(tlv2raw),
    /* K10  */  be_nested_str_weak(MAX_MESSAGE),
    /* K11  */  be_nested_str_weak(encode_frame),
    /* K12  */  be_nested_str_weak(encrypt),
    /* K13  */  be_nested_str_weak(send_response_frame),
    /* K14  */  be_nested_str_weak(last_counter),
    /* K15  */  be_nested_str_weak(message_counter),
    /* K16  */  be_nested_str_weak(finish),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[67]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0001,  //  0001  JMPT	R2	#0004
      0x50080000,  //  0002  LDBOOL	R2	0	0
      0x80040400,  //  0003  RET	1	R2
      0x88080101,  //  0004  GETMBR	R2	R0	K1
      0x880C0102,  //  0005  GETMBR	R3	R0	K2
      0x88100703,  //  0006  GETMBR	R4	R3	K3
      0x58140004,  //  0007  LDCONST	R5	K4
      0x4C180000,  //  0008  LDNIL	R6
      0x881C0705,  //  0009  GETMBR	R7	R3	K5
      0x4C200000,  //  000A  LDNIL	R8
      0x201C0E08,  //  000B  NE	R7	R7	R8
      0x781E000D,  //  000C  JMPF	R7	#001B
      0x401C0B06,  //  000D  CONNECT	R7	R5	K6
      0x88200705,  //  000E  GETMBR	R8	R3	K5
      0x94181007,  //  000F  GETIDX	R6	R8	R7
      0x04240B04,  //  0010  SUB	R9	R5	K4
      0x40260E09,  //  0011  CONNECT	R9	K7	R9
      0x88280705,  //  0012  GETMBR	R10	R3	K5
      0x94241409,  //  0013  GETIDX	R9	R10	R9
      0x900E0A09,  //  0014  SETMBR	R3	K5	R9
      0x6024000C,  //  0015  GETGBL	R9	G12
      0x5C280C00,  //  0016  MOVE	R10	R6
      0x7C240200,  //  0017  CALL	R9	1
      0x24241307,  //  0018  GT	R9	R9	K7
      0x900E0609,  //  0019  SETMBR	R3	K3	R9
      0x70020001,  //  001A  JMP		#001D
      0x501C0000,  //  001B  LDBOOL	R7	0	0
      0x900E0607,  //  001C  SETMBR	R3	K3	R7
      0x7811FFFF,  //  001D  JMPF	R4	#001E
      0x881C0703,  //  001E  GETMBR	R7	R3	K3
      0x781E0001,  //  001F  JMPF	R7	#0022
      0x5C1C0800,  //  0020  MOVE	R7	R4
      0x741DFFFF,  //  0021  JMPT	R7	#0022
      0x881C0102,  //  0022  GETMBR	R7	R0	K2
      0x8C1C0F08,  //  0023  GETMET	R7	R7	K8
      0x7C1C0200,  //  0024  CALL	R7	1
      0x8C200F09,  //  0025  GETMET	R8	R7	K9
      0x60280015,  //  0026  GETGBL	R10	G21
      0x882C010A,  //  0027  GETMBR	R11	R0	K10
      0x7C280200,  //  0028  CALL	R10	1
      0x7C200400,  //  0029  CALL	R8	2
      0x8C24050B,  //  002A  GETMET	R9	R2	K11
      0x5C2C1000,  //  002B  MOVE	R11	R8
      0x7C240400,  //  002C  CALL	R9	2
      0x8C24050C,  //  002D  GETMET	R9	R2	K12
      0x7C240200,  //  002E  CALL	R9	1
      0x8C24030D,  //  002F  GETMET	R9	R1	K13
      0x5C2C0400,  //  0030  MOVE	R11	R2
      0x7C240400,  //  0031  CALL	R9	2
      0x8824050F,  //  0032  GETMBR	R9	R2	K15
      0x90021C09,  //  0033  SETMBR	R0	K14	R9
      0x4C240000,  //  0034  LDNIL	R9
      0x20240C09,  //  0035  NE	R9	R6	R9
      0x78260008,  //  0036  JMPF	R9	#0040
      0x6024000C,  //  0037  GETGBL	R9	G12
      0x5C280C00,  //  0038  MOVE	R10	R6
      0x7C240200,  //  0039  CALL	R9	1
      0x24241307,  //  003A  GT	R9	R9	K7
      0x78260003,  //  003B  JMPF	R9	#0040
      0x900E0A06,  //  003C  SETMBR	R3	K5	R6
      0x50240000,  //  003D  LDBOOL	R9	0	0
      0x90020009,  //  003E  SETMBR	R0	K0	R9
      0x70020001,  //  003F  JMP		#0042
      0x50240200,  //  0040  LDBOOL	R9	1	0
      0x90022009,  //  0041  SETMBR	R0	K16	R9
      0x80000000,  //  0042  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_IM_ReportData_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
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
** Solidified class: Matter_IM_ReportData
********************************************************************/
extern const bclass be_class_Matter_IM_Message;
be_local_class(Matter_IM_ReportData,
    0,
    &be_class_Matter_IM_Message,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(send_im, 1), be_const_closure(Matter_IM_ReportData_send_im_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_IM_ReportData_init_closure) },
        { be_const_key_weak(MAX_MESSAGE, -1), be_const_int(1200) },
    })),
    be_str_weak(Matter_IM_ReportData)
);
/*******************************************************************/

void be_load_Matter_IM_ReportData_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_IM_ReportData);
    be_setglobal(vm, "Matter_IM_ReportData");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_IM_ReportDataSubscribed;

/********************************************************************
** Solidified function: ack_received
********************************************************************/
be_local_closure(Matter_IM_ReportDataSubscribed_ack_received,   /* name */
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
be_local_closure(Matter_IM_ReportDataSubscribed_send_im,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(ready),
    /* K1   */  be_nested_str_weak(data),
    /* K2   */  be_nested_str_weak(attribute_reports),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(report_data_phase),
    /* K5   */  be_nested_str_weak(send_im),
    /* K6   */  be_nested_str_weak(finish),
    /* K7   */  be_nested_str_weak(resp),
    /* K8   */  be_nested_str_weak(build_standalone_ack),
    /* K9   */  be_nested_str_weak(encode_frame),
    /* K10  */  be_nested_str_weak(encrypt),
    /* K11  */  be_nested_str_weak(tasmota),
    /* K12  */  be_nested_str_weak(loglevel),
    /* K13  */  be_nested_str_weak(log),
    /* K14  */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i),
    /* K15  */  be_nested_str_weak(session),
    /* K16  */  be_nested_str_weak(local_session_id),
    /* K17  */  be_nested_str_weak(ack_message_counter),
    /* K18  */  be_nested_str_weak(message_counter),
    /* K19  */  be_nested_str_weak(send_response_frame),
    /* K20  */  be_nested_str_weak(last_counter),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[74]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x740A0001,  //  0001  JMPT	R2	#0004
      0x50080000,  //  0002  LDBOOL	R2	0	0
      0x80040400,  //  0003  RET	1	R2
      0x6008000C,  //  0004  GETGBL	R2	G12
      0x880C0101,  //  0005  GETMBR	R3	R0	K1
      0x880C0702,  //  0006  GETMBR	R3	R3	K2
      0x7C080200,  //  0007  CALL	R2	1
      0x24080503,  //  0008  GT	R2	R2	K3
      0x780A0031,  //  0009  JMPF	R2	#003C
      0x88080104,  //  000A  GETMBR	R2	R0	K4
      0x780A000F,  //  000B  JMPF	R2	#001C
      0x60080003,  //  000C  GETGBL	R2	G3
      0x5C0C0000,  //  000D  MOVE	R3	R0
      0x7C080200,  //  000E  CALL	R2	1
      0x8C080505,  //  000F  GETMET	R2	R2	K5
      0x5C100200,  //  0010  MOVE	R4	R1
      0x7C080400,  //  0011  CALL	R2	2
      0x88080106,  //  0012  GETMBR	R2	R0	K6
      0x740A0000,  //  0013  JMPT	R2	#0015
      0x80000400,  //  0014  RET	0
      0x50080000,  //  0015  LDBOOL	R2	0	0
      0x90020802,  //  0016  SETMBR	R0	K4	R2
      0x50080000,  //  0017  LDBOOL	R2	0	0
      0x90020002,  //  0018  SETMBR	R0	K0	R2
      0x50080000,  //  0019  LDBOOL	R2	0	0
      0x90020C02,  //  001A  SETMBR	R0	K6	R2
      0x7002001E,  //  001B  JMP		#003B
      0x88080107,  //  001C  GETMBR	R2	R0	K7
      0x8C080508,  //  001D  GETMET	R2	R2	K8
      0x50100000,  //  001E  LDBOOL	R4	0	0
      0x7C080400,  //  001F  CALL	R2	2
      0x8C0C0509,  //  0020  GETMET	R3	R2	K9
      0x7C0C0200,  //  0021  CALL	R3	1
      0x8C0C050A,  //  0022  GETMET	R3	R2	K10
      0x7C0C0200,  //  0023  CALL	R3	1
      0xB80E1600,  //  0024  GETNGBL	R3	K11
      0x8C0C070C,  //  0025  GETMET	R3	R3	K12
      0x54160003,  //  0026  LDINT	R5	4
      0x7C0C0400,  //  0027  CALL	R3	2
      0x780E000A,  //  0028  JMPF	R3	#0034
      0xB80E1600,  //  0029  GETNGBL	R3	K11
      0x8C0C070D,  //  002A  GETMET	R3	R3	K13
      0x60140018,  //  002B  GETGBL	R5	G24
      0x5818000E,  //  002C  LDCONST	R6	K14
      0x881C050F,  //  002D  GETMBR	R7	R2	K15
      0x881C0F10,  //  002E  GETMBR	R7	R7	K16
      0x88200511,  //  002F  GETMBR	R8	R2	K17
      0x88240512,  //  0030  GETMBR	R9	R2	K18
      0x7C140800,  //  0031  CALL	R5	4
      0x541A0003,  //  0032  LDINT	R6	4
      0x7C0C0600,  //  0033  CALL	R3	3
      0x8C0C0313,  //  0034  GETMET	R3	R1	K19
      0x5C140400,  //  0035  MOVE	R5	R2
      0x7C0C0400,  //  0036  CALL	R3	2
      0x880C0512,  //  0037  GETMBR	R3	R2	K18
      0x90022803,  //  0038  SETMBR	R0	K20	R3
      0x500C0200,  //  0039  LDBOOL	R3	1	0
      0x90020C03,  //  003A  SETMBR	R0	K6	R3
      0x7002000C,  //  003B  JMP		#0049
      0x88080104,  //  003C  GETMBR	R2	R0	K4
      0x780A0008,  //  003D  JMPF	R2	#0047
      0x60080003,  //  003E  GETGBL	R2	G3
      0x5C0C0000,  //  003F  MOVE	R3	R0
      0x7C080200,  //  0040  CALL	R2	1
      0x8C080505,  //  0041  GETMET	R2	R2	K5
      0x5C100200,  //  0042  MOVE	R4	R1
      0x7C080400,  //  0043  CALL	R2	2
      0x50080000,  //  0044  LDBOOL	R2	0	0
      0x90020802,  //  0045  SETMBR	R0	K4	R2
      0x70020001,  //  0046  JMP		#0049
      0x50080200,  //  0047  LDBOOL	R2	1	0
      0x90020C02,  //  0048  SETMBR	R0	K6	R2
      0x80000000,  //  0049  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_IM_ReportDataSubscribed_init,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(resp),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(Frame),
    /* K3   */  be_nested_str_weak(initiate_response),
    /* K4   */  be_nested_str_weak(data),
    /* K5   */  be_nested_str_weak(ready),
    /* K6   */  be_nested_str_weak(expiration),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(millis),
    /* K9   */  be_nested_str_weak(MSG_TIMEOUT),
    /* K10  */  be_nested_str_weak(sub),
    /* K11  */  be_nested_str_weak(report_data_phase),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xB8160200,  //  0000  GETNGBL	R5	K1
      0x88140B02,  //  0001  GETMBR	R5	R5	K2
      0x8C140B03,  //  0002  GETMET	R5	R5	K3
      0x5C1C0200,  //  0003  MOVE	R7	R1
      0x5C200400,  //  0004  MOVE	R8	R2
      0x54260004,  //  0005  LDINT	R9	5
      0x50280200,  //  0006  LDBOOL	R10	1	0
      0x7C140A00,  //  0007  CALL	R5	5
      0x90020005,  //  0008  SETMBR	R0	K0	R5
      0x90020803,  //  0009  SETMBR	R0	K4	R3
      0x50140200,  //  000A  LDBOOL	R5	1	0
      0x90020A05,  //  000B  SETMBR	R0	K5	R5
      0xB8160E00,  //  000C  GETNGBL	R5	K7
      0x8C140B08,  //  000D  GETMET	R5	R5	K8
      0x7C140200,  //  000E  CALL	R5	1
      0x88180109,  //  000F  GETMBR	R6	R0	K9
      0x00140A06,  //  0010  ADD	R5	R5	R6
      0x90020C05,  //  0011  SETMBR	R0	K6	R5
      0x90021404,  //  0012  SETMBR	R0	K10	R4
      0x50140200,  //  0013  LDBOOL	R5	1	0
      0x90021605,  //  0014  SETMBR	R0	K11	R5
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: status_error_received
********************************************************************/
be_local_closure(Matter_IM_ReportDataSubscribed_status_error_received,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
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
be_local_closure(Matter_IM_ReportDataSubscribed_reached_timeout,   /* name */
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
be_local_closure(Matter_IM_ReportDataSubscribed_status_ok_received,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
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
** Solidified class: Matter_IM_ReportDataSubscribed
********************************************************************/
extern const bclass be_class_Matter_IM_ReportData;
be_local_class(Matter_IM_ReportDataSubscribed,
    2,
    &be_class_Matter_IM_ReportData,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ack_received, 1), be_const_closure(Matter_IM_ReportDataSubscribed_ack_received_closure) },
        { be_const_key_weak(status_ok_received, -1), be_const_closure(Matter_IM_ReportDataSubscribed_status_ok_received_closure) },
        { be_const_key_weak(send_im, -1), be_const_closure(Matter_IM_ReportDataSubscribed_send_im_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_IM_ReportDataSubscribed_init_closure) },
        { be_const_key_weak(report_data_phase, 7), be_const_var(1) },
        { be_const_key_weak(sub, 6), be_const_var(0) },
        { be_const_key_weak(reached_timeout, -1), be_const_closure(Matter_IM_ReportDataSubscribed_reached_timeout_closure) },
        { be_const_key_weak(status_error_received, -1), be_const_closure(Matter_IM_ReportDataSubscribed_status_error_received_closure) },
    })),
    be_str_weak(Matter_IM_ReportDataSubscribed)
);
/*******************************************************************/

void be_load_Matter_IM_ReportDataSubscribed_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_IM_ReportDataSubscribed);
    be_setglobal(vm, "Matter_IM_ReportDataSubscribed");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_IM_SubscribedHeartbeat;

/********************************************************************
** Solidified function: status_error_received
********************************************************************/
be_local_closure(Matter_IM_SubscribedHeartbeat_status_error_received,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
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
be_local_closure(Matter_IM_SubscribedHeartbeat_send_im,   /* name */
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
be_local_closure(Matter_IM_SubscribedHeartbeat_ack_received,   /* name */
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
be_local_closure(Matter_IM_SubscribedHeartbeat_reached_timeout,   /* name */
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
be_local_closure(Matter_IM_SubscribedHeartbeat_status_ok_received,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
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
be_local_closure(Matter_IM_SubscribedHeartbeat_init,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(resp),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(Frame),
    /* K3   */  be_nested_str_weak(initiate_response),
    /* K4   */  be_nested_str_weak(data),
    /* K5   */  be_nested_str_weak(ready),
    /* K6   */  be_nested_str_weak(expiration),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(millis),
    /* K9   */  be_nested_str_weak(MSG_TIMEOUT),
    /* K10  */  be_nested_str_weak(sub),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xB8160200,  //  0000  GETNGBL	R5	K1
      0x88140B02,  //  0001  GETMBR	R5	R5	K2
      0x8C140B03,  //  0002  GETMET	R5	R5	K3
      0x5C1C0200,  //  0003  MOVE	R7	R1
      0x5C200400,  //  0004  MOVE	R8	R2
      0x54260004,  //  0005  LDINT	R9	5
      0x50280200,  //  0006  LDBOOL	R10	1	0
      0x7C140A00,  //  0007  CALL	R5	5
      0x90020005,  //  0008  SETMBR	R0	K0	R5
      0x90020803,  //  0009  SETMBR	R0	K4	R3
      0x50140200,  //  000A  LDBOOL	R5	1	0
      0x90020A05,  //  000B  SETMBR	R0	K5	R5
      0xB8160E00,  //  000C  GETNGBL	R5	K7
      0x8C140B08,  //  000D  GETMET	R5	R5	K8
      0x7C140200,  //  000E  CALL	R5	1
      0x88180109,  //  000F  GETMBR	R6	R0	K9
      0x00140A06,  //  0010  ADD	R5	R5	R6
      0x90020C05,  //  0011  SETMBR	R0	K6	R5
      0x90021404,  //  0012  SETMBR	R0	K10	R4
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM_SubscribedHeartbeat
********************************************************************/
extern const bclass be_class_Matter_IM_ReportData;
be_local_class(Matter_IM_SubscribedHeartbeat,
    1,
    &be_class_Matter_IM_ReportData,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, 1), be_const_closure(Matter_IM_SubscribedHeartbeat_init_closure) },
        { be_const_key_weak(status_ok_received, -1), be_const_closure(Matter_IM_SubscribedHeartbeat_status_ok_received_closure) },
        { be_const_key_weak(sub, 6), be_const_var(0) },
        { be_const_key_weak(ack_received, -1), be_const_closure(Matter_IM_SubscribedHeartbeat_ack_received_closure) },
        { be_const_key_weak(reached_timeout, -1), be_const_closure(Matter_IM_SubscribedHeartbeat_reached_timeout_closure) },
        { be_const_key_weak(status_error_received, 0), be_const_closure(Matter_IM_SubscribedHeartbeat_status_error_received_closure) },
        { be_const_key_weak(send_im, -1), be_const_closure(Matter_IM_SubscribedHeartbeat_send_im_closure) },
    })),
    be_str_weak(Matter_IM_SubscribedHeartbeat)
);
/*******************************************************************/

void be_load_Matter_IM_SubscribedHeartbeat_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_IM_SubscribedHeartbeat);
    be_setglobal(vm, "Matter_IM_SubscribedHeartbeat");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_IM_SubscribeResponse;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_IM_SubscribeResponse_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(sub),
    /* K2   */  be_nested_str_weak(report_data_phase),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
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
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: status_ok_received
********************************************************************/
be_local_closure(Matter_IM_SubscribeResponse_status_ok_received,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
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
    ( &(const binstruction[23]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x7C080400,  //  0003  CALL	R2	2
      0x780A000A,  //  0004  JMPF	R2	#0010
      0xB80A0000,  //  0005  GETNGBL	R2	K0
      0x8C080503,  //  0006  GETMET	R2	R2	K3
      0x60100018,  //  0007  GETGBL	R4	G24
      0x58140004,  //  0008  LDCONST	R5	K4
      0x88180305,  //  0009  GETMBR	R6	R1	K5
      0x88180D06,  //  000A  GETMBR	R6	R6	K6
      0x881C0107,  //  000B  GETMBR	R7	R0	K7
      0x881C0F08,  //  000C  GETMBR	R7	R7	K8
      0x7C100600,  //  000D  CALL	R4	3
      0x58140002,  //  000E  LDCONST	R5	K2
      0x7C080600,  //  000F  CALL	R2	3
      0x60080003,  //  0010  GETGBL	R2	G3
      0x5C0C0000,  //  0011  MOVE	R3	R0
      0x7C080200,  //  0012  CALL	R2	1
      0x8C080509,  //  0013  GETMET	R2	R2	K9
      0x5C100200,  //  0014  MOVE	R4	R1
      0x7C080400,  //  0015  CALL	R2	2
      0x80040400,  //  0016  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_im
********************************************************************/
be_local_closure(Matter_IM_SubscribeResponse_send_im,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
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
** Solidified class: Matter_IM_SubscribeResponse
********************************************************************/
extern const bclass be_class_Matter_IM_ReportData;
be_local_class(Matter_IM_SubscribeResponse,
    2,
    &be_class_Matter_IM_ReportData,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, 4), be_const_closure(Matter_IM_SubscribeResponse_init_closure) },
        { be_const_key_weak(sub, -1), be_const_var(0) },
        { be_const_key_weak(status_ok_received, -1), be_const_closure(Matter_IM_SubscribeResponse_status_ok_received_closure) },
        { be_const_key_weak(send_im, -1), be_const_closure(Matter_IM_SubscribeResponse_send_im_closure) },
        { be_const_key_weak(report_data_phase, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_IM_SubscribeResponse)
);
/*******************************************************************/

void be_load_Matter_IM_SubscribeResponse_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_IM_SubscribeResponse);
    be_setglobal(vm, "Matter_IM_SubscribeResponse");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
