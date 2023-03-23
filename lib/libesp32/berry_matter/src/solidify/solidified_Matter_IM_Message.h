/* Solidification of Matter_IM_Message.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Path;

/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(Matter_Path_tostring,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(endpoint),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(_X5B_X2502X_X5D),
    /* K5   */  be_nested_str_weak(_X5B_X2A_X2A_X5D),
    /* K6   */  be_nested_str_weak(cluster),
    /* K7   */  be_nested_str_weak(_X2504X_X2F),
    /* K8   */  be_nested_str_weak(_X2A_X2A_X2A_X2A_X2F),
    /* K9   */  be_nested_str_weak(attribute),
    /* K10  */  be_nested_str_weak(_X2504X),
    /* K11  */  be_nested_str_weak(command),
    /* K12  */  be_nested_str_weak(_X2A_X2A_X2A_X2A),
    /* K13  */  be_nested_str_weak(Exception_X3E_X20),
    /* K14  */  be_nested_str_weak(_X2C_X20),
    }),
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[75]) {  /* code */
      0xA802003A,  //  0000  EXBLK	0	#003C
      0xA4060000,  //  0001  IMPORT	R1	K0
      0x58080001,  //  0002  LDCONST	R2	K1
      0x880C0102,  //  0003  GETMBR	R3	R0	K2
      0x4C100000,  //  0004  LDNIL	R4
      0x200C0604,  //  0005  NE	R3	R3	R4
      0x780E0004,  //  0006  JMPF	R3	#000C
      0x8C0C0303,  //  0007  GETMET	R3	R1	K3
      0x58140004,  //  0008  LDCONST	R5	K4
      0x88180102,  //  0009  GETMBR	R6	R0	K2
      0x7C0C0600,  //  000A  CALL	R3	3
      0x70020000,  //  000B  JMP		#000D
      0x580C0005,  //  000C  LDCONST	R3	K5
      0x00080403,  //  000D  ADD	R2	R2	R3
      0x880C0106,  //  000E  GETMBR	R3	R0	K6
      0x4C100000,  //  000F  LDNIL	R4
      0x200C0604,  //  0010  NE	R3	R3	R4
      0x780E0004,  //  0011  JMPF	R3	#0017
      0x8C0C0303,  //  0012  GETMET	R3	R1	K3
      0x58140007,  //  0013  LDCONST	R5	K7
      0x88180106,  //  0014  GETMBR	R6	R0	K6
      0x7C0C0600,  //  0015  CALL	R3	3
      0x70020000,  //  0016  JMP		#0018
      0x580C0008,  //  0017  LDCONST	R3	K8
      0x00080403,  //  0018  ADD	R2	R2	R3
      0x880C0109,  //  0019  GETMBR	R3	R0	K9
      0x4C100000,  //  001A  LDNIL	R4
      0x200C0604,  //  001B  NE	R3	R3	R4
      0x780E0004,  //  001C  JMPF	R3	#0022
      0x8C0C0303,  //  001D  GETMET	R3	R1	K3
      0x5814000A,  //  001E  LDCONST	R5	K10
      0x88180109,  //  001F  GETMBR	R6	R0	K9
      0x7C0C0600,  //  0020  CALL	R3	3
      0x70020000,  //  0021  JMP		#0023
      0x580C0001,  //  0022  LDCONST	R3	K1
      0x00080403,  //  0023  ADD	R2	R2	R3
      0x880C010B,  //  0024  GETMBR	R3	R0	K11
      0x4C100000,  //  0025  LDNIL	R4
      0x200C0604,  //  0026  NE	R3	R3	R4
      0x780E0004,  //  0027  JMPF	R3	#002D
      0x8C0C0303,  //  0028  GETMET	R3	R1	K3
      0x5814000A,  //  0029  LDCONST	R5	K10
      0x8818010B,  //  002A  GETMBR	R6	R0	K11
      0x7C0C0600,  //  002B  CALL	R3	3
      0x70020000,  //  002C  JMP		#002E
      0x580C0001,  //  002D  LDCONST	R3	K1
      0x00080403,  //  002E  ADD	R2	R2	R3
      0x880C0109,  //  002F  GETMBR	R3	R0	K9
      0x4C100000,  //  0030  LDNIL	R4
      0x1C0C0604,  //  0031  EQ	R3	R3	R4
      0x780E0004,  //  0032  JMPF	R3	#0038
      0x880C010B,  //  0033  GETMBR	R3	R0	K11
      0x4C100000,  //  0034  LDNIL	R4
      0x1C0C0604,  //  0035  EQ	R3	R3	R4
      0x780E0000,  //  0036  JMPF	R3	#0038
      0x0008050C,  //  0037  ADD	R2	R2	K12
      0xA8040001,  //  0038  EXBLK	1	1
      0x80040400,  //  0039  RET	1	R2
      0xA8040001,  //  003A  EXBLK	1	1
      0x7002000D,  //  003B  JMP		#004A
      0xAC040002,  //  003C  CATCH	R1	0	2
      0x7002000A,  //  003D  JMP		#0049
      0x600C0008,  //  003E  GETGBL	R3	G8
      0x5C100200,  //  003F  MOVE	R4	R1
      0x7C0C0200,  //  0040  CALL	R3	1
      0x000E1A03,  //  0041  ADD	R3	K13	R3
      0x000C070E,  //  0042  ADD	R3	R3	K14
      0x60100008,  //  0043  GETGBL	R4	G8
      0x5C140400,  //  0044  MOVE	R5	R2
      0x7C100200,  //  0045  CALL	R4	1
      0x000C0604,  //  0046  ADD	R3	R3	R4
      0x80040600,  //  0047  RET	1	R3
      0x70020000,  //  0048  JMP		#004A
      0xB0080000,  //  0049  RAISE	2	R0	R0
      0x80000000,  //  004A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Path
********************************************************************/
be_local_class(Matter_Path,
    5,
    NULL,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(tostring, -1), be_const_closure(Matter_Path_tostring_closure) },
        { be_const_key_weak(cluster, 3), be_const_var(1) },
        { be_const_key_weak(command, -1), be_const_var(3) },
        { be_const_key_weak(status, 0), be_const_var(4) },
        { be_const_key_weak(endpoint, -1), be_const_var(0) },
        { be_const_key_weak(attribute, -1), be_const_var(2) },
    })),
    be_str_weak(Matter_Path)
);
/*******************************************************************/

