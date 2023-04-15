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
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20IM_Message_X20send_im_X20exch_X3D_X25i_X20ready_X3D_X25i),
    /* K5   */  be_nested_str_weak(resp),
    /* K6   */  be_nested_str_weak(exchange_id),
    /* K7   */  be_nested_str_weak(ready),
    /* K8   */  be_const_int(1),
    /* K9   */  be_const_int(0),
    /* K10  */  be_const_int(3),
    /* K11  */  be_nested_str_weak(encode_frame),
    /* K12  */  be_nested_str_weak(data),
    /* K13  */  be_nested_str_weak(to_TLV),
    /* K14  */  be_nested_str_weak(tlv2raw),
    /* K15  */  be_nested_str_weak(encrypt),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20_X3Csnd_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20id_X3D_X25i_X20exch_X3D_X25i_X20rack_X3D_X25s),
    /* K17  */  be_nested_str_weak(session),
    /* K18  */  be_nested_str_weak(local_session_id),
    /* K19  */  be_nested_str_weak(message_counter),
    /* K20  */  be_nested_str_weak(ack_message_counter),
    /* K21  */  be_nested_str_weak(send_response_frame),
    /* K22  */  be_nested_str_weak(last_counter),
    /* K23  */  be_nested_str_weak(finish),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[49]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x88200105,  //  0005  GETMBR	R8	R0	K5
      0x88201106,  //  0006  GETMBR	R8	R8	K6
      0x88240107,  //  0007  GETMBR	R9	R0	K7
      0x78260001,  //  0008  JMPF	R9	#000B
      0x58240008,  //  0009  LDCONST	R9	K8
      0x70020000,  //  000A  JMP		#000C
      0x58240009,  //  000B  LDCONST	R9	K9
      0x7C140800,  //  000C  CALL	R5	4
      0x5818000A,  //  000D  LDCONST	R6	K10
      0x7C0C0600,  //  000E  CALL	R3	3
      0x880C0107,  //  000F  GETMBR	R3	R0	K7
      0x740E0001,  //  0010  JMPT	R3	#0013
      0x500C0000,  //  0011  LDBOOL	R3	0	0
      0x80040600,  //  0012  RET	1	R3
      0x880C0105,  //  0013  GETMBR	R3	R0	K5
      0x8C10070B,  //  0014  GETMET	R4	R3	K11
      0x8818010C,  //  0015  GETMBR	R6	R0	K12
      0x8C180D0D,  //  0016  GETMET	R6	R6	K13
      0x7C180200,  //  0017  CALL	R6	1
      0x8C180D0E,  //  0018  GETMET	R6	R6	K14
      0x7C180200,  //  0019  CALL	R6	1
      0x7C100400,  //  001A  CALL	R4	2
      0x8C10070F,  //  001B  GETMET	R4	R3	K15
      0x7C100200,  //  001C  CALL	R4	1
      0xB8120200,  //  001D  GETNGBL	R4	K1
      0x8C100902,  //  001E  GETMET	R4	R4	K2
      0x8C180503,  //  001F  GETMET	R6	R2	K3
      0x58200010,  //  0020  LDCONST	R8	K16
      0x88240711,  //  0021  GETMBR	R9	R3	K17
      0x88241312,  //  0022  GETMBR	R9	R9	K18
      0x88280713,  //  0023  GETMBR	R10	R3	K19
      0x882C0706,  //  0024  GETMBR	R11	R3	K6
      0x88300714,  //  0025  GETMBR	R12	R3	K20
      0x7C180C00,  //  0026  CALL	R6	6
      0x581C000A,  //  0027  LDCONST	R7	K10
      0x7C100600,  //  0028  CALL	R4	3
      0x8C100315,  //  0029  GETMET	R4	R1	K21
      0x5C180600,  //  002A  MOVE	R6	R3
      0x7C100400,  //  002B  CALL	R4	2
      0x88100713,  //  002C  GETMBR	R4	R3	K19
      0x90022C04,  //  002D  SETMBR	R0	K22	R4
      0x50100200,  //  002E  LDBOOL	R4	1	0
      0x90022E04,  //  002F  SETMBR	R0	K23	R4
      0x80000000,  //  0030  RET	0
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
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(MTR_X3A_X20IM_Message_X20ack_received_X20exch_X3D),
    /* K3   */  be_nested_str_weak(resp),
    /* K4   */  be_nested_str_weak(exchange_id),
    /* K5   */  be_const_int(3),
    /* K6   */  be_nested_str_weak(expiration),
    /* K7   */  be_nested_str_weak(millis),
    /* K8   */  be_nested_str_weak(MSG_TIMEOUT),
    }),
    be_str_weak(ack_received),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x60100008,  //  0002  GETGBL	R4	G8
      0x88140103,  //  0003  GETMBR	R5	R0	K3
      0x88140B04,  //  0004  GETMBR	R5	R5	K4
      0x7C100200,  //  0005  CALL	R4	1
      0x00120404,  //  0006  ADD	R4	K2	R4
      0x58140005,  //  0007  LDCONST	R5	K5
      0x7C080600,  //  0008  CALL	R2	3
      0xB80A0000,  //  0009  GETNGBL	R2	K0
      0x8C080507,  //  000A  GETMET	R2	R2	K7
      0x7C080200,  //  000B  CALL	R2	1
      0x880C0108,  //  000C  GETMBR	R3	R0	K8
      0x00080403,  //  000D  ADD	R2	R2	R3
      0x90020C02,  //  000E  SETMBR	R0	K6	R2
      0x50080000,  //  000F  LDBOOL	R2	0	0
      0x80040400,  //  0010  RET	1	R2
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
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20IM_Message_X20status_ok_received_X20exch_X3D_X25i),
    /* K5   */  be_nested_str_weak(resp),
    /* K6   */  be_nested_str_weak(exchange_id),
    /* K7   */  be_const_int(3),
    /* K8   */  be_nested_str_weak(expiration),
    /* K9   */  be_nested_str_weak(millis),
    /* K10  */  be_nested_str_weak(MSG_TIMEOUT),
    /* K11  */  be_nested_str_weak(build_response),
    /* K12  */  be_nested_str_weak(opcode),
    /* K13  */  be_nested_str_weak(x_flag_r),
    /* K14  */  be_nested_str_weak(ready),
    }),
    be_str_weak(status_ok_received),
    &be_const_str_solidified,
    ( &(const binstruction[29]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x88200105,  //  0005  GETMBR	R8	R0	K5
      0x88201106,  //  0006  GETMBR	R8	R8	K6
      0x7C140600,  //  0007  CALL	R5	3
      0x58180007,  //  0008  LDCONST	R6	K7
      0x7C0C0600,  //  0009  CALL	R3	3
      0xB80E0200,  //  000A  GETNGBL	R3	K1
      0x8C0C0709,  //  000B  GETMET	R3	R3	K9
      0x7C0C0200,  //  000C  CALL	R3	1
      0x8810010A,  //  000D  GETMBR	R4	R0	K10
      0x000C0604,  //  000E  ADD	R3	R3	R4
      0x90021003,  //  000F  SETMBR	R0	K8	R3
      0x78060007,  //  0010  JMPF	R1	#0019
      0x8C0C030B,  //  0011  GETMET	R3	R1	K11
      0x88140105,  //  0012  GETMBR	R5	R0	K5
      0x88140B0C,  //  0013  GETMBR	R5	R5	K12
      0x88180105,  //  0014  GETMBR	R6	R0	K5
      0x88180D0D,  //  0015  GETMBR	R6	R6	K13
      0x881C0105,  //  0016  GETMBR	R7	R0	K5
      0x7C0C0800,  //  0017  CALL	R3	4
      0x90020A03,  //  0018  SETMBR	R0	K5	R3
      0x500C0200,  //  0019  LDBOOL	R3	1	0
      0x90021C03,  //  001A  SETMBR	R0	K14	R3
      0x500C0200,  //  001B  LDBOOL	R3	1	0
      0x80040600,  //  001C  RET	1	R3
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
    21,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[34]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20IM_ReportData_X20send_im_X20exch_X3D_X25i_X20ready_X3D_X25i),
    /* K5   */  be_nested_str_weak(resp),
    /* K6   */  be_nested_str_weak(exchange_id),
    /* K7   */  be_nested_str_weak(ready),
    /* K8   */  be_const_int(1),
    /* K9   */  be_const_int(0),
    /* K10  */  be_const_int(3),
    /* K11  */  be_nested_str_weak(data),
    /* K12  */  be_nested_str_weak(more_chunked_messages),
    /* K13  */  be_nested_str_weak(attribute_reports),
    /* K14  */  be_nested_str_weak(to_TLV),
    /* K15  */  be_nested_str_weak(encode_len),
    /* K16  */  be_nested_str_weak(MAX_MESSAGE),
    /* K17  */  be_nested_str_weak(MTR_X3A_X20exch_X3D_X25i_X20elements_X3D_X25i_X20msg_sz_X3D_X25i_X20total_X3D_X25i),
    /* K18  */  be_nested_str_weak(get_exchangeid),
    /* K19  */  be_const_int(2147483647),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20_X2ERead_Attr_X20next_chunk_X20exch_X3D_X25i),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20_X2ERead_Attr_X20first_chunk_X20exch_X3D_X25i),
    /* K22  */  be_nested_str_weak(tlv2raw),
    /* K23  */  be_nested_str_weak(encode_frame),
    /* K24  */  be_nested_str_weak(encrypt),
    /* K25  */  be_nested_str_weak(MTR_X3A_X20_X3Csnd_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20id_X3D_X25i_X20exch_X3D_X25i_X20rack_X3D_X25s),
    /* K26  */  be_nested_str_weak(session),
    /* K27  */  be_nested_str_weak(local_session_id),
    /* K28  */  be_nested_str_weak(message_counter),
    /* K29  */  be_nested_str_weak(ack_message_counter),
    /* K30  */  be_nested_str_weak(send_response_frame),
    /* K31  */  be_nested_str_weak(last_counter),
    /* K32  */  be_nested_str_weak(MTR_X3A_X20to_be_sent_later_X20size_X3D_X25i_X20exch_X3D_X25i),
    /* K33  */  be_nested_str_weak(finish),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[173]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x88200105,  //  0005  GETMBR	R8	R0	K5
      0x88201106,  //  0006  GETMBR	R8	R8	K6
      0x88240107,  //  0007  GETMBR	R9	R0	K7
      0x78260001,  //  0008  JMPF	R9	#000B
      0x58240008,  //  0009  LDCONST	R9	K8
      0x70020000,  //  000A  JMP		#000C
      0x58240009,  //  000B  LDCONST	R9	K9
      0x7C140800,  //  000C  CALL	R5	4
      0x5818000A,  //  000D  LDCONST	R6	K10
      0x7C0C0600,  //  000E  CALL	R3	3
      0x880C0107,  //  000F  GETMBR	R3	R0	K7
      0x740E0001,  //  0010  JMPT	R3	#0013
      0x500C0000,  //  0011  LDBOOL	R3	0	0
      0x80040600,  //  0012  RET	1	R3
      0x880C0105,  //  0013  GETMBR	R3	R0	K5
      0x8810010B,  //  0014  GETMBR	R4	R0	K11
      0x8814090C,  //  0015  GETMBR	R5	R4	K12
      0x58180009,  //  0016  LDCONST	R6	K9
      0x581C0009,  //  0017  LDCONST	R7	K9
      0x8820090D,  //  0018  GETMBR	R8	R4	K13
      0x4C240000,  //  0019  LDNIL	R9
      0x20201009,  //  001A  NE	R8	R8	R9
      0x78220003,  //  001B  JMPF	R8	#0020
      0x6020000C,  //  001C  GETGBL	R8	G12
      0x8824090D,  //  001D  GETMBR	R9	R4	K13
      0x7C200200,  //  001E  CALL	R8	1
      0x70020000,  //  001F  JMP		#0021
      0x58200009,  //  0020  LDCONST	R8	K9
      0x24241109,  //  0021  GT	R9	R8	K9
      0x78260007,  //  0022  JMPF	R9	#002B
      0x8824090D,  //  0023  GETMBR	R9	R4	K13
      0x94241309,  //  0024  GETIDX	R9	R9	K9
      0x8C24130E,  //  0025  GETMET	R9	R9	K14
      0x7C240200,  //  0026  CALL	R9	1
      0x8C24130F,  //  0027  GETMET	R9	R9	K15
      0x7C240200,  //  0028  CALL	R9	1
      0x5C181200,  //  0029  MOVE	R6	R9
      0x581C0008,  //  002A  LDCONST	R7	K8
      0x88240110,  //  002B  GETMBR	R9	R0	K16
      0x14240C09,  //  002C  LT	R9	R6	R9
      0x78260010,  //  002D  JMPF	R9	#003F
      0x14240E08,  //  002E  LT	R9	R7	R8
      0x7826000E,  //  002F  JMPF	R9	#003F
      0x8824090D,  //  0030  GETMBR	R9	R4	K13
      0x94241207,  //  0031  GETIDX	R9	R9	R7
      0x8C24130E,  //  0032  GETMET	R9	R9	K14
      0x7C240200,  //  0033  CALL	R9	1
      0x8C24130F,  //  0034  GETMET	R9	R9	K15
      0x7C240200,  //  0035  CALL	R9	1
      0x00280C09,  //  0036  ADD	R10	R6	R9
      0x882C0110,  //  0037  GETMBR	R11	R0	K16
      0x1428140B,  //  0038  LT	R10	R10	R11
      0x782A0002,  //  0039  JMPF	R10	#003D
      0x00180C09,  //  003A  ADD	R6	R6	R9
      0x001C0F08,  //  003B  ADD	R7	R7	K8
      0x70020000,  //  003C  JMP		#003E
      0x70020000,  //  003D  JMP		#003F
      0x7001FFEB,  //  003E  JMP		#002B
      0xB8260200,  //  003F  GETNGBL	R9	K1
      0x8C241302,  //  0040  GETMET	R9	R9	K2
      0x8C2C0503,  //  0041  GETMET	R11	R2	K3
      0x58340011,  //  0042  LDCONST	R13	K17
      0x8C380112,  //  0043  GETMET	R14	R0	K18
      0x7C380200,  //  0044  CALL	R14	1
      0x5C3C0E00,  //  0045  MOVE	R15	R7
      0x5C400C00,  //  0046  MOVE	R16	R6
      0x5C441000,  //  0047  MOVE	R17	R8
      0x7C2C0C00,  //  0048  CALL	R11	6
      0x5830000A,  //  0049  LDCONST	R12	K10
      0x7C240600,  //  004A  CALL	R9	3
      0x60240012,  //  004B  GETGBL	R9	G18
      0x7C240000,  //  004C  CALL	R9	0
      0x8828090D,  //  004D  GETMBR	R10	R4	K13
      0x4C2C0000,  //  004E  LDNIL	R11
      0x2028140B,  //  004F  NE	R10	R10	R11
      0x782A000D,  //  0050  JMPF	R10	#005F
      0x40280F13,  //  0051  CONNECT	R10	R7	K19
      0x882C090D,  //  0052  GETMBR	R11	R4	K13
      0x9424160A,  //  0053  GETIDX	R9	R11	R10
      0x04300F08,  //  0054  SUB	R12	R7	K8
      0x4032120C,  //  0055  CONNECT	R12	K9	R12
      0x8834090D,  //  0056  GETMBR	R13	R4	K13
      0x94301A0C,  //  0057  GETIDX	R12	R13	R12
      0x90121A0C,  //  0058  SETMBR	R4	K13	R12
      0x6030000C,  //  0059  GETGBL	R12	G12
      0x5C341200,  //  005A  MOVE	R13	R9
      0x7C300200,  //  005B  CALL	R12	1
      0x24301909,  //  005C  GT	R12	R12	K9
      0x9012180C,  //  005D  SETMBR	R4	K12	R12
      0x70020001,  //  005E  JMP		#0061
      0x50280000,  //  005F  LDBOOL	R10	0	0
      0x9012180A,  //  0060  SETMBR	R4	K12	R10
      0x78160008,  //  0061  JMPF	R5	#006B
      0xB82A0200,  //  0062  GETNGBL	R10	K1
      0x8C281502,  //  0063  GETMET	R10	R10	K2
      0x8C300503,  //  0064  GETMET	R12	R2	K3
      0x58380014,  //  0065  LDCONST	R14	K20
      0x8C3C0112,  //  0066  GETMET	R15	R0	K18
      0x7C3C0200,  //  0067  CALL	R15	1
      0x7C300600,  //  0068  CALL	R12	3
      0x5834000A,  //  0069  LDCONST	R13	K10
      0x7C280600,  //  006A  CALL	R10	3
      0x8828090C,  //  006B  GETMBR	R10	R4	K12
      0x782A000A,  //  006C  JMPF	R10	#0078
      0x5C280A00,  //  006D  MOVE	R10	R5
      0x742A0008,  //  006E  JMPT	R10	#0078
      0xB82A0200,  //  006F  GETNGBL	R10	K1
      0x8C281502,  //  0070  GETMET	R10	R10	K2
      0x8C300503,  //  0071  GETMET	R12	R2	K3
      0x58380015,  //  0072  LDCONST	R14	K21
      0x8C3C0112,  //  0073  GETMET	R15	R0	K18
      0x7C3C0200,  //  0074  CALL	R15	1
      0x7C300600,  //  0075  CALL	R12	3
      0x5834000A,  //  0076  LDCONST	R13	K10
      0x7C280600,  //  0077  CALL	R10	3
      0x8828010B,  //  0078  GETMBR	R10	R0	K11
      0x8C28150E,  //  0079  GETMET	R10	R10	K14
      0x7C280200,  //  007A  CALL	R10	1
      0x8C2C1516,  //  007B  GETMET	R11	R10	K22
      0x60340015,  //  007C  GETGBL	R13	G21
      0x88380110,  //  007D  GETMBR	R14	R0	K16
      0x7C340200,  //  007E  CALL	R13	1
      0x7C2C0400,  //  007F  CALL	R11	2
      0x8C300717,  //  0080  GETMET	R12	R3	K23
      0x5C381600,  //  0081  MOVE	R14	R11
      0x7C300400,  //  0082  CALL	R12	2
      0x8C300718,  //  0083  GETMET	R12	R3	K24
      0x7C300200,  //  0084  CALL	R12	1
      0xB8320200,  //  0085  GETNGBL	R12	K1
      0x8C301902,  //  0086  GETMET	R12	R12	K2
      0x8C380503,  //  0087  GETMET	R14	R2	K3
      0x58400019,  //  0088  LDCONST	R16	K25
      0x8844071A,  //  0089  GETMBR	R17	R3	K26
      0x8844231B,  //  008A  GETMBR	R17	R17	K27
      0x8848071C,  //  008B  GETMBR	R18	R3	K28
      0x884C0706,  //  008C  GETMBR	R19	R3	K6
      0x8850071D,  //  008D  GETMBR	R20	R3	K29
      0x7C380C00,  //  008E  CALL	R14	6
      0x583C000A,  //  008F  LDCONST	R15	K10
      0x7C300600,  //  0090  CALL	R12	3
      0x8C30031E,  //  0091  GETMET	R12	R1	K30
      0x5C380600,  //  0092  MOVE	R14	R3
      0x7C300400,  //  0093  CALL	R12	2
      0x8830071C,  //  0094  GETMBR	R12	R3	K28
      0x90023E0C,  //  0095  SETMBR	R0	K31	R12
      0x6030000C,  //  0096  GETGBL	R12	G12
      0x5C341200,  //  0097  MOVE	R13	R9
      0x7C300200,  //  0098  CALL	R12	1
      0x24301909,  //  0099  GT	R12	R12	K9
      0x7832000E,  //  009A  JMPF	R12	#00AA
      0x90121A09,  //  009B  SETMBR	R4	K13	R9
      0xB8320200,  //  009C  GETNGBL	R12	K1
      0x8C301902,  //  009D  GETMET	R12	R12	K2
      0x8C380503,  //  009E  GETMET	R14	R2	K3
      0x58400020,  //  009F  LDCONST	R16	K32
      0x6044000C,  //  00A0  GETGBL	R17	G12
      0x8848090D,  //  00A1  GETMBR	R18	R4	K13
      0x7C440200,  //  00A2  CALL	R17	1
      0x88480706,  //  00A3  GETMBR	R18	R3	K6
      0x7C380800,  //  00A4  CALL	R14	4
      0x583C000A,  //  00A5  LDCONST	R15	K10
      0x7C300600,  //  00A6  CALL	R12	3
      0x50300000,  //  00A7  LDBOOL	R12	0	0
      0x90020E0C,  //  00A8  SETMBR	R0	K7	R12
      0x70020001,  //  00A9  JMP		#00AC
      0x50300200,  //  00AA  LDBOOL	R12	1	0
      0x9002420C,  //  00AB  SETMBR	R0	K33	R12
      0x80000000,  //  00AC  RET	0
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
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20IM_ReportDataSubscribed_X20ack_received_X20sub_X3D_X25i),
    /* K5   */  be_nested_str_weak(sub),
    /* K6   */  be_nested_str_weak(subscription_id),
    /* K7   */  be_const_int(3),
    /* K8   */  be_nested_str_weak(ack_received),
    /* K9   */  be_nested_str_weak(report_data_phase),
    /* K10  */  be_nested_str_weak(is_keep_alive),
    /* K11  */  be_nested_str_weak(re_arm),
    }),
    be_str_weak(ack_received),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x88200105,  //  0005  GETMBR	R8	R0	K5
      0x88201106,  //  0006  GETMBR	R8	R8	K6
      0x7C140600,  //  0007  CALL	R5	3
      0x58180007,  //  0008  LDCONST	R6	K7
      0x7C0C0600,  //  0009  CALL	R3	3
      0x600C0003,  //  000A  GETGBL	R3	G3
      0x5C100000,  //  000B  MOVE	R4	R0
      0x7C0C0200,  //  000C  CALL	R3	1
      0x8C0C0708,  //  000D  GETMET	R3	R3	K8
      0x5C140200,  //  000E  MOVE	R5	R1
      0x7C0C0400,  //  000F  CALL	R3	2
      0x880C0109,  //  0010  GETMBR	R3	R0	K9
      0x740E0008,  //  0011  JMPT	R3	#001B
      0x880C0105,  //  0012  GETMBR	R3	R0	K5
      0x880C070A,  //  0013  GETMBR	R3	R3	K10
      0x780E0002,  //  0014  JMPF	R3	#0018
      0x880C0105,  //  0015  GETMBR	R3	R0	K5
      0x8C0C070B,  //  0016  GETMET	R3	R3	K11
      0x7C0C0200,  //  0017  CALL	R3	1
      0x500C0200,  //  0018  LDBOOL	R3	1	0
      0x80040600,  //  0019  RET	1	R3
      0x70020001,  //  001A  JMP		#001D
      0x500C0000,  //  001B  LDBOOL	R3	0	0
      0x80040600,  //  001C  RET	1	R3
      0x80000000,  //  001D  RET	0
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
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20IM_ReportDataSubscribed_X20send_X20sub_X3D_X25i_X20exch_X3D_X25i_X20ready_X3D_X25i),
    /* K5   */  be_nested_str_weak(sub),
    /* K6   */  be_nested_str_weak(subscription_id),
    /* K7   */  be_nested_str_weak(resp),
    /* K8   */  be_nested_str_weak(exchange_id),
    /* K9   */  be_nested_str_weak(ready),
    /* K10  */  be_const_int(1),
    /* K11  */  be_const_int(0),
    /* K12  */  be_const_int(3),
    /* K13  */  be_nested_str_weak(MTR_X3A_X20ReportDataSubscribed_X3A_X3Asend_im_X20size_X28self_X2Edata_X2Eattribute_reports_X29_X3D_X25i_X20ready_X3D_X25s_X20report_data_phase_X3D_X25s),
    /* K14  */  be_nested_str_weak(data),
    /* K15  */  be_nested_str_weak(attribute_reports),
    /* K16  */  be_nested_str_weak(report_data_phase),
    /* K17  */  be_nested_str_weak(send_im),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20ReportDataSubscribed_X3A_X3Asend_im_X20called_X20super_X20finish_X3D_X25i),
    /* K19  */  be_nested_str_weak(finish),
    /* K20  */  be_nested_str_weak(build_standalone_ack),
    /* K21  */  be_nested_str_weak(encode_frame),
    /* K22  */  be_nested_str_weak(encrypt),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i),
    /* K24  */  be_nested_str_weak(session),
    /* K25  */  be_nested_str_weak(local_session_id),
    /* K26  */  be_nested_str_weak(ack_message_counter),
    /* K27  */  be_nested_str_weak(message_counter),
    /* K28  */  be_nested_str_weak(send_response_frame),
    /* K29  */  be_nested_str_weak(last_counter),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[111]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x88200105,  //  0005  GETMBR	R8	R0	K5
      0x88201106,  //  0006  GETMBR	R8	R8	K6
      0x88240107,  //  0007  GETMBR	R9	R0	K7
      0x88241308,  //  0008  GETMBR	R9	R9	K8
      0x88280109,  //  0009  GETMBR	R10	R0	K9
      0x782A0001,  //  000A  JMPF	R10	#000D
      0x5828000A,  //  000B  LDCONST	R10	K10
      0x70020000,  //  000C  JMP		#000E
      0x5828000B,  //  000D  LDCONST	R10	K11
      0x7C140A00,  //  000E  CALL	R5	5
      0x5818000C,  //  000F  LDCONST	R6	K12
      0x7C0C0600,  //  0010  CALL	R3	3
      0xB80E0200,  //  0011  GETNGBL	R3	K1
      0x8C0C0702,  //  0012  GETMET	R3	R3	K2
      0x8C140503,  //  0013  GETMET	R5	R2	K3
      0x581C000D,  //  0014  LDCONST	R7	K13
      0x6020000C,  //  0015  GETGBL	R8	G12
      0x8824010E,  //  0016  GETMBR	R9	R0	K14
      0x8824130F,  //  0017  GETMBR	R9	R9	K15
      0x7C200200,  //  0018  CALL	R8	1
      0x60240008,  //  0019  GETGBL	R9	G8
      0x88280109,  //  001A  GETMBR	R10	R0	K9
      0x7C240200,  //  001B  CALL	R9	1
      0x60280008,  //  001C  GETGBL	R10	G8
      0x882C0110,  //  001D  GETMBR	R11	R0	K16
      0x7C280200,  //  001E  CALL	R10	1
      0x7C140A00,  //  001F  CALL	R5	5
      0x5818000C,  //  0020  LDCONST	R6	K12
      0x7C0C0600,  //  0021  CALL	R3	3
      0x880C0109,  //  0022  GETMBR	R3	R0	K9
      0x740E0001,  //  0023  JMPT	R3	#0026
      0x500C0000,  //  0024  LDBOOL	R3	0	0
      0x80040600,  //  0025  RET	1	R3
      0x600C000C,  //  0026  GETGBL	R3	G12
      0x8810010E,  //  0027  GETMBR	R4	R0	K14
      0x8810090F,  //  0028  GETMBR	R4	R4	K15
      0x7C0C0200,  //  0029  CALL	R3	1
      0x240C070B,  //  002A  GT	R3	R3	K11
      0x780E0034,  //  002B  JMPF	R3	#0061
      0x880C0110,  //  002C  GETMBR	R3	R0	K16
      0x780E0017,  //  002D  JMPF	R3	#0046
      0x600C0003,  //  002E  GETGBL	R3	G3
      0x5C100000,  //  002F  MOVE	R4	R0
      0x7C0C0200,  //  0030  CALL	R3	1
      0x8C0C0711,  //  0031  GETMET	R3	R3	K17
      0x5C140200,  //  0032  MOVE	R5	R1
      0x7C0C0400,  //  0033  CALL	R3	2
      0xB80E0200,  //  0034  GETNGBL	R3	K1
      0x8C0C0702,  //  0035  GETMET	R3	R3	K2
      0x8C140503,  //  0036  GETMET	R5	R2	K3
      0x581C0012,  //  0037  LDCONST	R7	K18
      0x88200113,  //  0038  GETMBR	R8	R0	K19
      0x7C140600,  //  0039  CALL	R5	3
      0x5818000C,  //  003A  LDCONST	R6	K12
      0x7C0C0600,  //  003B  CALL	R3	3
      0x880C0113,  //  003C  GETMBR	R3	R0	K19
      0x740E0000,  //  003D  JMPT	R3	#003F
      0x80000600,  //  003E  RET	0
      0x500C0000,  //  003F  LDBOOL	R3	0	0
      0x90022003,  //  0040  SETMBR	R0	K16	R3
      0x500C0000,  //  0041  LDBOOL	R3	0	0
      0x90021203,  //  0042  SETMBR	R0	K9	R3
      0x500C0000,  //  0043  LDBOOL	R3	0	0
      0x90022603,  //  0044  SETMBR	R0	K19	R3
      0x70020019,  //  0045  JMP		#0060
      0x880C0107,  //  0046  GETMBR	R3	R0	K7
      0x8C0C0714,  //  0047  GETMET	R3	R3	K20
      0x50140000,  //  0048  LDBOOL	R5	0	0
      0x7C0C0400,  //  0049  CALL	R3	2
      0x8C100715,  //  004A  GETMET	R4	R3	K21
      0x7C100200,  //  004B  CALL	R4	1
      0x8C100716,  //  004C  GETMET	R4	R3	K22
      0x7C100200,  //  004D  CALL	R4	1
      0xB8120200,  //  004E  GETNGBL	R4	K1
      0x8C100902,  //  004F  GETMET	R4	R4	K2
      0x8C180503,  //  0050  GETMET	R6	R2	K3
      0x58200017,  //  0051  LDCONST	R8	K23
      0x88240718,  //  0052  GETMBR	R9	R3	K24
      0x88241319,  //  0053  GETMBR	R9	R9	K25
      0x8828071A,  //  0054  GETMBR	R10	R3	K26
      0x882C071B,  //  0055  GETMBR	R11	R3	K27
      0x7C180A00,  //  0056  CALL	R6	5
      0x581C000C,  //  0057  LDCONST	R7	K12
      0x7C100600,  //  0058  CALL	R4	3
      0x8C10031C,  //  0059  GETMET	R4	R1	K28
      0x5C180600,  //  005A  MOVE	R6	R3
      0x7C100400,  //  005B  CALL	R4	2
      0x8810071B,  //  005C  GETMBR	R4	R3	K27
      0x90023A04,  //  005D  SETMBR	R0	K29	R4
      0x50100200,  //  005E  LDBOOL	R4	1	0
      0x90022604,  //  005F  SETMBR	R0	K19	R4
      0x7002000C,  //  0060  JMP		#006E
      0x880C0110,  //  0061  GETMBR	R3	R0	K16
      0x780E0008,  //  0062  JMPF	R3	#006C
      0x600C0003,  //  0063  GETGBL	R3	G3
      0x5C100000,  //  0064  MOVE	R4	R0
      0x7C0C0200,  //  0065  CALL	R3	1
      0x8C0C0711,  //  0066  GETMET	R3	R3	K17
      0x5C140200,  //  0067  MOVE	R5	R1
      0x7C0C0400,  //  0068  CALL	R3	2
      0x500C0000,  //  0069  LDBOOL	R3	0	0
      0x90022003,  //  006A  SETMBR	R0	K16	R3
      0x70020001,  //  006B  JMP		#006E
      0x500C0200,  //  006C  LDBOOL	R3	1	0
      0x90022603,  //  006D  SETMBR	R0	K19	R3
      0x80000000,  //  006E  RET	0
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
    /* K4   */  be_nested_str_weak(MTR_X3A_X20IM_ReportDataSubscribed_X20status_error_received_X20sub_X3D_X25i_X20exch_X3D_X25i),
    /* K5   */  be_nested_str_weak(sub),
    /* K6   */  be_nested_str_weak(subscription_id),
    /* K7   */  be_nested_str_weak(resp),
    /* K8   */  be_nested_str_weak(exchange_id),
    /* K9   */  be_const_int(3),
    /* K10  */  be_nested_str_weak(remove_self),
    }),
    be_str_weak(status_error_received),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x88200105,  //  0005  GETMBR	R8	R0	K5
      0x88201106,  //  0006  GETMBR	R8	R8	K6
      0x88240107,  //  0007  GETMBR	R9	R0	K7
      0x88241308,  //  0008  GETMBR	R9	R9	K8
      0x7C140800,  //  0009  CALL	R5	4
      0x58180009,  //  000A  LDCONST	R6	K9
      0x7C0C0600,  //  000B  CALL	R3	3
      0x880C0105,  //  000C  GETMBR	R3	R0	K5
      0x8C0C070A,  //  000D  GETMET	R3	R3	K10
      0x7C0C0200,  //  000E  CALL	R3	1
      0x80000000,  //  000F  RET	0
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
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20IM_ReportDataSubscribed_X20status_ok_received_X20sub_X3D_X25i_X20exch_X3D_X25i),
    /* K5   */  be_nested_str_weak(sub),
    /* K6   */  be_nested_str_weak(subscription_id),
    /* K7   */  be_nested_str_weak(resp),
    /* K8   */  be_nested_str_weak(exchange_id),
    /* K9   */  be_const_int(3),
    /* K10  */  be_nested_str_weak(report_data_phase),
    /* K11  */  be_nested_str_weak(status_ok_received),
    /* K12  */  be_nested_str_weak(re_arm),
    }),
    be_str_weak(status_ok_received),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x88200105,  //  0005  GETMBR	R8	R0	K5
      0x88201106,  //  0006  GETMBR	R8	R8	K6
      0x88240107,  //  0007  GETMBR	R9	R0	K7
      0x88241308,  //  0008  GETMBR	R9	R9	K8
      0x7C140800,  //  0009  CALL	R5	4
      0x58180009,  //  000A  LDCONST	R6	K9
      0x7C0C0600,  //  000B  CALL	R3	3
      0x880C010A,  //  000C  GETMBR	R3	R0	K10
      0x780E0007,  //  000D  JMPF	R3	#0016
      0x600C0003,  //  000E  GETGBL	R3	G3
      0x5C100000,  //  000F  MOVE	R4	R0
      0x7C0C0200,  //  0010  CALL	R3	1
      0x8C0C070B,  //  0011  GETMET	R3	R3	K11
      0x5C140200,  //  0012  MOVE	R5	R1
      0x7C0C0400,  //  0013  CALL	R3	2
      0x80040600,  //  0014  RET	1	R3
      0x7002000A,  //  0015  JMP		#0021
      0x880C0105,  //  0016  GETMBR	R3	R0	K5
      0x8C0C070C,  //  0017  GETMET	R3	R3	K12
      0x7C0C0200,  //  0018  CALL	R3	1
      0x600C0003,  //  0019  GETGBL	R3	G3
      0x5C100000,  //  001A  MOVE	R4	R0
      0x7C0C0200,  //  001B  CALL	R3	1
      0x8C0C070B,  //  001C  GETMET	R3	R3	K11
      0x4C140000,  //  001D  LDNIL	R5
      0x7C0C0400,  //  001E  CALL	R3	2
      0x500C0000,  //  001F  LDBOOL	R3	0	0
      0x80040600,  //  0020  RET	1	R3
      0x80000000,  //  0021  RET	0
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
    /* K4   */  be_nested_str_weak(MTR_X3A_X20Matter_IM_SubscribedHeartbeat_X20status_error_received_X20sub_X3D_X25i_X20exch_X3D_X25i),
    /* K5   */  be_nested_str_weak(sub),
    /* K6   */  be_nested_str_weak(subscription_id),
    /* K7   */  be_nested_str_weak(resp),
    /* K8   */  be_nested_str_weak(exchange_id),
    /* K9   */  be_const_int(3),
    /* K10  */  be_nested_str_weak(remove_self),
    }),
    be_str_weak(status_error_received),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x88200105,  //  0005  GETMBR	R8	R0	K5
      0x88201106,  //  0006  GETMBR	R8	R8	K6
      0x88240107,  //  0007  GETMBR	R9	R0	K7
      0x88241308,  //  0008  GETMBR	R9	R9	K8
      0x7C140800,  //  0009  CALL	R5	4
      0x58180009,  //  000A  LDCONST	R6	K9
      0x7C0C0600,  //  000B  CALL	R3	3
      0x880C0105,  //  000C  GETMBR	R3	R0	K5
      0x8C0C070A,  //  000D  GETMET	R3	R3	K10
      0x7C0C0200,  //  000E  CALL	R3	1
      0x500C0000,  //  000F  LDBOOL	R3	0	0
      0x80040600,  //  0010  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_im
