/* Solidification of Matter_Control_Message.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Control_Message;

/********************************************************************
** Solidified function: parse_MsgCounterSyncRsp
********************************************************************/
be_local_closure(Matter_Control_Message_parse_MsgCounterSyncRsp,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(log),
    /* K4   */  be_nested_str_weak(format),
    /* K5   */  be_nested_str_weak(MTR_X3A_X20_X3EMCSyncRsp_X20_X2A_X20Not_X20implemented_X20_X25s),
    /* K6   */  be_nested_str_weak(raw),
    /* K7   */  be_nested_str_weak(app_payload_idx),
    /* K8   */  be_const_int(2147483647),
    /* K9   */  be_nested_str_weak(tohex),
    /* K10  */  be_const_int(2),
    }),
    be_str_weak(parse_MsgCounterSyncRsp),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0301,  //  0001  GETMBR	R3	R1	K1
      0xB8120400,  //  0002  GETNGBL	R4	K2
      0x8C100903,  //  0003  GETMET	R4	R4	K3
      0x8C180504,  //  0004  GETMET	R6	R2	K4
      0x58200005,  //  0005  LDCONST	R8	K5
      0x88240307,  //  0006  GETMBR	R9	R1	K7
      0x40241308,  //  0007  CONNECT	R9	R9	K8
      0x88280306,  //  0008  GETMBR	R10	R1	K6
      0x94241409,  //  0009  GETIDX	R9	R10	R9
      0x8C241309,  //  000A  GETMET	R9	R9	K9
      0x7C240200,  //  000B  CALL	R9	1
      0x7C180600,  //  000C  CALL	R6	3
      0x581C000A,  //  000D  LDCONST	R7	K10
      0x7C100600,  //  000E  CALL	R4	3
      0x50100000,  //  000F  LDBOOL	R4	0	0
      0x80040800,  //  0010  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_MsgCounterSyncReq
********************************************************************/
be_local_closure(Matter_Control_Message_parse_MsgCounterSyncReq,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(log),
    /* K4   */  be_nested_str_weak(format),
    /* K5   */  be_nested_str_weak(MTR_X3A_X20_X3EMCSyncReq_X20_X2A_X20Not_X20implemented_X20_X25s),
    /* K6   */  be_nested_str_weak(raw),
    /* K7   */  be_nested_str_weak(app_payload_idx),
    /* K8   */  be_const_int(2147483647),
    /* K9   */  be_nested_str_weak(tohex),
    /* K10  */  be_const_int(2),
    }),
    be_str_weak(parse_MsgCounterSyncReq),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0301,  //  0001  GETMBR	R3	R1	K1
      0xB8120400,  //  0002  GETNGBL	R4	K2
      0x8C100903,  //  0003  GETMET	R4	R4	K3
      0x8C180504,  //  0004  GETMET	R6	R2	K4
      0x58200005,  //  0005  LDCONST	R8	K5
      0x88240307,  //  0006  GETMBR	R9	R1	K7
      0x40241308,  //  0007  CONNECT	R9	R9	K8
      0x88280306,  //  0008  GETMBR	R10	R1	K6
      0x94241409,  //  0009  GETIDX	R9	R10	R9
      0x8C241309,  //  000A  GETMET	R9	R9	K9
      0x7C240200,  //  000B  CALL	R9	1
      0x7C180600,  //  000C  CALL	R6	3
      0x581C000A,  //  000D  LDCONST	R7	K10
      0x7C100600,  //  000E  CALL	R4	3
      0x50100000,  //  000F  LDBOOL	R4	0	0
      0x80040800,  //  0010  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Control_Message_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(responder),
    /* K2   */  be_nested_str_weak(device),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x90020201,  //  0001  SETMBR	R0	K1	R1
      0x880C0302,  //  0002  GETMBR	R3	R1	K2
      0x90020403,  //  0003  SETMBR	R0	K2	R3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_incoming_control_message