void be_load_Matter_Path_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Path);
    be_setglobal(vm, "Matter_Path");
    be_pop(vm, 1);
}

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
** Solidified function: send
********************************************************************/
be_local_closure(Matter_IM_Message_send,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(resp),
    /* K1   */  be_nested_str_weak(encode_frame),
    /* K2   */  be_nested_str_weak(data),
    /* K3   */  be_nested_str_weak(to_TLV),
    /* K4   */  be_nested_str_weak(encode),
    /* K5   */  be_nested_str_weak(encrypt),
    /* K6   */  be_nested_str_weak(send_response),
    /* K7   */  be_nested_str_weak(raw),
    /* K8   */  be_nested_str_weak(remote_ip),
    /* K9   */  be_nested_str_weak(remote_port),
    /* K10  */  be_nested_str_weak(message_counter),
    }),
    be_str_weak(send),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x88140102,  //  0002  GETMBR	R5	R0	K2
      0x8C140B03,  //  0003  GETMET	R5	R5	K3
      0x7C140200,  //  0004  CALL	R5	1
      0x8C140B04,  //  0005  GETMET	R5	R5	K4
      0x7C140200,  //  0006  CALL	R5	1
      0x7C0C0400,  //  0007  CALL	R3	2
      0x8C0C0505,  //  0008  GETMET	R3	R2	K5
      0x7C0C0200,  //  0009  CALL	R3	1
      0x8C0C0306,  //  000A  GETMET	R3	R1	K6
      0x88140507,  //  000B  GETMBR	R5	R2	K7
      0x88180508,  //  000C  GETMBR	R6	R2	K8
      0x881C0509,  //  000D  GETMBR	R7	R2	K9
      0x8820050A,  //  000E  GETMBR	R8	R2	K10
      0x7C0C0A00,  //  000F  CALL	R3	5
      0x500C0200,  //  0010  LDBOOL	R3	1	0
      0x80040600,  //  0011  RET	1	R3
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
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(resp),
    /* K1   */  be_nested_str_weak(build_response),
    /* K2   */  be_nested_str_weak(ready),
    /* K3   */  be_nested_str_weak(expiration),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(millis),
    /* K6   */  be_nested_str_weak(MSG_TIMEOUT),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
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
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM_Message
********************************************************************/
be_local_class(Matter_IM_Message,
    4,
    NULL,
    be_nested_map(12,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(expiration, 9), be_const_var(0) },
        { be_const_key_weak(data, -1), be_const_var(3) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_IM_Message_init_closure) },
        { be_const_key_weak(resp, 2), be_const_var(1) },
        { be_const_key_weak(status_error_received, 6), be_const_closure(Matter_IM_Message_status_error_received_closure) },
        { be_const_key_weak(reached_timeout, -1), be_const_closure(Matter_IM_Message_reached_timeout_closure) },
        { be_const_key_weak(get_exchangeid, -1), be_const_closure(Matter_IM_Message_get_exchangeid_closure) },
        { be_const_key_weak(send, -1), be_const_closure(Matter_IM_Message_send_closure) },
        { be_const_key_weak(ack_received, -1), be_const_closure(Matter_IM_Message_ack_received_closure) },
        { be_const_key_weak(status_ok_received, 11), be_const_closure(Matter_IM_Message_status_ok_received_closure) },
        { be_const_key_weak(MSG_TIMEOUT, -1), be_const_int(5000) },
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
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_const_int(1),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(StatusResponseMessage),
    /* K4   */  be_nested_str_weak(status),
    /* K5   */  be_nested_str_weak(data),
    /* K6   */  be_nested_str_weak(ready),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
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
      0x50100200,  //  000D  LDBOOL	R4	1	0
      0x90020C04,  //  000E  SETMBR	R0	K6	R4
      0x80000000,  //  000F  RET	0
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
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(data),
    /* K2   */  be_nested_str_weak(ready),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x541A0008,  //  0005  LDINT	R6	9
      0x501C0200,  //  0006  LDBOOL	R7	1	0
      0x7C0C0800,  //  0007  CALL	R3	4
      0x90020202,  //  0008  SETMBR	R0	K1	R2
      0x500C0200,  //  0009  LDBOOL	R3	1	0
      0x90020403,  //  000A  SETMBR	R0	K2	R3
      0x80000000,  //  000B  RET	0
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
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(data),
    /* K2   */  be_nested_str_weak(ready),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x541A0006,  //  0005  LDINT	R6	7
      0x501C0200,  //  0006  LDBOOL	R7	1	0
      0x7C0C0800,  //  0007  CALL	R3	4
      0x90020202,  //  0008  SETMBR	R0	K1	R2
      0x500C0200,  //  0009  LDBOOL	R3	1	0
      0x90020403,  //  000A  SETMBR	R0	K2	R3
      0x80000000,  //  000B  RET	0
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
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(init),
    /* K1   */  be_nested_str_weak(data),
    /* K2   */  be_nested_str_weak(ready),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0700,  //  0003  GETMET	R3	R3	K0
      0x5C140200,  //  0004  MOVE	R5	R1
      0x541A0004,  //  0005  LDINT	R6	5
      0x501C0200,  //  0006  LDBOOL	R7	1	0
      0x7C0C0800,  //  0007  CALL	R3	4
      0x90020202,  //  0008  SETMBR	R0	K1	R2
      0x500C0200,  //  0009  LDBOOL	R3	1	0
      0x90020403,  //  000A  SETMBR	R0	K2	R3
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send
********************************************************************/
be_local_closure(Matter_IM_ReportData_send,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[27]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(resp),
    /* K2   */  be_nested_str_weak(data),
    /* K3   */  be_nested_str_weak(more_chunked_messages),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(attribute_reports),
    /* K6   */  be_nested_str_weak(to_TLV),
    /* K7   */  be_nested_str_weak(encode_len),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(MAX_MESSAGE),
    /* K10  */  be_nested_str_weak(tasmota),
    /* K11  */  be_nested_str_weak(log),
    /* K12  */  be_nested_str_weak(format),
    /* K13  */  be_nested_str_weak(MTR_X3A_X20elements_X3D_X25i_X20msg_sz_X3D_X25i_X20total_X3D_X25i),
    /* K14  */  be_const_int(3),
    /* K15  */  be_const_int(2147483647),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20Read_Attr_X20_X20next_chunk_X20exch_X3D_X25i),
    /* K17  */  be_nested_str_weak(get_exchangeid),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20Read_Attr_X20_X20first_chunk_X20exch_X3D_X25i),
    /* K19  */  be_nested_str_weak(encode),
    /* K20  */  be_nested_str_weak(encode_frame),
    /* K21  */  be_nested_str_weak(encrypt),
    /* K22  */  be_nested_str_weak(send_response),
    /* K23  */  be_nested_str_weak(raw),
    /* K24  */  be_nested_str_weak(remote_ip),
    /* K25  */  be_nested_str_weak(remote_port),
    /* K26  */  be_nested_str_weak(message_counter),
    }),
    be_str_weak(send),
    &be_const_str_solidified,
    ( &(const binstruction[121]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x88140903,  //  0003  GETMBR	R5	R4	K3
      0x58180004,  //  0004  LDCONST	R6	K4
      0x581C0004,  //  0005  LDCONST	R7	K4
      0x6020000C,  //  0006  GETGBL	R8	G12
      0x88240905,  //  0007  GETMBR	R9	R4	K5
      0x7C200200,  //  0008  CALL	R8	1
      0x24201104,  //  0009  GT	R8	R8	K4
      0x78220007,  //  000A  JMPF	R8	#0013
      0x88200905,  //  000B  GETMBR	R8	R4	K5
      0x94201104,  //  000C  GETIDX	R8	R8	K4
      0x8C201106,  //  000D  GETMET	R8	R8	K6
      0x7C200200,  //  000E  CALL	R8	1
      0x8C201107,  //  000F  GETMET	R8	R8	K7
      0x7C200200,  //  0010  CALL	R8	1
      0x5C181000,  //  0011  MOVE	R6	R8
      0x581C0008,  //  0012  LDCONST	R7	K8
      0x88200109,  //  0013  GETMBR	R8	R0	K9
      0x14200C08,  //  0014  LT	R8	R6	R8
      0x78220013,  //  0015  JMPF	R8	#002A
      0x6020000C,  //  0016  GETGBL	R8	G12
      0x88240905,  //  0017  GETMBR	R9	R4	K5
      0x7C200200,  //  0018  CALL	R8	1
      0x14200E08,  //  0019  LT	R8	R7	R8
      0x7822000E,  //  001A  JMPF	R8	#002A
      0x88200905,  //  001B  GETMBR	R8	R4	K5
      0x94201007,  //  001C  GETIDX	R8	R8	R7
      0x8C201106,  //  001D  GETMET	R8	R8	K6
      0x7C200200,  //  001E  CALL	R8	1
      0x8C201107,  //  001F  GETMET	R8	R8	K7
      0x7C200200,  //  0020  CALL	R8	1
      0x00240C08,  //  0021  ADD	R9	R6	R8
      0x88280109,  //  0022  GETMBR	R10	R0	K9
      0x1424120A,  //  0023  LT	R9	R9	R10
      0x78260002,  //  0024  JMPF	R9	#0028
      0x00180C08,  //  0025  ADD	R6	R6	R8
      0x001C0F08,  //  0026  ADD	R7	R7	K8
      0x70020000,  //  0027  JMP		#0029
      0x70020000,  //  0028  JMP		#002A
      0x7001FFE8,  //  0029  JMP		#0013
      0xB8221400,  //  002A  GETNGBL	R8	K10
      0x8C20110B,  //  002B  GETMET	R8	R8	K11
      0x8C28050C,  //  002C  GETMET	R10	R2	K12
      0x5830000D,  //  002D  LDCONST	R12	K13
      0x5C340E00,  //  002E  MOVE	R13	R7
      0x5C380C00,  //  002F  MOVE	R14	R6
      0x603C000C,  //  0030  GETGBL	R15	G12
      0x88400905,  //  0031  GETMBR	R16	R4	K5
      0x7C3C0200,  //  0032  CALL	R15	1
      0x7C280A00,  //  0033  CALL	R10	5
      0x582C000E,  //  0034  LDCONST	R11	K14
      0x7C200600,  //  0035  CALL	R8	3
      0x40200F0F,  //  0036  CONNECT	R8	R7	K15
      0x88240905,  //  0037  GETMBR	R9	R4	K5
      0x94201208,  //  0038  GETIDX	R8	R9	R8
      0x04280F08,  //  0039  SUB	R10	R7	K8
      0x402A080A,  //  003A  CONNECT	R10	K4	R10
      0x882C0905,  //  003B  GETMBR	R11	R4	K5
      0x9428160A,  //  003C  GETIDX	R10	R11	R10
      0x90120A0A,  //  003D  SETMBR	R4	K5	R10
      0x6028000C,  //  003E  GETGBL	R10	G12
      0x5C2C1000,  //  003F  MOVE	R11	R8
      0x7C280200,  //  0040  CALL	R10	1
      0x24281504,  //  0041  GT	R10	R10	K4
      0x9012060A,  //  0042  SETMBR	R4	K3	R10
      0x78160008,  //  0043  JMPF	R5	#004D
      0xB82A1400,  //  0044  GETNGBL	R10	K10
      0x8C28150B,  //  0045  GETMET	R10	R10	K11
      0x8C30050C,  //  0046  GETMET	R12	R2	K12
      0x58380010,  //  0047  LDCONST	R14	K16
      0x8C3C0111,  //  0048  GETMET	R15	R0	K17
      0x7C3C0200,  //  0049  CALL	R15	1
      0x7C300600,  //  004A  CALL	R12	3
      0x5834000E,  //  004B  LDCONST	R13	K14
      0x7C280600,  //  004C  CALL	R10	3
      0x88240903,  //  004D  GETMBR	R9	R4	K3
      0x7826000A,  //  004E  JMPF	R9	#005A
      0x5C240A00,  //  004F  MOVE	R9	R5
      0x74260008,  //  0050  JMPT	R9	#005A
      0xB8261400,  //  0051  GETNGBL	R9	K10
      0x8C24130B,  //  0052  GETMET	R9	R9	K11
      0x8C2C050C,  //  0053  GETMET	R11	R2	K12
      0x58340012,  //  0054  LDCONST	R13	K18
      0x8C380111,  //  0055  GETMET	R14	R0	K17
      0x7C380200,  //  0056  CALL	R14	1
      0x7C2C0600,  //  0057  CALL	R11	3
      0x5830000E,  //  0058  LDCONST	R12	K14
      0x7C240600,  //  0059  CALL	R9	3
      0x88240102,  //  005A  GETMBR	R9	R0	K2
      0x8C241306,  //  005B  GETMET	R9	R9	K6
      0x7C240200,  //  005C  CALL	R9	1
      0x8C281313,  //  005D  GETMET	R10	R9	K19
      0x60300015,  //  005E  GETGBL	R12	G21
      0x88340109,  //  005F  GETMBR	R13	R0	K9
      0x7C300200,  //  0060  CALL	R12	1
      0x7C280400,  //  0061  CALL	R10	2
      0x8C2C0714,  //  0062  GETMET	R11	R3	K20
      0x5C341400,  //  0063  MOVE	R13	R10
      0x7C2C0400,  //  0064  CALL	R11	2
      0x8C2C0715,  //  0065  GETMET	R11	R3	K21
      0x7C2C0200,  //  0066  CALL	R11	1
      0x8C2C0316,  //  0067  GETMET	R11	R1	K22
      0x88340717,  //  0068  GETMBR	R13	R3	K23
      0x88380718,  //  0069  GETMBR	R14	R3	K24
      0x883C0719,  //  006A  GETMBR	R15	R3	K25
      0x8840071A,  //  006B  GETMBR	R16	R3	K26
      0x7C2C0A00,  //  006C  CALL	R11	5
      0x602C000C,  //  006D  GETGBL	R11	G12
      0x5C301000,  //  006E  MOVE	R12	R8
      0x7C2C0200,  //  006F  CALL	R11	1
      0x242C1704,  //  0070  GT	R11	R11	K4
      0x782E0003,  //  0071  JMPF	R11	#0076
      0x90120A08,  //  0072  SETMBR	R4	K5	R8
      0x502C0000,  //  0073  LDBOOL	R11	0	0
      0x80041600,  //  0074  RET	1	R11
      0x70020001,  //  0075  JMP		#0078
      0x502C0200,  //  0076  LDBOOL	R11	1	0
      0x80041600,  //  0077  RET	1	R11
      0x80000000,  //  0078  RET	0
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
        { be_const_key_weak(init, -1), be_const_closure(Matter_IM_ReportData_init_closure) },
        { be_const_key_weak(send, -1), be_const_closure(Matter_IM_ReportData_send_closure) },
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
    /* K2   */  be_nested_str_weak(ready),
    /* K3   */  be_nested_str_weak(sub),
    /* K4   */  be_nested_str_weak(re_arm),
    }),
    be_str_weak(ack_received),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x88080101,  //  0006  GETMBR	R2	R0	K1
      0x740A0007,  //  0007  JMPT	R2	#0010
      0x50080200,  //  0008  LDBOOL	R2	1	0
      0x90020402,  //  0009  SETMBR	R0	K2	R2
      0x88080103,  //  000A  GETMBR	R2	R0	K3
      0x8C080504,  //  000B  GETMET	R2	R2	K4
      0x7C080200,  //  000C  CALL	R2	1
      0x50080200,  //  000D  LDBOOL	R2	1	0
      0x80040400,  //  000E  RET	1	R2
      0x70020001,  //  000F  JMP		#0012
      0x50080000,  //  0010  LDBOOL	R2	0	0
      0x80040400,  //  0011  RET	1	R2
      0x80000000,  //  0012  RET	0
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
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(report_data_phase),
    /* K1   */  be_nested_str_weak(status_ok_received),
    }),
    be_str_weak(status_ok_received),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A0007,  //  0001  JMPF	R2	#000A
      0x60080003,  //  0002  GETGBL	R2	G3
      0x5C0C0000,  //  0003  MOVE	R3	R0
      0x7C080200,  //  0004  CALL	R2	1
      0x8C080501,  //  0005  GETMET	R2	R2	K1
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x80040400,  //  0008  RET	1	R2
      0x70020007,  //  0009  JMP		#0012
      0x60080003,  //  000A  GETGBL	R2	G3
      0x5C0C0000,  //  000B  MOVE	R3	R0
      0x7C080200,  //  000C  CALL	R2	1
      0x8C080501,  //  000D  GETMET	R2	R2	K1
      0x4C100000,  //  000E  LDNIL	R4
      0x7C080400,  //  000F  CALL	R2	2
      0x50080000,  //  0010  LDBOOL	R2	0	0
      0x80040400,  //  0011  RET	1	R2
      0x80000000,  //  0012  RET	0
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
** Solidified function: send
********************************************************************/
be_local_closure(Matter_IM_ReportDataSubscribed_send,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(data),
    /* K1   */  be_nested_str_weak(attribute_reports),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(report_data_phase),
    /* K4   */  be_nested_str_weak(send),
    /* K5   */  be_nested_str_weak(resp),
    /* K6   */  be_nested_str_weak(build_standalone_ack),
    /* K7   */  be_nested_str_weak(encode_frame),
    /* K8   */  be_nested_str_weak(encrypt),
    /* K9   */  be_nested_str_weak(send_response),
    /* K10  */  be_nested_str_weak(raw),
    /* K11  */  be_nested_str_weak(remote_ip),
    /* K12  */  be_nested_str_weak(remote_port),
    /* K13  */  be_nested_str_weak(message_counter),
    }),
    be_str_weak(send),
    &be_const_str_solidified,
    ( &(const binstruction[55]) {  /* code */
      0x6008000C,  //  0000  GETGBL	R2	G12
      0x880C0100,  //  0001  GETMBR	R3	R0	K0
      0x880C0701,  //  0002  GETMBR	R3	R3	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x24080502,  //  0004  GT	R2	R2	K2
      0x780A0020,  //  0005  JMPF	R2	#0027
      0x88080103,  //  0006  GETMBR	R2	R0	K3
      0x780A000E,  //  0007  JMPF	R2	#0017
      0x60080003,  //  0008  GETGBL	R2	G3
      0x5C0C0000,  //  0009  MOVE	R3	R0
      0x7C080200,  //  000A  CALL	R2	1
      0x8C080504,  //  000B  GETMET	R2	R2	K4
      0x5C100200,  //  000C  MOVE	R4	R1
      0x7C080400,  //  000D  CALL	R2	2
      0x5C0C0400,  //  000E  MOVE	R3	R2
      0x740E0001,  //  000F  JMPT	R3	#0012
      0x500C0000,  //  0010  LDBOOL	R3	0	0
      0x80040600,  //  0011  RET	1	R3
      0x500C0000,  //  0012  LDBOOL	R3	0	0
      0x90020603,  //  0013  SETMBR	R0	K3	R3
      0x500C0000,  //  0014  LDBOOL	R3	0	0
      0x80040600,  //  0015  RET	1	R3
      0x7002000E,  //  0016  JMP		#0026
      0x88080105,  //  0017  GETMBR	R2	R0	K5
      0x8C080506,  //  0018  GETMET	R2	R2	K6
      0x7C080200,  //  0019  CALL	R2	1
      0x8C0C0507,  //  001A  GETMET	R3	R2	K7
      0x7C0C0200,  //  001B  CALL	R3	1
      0x8C0C0508,  //  001C  GETMET	R3	R2	K8
      0x7C0C0200,  //  001D  CALL	R3	1
      0x8C0C0309,  //  001E  GETMET	R3	R1	K9
      0x8814050A,  //  001F  GETMBR	R5	R2	K10
      0x8818050B,  //  0020  GETMBR	R6	R2	K11
      0x881C050C,  //  0021  GETMBR	R7	R2	K12
      0x8820050D,  //  0022  GETMBR	R8	R2	K13
      0x7C0C0A00,  //  0023  CALL	R3	5
      0x500C0200,  //  0024  LDBOOL	R3	1	0
      0x80040600,  //  0025  RET	1	R3
      0x7002000E,  //  0026  JMP		#0036
      0x88080103,  //  0027  GETMBR	R2	R0	K3
      0x780A000A,  //  0028  JMPF	R2	#0034
      0x60080003,  //  0029  GETGBL	R2	G3
      0x5C0C0000,  //  002A  MOVE	R3	R0
      0x7C080200,  //  002B  CALL	R2	1
      0x8C080504,  //  002C  GETMET	R2	R2	K4
      0x5C100200,  //  002D  MOVE	R4	R1
      0x7C080400,  //  002E  CALL	R2	2
      0x50080000,  //  002F  LDBOOL	R2	0	0
      0x90020602,  //  0030  SETMBR	R0	K3	R2
      0x50080000,  //  0031  LDBOOL	R2	0	0
      0x80040400,  //  0032  RET	1	R2
      0x70020001,  //  0033  JMP		#0036
      0x50080200,  //  0034  LDBOOL	R2	1	0
      0x80040400,  //  0035  RET	1	R2
      0x80000000,  //  0036  RET	0
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
        { be_const_key_weak(status_error_received, -1), be_const_closure(Matter_IM_ReportDataSubscribed_status_error_received_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_IM_ReportDataSubscribed_init_closure) },
        { be_const_key_weak(report_data_phase, 2), be_const_var(1) },
        { be_const_key_weak(reached_timeout, 6), be_const_closure(Matter_IM_ReportDataSubscribed_reached_timeout_closure) },
        { be_const_key_weak(sub, -1), be_const_var(0) },
        { be_const_key_weak(send, -1), be_const_closure(Matter_IM_ReportDataSubscribed_send_closure) },
    })),
    be_str_weak(Matter_IM_ReportDataSubscribed)
);
/*******************************************************************/

