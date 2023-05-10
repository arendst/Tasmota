/* Solidification of Matter_IM_Message.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_IM_Message;

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
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(ready),
    /* K2   */  be_nested_str_weak(resp),
    /* K3   */  be_nested_str_weak(encode_frame),
    /* K4   */  be_nested_str_weak(data),
    /* K5   */  be_nested_str_weak(to_TLV),
    /* K6   */  be_nested_str_weak(tlv2raw),
    /* K7   */  be_nested_str_weak(encrypt),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(format),
    /* K11  */  be_nested_str_weak(MTR_X3A_X20_X3Csnd_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20id_X3D_X25i_X20exch_X3D_X25i_X20rack_X3D_X25s),
    /* K12  */  be_nested_str_weak(session),
    /* K13  */  be_nested_str_weak(local_session_id),
    /* K14  */  be_nested_str_weak(message_counter),
    /* K15  */  be_nested_str_weak(exchange_id),
    /* K16  */  be_nested_str_weak(ack_message_counter),
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(send_response_frame),
    /* K19  */  be_nested_str_weak(last_counter),
    /* K20  */  be_nested_str_weak(finish),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x740E0001,  //  0002  JMPT	R3	#0005
      0x500C0000,  //  0003  LDBOOL	R3	0	0
      0x80040600,  //  0004  RET	1	R3
      0x880C0102,  //  0005  GETMBR	R3	R0	K2
      0x8C100703,  //  0006  GETMET	R4	R3	K3
      0x88180104,  //  0007  GETMBR	R6	R0	K4
      0x8C180D05,  //  0008  GETMET	R6	R6	K5
      0x7C180200,  //  0009  CALL	R6	1
      0x8C180D06,  //  000A  GETMET	R6	R6	K6
      0x7C180200,  //  000B  CALL	R6	1
      0x7C100400,  //  000C  CALL	R4	2
      0x8C100707,  //  000D  GETMET	R4	R3	K7
      0x7C100200,  //  000E  CALL	R4	1
      0xB8121000,  //  000F  GETNGBL	R4	K8
      0x8C100909,  //  0010  GETMET	R4	R4	K9
      0x8C18050A,  //  0011  GETMET	R6	R2	K10
      0x5820000B,  //  0012  LDCONST	R8	K11
      0x8824070C,  //  0013  GETMBR	R9	R3	K12
      0x8824130D,  //  0014  GETMBR	R9	R9	K13
      0x8828070E,  //  0015  GETMBR	R10	R3	K14
      0x882C070F,  //  0016  GETMBR	R11	R3	K15
      0x88300710,  //  0017  GETMBR	R12	R3	K16
      0x7C180C00,  //  0018  CALL	R6	6
      0x581C0011,  //  0019  LDCONST	R7	K17
      0x7C100600,  //  001A  CALL	R4	3
      0x8C100312,  //  001B  GETMET	R4	R1	K18
      0x5C180600,  //  001C  MOVE	R6	R3
      0x7C100400,  //  001D  CALL	R4	2
      0x8810070E,  //  001E  GETMBR	R4	R3	K14
      0x90022604,  //  001F  SETMBR	R0	K19	R4
      0x50100200,  //  0020  LDBOOL	R4	1	0
      0x90022804,  //  0021  SETMBR	R0	K20	R4
      0x80000000,  //  0022  RET	0
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
** Solidified function: init
********************************************************************/
be_local_closure(Matter_IM_Message_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
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
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
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
      0x80000000,  //  0010  RET	0
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
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(expiration),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(millis),
    /* K4   */  be_nested_str_weak(MSG_TIMEOUT),
    /* K5   */  be_nested_str_weak(resp),
    /* K6   */  be_nested_str_weak(build_response),
    /* K7   */  be_nested_str_weak(opcode),
    /* K8   */  be_nested_str_weak(x_flag_r),
    /* K9   */  be_nested_str_weak(ready),
    }),
    be_str_weak(status_ok_received),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0400,  //  0001  GETNGBL	R3	K2
      0x8C0C0703,  //  0002  GETMET	R3	R3	K3
      0x7C0C0200,  //  0003  CALL	R3	1
      0x88100104,  //  0004  GETMBR	R4	R0	K4
      0x000C0604,  //  0005  ADD	R3	R3	R4
      0x90020203,  //  0006  SETMBR	R0	K1	R3
      0x78060007,  //  0007  JMPF	R1	#0010
      0x8C0C0306,  //  0008  GETMET	R3	R1	K6
      0x88140105,  //  0009  GETMBR	R5	R0	K5
      0x88140B07,  //  000A  GETMBR	R5	R5	K7
      0x88180105,  //  000B  GETMBR	R6	R0	K5
      0x88180D08,  //  000C  GETMBR	R6	R6	K8
      0x881C0105,  //  000D  GETMBR	R7	R0	K5
      0x7C0C0800,  //  000E  CALL	R3	4
      0x90020A03,  //  000F  SETMBR	R0	K5	R3
      0x500C0200,  //  0010  LDBOOL	R3	1	0
      0x90021203,  //  0011  SETMBR	R0	K9	R3
      0x500C0200,  //  0012  LDBOOL	R3	1	0
      0x80040600,  //  0013  RET	1	R3
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
    be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(MSG_TIMEOUT, -1), be_const_int(5000) },
        { be_const_key_weak(data, -1), be_const_var(4) },
        { be_const_key_weak(send_im, -1), be_const_closure(Matter_IM_Message_send_im_closure) },
        { be_const_key_weak(status_ok_received, -1), be_const_closure(Matter_IM_Message_status_ok_received_closure) },
        { be_const_key_weak(get_exchangeid, 0), be_const_closure(Matter_IM_Message_get_exchangeid_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_IM_Message_init_closure) },
        { be_const_key_weak(expiration, -1), be_const_var(0) },
        { be_const_key_weak(resp, -1), be_const_var(1) },
        { be_const_key_weak(finish, 13), be_const_var(3) },
        { be_const_key_weak(last_counter, 6), be_const_var(5) },
        { be_const_key_weak(ack_received, 9), be_const_closure(Matter_IM_Message_ack_received_closure) },
        { be_const_key_weak(reached_timeout, -1), be_const_closure(Matter_IM_Message_reached_timeout_closure) },
        { be_const_key_weak(status_error_received, 3), be_const_closure(Matter_IM_Message_status_error_received_closure) },
        { be_const_key_weak(ready, -1), be_const_var(2) },
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
    19,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[31]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(ready),
    /* K2   */  be_nested_str_weak(resp),
    /* K3   */  be_nested_str_weak(data),
    /* K4   */  be_nested_str_weak(more_chunked_messages),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_weak(attribute_reports),
    /* K7   */  be_const_int(2147483647),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(tasmota),
    /* K10  */  be_nested_str_weak(log),
    /* K11  */  be_nested_str_weak(format),
    /* K12  */  be_nested_str_weak(MTR_X3A_X20_X2ERead_Attr_X20next_chunk_X20exch_X3D_X25i),
    /* K13  */  be_nested_str_weak(get_exchangeid),
    /* K14  */  be_const_int(3),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20_X2ERead_Attr_X20first_chunk_X20exch_X3D_X25i),
    /* K16  */  be_nested_str_weak(to_TLV),
    /* K17  */  be_nested_str_weak(tlv2raw),
    /* K18  */  be_nested_str_weak(MAX_MESSAGE),
    /* K19  */  be_nested_str_weak(encode_frame),
    /* K20  */  be_nested_str_weak(encrypt),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20_X3Csnd_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20id_X3D_X25i_X20exch_X3D_X25i_X20rack_X3D_X25s),
    /* K22  */  be_nested_str_weak(session),
    /* K23  */  be_nested_str_weak(local_session_id),
    /* K24  */  be_nested_str_weak(message_counter),
    /* K25  */  be_nested_str_weak(exchange_id),
    /* K26  */  be_nested_str_weak(ack_message_counter),
    /* K27  */  be_nested_str_weak(send_response_frame),
    /* K28  */  be_nested_str_weak(last_counter),
    /* K29  */  be_nested_str_weak(MTR_X3A_X20to_be_sent_later_X20size_X3D_X25i_X20exch_X3D_X25i),
    /* K30  */  be_nested_str_weak(finish),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[109]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x740E0001,  //  0002  JMPT	R3	#0005
      0x500C0000,  //  0003  LDBOOL	R3	0	0
      0x80040600,  //  0004  RET	1	R3
      0x880C0102,  //  0005  GETMBR	R3	R0	K2
      0x88100103,  //  0006  GETMBR	R4	R0	K3
      0x88140904,  //  0007  GETMBR	R5	R4	K4
      0x58180005,  //  0008  LDCONST	R6	K5
      0x4C1C0000,  //  0009  LDNIL	R7
      0x88200906,  //  000A  GETMBR	R8	R4	K6
      0x4C240000,  //  000B  LDNIL	R9
      0x20201009,  //  000C  NE	R8	R8	R9
      0x7822000D,  //  000D  JMPF	R8	#001C
      0x40200D07,  //  000E  CONNECT	R8	R6	K7
      0x88240906,  //  000F  GETMBR	R9	R4	K6
      0x941C1208,  //  0010  GETIDX	R7	R9	R8
      0x04280D05,  //  0011  SUB	R10	R6	K5
      0x402A100A,  //  0012  CONNECT	R10	K8	R10
      0x882C0906,  //  0013  GETMBR	R11	R4	K6
      0x9428160A,  //  0014  GETIDX	R10	R11	R10
      0x90120C0A,  //  0015  SETMBR	R4	K6	R10
      0x6028000C,  //  0016  GETGBL	R10	G12
      0x5C2C0E00,  //  0017  MOVE	R11	R7
      0x7C280200,  //  0018  CALL	R10	1
      0x24281508,  //  0019  GT	R10	R10	K8
      0x9012080A,  //  001A  SETMBR	R4	K4	R10
      0x70020001,  //  001B  JMP		#001E
      0x50200000,  //  001C  LDBOOL	R8	0	0
      0x90120808,  //  001D  SETMBR	R4	K4	R8
      0x78160008,  //  001E  JMPF	R5	#0028
      0xB8221200,  //  001F  GETNGBL	R8	K9
      0x8C20110A,  //  0020  GETMET	R8	R8	K10
      0x8C28050B,  //  0021  GETMET	R10	R2	K11
      0x5830000C,  //  0022  LDCONST	R12	K12
      0x8C34010D,  //  0023  GETMET	R13	R0	K13
      0x7C340200,  //  0024  CALL	R13	1
      0x7C280600,  //  0025  CALL	R10	3
      0x582C000E,  //  0026  LDCONST	R11	K14
      0x7C200600,  //  0027  CALL	R8	3
      0x88200904,  //  0028  GETMBR	R8	R4	K4
      0x7822000A,  //  0029  JMPF	R8	#0035
      0x5C200A00,  //  002A  MOVE	R8	R5
      0x74220008,  //  002B  JMPT	R8	#0035
      0xB8221200,  //  002C  GETNGBL	R8	K9
      0x8C20110A,  //  002D  GETMET	R8	R8	K10
      0x8C28050B,  //  002E  GETMET	R10	R2	K11
      0x5830000F,  //  002F  LDCONST	R12	K15
      0x8C34010D,  //  0030  GETMET	R13	R0	K13
      0x7C340200,  //  0031  CALL	R13	1
      0x7C280600,  //  0032  CALL	R10	3
      0x582C000E,  //  0033  LDCONST	R11	K14
      0x7C200600,  //  0034  CALL	R8	3
      0x88200103,  //  0035  GETMBR	R8	R0	K3
      0x8C201110,  //  0036  GETMET	R8	R8	K16
      0x7C200200,  //  0037  CALL	R8	1
      0x8C241111,  //  0038  GETMET	R9	R8	K17
      0x602C0015,  //  0039  GETGBL	R11	G21
      0x88300112,  //  003A  GETMBR	R12	R0	K18
      0x7C2C0200,  //  003B  CALL	R11	1
      0x7C240400,  //  003C  CALL	R9	2
      0x8C280713,  //  003D  GETMET	R10	R3	K19
      0x5C301200,  //  003E  MOVE	R12	R9
      0x7C280400,  //  003F  CALL	R10	2
      0x8C280714,  //  0040  GETMET	R10	R3	K20
      0x7C280200,  //  0041  CALL	R10	1
      0xB82A1200,  //  0042  GETNGBL	R10	K9
      0x8C28150A,  //  0043  GETMET	R10	R10	K10
      0x8C30050B,  //  0044  GETMET	R12	R2	K11
      0x58380015,  //  0045  LDCONST	R14	K21
      0x883C0716,  //  0046  GETMBR	R15	R3	K22
      0x883C1F17,  //  0047  GETMBR	R15	R15	K23
      0x88400718,  //  0048  GETMBR	R16	R3	K24
      0x88440719,  //  0049  GETMBR	R17	R3	K25
      0x8848071A,  //  004A  GETMBR	R18	R3	K26
      0x7C300C00,  //  004B  CALL	R12	6
      0x5834000E,  //  004C  LDCONST	R13	K14
      0x7C280600,  //  004D  CALL	R10	3
      0x8C28031B,  //  004E  GETMET	R10	R1	K27
      0x5C300600,  //  004F  MOVE	R12	R3
      0x7C280400,  //  0050  CALL	R10	2
      0x88280718,  //  0051  GETMBR	R10	R3	K24
      0x9002380A,  //  0052  SETMBR	R0	K28	R10
      0x4C280000,  //  0053  LDNIL	R10
      0x20280E0A,  //  0054  NE	R10	R7	R10
      0x782A0013,  //  0055  JMPF	R10	#006A
      0x6028000C,  //  0056  GETGBL	R10	G12
      0x5C2C0E00,  //  0057  MOVE	R11	R7
      0x7C280200,  //  0058  CALL	R10	1
      0x24281508,  //  0059  GT	R10	R10	K8
      0x782A000E,  //  005A  JMPF	R10	#006A
      0x90120C07,  //  005B  SETMBR	R4	K6	R7
      0xB82A1200,  //  005C  GETNGBL	R10	K9
      0x8C28150A,  //  005D  GETMET	R10	R10	K10
      0x8C30050B,  //  005E  GETMET	R12	R2	K11
      0x5838001D,  //  005F  LDCONST	R14	K29
      0x603C000C,  //  0060  GETGBL	R15	G12
      0x88400906,  //  0061  GETMBR	R16	R4	K6
      0x7C3C0200,  //  0062  CALL	R15	1
      0x88400719,  //  0063  GETMBR	R16	R3	K25
      0x7C300800,  //  0064  CALL	R12	4
      0x5834000E,  //  0065  LDCONST	R13	K14
      0x7C280600,  //  0066  CALL	R10	3
      0x50280000,  //  0067  LDBOOL	R10	0	0
      0x9002020A,  //  0068  SETMBR	R0	K1	R10
      0x70020001,  //  0069  JMP		#006C
      0x50280200,  //  006A  LDBOOL	R10	1	0
      0x90023C0A,  //  006B  SETMBR	R0	K30	R10
      0x80000000,  //  006C  RET	0
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
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(ready),
    /* K2   */  be_nested_str_weak(data),
    /* K3   */  be_nested_str_weak(attribute_reports),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(report_data_phase),
    /* K6   */  be_nested_str_weak(send_im),
    /* K7   */  be_nested_str_weak(finish),
    /* K8   */  be_nested_str_weak(resp),
    /* K9   */  be_nested_str_weak(build_standalone_ack),
    /* K10  */  be_nested_str_weak(encode_frame),
    /* K11  */  be_nested_str_weak(encrypt),
    /* K12  */  be_nested_str_weak(tasmota),
    /* K13  */  be_nested_str_weak(log),
    /* K14  */  be_nested_str_weak(format),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i),
    /* K16  */  be_nested_str_weak(session),
    /* K17  */  be_nested_str_weak(local_session_id),
    /* K18  */  be_nested_str_weak(ack_message_counter),
    /* K19  */  be_nested_str_weak(message_counter),
    /* K20  */  be_const_int(3),
    /* K21  */  be_nested_str_weak(send_response_frame),
    /* K22  */  be_nested_str_weak(last_counter),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[70]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x740E0001,  //  0002  JMPT	R3	#0005
      0x500C0000,  //  0003  LDBOOL	R3	0	0
      0x80040600,  //  0004  RET	1	R3
      0x600C000C,  //  0005  GETGBL	R3	G12
      0x88100102,  //  0006  GETMBR	R4	R0	K2
      0x88100903,  //  0007  GETMBR	R4	R4	K3
      0x7C0C0200,  //  0008  CALL	R3	1
      0x240C0704,  //  0009  GT	R3	R3	K4
      0x780E002C,  //  000A  JMPF	R3	#0038
      0x880C0105,  //  000B  GETMBR	R3	R0	K5
      0x780E000F,  //  000C  JMPF	R3	#001D
      0x600C0003,  //  000D  GETGBL	R3	G3
      0x5C100000,  //  000E  MOVE	R4	R0
      0x7C0C0200,  //  000F  CALL	R3	1
      0x8C0C0706,  //  0010  GETMET	R3	R3	K6
      0x5C140200,  //  0011  MOVE	R5	R1
      0x7C0C0400,  //  0012  CALL	R3	2
      0x880C0107,  //  0013  GETMBR	R3	R0	K7
      0x740E0000,  //  0014  JMPT	R3	#0016
      0x80000600,  //  0015  RET	0
      0x500C0000,  //  0016  LDBOOL	R3	0	0
      0x90020A03,  //  0017  SETMBR	R0	K5	R3
      0x500C0000,  //  0018  LDBOOL	R3	0	0
      0x90020203,  //  0019  SETMBR	R0	K1	R3
      0x500C0000,  //  001A  LDBOOL	R3	0	0
      0x90020E03,  //  001B  SETMBR	R0	K7	R3
      0x70020019,  //  001C  JMP		#0037
      0x880C0108,  //  001D  GETMBR	R3	R0	K8
      0x8C0C0709,  //  001E  GETMET	R3	R3	K9
      0x50140000,  //  001F  LDBOOL	R5	0	0
      0x7C0C0400,  //  0020  CALL	R3	2
      0x8C10070A,  //  0021  GETMET	R4	R3	K10
      0x7C100200,  //  0022  CALL	R4	1
      0x8C10070B,  //  0023  GETMET	R4	R3	K11
      0x7C100200,  //  0024  CALL	R4	1
      0xB8121800,  //  0025  GETNGBL	R4	K12
      0x8C10090D,  //  0026  GETMET	R4	R4	K13
      0x8C18050E,  //  0027  GETMET	R6	R2	K14
      0x5820000F,  //  0028  LDCONST	R8	K15
      0x88240710,  //  0029  GETMBR	R9	R3	K16
      0x88241311,  //  002A  GETMBR	R9	R9	K17
      0x88280712,  //  002B  GETMBR	R10	R3	K18
      0x882C0713,  //  002C  GETMBR	R11	R3	K19
      0x7C180A00,  //  002D  CALL	R6	5
      0x581C0014,  //  002E  LDCONST	R7	K20
      0x7C100600,  //  002F  CALL	R4	3
      0x8C100315,  //  0030  GETMET	R4	R1	K21
      0x5C180600,  //  0031  MOVE	R6	R3
      0x7C100400,  //  0032  CALL	R4	2
      0x88100713,  //  0033  GETMBR	R4	R3	K19
      0x90022C04,  //  0034  SETMBR	R0	K22	R4
      0x50100200,  //  0035  LDBOOL	R4	1	0
      0x90020E04,  //  0036  SETMBR	R0	K7	R4
      0x7002000C,  //  0037  JMP		#0045
      0x880C0105,  //  0038  GETMBR	R3	R0	K5
      0x780E0008,  //  0039  JMPF	R3	#0043
      0x600C0003,  //  003A  GETGBL	R3	G3
      0x5C100000,  //  003B  MOVE	R4	R0
      0x7C0C0200,  //  003C  CALL	R3	1
      0x8C0C0706,  //  003D  GETMET	R3	R3	K6
      0x5C140200,  //  003E  MOVE	R5	R1
      0x7C0C0400,  //  003F  CALL	R3	2
      0x500C0000,  //  0040  LDBOOL	R3	0	0
      0x90020A03,  //  0041  SETMBR	R0	K5	R3
      0x70020001,  //  0042  JMP		#0045
      0x500C0200,  //  0043  LDBOOL	R3	1	0
      0x90020E03,  //  0044  SETMBR	R0	K7	R3
      0x80000000,  //  0045  RET	0
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
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20_X3ESub_OK_X20_X20_X20_X20_X28_X256i_X29_X20sub_X3D_X25i),
    /* K5   */  be_nested_str_weak(session),
    /* K6   */  be_nested_str_weak(local_session_id),
    /* K7   */  be_nested_str_weak(sub),
    /* K8   */  be_nested_str_weak(subscription_id),
    /* K9   */  be_const_int(2),
    /* K10  */  be_nested_str_weak(status_ok_received),
    }),
    be_str_weak(status_ok_received),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x88200305,  //  0005  GETMBR	R8	R1	K5
      0x88201106,  //  0006  GETMBR	R8	R8	K6
      0x88240107,  //  0007  GETMBR	R9	R0	K7
      0x88241308,  //  0008  GETMBR	R9	R9	K8
      0x7C140800,  //  0009  CALL	R5	4
      0x58180009,  //  000A  LDCONST	R6	K9
      0x7C0C0600,  //  000B  CALL	R3	3
      0x600C0003,  //  000C  GETGBL	R3	G3
      0x5C100000,  //  000D  MOVE	R4	R0
      0x7C0C0200,  //  000E  CALL	R3	1
      0x8C0C070A,  //  000F  GETMET	R3	R3	K10
      0x5C140200,  //  0010  MOVE	R5	R1
      0x7C0C0400,  //  0011  CALL	R3	2
      0x80040600,  //  0012  RET	1	R3
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