********************************************************************/
be_local_closure(Matter_IM_SubscribedHeartbeat_send_im,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20Matter_IM_SubscribedHeartbeat_X20send_X20sub_X3D_X25i_X20exch_X3D_X25i_X20ready_X3D_X25i),
    /* K5   */  be_nested_str_weak(sub),
    /* K6   */  be_nested_str_weak(subscription_id),
    /* K7   */  be_nested_str_weak(resp),
    /* K8   */  be_nested_str_weak(exchange_id),
    /* K9   */  be_nested_str_weak(ready),
    /* K10  */  be_const_int(1),
    /* K11  */  be_const_int(0),
    /* K12  */  be_const_int(3),
    /* K13  */  be_nested_str_weak(send_im),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x88200105,  //  0005  GETMBR	R8	R0	K5
      0x88201106,  //  0006  GETMBR	R8	R8	K6
      0x88240107,  //  0007  GETMBR	R9	R0	K7
      0x88241308,  //  0008  GETMBR	R9	R9	K8
      0x88280109,  //  0009  GETMBR	R10	R0	K9
      0x782A0001,  //  000A  JMPF	R10	#000D
      0x5828000A,  //  000B  LDCONST	R10	K10
      0x70020000,  //  000C  JMP		#000E
      0x5828000B,  //  000D  LDCONST	R10	K11
      0x7C140A00,  //  000E  CALL	R5	5
      0x5818000C,  //  000F  LDCONST	R6	K12
      0x7C0C0600,  //  0010  CALL	R3	3
      0x880C0109,  //  0011  GETMBR	R3	R0	K9
      0x740E0001,  //  0012  JMPT	R3	#0015
      0x500C0000,  //  0013  LDBOOL	R3	0	0
      0x80040600,  //  0014  RET	1	R3
      0x600C0003,  //  0015  GETGBL	R3	G3
      0x5C100000,  //  0016  MOVE	R4	R0
      0x7C0C0200,  //  0017  CALL	R3	1
      0x8C0C070D,  //  0018  GETMET	R3	R3	K13
      0x5C140200,  //  0019  MOVE	R5	R1
      0x7C0C0400,  //  001A  CALL	R3	2
      0x500C0000,  //  001B  LDBOOL	R3	0	0
      0x90021203,  //  001C  SETMBR	R0	K9	R3
      0x80000000,  //  001D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ack_received
