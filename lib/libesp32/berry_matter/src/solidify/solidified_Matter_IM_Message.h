/* Solidification of Matter_IM_Message.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_IM_Message' ktab size: 28, total: 48 (saved 160 bytes)
static const bvalue be_ktab_class_Matter_IM_Message[28] = {
  /* K0   */  be_nested_str_weak(resp),
  /* K1   */  be_nested_str_weak(build_response),
  /* K2   */  be_nested_str_weak(ready),
  /* K3   */  be_nested_str_weak(expiration),
  /* K4   */  be_nested_str_weak(tasmota),
  /* K5   */  be_nested_str_weak(millis),
  /* K6   */  be_nested_str_weak(MSG_TIMEOUT),
  /* K7   */  be_nested_str_weak(last_counter),
  /* K8   */  be_const_int(0),
  /* K9   */  be_nested_str_weak(finishing),
  /* K10  */  be_nested_str_weak(finished),
  /* K11  */  be_nested_str_weak(data),
  /* K12  */  be_nested_str_weak(reset),
  /* K13  */  be_nested_str_weak(to_TLV),
  /* K14  */  be_nested_str_weak(tlv2raw),
  /* K15  */  be_nested_str_weak(encode_frame),
  /* K16  */  be_nested_str_weak(encrypt),
  /* K17  */  be_nested_str_weak(loglevel),
  /* K18  */  be_nested_str_weak(log),
  /* K19  */  be_nested_str_weak(MTR_X3A_X20_X3Csnd_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20id_X3D_X25s_X20exch_X3D_X25s_X20rack_X3D_X25s),
  /* K20  */  be_nested_str_weak(session),
  /* K21  */  be_nested_str_weak(local_session_id),
  /* K22  */  be_nested_str_weak(message_counter),
  /* K23  */  be_nested_str_weak(exchange_id),
  /* K24  */  be_nested_str_weak(ack_message_counter),
  /* K25  */  be_nested_str_weak(send_response_frame),
  /* K26  */  be_nested_str_weak(opcode),
  /* K27  */  be_nested_str_weak(x_flag_r),
};


extern const bclass be_class_Matter_IM_Message;

/********************************************************************
** Solidified function: reset
********************************************************************/
be_local_closure(class_Matter_IM_Message_reset,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_Message,     /* shared constants */
    be_str_weak(reset),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
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
      0x50100000,  //  0015  LDBOOL	R4	0	0
      0x90021404,  //  0016  SETMBR	R0	K10	R4
      0x4C100000,  //  0017  LDNIL	R4
      0x90021604,  //  0018  SETMBR	R0	K11	R4
      0x80000000,  //  0019  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_IM_Message_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_Message,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x8C10010C,  //  0000  GETMET	R4	R0	K12
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
** Solidified function: send_im
********************************************************************/
be_local_closure(class_Matter_IM_Message_send_im,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_Message,     /* shared constants */
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x880C010B,  //  0001  GETMBR	R3	R0	K11
      0x8C0C070D,  //  0002  GETMET	R3	R3	K13
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8C0C070E,  //  0004  GETMET	R3	R3	K14
      0x7C0C0200,  //  0005  CALL	R3	1
      0x8C10050F,  //  0006  GETMET	R4	R2	K15
      0x5C180600,  //  0007  MOVE	R6	R3
      0x7C100400,  //  0008  CALL	R4	2
      0x8C100510,  //  0009  GETMET	R4	R2	K16
      0x7C100200,  //  000A  CALL	R4	1
      0xB8120800,  //  000B  GETNGBL	R4	K4
      0x8C100911,  //  000C  GETMET	R4	R4	K17
      0x541A0003,  //  000D  LDINT	R6	4
      0x7C100400,  //  000E  CALL	R4	2
      0x7812000A,  //  000F  JMPF	R4	#001B
      0xB8122400,  //  0010  GETNGBL	R4	K18
      0x60140018,  //  0011  GETGBL	R5	G24
      0x58180013,  //  0012  LDCONST	R6	K19
      0x881C0514,  //  0013  GETMBR	R7	R2	K20
      0x881C0F15,  //  0014  GETMBR	R7	R7	K21
      0x88200516,  //  0015  GETMBR	R8	R2	K22
      0x88240517,  //  0016  GETMBR	R9	R2	K23
      0x88280518,  //  0017  GETMBR	R10	R2	K24
      0x7C140A00,  //  0018  CALL	R5	5
      0x541A0003,  //  0019  LDINT	R6	4
      0x7C100400,  //  001A  CALL	R4	2
      0x8C100319,  //  001B  GETMET	R4	R1	K25
      0x5C180400,  //  001C  MOVE	R6	R2
      0x7C100400,  //  001D  CALL	R4	2
      0x88100516,  //  001E  GETMBR	R4	R2	K22
      0x90020E04,  //  001F  SETMBR	R0	K7	R4
      0x50100200,  //  0020  LDBOOL	R4	1	0
      0x90021204,  //  0021  SETMBR	R0	K9	R4
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: status_error_received
********************************************************************/
be_local_closure(class_Matter_IM_Message_status_error_received,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_Message,     /* shared constants */
    be_str_weak(status_error_received),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_exchangeid
********************************************************************/
be_local_closure(class_Matter_IM_Message_get_exchangeid,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_Message,     /* shared constants */
    be_str_weak(get_exchangeid),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040317,  //  0001  GETMBR	R1	R1	K23
      0x80040200,  //  0002  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ack_received
********************************************************************/
be_local_closure(class_Matter_IM_Message_ack_received,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_Message,     /* shared constants */
    be_str_weak(ack_received),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88080109,  //  0000  GETMBR	R2	R0	K9
      0x780A0002,  //  0001  JMPF	R2	#0005
      0x50080200,  //  0002  LDBOOL	R2	1	0
      0x90021402,  //  0003  SETMBR	R0	K10	R2
      0x70020005,  //  0004  JMP		#000B
      0xB80A0800,  //  0005  GETNGBL	R2	K4
      0x8C080505,  //  0006  GETMET	R2	R2	K5
      0x7C080200,  //  0007  CALL	R2	1
      0x880C0106,  //  0008  GETMBR	R3	R0	K6
      0x00080403,  //  0009  ADD	R2	R2	R3
      0x90020602,  //  000A  SETMBR	R0	K3	R2
      0x50080000,  //  000B  LDBOOL	R2	0	0
      0x80040400,  //  000C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reached_timeout
********************************************************************/
be_local_closure(class_Matter_IM_Message_reached_timeout,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_Message,     /* shared constants */
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
be_local_closure(class_Matter_IM_Message_status_ok_received,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_Message,     /* shared constants */
    be_str_weak(status_ok_received),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0xB80A0800,  //  0000  GETNGBL	R2	K4
      0x8C080505,  //  0001  GETMET	R2	R2	K5
      0x7C080200,  //  0002  CALL	R2	1
      0x880C0106,  //  0003  GETMBR	R3	R0	K6
      0x00080403,  //  0004  ADD	R2	R2	R3
      0x90020602,  //  0005  SETMBR	R0	K3	R2
      0x78060007,  //  0006  JMPF	R1	#000F
      0x8C080301,  //  0007  GETMET	R2	R1	K1
      0x88100100,  //  0008  GETMBR	R4	R0	K0
      0x8810091A,  //  0009  GETMBR	R4	R4	K26
      0x88140100,  //  000A  GETMBR	R5	R0	K0
      0x88140B1B,  //  000B  GETMBR	R5	R5	K27
      0x88180100,  //  000C  GETMBR	R6	R0	K0
      0x7C080800,  //  000D  CALL	R2	4
      0x90020002,  //  000E  SETMBR	R0	K0	R2
      0x50080200,  //  000F  LDBOOL	R2	1	0
      0x90020402,  //  0010  SETMBR	R0	K2	R2
      0x50080200,  //  0011  LDBOOL	R2	1	0
      0x80040400,  //  0012  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM_Message
********************************************************************/
be_local_class(Matter_IM_Message,
    7,
    NULL,
    be_nested_map(16,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(reset, 6), be_const_closure(class_Matter_IM_Message_reset_closure) },
        { be_const_key_weak(last_counter, -1), be_const_var(6) },
        { be_const_key_weak(send_im, 1), be_const_closure(class_Matter_IM_Message_send_im_closure) },
        { be_const_key_weak(init, 15), be_const_closure(class_Matter_IM_Message_init_closure) },
        { be_const_key_weak(status_error_received, 13), be_const_closure(class_Matter_IM_Message_status_error_received_closure) },
        { be_const_key_weak(data, 9), be_const_var(5) },
        { be_const_key_weak(get_exchangeid, -1), be_const_closure(class_Matter_IM_Message_get_exchangeid_closure) },
        { be_const_key_weak(status_ok_received, 12), be_const_closure(class_Matter_IM_Message_status_ok_received_closure) },
        { be_const_key_weak(ack_received, 7), be_const_closure(class_Matter_IM_Message_ack_received_closure) },
        { be_const_key_weak(reached_timeout, -1), be_const_closure(class_Matter_IM_Message_reached_timeout_closure) },
        { be_const_key_weak(MSG_TIMEOUT, -1), be_const_int(5000) },
        { be_const_key_weak(resp, -1), be_const_var(1) },
        { be_const_key_weak(expiration, -1), be_const_var(0) },
        { be_const_key_weak(ready, -1), be_const_var(2) },
        { be_const_key_weak(finishing, -1), be_const_var(3) },
        { be_const_key_weak(finished, -1), be_const_var(4) },
    })),
    be_str_weak(Matter_IM_Message)
);

extern const bclass be_class_Matter_IM_Status;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_IM_Status_init,   /* name */
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
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_IM_Status_init_closure) },
    })),
    be_str_weak(Matter_IM_Status)
);