********************************************************************/
be_local_closure(Matter_Control_Message_process_incoming_control_message,   /* name */
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
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(MTR_X3A_X20received_X20control_X20message_X20),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(inspect),
    /* K5   */  be_const_int(2),
    /* K6   */  be_nested_str_weak(opcode),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(parse_MsgCounterSyncReq),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(parse_MsgCounterSyncRsp),
    /* K11  */  be_nested_str_weak(string),
    /* K12  */  be_nested_str_weak(format),
    /* K13  */  be_nested_str_weak(MTR_X3A_X20_X3E_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X20Unknown_X20OpCode_X20_X28control_X20message_X29_X20_X2502X),
    }),
    be_str_weak(process_incoming_control_message),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0xB8120600,  //  0002  GETNGBL	R4	K3
      0x8C100904,  //  0003  GETMET	R4	R4	K4
      0x5C180200,  //  0004  MOVE	R6	R1
      0x7C100400,  //  0005  CALL	R4	2
      0x00120404,  //  0006  ADD	R4	K2	R4
      0x58140005,  //  0007  LDCONST	R5	K5
      0x7C080600,  //  0008  CALL	R2	3
      0x88080306,  //  0009  GETMBR	R2	R1	K6
      0x1C080507,  //  000A  EQ	R2	R2	K7
      0x780A0004,  //  000B  JMPF	R2	#0011
      0x8C080108,  //  000C  GETMET	R2	R0	K8
      0x5C100200,  //  000D  MOVE	R4	R1
      0x7C080400,  //  000E  CALL	R2	2
      0x80040400,  //  000F  RET	1	R2
      0x70020012,  //  0010  JMP		#0024
      0x88080306,  //  0011  GETMBR	R2	R1	K6
      0x1C080509,  //  0012  EQ	R2	R2	K9
      0x780A0004,  //  0013  JMPF	R2	#0019
      0x8C08010A,  //  0014  GETMET	R2	R0	K10
      0x5C100200,  //  0015  MOVE	R4	R1
      0x7C080400,  //  0016  CALL	R2	2
      0x80040400,  //  0017  RET	1	R2
      0x7002000A,  //  0018  JMP		#0024
      0xA40A1600,  //  0019  IMPORT	R2	K11
      0xB80E0000,  //  001A  GETNGBL	R3	K0
      0x8C0C0701,  //  001B  GETMET	R3	R3	K1
      0x8C14050C,  //  001C  GETMET	R5	R2	K12
      0x581C000D,  //  001D  LDCONST	R7	K13
      0x88200306,  //  001E  GETMBR	R8	R1	K6
      0x7C140600,  //  001F  CALL	R5	3
      0x58180005,  //  0020  LDCONST	R6	K5
      0x7C0C0600,  //  0021  CALL	R3	3
      0x500C0000,  //  0022  LDBOOL	R3	0	0
      0x80040600,  //  0023  RET	1	R3
      0x50080000,  //  0024  LDBOOL	R2	0	0
      0x80040400,  //  0025  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Control_Message
********************************************************************/
be_local_class(Matter_Control_Message,
    2,
    NULL,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(parse_MsgCounterSyncRsp, -1), be_const_closure(Matter_Control_Message_parse_MsgCounterSyncRsp_closure) },
        { be_const_key_weak(responder, 2), be_const_var(0) },
        { be_const_key_weak(parse_MsgCounterSyncReq, -1), be_const_closure(Matter_Control_Message_parse_MsgCounterSyncReq_closure) },
        { be_const_key_weak(init, 4), be_const_closure(Matter_Control_Message_init_closure) },
        { be_const_key_weak(device, -1), be_const_var(1) },
        { be_const_key_weak(process_incoming_control_message, -1), be_const_closure(Matter_Control_Message_process_incoming_control_message_closure) },
    })),
    be_str_weak(Matter_Control_Message)
);
/*******************************************************************/

void be_load_Matter_Control_Message_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Control_Message);
    be_setglobal(vm, "Matter_Control_Message");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