********************************************************************/
be_local_closure(Matter_IM_SubscribedHeartbeat_ack_received,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20Matter_IM_SubscribedHeartbeat_X20ack_received_X20sub_X3D_X25i),
    /* K5   */  be_nested_str_weak(sub),
    /* K6   */  be_nested_str_weak(subscription_id),
    /* K7   */  be_const_int(3),
    /* K8   */  be_nested_str_weak(ack_received),
    /* K9   */  be_nested_str_weak(finish),
    }),
    be_str_weak(ack_received),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x88200105,  //  0005  GETMBR	R8	R0	K5
      0x88201106,  //  0006  GETMBR	R8	R8	K6
      0x7C140600,  //  0007  CALL	R5	3
      0x58180007,  //  0008  LDCONST	R6	K7
      0x7C0C0600,  //  0009  CALL	R3	3
      0x600C0003,  //  000A  GETGBL	R3	G3
      0x5C100000,  //  000B  MOVE	R4	R0
      0x7C0C0200,  //  000C  CALL	R3	1
      0x8C0C0708,  //  000D  GETMET	R3	R3	K8
      0x5C140200,  //  000E  MOVE	R5	R1
      0x7C0C0400,  //  000F  CALL	R3	2
      0x500C0200,  //  0010  LDBOOL	R3	1	0
      0x90021203,  //  0011  SETMBR	R0	K9	R3
      0x500C0200,  //  0012  LDBOOL	R3	1	0
      0x80040600,  //  0013  RET	1	R3
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
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20Matter_IM_SubscribedHeartbeat_X20status_ok_received_X20sub_X3D_X25i_X20exch_X3D_X25i),
    /* K5   */  be_nested_str_weak(sub),
    /* K6   */  be_nested_str_weak(subscription_id),
    /* K7   */  be_nested_str_weak(resp),
    /* K8   */  be_nested_str_weak(exchange_id),
    /* K9   */  be_const_int(3),
    }),
    be_str_weak(status_ok_received),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x88200105,  //  0005  GETMBR	R8	R0	K5
      0x88201106,  //  0006  GETMBR	R8	R8	K6
      0x88240107,  //  0007  GETMBR	R9	R0	K7
      0x88241308,  //  0008  GETMBR	R9	R9	K8
      0x7C140800,  //  0009  CALL	R5	4
      0x58180009,  //  000A  LDCONST	R6	K9
      0x7C0C0600,  //  000B  CALL	R3	3
      0x500C0000,  //  000C  LDBOOL	R3	0	0
      0x80040600,  //  000D  RET	1	R3
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
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20IM_SubscribeResponse_X20status_ok_received_X20sub_X3D_X25i_X20exch_X3D_X25i_X20ack_X3D_X25i_X20last_counter_X3D_X25i),
    /* K5   */  be_nested_str_weak(sub),
    /* K6   */  be_nested_str_weak(subscription_id),
    /* K7   */  be_nested_str_weak(resp),
    /* K8   */  be_nested_str_weak(exchange_id),
    /* K9   */  be_nested_str_weak(ack_message_counter),
    /* K10  */  be_const_int(0),
    /* K11  */  be_nested_str_weak(last_counter),
    /* K12  */  be_const_int(3),
    /* K13  */  be_nested_str_weak(MTR_X3A_X20_X3ESub_OK_X20_X20_X20_X20_X28_X256i_X29_X20sub_X3D_X25i),
    /* K14  */  be_nested_str_weak(session),
    /* K15  */  be_nested_str_weak(local_session_id),
    /* K16  */  be_const_int(2),
    /* K17  */  be_nested_str_weak(status_ok_received),
    }),
    be_str_weak(status_ok_received),
    &be_const_str_solidified,
    ( &(const binstruction[36]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x88200105,  //  0005  GETMBR	R8	R0	K5
      0x88201106,  //  0006  GETMBR	R8	R8	K6
      0x88240107,  //  0007  GETMBR	R9	R0	K7
      0x88241308,  //  0008  GETMBR	R9	R9	K8
      0x88280309,  //  0009  GETMBR	R10	R1	K9
      0x782A0001,  //  000A  JMPF	R10	#000D
      0x88280309,  //  000B  GETMBR	R10	R1	K9
      0x70020000,  //  000C  JMP		#000E
      0x5828000A,  //  000D  LDCONST	R10	K10
      0x882C010B,  //  000E  GETMBR	R11	R0	K11
      0x7C140C00,  //  000F  CALL	R5	6
      0x5818000C,  //  0010  LDCONST	R6	K12
      0x7C0C0600,  //  0011  CALL	R3	3
      0xB80E0200,  //  0012  GETNGBL	R3	K1
      0x8C0C0702,  //  0013  GETMET	R3	R3	K2
      0x8C140503,  //  0014  GETMET	R5	R2	K3
      0x581C000D,  //  0015  LDCONST	R7	K13
      0x8820030E,  //  0016  GETMBR	R8	R1	K14
      0x8820110F,  //  0017  GETMBR	R8	R8	K15
      0x88240105,  //  0018  GETMBR	R9	R0	K5
      0x88241306,  //  0019  GETMBR	R9	R9	K6
      0x7C140800,  //  001A  CALL	R5	4
      0x58180010,  //  001B  LDCONST	R6	K16
      0x7C0C0600,  //  001C  CALL	R3	3
      0x600C0003,  //  001D  GETGBL	R3	G3
      0x5C100000,  //  001E  MOVE	R4	R0
      0x7C0C0200,  //  001F  CALL	R3	1
      0x8C0C0711,  //  0020  GETMET	R3	R3	K17
      0x5C140200,  //  0021  MOVE	R5	R1
      0x7C0C0400,  //  0022  CALL	R3	2
      0x80040600,  //  0023  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_im
********************************************************************/
be_local_closure(Matter_IM_SubscribeResponse_send_im,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[28]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20Matter_IM_SubscribeResponse_X20send_X20sub_X3D_X25i_X20ready_X3D_X25i),
    /* K5   */  be_nested_str_weak(sub),
    /* K6   */  be_nested_str_weak(subscription_id),
    /* K7   */  be_nested_str_weak(ready),
    /* K8   */  be_const_int(1),
    /* K9   */  be_const_int(0),
    /* K10  */  be_const_int(3),
    /* K11  */  be_nested_str_weak(report_data_phase),
    /* K12  */  be_nested_str_weak(send_im),
    /* K13  */  be_nested_str_weak(finish),
    /* K14  */  be_nested_str_weak(resp),
    /* K15  */  be_nested_str_weak(matter),
    /* K16  */  be_nested_str_weak(SubscribeResponseMessage),
    /* K17  */  be_nested_str_weak(max_interval),
    /* K18  */  be_nested_str_weak(opcode),
    /* K19  */  be_nested_str_weak(encode_frame),
    /* K20  */  be_nested_str_weak(to_TLV),
    /* K21  */  be_nested_str_weak(tlv2raw),
    /* K22  */  be_nested_str_weak(encrypt),
    /* K23  */  be_nested_str_weak(send_response_frame),
    /* K24  */  be_nested_str_weak(last_counter),
    /* K25  */  be_nested_str_weak(message_counter),
    /* K26  */  be_nested_str_weak(MTR_X3A_X20Send_X20SubscribeResponseMessage_X20sub_X3D_X25i_X20id_X3D_X25i),
    /* K27  */  be_nested_str_weak(re_arm),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[78]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x8C140503,  //  0003  GETMET	R5	R2	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x88200105,  //  0005  GETMBR	R8	R0	K5
      0x88201106,  //  0006  GETMBR	R8	R8	K6
      0x88240107,  //  0007  GETMBR	R9	R0	K7
      0x78260001,  //  0008  JMPF	R9	#000B
      0x58240008,  //  0009  LDCONST	R9	K8
      0x70020000,  //  000A  JMP		#000C
      0x58240009,  //  000B  LDCONST	R9	K9
      0x7C140800,  //  000C  CALL	R5	4
      0x5818000A,  //  000D  LDCONST	R6	K10
      0x7C0C0600,  //  000E  CALL	R3	3
      0x880C0107,  //  000F  GETMBR	R3	R0	K7
      0x740E0001,  //  0010  JMPT	R3	#0013
      0x500C0000,  //  0011  LDBOOL	R3	0	0
      0x80040600,  //  0012  RET	1	R3
      0x880C010B,  //  0013  GETMBR	R3	R0	K11
      0x780E000E,  //  0014  JMPF	R3	#0024
      0x600C0003,  //  0015  GETGBL	R3	G3
      0x5C100000,  //  0016  MOVE	R4	R0
      0x7C0C0200,  //  0017  CALL	R3	1
      0x8C0C070C,  //  0018  GETMET	R3	R3	K12
      0x5C140200,  //  0019  MOVE	R5	R1
      0x7C0C0400,  //  001A  CALL	R3	2
      0x880C010D,  //  001B  GETMBR	R3	R0	K13
      0x780E0003,  //  001C  JMPF	R3	#0021
      0x500C0000,  //  001D  LDBOOL	R3	0	0
      0x90021603,  //  001E  SETMBR	R0	K11	R3
      0x500C0000,  //  001F  LDBOOL	R3	0	0
      0x90021A03,  //  0020  SETMBR	R0	K13	R3
      0x500C0000,  //  0021  LDBOOL	R3	0	0
      0x90020E03,  //  0022  SETMBR	R0	K7	R3
      0x70020028,  //  0023  JMP		#004D
      0x880C010E,  //  0024  GETMBR	R3	R0	K14
      0xB8121E00,  //  0025  GETNGBL	R4	K15
      0x8C100910,  //  0026  GETMET	R4	R4	K16
      0x7C100200,  //  0027  CALL	R4	1
      0x88140105,  //  0028  GETMBR	R5	R0	K5
      0x88140B06,  //  0029  GETMBR	R5	R5	K6
      0x90120C05,  //  002A  SETMBR	R4	K6	R5
      0x88140105,  //  002B  GETMBR	R5	R0	K5
      0x88140B11,  //  002C  GETMBR	R5	R5	K17
      0x90122205,  //  002D  SETMBR	R4	K17	R5
      0x8814010E,  //  002E  GETMBR	R5	R0	K14
      0x541A0003,  //  002F  LDINT	R6	4
      0x90162406,  //  0030  SETMBR	R5	K18	R6
      0x8C140713,  //  0031  GETMET	R5	R3	K19
      0x8C1C0914,  //  0032  GETMET	R7	R4	K20
      0x7C1C0200,  //  0033  CALL	R7	1
      0x8C1C0F15,  //  0034  GETMET	R7	R7	K21
      0x7C1C0200,  //  0035  CALL	R7	1
      0x7C140400,  //  0036  CALL	R5	2
      0x8C140716,  //  0037  GETMET	R5	R3	K22
      0x7C140200,  //  0038  CALL	R5	1
      0x8C140317,  //  0039  GETMET	R5	R1	K23
      0x5C1C0600,  //  003A  MOVE	R7	R3
      0x7C140400,  //  003B  CALL	R5	2
      0x88140719,  //  003C  GETMBR	R5	R3	K25
      0x90023005,  //  003D  SETMBR	R0	K24	R5
      0xB8160200,  //  003E  GETNGBL	R5	K1
      0x8C140B02,  //  003F  GETMET	R5	R5	K2
      0x8C1C0503,  //  0040  GETMET	R7	R2	K3
      0x5824001A,  //  0041  LDCONST	R9	K26
      0x88280105,  //  0042  GETMBR	R10	R0	K5
      0x88281506,  //  0043  GETMBR	R10	R10	K6
      0x882C0719,  //  0044  GETMBR	R11	R3	K25
      0x7C1C0800,  //  0045  CALL	R7	4
      0x5820000A,  //  0046  LDCONST	R8	K10
      0x7C140600,  //  0047  CALL	R5	3
      0x88140105,  //  0048  GETMBR	R5	R0	K5
      0x8C140B1B,  //  0049  GETMET	R5	R5	K27
      0x7C140200,  //  004A  CALL	R5	1
      0x50140200,  //  004B  LDBOOL	R5	1	0
      0x90021A05,  //  004C  SETMBR	R0	K13	R5
      0x80000000,  //  004D  RET	0
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