extern const bclass be_class_Matter_IM_InvokeResponse;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_IM_InvokeResponse_init,   /* name */
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
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_IM_InvokeResponse_init_closure) },
    })),
    be_str_weak(Matter_IM_InvokeResponse)
);

extern const bclass be_class_Matter_IM_WriteResponse;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_IM_WriteResponse_init,   /* name */
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
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_IM_WriteResponse_init_closure) },
    })),
    be_str_weak(Matter_IM_WriteResponse)
);
// compact class 'Matter_IM_ReportData_Pull' ktab size: 51, total: 57 (saved 48 bytes)
static const bvalue be_ktab_class_Matter_IM_ReportData_Pull[51] = {
  /* K0   */  be_nested_str_weak(resp),
  /* K1   */  be_nested_str_weak(data),
  /* K2   */  be_nested_str_weak(device),
  /* K3   */  be_nested_str_weak(debug),
  /* K4   */  be_nested_str_weak(data_ev),
  /* K5   */  be_nested_str_weak(event_generator_or_arr),
  /* K6   */  be_nested_str_weak(generator_or_arr),
  /* K7   */  be_const_int(0),
  /* K8   */  be_nested_str_weak(next_attribute),
  /* K9   */  be_nested_str_weak(is_direct),
  /* K10  */  be_nested_str_weak(im),
  /* K11  */  be_nested_str_weak(read_single_attribute_to_bytes),
  /* K12  */  be_nested_str_weak(get_pi),
  /* K13  */  be_nested_str_weak(session),
  /* K14  */  be_nested_str_weak(MAX_MESSAGE),
  /* K15  */  be_nested_str_weak(append),
  /* K16  */  be_nested_str_weak(remove),
  /* K17  */  be_nested_str_weak(next_event),
  /* K18  */  be_nested_str_weak(tasmota),
  /* K19  */  be_nested_str_weak(loglevel),
  /* K20  */  be_const_int(3),
  /* K21  */  be_nested_str_weak(),
  /* K22  */  be_nested_str_weak(data0),
  /* K23  */  be_nested_str_weak(_X20_X2D_X20),
  /* K24  */  be_nested_str_weak(data1),
  /* K25  */  be_nested_str_weak(_X2C_X20),
  /* K26  */  be_nested_str_weak(data2),
  /* K27  */  be_nested_str_weak(log),
  /* K28  */  be_nested_str_weak(MTR_X3A_X20_X3ERead_Event_X28_X256i_X7C_X258s_X29_X20_X5B_X2502X_X5D_X2504X_X2F_X2502X_X25s),
  /* K29  */  be_nested_str_weak(local_session_id),
  /* K30  */  be_nested_str_weak(event_no),
  /* K31  */  be_nested_str_weak(endpoint),
  /* K32  */  be_nested_str_weak(cluster),
  /* K33  */  be_nested_str_weak(event_id),
  /* K34  */  be_nested_str_weak(to_raw_bytes),
  /* K35  */  be_nested_str_weak(add),
  /* K36  */  be_const_int(1),
  /* K37  */  be_nested_str_weak(subscription_id),
  /* K38  */  be_const_int(2),
  /* K39  */  be_nested_str_weak(suppress_response),
  /* K40  */  be_nested_str_weak(encode_frame),
  /* K41  */  be_nested_str_weak(encrypt),
  /* K42  */  be_nested_str_weak(send_response_frame),
  /* K43  */  be_nested_str_weak(last_counter),
  /* K44  */  be_nested_str_weak(message_counter),
  /* K45  */  be_nested_str_weak(ready),
  /* K46  */  be_nested_str_weak(finishing),
  /* K47  */  be_nested_str_weak(status_ok_received),
  /* K48  */  be_nested_str_weak(_message_handler),
  /* K49  */  be_nested_str_weak(send_encrypted_ack),
  /* K50  */  be_nested_str_weak(init),
};


extern const bclass be_class_Matter_IM_ReportData_Pull;