void be_load_Matter_IM_ReportDataSubscribed_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_IM_ReportDataSubscribed);
    be_setglobal(vm, "Matter_IM_ReportDataSubscribed");
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
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(status_ok_received),
    }),
    be_str_weak(status_ok_received),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080500,  //  0003  GETMET	R2	R2	K0
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send
********************************************************************/
be_local_closure(Matter_IM_SubscribeResponse_send,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(report_data_phase),
    /* K1   */  be_nested_str_weak(send),
    /* K2   */  be_nested_str_weak(resp),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(SubscribeResponseMessage),
    /* K5   */  be_nested_str_weak(subscription_id),
    /* K6   */  be_nested_str_weak(sub),
    /* K7   */  be_nested_str_weak(max_interval),
    /* K8   */  be_nested_str_weak(opcode),
    /* K9   */  be_nested_str_weak(encode_frame),
    /* K10  */  be_nested_str_weak(to_TLV),
    /* K11  */  be_nested_str_weak(encode),
    /* K12  */  be_nested_str_weak(encrypt),
    /* K13  */  be_nested_str_weak(send_response),
    /* K14  */  be_nested_str_weak(raw),
    /* K15  */  be_nested_str_weak(remote_ip),
    /* K16  */  be_nested_str_weak(remote_port),
    /* K17  */  be_nested_str_weak(message_counter),
    /* K18  */  be_nested_str_weak(re_arm),
    }),
    be_str_weak(send),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A000E,  //  0001  JMPF	R2	#0011
      0x60080003,  //  0002  GETGBL	R2	G3
      0x5C0C0000,  //  0003  MOVE	R3	R0
      0x7C080200,  //  0004  CALL	R2	1
      0x8C080501,  //  0005  GETMET	R2	R2	K1
      0x5C100200,  //  0006  MOVE	R4	R1
      0x7C080400,  //  0007  CALL	R2	2
      0x5C0C0400,  //  0008  MOVE	R3	R2
      0x740E0001,  //  0009  JMPT	R3	#000C
      0x500C0000,  //  000A  LDBOOL	R3	0	0
      0x80040600,  //  000B  RET	1	R3
      0x500C0000,  //  000C  LDBOOL	R3	0	0
      0x90020003,  //  000D  SETMBR	R0	K0	R3
      0x500C0000,  //  000E  LDBOOL	R3	0	0
      0x80040600,  //  000F  RET	1	R3
      0x7002001F,  //  0010  JMP		#0031
      0x88080102,  //  0011  GETMBR	R2	R0	K2
      0xB80E0600,  //  0012  GETNGBL	R3	K3
      0x8C0C0704,  //  0013  GETMET	R3	R3	K4
      0x7C0C0200,  //  0014  CALL	R3	1
      0x88100106,  //  0015  GETMBR	R4	R0	K6
      0x88100905,  //  0016  GETMBR	R4	R4	K5
      0x900E0A04,  //  0017  SETMBR	R3	K5	R4
      0x88100106,  //  0018  GETMBR	R4	R0	K6
      0x88100907,  //  0019  GETMBR	R4	R4	K7
      0x900E0E04,  //  001A  SETMBR	R3	K7	R4
      0x88100102,  //  001B  GETMBR	R4	R0	K2
      0x54160003,  //  001C  LDINT	R5	4
      0x90121005,  //  001D  SETMBR	R4	K8	R5
      0x8C100509,  //  001E  GETMET	R4	R2	K9
      0x8C18070A,  //  001F  GETMET	R6	R3	K10
      0x7C180200,  //  0020  CALL	R6	1
      0x8C180D0B,  //  0021  GETMET	R6	R6	K11
      0x7C180200,  //  0022  CALL	R6	1
      0x7C100400,  //  0023  CALL	R4	2
      0x8C10050C,  //  0024  GETMET	R4	R2	K12
      0x7C100200,  //  0025  CALL	R4	1
      0x8C10030D,  //  0026  GETMET	R4	R1	K13
      0x8818050E,  //  0027  GETMBR	R6	R2	K14
      0x881C050F,  //  0028  GETMBR	R7	R2	K15
      0x88200510,  //  0029  GETMBR	R8	R2	K16
      0x88240511,  //  002A  GETMBR	R9	R2	K17
      0x7C100A00,  //  002B  CALL	R4	5
      0x88100106,  //  002C  GETMBR	R4	R0	K6
      0x8C100912,  //  002D  GETMET	R4	R4	K18
      0x7C100200,  //  002E  CALL	R4	1
      0x50100200,  //  002F  LDBOOL	R4	1	0
      0x80040800,  //  0030  RET	1	R4
      0x80000000,  //  0031  RET	0
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
        { be_const_key_weak(init, 3), be_const_closure(Matter_IM_SubscribeResponse_init_closure) },
        { be_const_key_weak(sub, 4), be_const_var(0) },
        { be_const_key_weak(status_ok_received, -1), be_const_closure(Matter_IM_SubscribeResponse_status_ok_received_closure) },
        { be_const_key_weak(report_data_phase, -1), be_const_var(1) },
        { be_const_key_weak(send, -1), be_const_closure(Matter_IM_SubscribeResponse_send_closure) },
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