/********************************************************************
** Solidified function: send_im
********************************************************************/
be_local_closure(class_Matter_IM_ReportData_Pull_send_im,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_ReportData_Pull,     /* shared constants */
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[361]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x4C100000,  //  0002  LDNIL	R4
      0x200C0604,  //  0003  NE	R3	R3	R4
      0x780E0001,  //  0004  JMPF	R3	#0007
      0x880C0101,  //  0005  GETMBR	R3	R0	K1
      0x70020001,  //  0006  JMP		#0009
      0x600C0015,  //  0007  GETGBL	R3	G21
      0x7C0C0000,  //  0008  CALL	R3	0
      0x4C100000,  //  0009  LDNIL	R4
      0x90020204,  //  000A  SETMBR	R0	K1	R4
      0x50100200,  //  000B  LDBOOL	R4	1	0
      0x88140302,  //  000C  GETMBR	R5	R1	K2
      0x88140B03,  //  000D  GETMBR	R5	R5	K3
      0x88180104,  //  000E  GETMBR	R6	R0	K4
      0x4C1C0000,  //  000F  LDNIL	R7
      0x20180C07,  //  0010  NE	R6	R6	R7
      0x781A0001,  //  0011  JMPF	R6	#0014
      0x88180104,  //  0012  GETMBR	R6	R0	K4
      0x70020007,  //  0013  JMP		#001C
      0x88180105,  //  0014  GETMBR	R6	R0	K5
      0x4C1C0000,  //  0015  LDNIL	R7
      0x20180C07,  //  0016  NE	R6	R6	R7
      0x781A0002,  //  0017  JMPF	R6	#001B
      0x60180015,  //  0018  GETGBL	R6	G21
      0x7C180000,  //  0019  CALL	R6	0
      0x70020000,  //  001A  JMP		#001C
      0x4C180000,  //  001B  LDNIL	R6
      0x7812004A,  //  001C  JMPF	R4	#0068
      0x881C0106,  //  001D  GETMBR	R7	R0	K6
      0x4C200000,  //  001E  LDNIL	R8
      0x201C0E08,  //  001F  NE	R7	R7	R8
      0x781E0046,  //  0020  JMPF	R7	#0068
      0x601C000F,  //  0021  GETGBL	R7	G15
      0x88200106,  //  0022  GETMBR	R8	R0	K6
      0x60240012,  //  0023  GETGBL	R9	G18
      0x7C1C0400,  //  0024  CALL	R7	2
      0x781E0002,  //  0025  JMPF	R7	#0029
      0x881C0106,  //  0026  GETMBR	R7	R0	K6
      0x941C0F07,  //  0027  GETIDX	R7	R7	K7
      0x70020000,  //  0028  JMP		#002A
      0x881C0106,  //  0029  GETMBR	R7	R0	K6
      0x4C200000,  //  002A  LDNIL	R8
      0x78120026,  //  002B  JMPF	R4	#0053
      0x8C240F08,  //  002C  GETMET	R9	R7	K8
      0x7C240200,  //  002D  CALL	R9	1
      0x5C201200,  //  002E  MOVE	R8	R9
      0x78260022,  //  002F  JMPF	R9	#0053
      0x8C240F09,  //  0030  GETMET	R9	R7	K9
      0x7C240200,  //  0031  CALL	R9	1
      0x74260001,  //  0032  JMPT	R9	#0035
      0x74160000,  //  0033  JMPT	R5	#0035
      0x50240001,  //  0034  LDBOOL	R9	0	1
      0x50240200,  //  0035  LDBOOL	R9	1	0
      0x8828030A,  //  0036  GETMBR	R10	R1	K10
      0x8C28150B,  //  0037  GETMET	R10	R10	K11
      0x8C300F0C,  //  0038  GETMET	R12	R7	K12
      0x7C300200,  //  0039  CALL	R12	1
      0x5C341000,  //  003A  MOVE	R13	R8
      0x8838050D,  //  003B  GETMBR	R14	R2	K13
      0x5C3C1200,  //  003C  MOVE	R15	R9
      0x7C280A00,  //  003D  CALL	R10	5
      0x4C2C0000,  //  003E  LDNIL	R11
      0x1C2C140B,  //  003F  EQ	R11	R10	R11
      0x782E0000,  //  0040  JMPF	R11	#0042
      0x7001FFE8,  //  0041  JMP		#002B
      0x602C000C,  //  0042  GETGBL	R11	G12
      0x5C300600,  //  0043  MOVE	R12	R3
      0x7C2C0200,  //  0044  CALL	R11	1
      0x6030000C,  //  0045  GETGBL	R12	G12
      0x5C341400,  //  0046  MOVE	R13	R10
      0x7C300200,  //  0047  CALL	R12	1
      0x002C160C,  //  0048  ADD	R11	R11	R12
      0x8830010E,  //  0049  GETMBR	R12	R0	K14
      0x242C160C,  //  004A  GT	R11	R11	R12
      0x782E0002,  //  004B  JMPF	R11	#004F
      0x9002020A,  //  004C  SETMBR	R0	K1	R10
      0x50100000,  //  004D  LDBOOL	R4	0	0
      0x70020002,  //  004E  JMP		#0052
      0x8C2C070F,  //  004F  GETMET	R11	R3	K15
      0x5C341400,  //  0050  MOVE	R13	R10
      0x7C2C0400,  //  0051  CALL	R11	2
      0x7001FFD7,  //  0052  JMP		#002B
      0x78120012,  //  0053  JMPF	R4	#0067
      0x6024000F,  //  0054  GETGBL	R9	G15
      0x88280106,  //  0055  GETMBR	R10	R0	K6
      0x602C0012,  //  0056  GETGBL	R11	G18
      0x7C240400,  //  0057  CALL	R9	2
      0x7826000B,  //  0058  JMPF	R9	#0065
      0x88240106,  //  0059  GETMBR	R9	R0	K6
      0x8C241310,  //  005A  GETMET	R9	R9	K16
      0x582C0007,  //  005B  LDCONST	R11	K7
      0x7C240400,  //  005C  CALL	R9	2
      0x6024000C,  //  005D  GETGBL	R9	G12
      0x88280106,  //  005E  GETMBR	R10	R0	K6
      0x7C240200,  //  005F  CALL	R9	1
      0x1C241307,  //  0060  EQ	R9	R9	K7
      0x78260001,  //  0061  JMPF	R9	#0064
      0x4C240000,  //  0062  LDNIL	R9
      0x90020C09,  //  0063  SETMBR	R0	K6	R9
      0x70020001,  //  0064  JMP		#0067
      0x4C240000,  //  0065  LDNIL	R9
      0x90020C09,  //  0066  SETMBR	R0	K6	R9
      0x7001FFB3,  //  0067  JMP		#001C
      0x78120017,  //  0068  JMPF	R4	#0081
      0x881C0104,  //  0069  GETMBR	R7	R0	K4
      0x4C200000,  //  006A  LDNIL	R8
      0x201C0E08,  //  006B  NE	R7	R7	R8
      0x781E0013,  //  006C  JMPF	R7	#0081
      0x601C000C,  //  006D  GETGBL	R7	G12
      0x88200104,  //  006E  GETMBR	R8	R0	K4
      0x7C1C0200,  //  006F  CALL	R7	1
      0x241C0F07,  //  0070  GT	R7	R7	K7
      0x781E000E,  //  0071  JMPF	R7	#0081
      0x601C000C,  //  0072  GETGBL	R7	G12
      0x5C200600,  //  0073  MOVE	R8	R3
      0x7C1C0200,  //  0074  CALL	R7	1
      0x6020000C,  //  0075  GETGBL	R8	G12
      0x88240104,  //  0076  GETMBR	R9	R0	K4
      0x7C200200,  //  0077  CALL	R8	1
      0x001C0E08,  //  0078  ADD	R7	R7	R8
      0x8820010E,  //  0079  GETMBR	R8	R0	K14
      0x241C0E08,  //  007A  GT	R7	R7	R8
      0x781E0002,  //  007B  JMPF	R7	#007F
      0x50100000,  //  007C  LDBOOL	R4	0	0
      0x4C180000,  //  007D  LDNIL	R6
      0x70020001,  //  007E  JMP		#0081
      0x4C1C0000,  //  007F  LDNIL	R7
      0x90020807,  //  0080  SETMBR	R0	K4	R7
      0x7812006D,  //  0081  JMPF	R4	#00F0
      0x881C0105,  //  0082  GETMBR	R7	R0	K5
      0x4C200000,  //  0083  LDNIL	R8
      0x201C0E08,  //  0084  NE	R7	R7	R8
      0x781E0069,  //  0085  JMPF	R7	#00F0
      0x601C000F,  //  0086  GETGBL	R7	G15
      0x88200105,  //  0087  GETMBR	R8	R0	K5
      0x60240012,  //  0088  GETGBL	R9	G18
      0x7C1C0400,  //  0089  CALL	R7	2
      0x781E0002,  //  008A  JMPF	R7	#008E
      0x881C0105,  //  008B  GETMBR	R7	R0	K5
      0x941C0F07,  //  008C  GETIDX	R7	R7	K7
      0x70020000,  //  008D  JMP		#008F
      0x881C0105,  //  008E  GETMBR	R7	R0	K5
      0x4C200000,  //  008F  LDNIL	R8
      0x78120049,  //  0090  JMPF	R4	#00DB
      0x8C240F11,  //  0091  GETMET	R9	R7	K17
      0x7C240200,  //  0092  CALL	R9	1
      0x5C201200,  //  0093  MOVE	R8	R9
      0x78260045,  //  0094  JMPF	R9	#00DB
      0x7816002D,  //  0095  JMPF	R5	#00C4
      0xB8262400,  //  0096  GETNGBL	R9	K18
      0x8C241313,  //  0097  GETMET	R9	R9	K19
      0x582C0014,  //  0098  LDCONST	R11	K20
      0x7C240400,  //  0099  CALL	R9	2
      0x78260028,  //  009A  JMPF	R9	#00C4
      0x58240015,  //  009B  LDCONST	R9	K21
      0x88281116,  //  009C  GETMBR	R10	R8	K22
      0x4C2C0000,  //  009D  LDNIL	R11
      0x2028140B,  //  009E  NE	R10	R10	R11
      0x782A0004,  //  009F  JMPF	R10	#00A5
      0x60280008,  //  00A0  GETGBL	R10	G8
      0x882C1116,  //  00A1  GETMBR	R11	R8	K22
      0x7C280200,  //  00A2  CALL	R10	1
      0x002A2E0A,  //  00A3  ADD	R10	K23	R10
      0x5C241400,  //  00A4  MOVE	R9	R10
      0x88281118,  //  00A5  GETMBR	R10	R8	K24
      0x4C2C0000,  //  00A6  LDNIL	R11
      0x2028140B,  //  00A7  NE	R10	R10	R11
      0x782A0004,  //  00A8  JMPF	R10	#00AE
      0x60280008,  //  00A9  GETGBL	R10	G8
      0x882C1118,  //  00AA  GETMBR	R11	R8	K24
      0x7C280200,  //  00AB  CALL	R10	1
      0x002A320A,  //  00AC  ADD	R10	K25	R10
      0x0024120A,  //  00AD  ADD	R9	R9	R10
      0x8828111A,  //  00AE  GETMBR	R10	R8	K26
      0x4C2C0000,  //  00AF  LDNIL	R11
      0x2028140B,  //  00B0  NE	R10	R10	R11
      0x782A0004,  //  00B1  JMPF	R10	#00B7
      0x60280008,  //  00B2  GETGBL	R10	G8
      0x882C111A,  //  00B3  GETMBR	R11	R8	K26
      0x7C280200,  //  00B4  CALL	R10	1
      0x002A320A,  //  00B5  ADD	R10	K25	R10
      0x0024120A,  //  00B6  ADD	R9	R9	R10
      0xB82A3600,  //  00B7  GETNGBL	R10	K27
      0x602C0018,  //  00B8  GETGBL	R11	G24
      0x5830001C,  //  00B9  LDCONST	R12	K28
      0x8834050D,  //  00BA  GETMBR	R13	R2	K13
      0x88341B1D,  //  00BB  GETMBR	R13	R13	K29
      0x8838111E,  //  00BC  GETMBR	R14	R8	K30
      0x883C111F,  //  00BD  GETMBR	R15	R8	K31
      0x88401120,  //  00BE  GETMBR	R16	R8	K32
      0x88441121,  //  00BF  GETMBR	R17	R8	K33
      0x5C481200,  //  00C0  MOVE	R18	R9
      0x7C2C0E00,  //  00C1  CALL	R11	7
      0x58300014,  //  00C2  LDCONST	R12	K20
      0x7C280400,  //  00C3  CALL	R10	2
      0x8C241122,  //  00C4  GETMET	R9	R8	K34
      0x7C240200,  //  00C5  CALL	R9	1
      0x6028000C,  //  00C6  GETGBL	R10	G12
      0x5C2C0600,  //  00C7  MOVE	R11	R3
      0x7C280200,  //  00C8  CALL	R10	1
      0x602C000C,  //  00C9  GETGBL	R11	G12
      0x5C300C00,  //  00CA  MOVE	R12	R6
      0x7C2C0200,  //  00CB  CALL	R11	1
      0x0028140B,  //  00CC  ADD	R10	R10	R11
      0x602C000C,  //  00CD  GETGBL	R11	G12
      0x5C301200,  //  00CE  MOVE	R12	R9
      0x7C2C0200,  //  00CF  CALL	R11	1
      0x0028140B,  //  00D0  ADD	R10	R10	R11
      0x882C010E,  //  00D1  GETMBR	R11	R0	K14
      0x2428140B,  //  00D2  GT	R10	R10	R11
      0x782A0002,  //  00D3  JMPF	R10	#00D7
      0x90020809,  //  00D4  SETMBR	R0	K4	R9
      0x50100000,  //  00D5  LDBOOL	R4	0	0
      0x70020002,  //  00D6  JMP		#00DA
      0x8C280D0F,  //  00D7  GETMET	R10	R6	K15
      0x5C301200,  //  00D8  MOVE	R12	R9
      0x7C280400,  //  00D9  CALL	R10	2
      0x7001FFB4,  //  00DA  JMP		#0090
      0x78120012,  //  00DB  JMPF	R4	#00EF
      0x6024000F,  //  00DC  GETGBL	R9	G15
      0x88280105,  //  00DD  GETMBR	R10	R0	K5
      0x602C0012,  //  00DE  GETGBL	R11	G18
      0x7C240400,  //  00DF  CALL	R9	2
      0x7826000B,  //  00E0  JMPF	R9	#00ED
      0x88240105,  //  00E1  GETMBR	R9	R0	K5
      0x8C241310,  //  00E2  GETMET	R9	R9	K16
      0x582C0007,  //  00E3  LDCONST	R11	K7
      0x7C240400,  //  00E4  CALL	R9	2
      0x6024000C,  //  00E5  GETGBL	R9	G12
      0x88280105,  //  00E6  GETMBR	R10	R0	K5
      0x7C240200,  //  00E7  CALL	R9	1
      0x1C241307,  //  00E8  EQ	R9	R9	K7
      0x78260001,  //  00E9  JMPF	R9	#00EC
      0x4C240000,  //  00EA  LDNIL	R9
      0x90020A09,  //  00EB  SETMBR	R0	K5	R9
      0x70020001,  //  00EC  JMP		#00EF
      0x4C240000,  //  00ED  LDNIL	R9
      0x90020A09,  //  00EE  SETMBR	R0	K5	R9
      0x7001FF90,  //  00EF  JMP		#0081
      0x881C0101,  //  00F0  GETMBR	R7	R0	K1
      0x4C200000,  //  00F1  LDNIL	R8
      0x201C0E08,  //  00F2  NE	R7	R7	R8
      0x741E0004,  //  00F3  JMPT	R7	#00F9
      0x881C0104,  //  00F4  GETMBR	R7	R0	K4
      0x4C200000,  //  00F5  LDNIL	R8
      0x201C0E08,  //  00F6  NE	R7	R7	R8
      0x741E0000,  //  00F7  JMPT	R7	#00F9
      0x501C0001,  //  00F8  LDBOOL	R7	0	1
      0x501C0200,  //  00F9  LDBOOL	R7	1	0
      0x60200015,  //  00FA  GETGBL	R8	G21
      0x8824010E,  //  00FB  GETMBR	R9	R0	K14
      0x7C200200,  //  00FC  CALL	R8	1
      0x8C241123,  //  00FD  GETMET	R9	R8	K35
      0x542E0014,  //  00FE  LDINT	R11	21
      0x58300024,  //  00FF  LDCONST	R12	K36
      0x7C240600,  //  0100  CALL	R9	3
      0x88240125,  //  0101  GETMBR	R9	R0	K37
      0x4C280000,  //  0102  LDNIL	R10
      0x2024120A,  //  0103  NE	R9	R9	R10
      0x78260007,  //  0104  JMPF	R9	#010D
      0x8C241123,  //  0105  GETMET	R9	R8	K35
      0x542E24FF,  //  0106  LDINT	R11	9472
      0x5431FFFD,  //  0107  LDINT	R12	-2
      0x7C240600,  //  0108  CALL	R9	3
      0x8C241123,  //  0109  GETMET	R9	R8	K35
      0x882C0125,  //  010A  GETMBR	R11	R0	K37
      0x58300026,  //  010B  LDCONST	R12	K38
      0x7C240600,  //  010C  CALL	R9	3
      0x4C240000,  //  010D  LDNIL	R9
      0x20240609,  //  010E  NE	R9	R3	R9
      0x7826000F,  //  010F  JMPF	R9	#0120
      0x6024000C,  //  0110  GETGBL	R9	G12
      0x5C280600,  //  0111  MOVE	R10	R3
      0x7C240200,  //  0112  CALL	R9	1
      0x24241307,  //  0113  GT	R9	R9	K7
      0x7826000A,  //  0114  JMPF	R9	#0120
      0x8C241123,  //  0115  GETMET	R9	R8	K35
      0x542E3600,  //  0116  LDINT	R11	13825
      0x5431FFFD,  //  0117  LDINT	R12	-2
      0x7C240600,  //  0118  CALL	R9	3
      0x8C24110F,  //  0119  GETMET	R9	R8	K15
      0x5C2C0600,  //  011A  MOVE	R11	R3
      0x7C240400,  //  011B  CALL	R9	2
      0x8C241123,  //  011C  GETMET	R9	R8	K35
      0x542E0017,  //  011D  LDINT	R11	24
      0x58300024,  //  011E  LDCONST	R12	K36
      0x7C240600,  //  011F  CALL	R9	3
      0x4C240000,  //  0120  LDNIL	R9
      0x20240C09,  //  0121  NE	R9	R6	R9
      0x7826000F,  //  0122  JMPF	R9	#0133
      0x6024000C,  //  0123  GETGBL	R9	G12
      0x5C280C00,  //  0124  MOVE	R10	R6
      0x7C240200,  //  0125  CALL	R9	1
      0x24241307,  //  0126  GT	R9	R9	K7
      0x7826000A,  //  0127  JMPF	R9	#0133
      0x8C241123,  //  0128  GETMET	R9	R8	K35
      0x542E3601,  //  0129  LDINT	R11	13826
      0x5431FFFD,  //  012A  LDINT	R12	-2
      0x7C240600,  //  012B  CALL	R9	3
      0x8C24110F,  //  012C  GETMET	R9	R8	K15
      0x5C2C0C00,  //  012D  MOVE	R11	R6
      0x7C240400,  //  012E  CALL	R9	2
      0x8C241123,  //  012F  GETMET	R9	R8	K35
      0x542E0017,  //  0130  LDINT	R11	24
      0x58300024,  //  0131  LDCONST	R12	K36
      0x7C240600,  //  0132  CALL	R9	3
      0x781E0004,  //  0133  JMPF	R7	#0139
      0x8C241123,  //  0134  GETMET	R9	R8	K35
      0x542E2902,  //  0135  LDINT	R11	10499
      0x5431FFFD,  //  0136  LDINT	R12	-2
      0x7C240600,  //  0137  CALL	R9	3
      0x70020003,  //  0138  JMP		#013D
      0x8C241123,  //  0139  GETMET	R9	R8	K35
      0x542E2802,  //  013A  LDINT	R11	10243
      0x5431FFFD,  //  013B  LDINT	R12	-2
      0x7C240600,  //  013C  CALL	R9	3
      0x88240127,  //  013D  GETMBR	R9	R0	K39
      0x4C280000,  //  013E  LDNIL	R10
      0x2024120A,  //  013F  NE	R9	R9	R10
      0x7826000A,  //  0140  JMPF	R9	#014C
      0x88240127,  //  0141  GETMBR	R9	R0	K39
      0x78260004,  //  0142  JMPF	R9	#0148
      0x8C241123,  //  0143  GETMET	R9	R8	K35
      0x542E2903,  //  0144  LDINT	R11	10500
      0x5431FFFD,  //  0145  LDINT	R12	-2
      0x7C240600,  //  0146  CALL	R9	3
      0x70020003,  //  0147  JMP		#014C
      0x8C241123,  //  0148  GETMET	R9	R8	K35
      0x542E2803,  //  0149  LDINT	R11	10244
      0x5431FFFD,  //  014A  LDINT	R12	-2
      0x7C240600,  //  014B  CALL	R9	3
      0x8C241123,  //  014C  GETMET	R9	R8	K35
      0x542E24FE,  //  014D  LDINT	R11	9471
      0x5431FFFD,  //  014E  LDINT	R12	-2
      0x7C240600,  //  014F  CALL	R9	3
      0x8C241123,  //  0150  GETMET	R9	R8	K35
      0x582C0024,  //  0151  LDCONST	R11	K36
      0x58300024,  //  0152  LDCONST	R12	K36
      0x7C240600,  //  0153  CALL	R9	3
      0x8C241123,  //  0154  GETMET	R9	R8	K35
      0x542E0017,  //  0155  LDINT	R11	24
      0x58300024,  //  0156  LDCONST	R12	K36
      0x7C240600,  //  0157  CALL	R9	3
      0x8C240528,  //  0158  GETMET	R9	R2	K40
      0x5C2C1000,  //  0159  MOVE	R11	R8
      0x7C240400,  //  015A  CALL	R9	2
      0x8C240529,  //  015B  GETMET	R9	R2	K41
      0x7C240200,  //  015C  CALL	R9	1
      0x8C24032A,  //  015D  GETMET	R9	R1	K42
      0x5C2C0400,  //  015E  MOVE	R11	R2
      0x7C240400,  //  015F  CALL	R9	2
      0x8824052C,  //  0160  GETMBR	R9	R2	K44
      0x90025609,  //  0161  SETMBR	R0	K43	R9
      0x781E0002,  //  0162  JMPF	R7	#0166
      0x50240000,  //  0163  LDBOOL	R9	0	0
      0x90025A09,  //  0164  SETMBR	R0	K45	R9
      0x70020001,  //  0165  JMP		#0168
      0x50240200,  //  0166  LDBOOL	R9	1	0
      0x90025C09,  //  0167  SETMBR	R0	K46	R9
      0x80000000,  //  0168  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: status_ok_received
********************************************************************/
be_local_closure(class_Matter_IM_ReportData_Pull_status_ok_received,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_ReportData_Pull,     /* shared constants */
    be_str_weak(status_ok_received),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08052F,  //  0003  GETMET	R2	R2	K47
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x8808012E,  //  0006  GETMBR	R2	R0	K46
      0x740A0005,  //  0007  JMPT	R2	#000E
      0x8808030D,  //  0008  GETMBR	R2	R1	K13
      0x88080530,  //  0009  GETMBR	R2	R2	K48
      0x8C080531,  //  000A  GETMET	R2	R2	K49
      0x5C100200,  //  000B  MOVE	R4	R1
      0x50140000,  //  000C  LDBOOL	R5	0	0
      0x7C080600,  //  000D  CALL	R2	3
      0x50080200,  //  000E  LDBOOL	R2	1	0
      0x80040400,  //  000F  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_subscription_id
********************************************************************/
be_local_closure(class_Matter_IM_ReportData_Pull_set_subscription_id,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_ReportData_Pull,     /* shared constants */
    be_str_weak(set_subscription_id),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90024A01,  //  0000  SETMBR	R0	K37	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_suppress_response
********************************************************************/
be_local_closure(class_Matter_IM_ReportData_Pull_set_suppress_response,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_ReportData_Pull,     /* shared constants */
    be_str_weak(set_suppress_response),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90024E01,  //  0000  SETMBR	R0	K39	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_IM_ReportData_Pull_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_ReportData_Pull,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100932,  //  0003  GETMET	R4	R4	K50
      0x5C180200,  //  0004  MOVE	R6	R1
      0x541E0004,  //  0005  LDINT	R7	5
      0x50200200,  //  0006  LDBOOL	R8	1	0
      0x7C100800,  //  0007  CALL	R4	4
      0x90020C02,  //  0008  SETMBR	R0	K6	R2
      0x90020A03,  //  0009  SETMBR	R0	K5	R3
      0x80000000,  //  000A  RET	0
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
    be_nested_map(11,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(send_im, -1), be_const_closure(class_Matter_IM_ReportData_Pull_send_im_closure) },
        { be_const_key_weak(status_ok_received, -1), be_const_closure(class_Matter_IM_ReportData_Pull_status_ok_received_closure) },
        { be_const_key_weak(data_ev, -1), be_const_var(4) },
        { be_const_key_weak(suppress_response, 8), be_const_var(3) },
        { be_const_key_weak(MAX_MESSAGE, -1), be_const_int(1200) },
        { be_const_key_weak(set_suppress_response, -1), be_const_closure(class_Matter_IM_ReportData_Pull_set_suppress_response_closure) },
        { be_const_key_weak(set_subscription_id, -1), be_const_closure(class_Matter_IM_ReportData_Pull_set_subscription_id_closure) },
        { be_const_key_weak(generator_or_arr, 5), be_const_var(0) },
        { be_const_key_weak(event_generator_or_arr, 4), be_const_var(1) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_IM_ReportData_Pull_init_closure) },
        { be_const_key_weak(subscription_id, -1), be_const_var(2) },
    })),
    be_str_weak(Matter_IM_ReportData_Pull)
);
// compact class 'Matter_IM_ReportDataSubscribed_Pull' ktab size: 34, total: 46 (saved 96 bytes)
static const bvalue be_ktab_class_Matter_IM_ReportDataSubscribed_Pull[34] = {
  /* K0   */  be_nested_str_weak(ack_received),
  /* K1   */  be_nested_str_weak(report_data_phase),
  /* K2   */  be_nested_str_weak(sub),
  /* K3   */  be_nested_str_weak(is_keep_alive),
  /* K4   */  be_nested_str_weak(re_arm),
  /* K5   */  be_nested_str_weak(ready),
  /* K6   */  be_nested_str_weak(generator_or_arr),
  /* K7   */  be_nested_str_weak(event_generator_or_arr),
  /* K8   */  be_nested_str_weak(send_im),
  /* K9   */  be_nested_str_weak(finishing),
  /* K10  */  be_nested_str_weak(finished),
  /* K11  */  be_nested_str_weak(resp),
  /* K12  */  be_nested_str_weak(build_standalone_ack),
  /* K13  */  be_nested_str_weak(encode_frame),
  /* K14  */  be_nested_str_weak(encrypt),
  /* K15  */  be_nested_str_weak(tasmota),
  /* K16  */  be_nested_str_weak(loglevel),
  /* K17  */  be_nested_str_weak(log),
  /* K18  */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i),
  /* K19  */  be_nested_str_weak(session),
  /* K20  */  be_nested_str_weak(local_session_id),
  /* K21  */  be_nested_str_weak(ack_message_counter),
  /* K22  */  be_nested_str_weak(message_counter),
  /* K23  */  be_nested_str_weak(send_response_frame),
  /* K24  */  be_nested_str_weak(last_counter),
  /* K25  */  be_nested_str_weak(init),
  /* K26  */  be_nested_str_weak(matter),
  /* K27  */  be_nested_str_weak(Frame),
  /* K28  */  be_nested_str_weak(initiate_response),
  /* K29  */  be_nested_str_weak(set_subscription_id),
  /* K30  */  be_nested_str_weak(subscription_id),
  /* K31  */  be_nested_str_weak(set_suppress_response),
  /* K32  */  be_nested_str_weak(remove_self),
  /* K33  */  be_nested_str_weak(status_ok_received),
};


extern const bclass be_class_Matter_IM_ReportDataSubscribed_Pull;

/********************************************************************
** Solidified function: ack_received
********************************************************************/
be_local_closure(class_Matter_IM_ReportDataSubscribed_Pull_ack_received,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_ReportDataSubscribed_Pull,     /* shared constants */
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
      0x50080000,  //  000E  LDBOOL	R2	0	0
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
be_local_closure(class_Matter_IM_ReportDataSubscribed_Pull_send_im,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_ReportDataSubscribed_Pull,     /* shared constants */
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[77]) {  /* code */
      0x88080105,  //  0000  GETMBR	R2	R0	K5
      0x740A0001,  //  0001  JMPT	R2	#0004
      0x50080000,  //  0002  LDBOOL	R2	0	0
      0x80040400,  //  0003  RET	1	R2
      0x88080106,  //  0004  GETMBR	R2	R0	K6
      0x4C0C0000,  //  0005  LDNIL	R3
      0x20080403,  //  0006  NE	R2	R2	R3
      0x740A0003,  //  0007  JMPT	R2	#000C
      0x88080107,  //  0008  GETMBR	R2	R0	K7
      0x4C0C0000,  //  0009  LDNIL	R3
      0x20080403,  //  000A  NE	R2	R2	R3
      0x780A0031,  //  000B  JMPF	R2	#003E
      0x88080101,  //  000C  GETMBR	R2	R0	K1
      0x780A000F,  //  000D  JMPF	R2	#001E
      0x60080003,  //  000E  GETGBL	R2	G3
      0x5C0C0000,  //  000F  MOVE	R3	R0
      0x7C080200,  //  0010  CALL	R2	1
      0x8C080508,  //  0011  GETMET	R2	R2	K8
      0x5C100200,  //  0012  MOVE	R4	R1
      0x7C080400,  //  0013  CALL	R2	2
      0x88080109,  //  0014  GETMBR	R2	R0	K9
      0x740A0000,  //  0015  JMPT	R2	#0017
      0x80000400,  //  0016  RET	0
      0x50080000,  //  0017  LDBOOL	R2	0	0
      0x90020202,  //  0018  SETMBR	R0	K1	R2
      0x50080000,  //  0019  LDBOOL	R2	0	0
      0x90020A02,  //  001A  SETMBR	R0	K5	R2
      0x50080000,  //  001B  LDBOOL	R2	0	0
      0x90021402,  //  001C  SETMBR	R0	K10	R2
      0x7002001E,  //  001D  JMP		#003D
      0x8808010B,  //  001E  GETMBR	R2	R0	K11
      0x8C08050C,  //  001F  GETMET	R2	R2	K12
      0x50100000,  //  0020  LDBOOL	R4	0	0
      0x7C080400,  //  0021  CALL	R2	2
      0x8C0C050D,  //  0022  GETMET	R3	R2	K13
      0x7C0C0200,  //  0023  CALL	R3	1
      0x8C0C050E,  //  0024  GETMET	R3	R2	K14
      0x7C0C0200,  //  0025  CALL	R3	1
      0xB80E1E00,  //  0026  GETNGBL	R3	K15
      0x8C0C0710,  //  0027  GETMET	R3	R3	K16
      0x54160003,  //  0028  LDINT	R5	4
      0x7C0C0400,  //  0029  CALL	R3	2
      0x780E0009,  //  002A  JMPF	R3	#0035
      0xB80E2200,  //  002B  GETNGBL	R3	K17
      0x60100018,  //  002C  GETGBL	R4	G24
      0x58140012,  //  002D  LDCONST	R5	K18
      0x88180513,  //  002E  GETMBR	R6	R2	K19
      0x88180D14,  //  002F  GETMBR	R6	R6	K20
      0x881C0515,  //  0030  GETMBR	R7	R2	K21
      0x88200516,  //  0031  GETMBR	R8	R2	K22
      0x7C100800,  //  0032  CALL	R4	4
      0x54160003,  //  0033  LDINT	R5	4
      0x7C0C0400,  //  0034  CALL	R3	2
      0x8C0C0317,  //  0035  GETMET	R3	R1	K23
      0x5C140400,  //  0036  MOVE	R5	R2
      0x7C0C0400,  //  0037  CALL	R3	2
      0x880C0516,  //  0038  GETMBR	R3	R2	K22
      0x90023003,  //  0039  SETMBR	R0	K24	R3
      0x880C0102,  //  003A  GETMBR	R3	R0	K2
      0x8C0C0704,  //  003B  GETMET	R3	R3	K4
      0x7C0C0200,  //  003C  CALL	R3	1
      0x7002000D,  //  003D  JMP		#004C
      0x88080101,  //  003E  GETMBR	R2	R0	K1
      0x780A0008,  //  003F  JMPF	R2	#0049
      0x60080003,  //  0040  GETGBL	R2	G3
      0x5C0C0000,  //  0041  MOVE	R3	R0
      0x7C080200,  //  0042  CALL	R2	1
      0x8C080508,  //  0043  GETMET	R2	R2	K8
      0x5C100200,  //  0044  MOVE	R4	R1
      0x7C080400,  //  0045  CALL	R2	2
      0x50080000,  //  0046  LDBOOL	R2	0	0
      0x90020202,  //  0047  SETMBR	R0	K1	R2
      0x70020002,  //  0048  JMP		#004C
      0x88080102,  //  0049  GETMBR	R2	R0	K2
      0x8C080504,  //  004A  GETMET	R2	R2	K4
      0x7C080200,  //  004B  CALL	R2	1
      0x80000000,  //  004C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_IM_ReportDataSubscribed_Pull_init,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    6,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_ReportDataSubscribed_Pull,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x60180003,  //  0000  GETGBL	R6	G3
      0x5C1C0000,  //  0001  MOVE	R7	R0
      0x7C180200,  //  0002  CALL	R6	1
      0x8C180D19,  //  0003  GETMET	R6	R6	K25
      0x4C200000,  //  0004  LDNIL	R8
      0x5C240600,  //  0005  MOVE	R9	R3
      0x5C280800,  //  0006  MOVE	R10	R4
      0x7C180800,  //  0007  CALL	R6	4
      0xB81A3400,  //  0008  GETNGBL	R6	K26
      0x88180D1B,  //  0009  GETMBR	R6	R6	K27
      0x8C180D1C,  //  000A  GETMET	R6	R6	K28
      0x5C200200,  //  000B  MOVE	R8	R1
      0x5C240400,  //  000C  MOVE	R9	R2
      0x542A0004,  //  000D  LDINT	R10	5
      0x502C0200,  //  000E  LDBOOL	R11	1	0
      0x7C180A00,  //  000F  CALL	R6	5
      0x90021606,  //  0010  SETMBR	R0	K11	R6
      0x90020405,  //  0011  SETMBR	R0	K2	R5
      0x50180200,  //  0012  LDBOOL	R6	1	0
      0x90020206,  //  0013  SETMBR	R0	K1	R6
      0x8C18011D,  //  0014  GETMET	R6	R0	K29
      0x88200B1E,  //  0015  GETMBR	R8	R5	K30
      0x7C180400,  //  0016  CALL	R6	2
      0x8C18011F,  //  0017  GETMET	R6	R0	K31
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
be_local_closure(class_Matter_IM_ReportDataSubscribed_Pull_status_error_received,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_ReportDataSubscribed_Pull,     /* shared constants */
    be_str_weak(status_error_received),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88080102,  //  0000  GETMBR	R2	R0	K2
      0x8C080520,  //  0001  GETMET	R2	R2	K32
      0x7C080200,  //  0002  CALL	R2	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reached_timeout
********************************************************************/
be_local_closure(class_Matter_IM_ReportDataSubscribed_Pull_reached_timeout,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_ReportDataSubscribed_Pull,     /* shared constants */
    be_str_weak(reached_timeout),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040102,  //  0000  GETMBR	R1	R0	K2
      0x8C040320,  //  0001  GETMET	R1	R1	K32
      0x7C040200,  //  0002  CALL	R1	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: status_ok_received
********************************************************************/
be_local_closure(class_Matter_IM_ReportDataSubscribed_Pull_status_ok_received,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_ReportDataSubscribed_Pull,     /* shared constants */
    be_str_weak(status_ok_received),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x88080101,  //  0000  GETMBR	R2	R0	K1
      0x780A0007,  //  0001  JMPF	R2	#000A
      0x60080003,  //  0002  GETGBL	R2	G3
      0x5C0C0000,  //  0003  MOVE	R3	R0
      0x7C080200,  //  0004  CALL	R2	1
      0x8C080521,  //  0005  GETMET	R2	R2	K33
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x80040400,  //  0008  RET	1	R2
      0x7002000A,  //  0009  JMP		#0015
      0x88080102,  //  000A  GETMBR	R2	R0	K2
      0x8C080504,  //  000B  GETMET	R2	R2	K4
      0x7C080200,  //  000C  CALL	R2	1
      0x60080003,  //  000D  GETGBL	R2	G3
      0x5C0C0000,  //  000E  MOVE	R3	R0
      0x7C080200,  //  000F  CALL	R2	1
      0x8C080521,  //  0010  GETMET	R2	R2	K33
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
// compact class 'Matter_IM_SubscribedHeartbeat' ktab size: 13, total: 16 (saved 24 bytes)
static const bvalue be_ktab_class_Matter_IM_SubscribedHeartbeat[13] = {
  /* K0   */  be_nested_str_weak(sub),
  /* K1   */  be_nested_str_weak(remove_self),
  /* K2   */  be_nested_str_weak(send_im),
  /* K3   */  be_nested_str_weak(ready),
  /* K4   */  be_nested_str_weak(ack_received),
  /* K5   */  be_nested_str_weak(init),
  /* K6   */  be_nested_str_weak(resp),
  /* K7   */  be_nested_str_weak(matter),
  /* K8   */  be_nested_str_weak(Frame),
  /* K9   */  be_nested_str_weak(initiate_response),
  /* K10  */  be_nested_str_weak(set_subscription_id),
  /* K11  */  be_nested_str_weak(subscription_id),
  /* K12  */  be_nested_str_weak(set_suppress_response),
};


extern const bclass be_class_Matter_IM_SubscribedHeartbeat;

/********************************************************************
** Solidified function: status_error_received
********************************************************************/
be_local_closure(class_Matter_IM_SubscribedHeartbeat_status_error_received,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_SubscribedHeartbeat,     /* shared constants */
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
be_local_closure(class_Matter_IM_SubscribedHeartbeat_send_im,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_SubscribedHeartbeat,     /* shared constants */
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080502,  //  0003  GETMET	R2	R2	K2
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x50080000,  //  0006  LDBOOL	R2	0	0
      0x90020602,  //  0007  SETMBR	R0	K3	R2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: ack_received
********************************************************************/
be_local_closure(class_Matter_IM_SubscribedHeartbeat_ack_received,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_SubscribedHeartbeat,     /* shared constants */
    be_str_weak(ack_received),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080504,  //  0003  GETMET	R2	R2	K4
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x50080000,  //  0006  LDBOOL	R2	0	0
      0x80040400,  //  0007  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reached_timeout
********************************************************************/
be_local_closure(class_Matter_IM_SubscribedHeartbeat_reached_timeout,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_SubscribedHeartbeat,     /* shared constants */
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
be_local_closure(class_Matter_IM_SubscribedHeartbeat_status_ok_received,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_SubscribedHeartbeat,     /* shared constants */
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
be_local_closure(class_Matter_IM_SubscribedHeartbeat_init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_SubscribedHeartbeat,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100905,  //  0003  GETMET	R4	R4	K5
      0x4C180000,  //  0004  LDNIL	R6
      0x4C1C0000,  //  0005  LDNIL	R7
      0x4C200000,  //  0006  LDNIL	R8
      0x7C100800,  //  0007  CALL	R4	4
      0xB8120E00,  //  0008  GETNGBL	R4	K7
      0x88100908,  //  0009  GETMBR	R4	R4	K8
      0x8C100909,  //  000A  GETMET	R4	R4	K9
      0x5C180200,  //  000B  MOVE	R6	R1
      0x5C1C0400,  //  000C  MOVE	R7	R2
      0x54220004,  //  000D  LDINT	R8	5
      0x50240200,  //  000E  LDBOOL	R9	1	0
      0x7C100A00,  //  000F  CALL	R4	5
      0x90020C04,  //  0010  SETMBR	R0	K6	R4
      0x90020003,  //  0011  SETMBR	R0	K0	R3
      0x8C10010A,  //  0012  GETMET	R4	R0	K10
      0x8818070B,  //  0013  GETMBR	R6	R3	K11
      0x7C100400,  //  0014  CALL	R4	2
      0x8C10010C,  //  0015  GETMET	R4	R0	K12
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
// compact class 'Matter_IM_SubscribeResponse_Pull' ktab size: 29, total: 36 (saved 56 bytes)
static const bvalue be_ktab_class_Matter_IM_SubscribeResponse_Pull[29] = {
  /* K0   */  be_nested_str_weak(init),
  /* K1   */  be_nested_str_weak(sub),
  /* K2   */  be_nested_str_weak(report_data_phase),
  /* K3   */  be_nested_str_weak(set_subscription_id),
  /* K4   */  be_nested_str_weak(subscription_id),
  /* K5   */  be_nested_str_weak(tasmota),
  /* K6   */  be_nested_str_weak(loglevel),
  /* K7   */  be_const_int(3),
  /* K8   */  be_nested_str_weak(log),
  /* K9   */  be_nested_str_weak(MTR_X3A_X20_X3ESub_OK_X20_X20_X20_X20_X28_X256i_X29_X20sub_X3D_X25i),
  /* K10  */  be_nested_str_weak(session),
  /* K11  */  be_nested_str_weak(local_session_id),
  /* K12  */  be_nested_str_weak(status_ok_received),
  /* K13  */  be_nested_str_weak(finishing),
  /* K14  */  be_nested_str_weak(send_im),
  /* K15  */  be_nested_str_weak(ready),
  /* K16  */  be_nested_str_weak(resp),
  /* K17  */  be_nested_str_weak(matter),
  /* K18  */  be_nested_str_weak(SubscribeResponseMessage),
  /* K19  */  be_nested_str_weak(max_interval),
  /* K20  */  be_nested_str_weak(opcode),
  /* K21  */  be_nested_str_weak(encode_frame),
  /* K22  */  be_nested_str_weak(to_TLV),
  /* K23  */  be_nested_str_weak(tlv2raw),
  /* K24  */  be_nested_str_weak(encrypt),
  /* K25  */  be_nested_str_weak(send_response_frame),
  /* K26  */  be_nested_str_weak(last_counter),
  /* K27  */  be_nested_str_weak(message_counter),
  /* K28  */  be_nested_str_weak(re_arm),
};


extern const bclass be_class_Matter_IM_SubscribeResponse_Pull;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_IM_SubscribeResponse_Pull_init,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_SubscribeResponse_Pull,     /* shared constants */
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
be_local_closure(class_Matter_IM_SubscribeResponse_Pull_status_ok_received,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_SubscribeResponse_Pull,     /* shared constants */
    be_str_weak(status_ok_received),
    &be_const_str_solidified,
    ( &(const binstruction[26]) {  /* code */
      0xB80A0A00,  //  0000  GETNGBL	R2	K5
      0x8C080506,  //  0001  GETMET	R2	R2	K6
      0x58100007,  //  0002  LDCONST	R4	K7
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0009,  //  0004  JMPF	R2	#000F
      0xB80A1000,  //  0005  GETNGBL	R2	K8
      0x600C0018,  //  0006  GETGBL	R3	G24
      0x58100009,  //  0007  LDCONST	R4	K9
      0x8814030A,  //  0008  GETMBR	R5	R1	K10
      0x88140B0B,  //  0009  GETMBR	R5	R5	K11
      0x88180101,  //  000A  GETMBR	R6	R0	K1
      0x88180D04,  //  000B  GETMBR	R6	R6	K4
      0x7C0C0600,  //  000C  CALL	R3	3
      0x58100007,  //  000D  LDCONST	R4	K7
      0x7C080400,  //  000E  CALL	R2	2
      0x60080003,  //  000F  GETGBL	R2	G3
      0x5C0C0000,  //  0010  MOVE	R3	R0
      0x7C080200,  //  0011  CALL	R2	1
      0x8C08050C,  //  0012  GETMET	R2	R2	K12
      0x5C100200,  //  0013  MOVE	R4	R1
      0x7C080400,  //  0014  CALL	R2	2
      0x80040400,  //  0015  RET	1	R2
      0x88080102,  //  0016  GETMBR	R2	R0	K2
      0x740A0001,  //  0017  JMPT	R2	#001A
      0x50080200,  //  0018  LDBOOL	R2	1	0
      0x90021A02,  //  0019  SETMBR	R0	K13	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_im
********************************************************************/
be_local_closure(class_Matter_IM_SubscribeResponse_Pull_send_im,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_IM_SubscribeResponse_Pull,     /* shared constants */
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[49]) {  /* code */
      0x88080102,  //  0000  GETMBR	R2	R0	K2
      0x780A000E,  //  0001  JMPF	R2	#0011
      0x60080003,  //  0002  GETGBL	R2	G3
      0x5C0C0000,  //  0003  MOVE	R3	R0
      0x7C080200,  //  0004  CALL	R2	1
      0x8C08050E,  //  0005  GETMET	R2	R2	K14
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x8808010D,  //  0008  GETMBR	R2	R0	K13
      0x780A0003,  //  0009  JMPF	R2	#000E
      0x50080000,  //  000A  LDBOOL	R2	0	0
      0x90020402,  //  000B  SETMBR	R0	K2	R2
      0x50080000,  //  000C  LDBOOL	R2	0	0
      0x90021A02,  //  000D  SETMBR	R0	K13	R2
      0x50080000,  //  000E  LDBOOL	R2	0	0
      0x90021E02,  //  000F  SETMBR	R0	K15	R2
      0x7002001E,  //  0010  JMP		#0030
      0x88080110,  //  0011  GETMBR	R2	R0	K16
      0xB80E2200,  //  0012  GETNGBL	R3	K17
      0x8C0C0712,  //  0013  GETMET	R3	R3	K18
      0x7C0C0200,  //  0014  CALL	R3	1
      0x88100101,  //  0015  GETMBR	R4	R0	K1
      0x88100904,  //  0016  GETMBR	R4	R4	K4
      0x900E0804,  //  0017  SETMBR	R3	K4	R4
      0x88100101,  //  0018  GETMBR	R4	R0	K1
      0x88100913,  //  0019  GETMBR	R4	R4	K19
      0x900E2604,  //  001A  SETMBR	R3	K19	R4
      0x88100110,  //  001B  GETMBR	R4	R0	K16
      0x54160003,  //  001C  LDINT	R5	4
      0x90122805,  //  001D  SETMBR	R4	K20	R5
      0x8C100515,  //  001E  GETMET	R4	R2	K21
      0x8C180716,  //  001F  GETMET	R6	R3	K22
      0x7C180200,  //  0020  CALL	R6	1
      0x8C180D17,  //  0021  GETMET	R6	R6	K23
      0x7C180200,  //  0022  CALL	R6	1
      0x7C100400,  //  0023  CALL	R4	2
      0x8C100518,  //  0024  GETMET	R4	R2	K24
      0x7C100200,  //  0025  CALL	R4	1
      0x8C100319,  //  0026  GETMET	R4	R1	K25
      0x5C180400,  //  0027  MOVE	R6	R2
      0x7C100400,  //  0028  CALL	R4	2
      0x8810051B,  //  0029  GETMBR	R4	R2	K27
      0x90023404,  //  002A  SETMBR	R0	K26	R4
      0x88100101,  //  002B  GETMBR	R4	R0	K1
      0x8C10091C,  //  002C  GETMET	R4	R4	K28
      0x7C100200,  //  002D  CALL	R4	1
      0x50100200,  //  002E  LDBOOL	R4	1	0
      0x90021A04,  //  002F  SETMBR	R0	K13	R4
      0x80000000,  //  0030  RET	0
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
