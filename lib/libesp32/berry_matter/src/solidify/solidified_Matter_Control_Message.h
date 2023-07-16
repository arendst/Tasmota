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
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(session),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20_X3EMCSyncRsp_X20_X2A_X20Not_X20implemented_X20_X25s),
    /* K4   */  be_nested_str_weak(raw),
    /* K5   */  be_nested_str_weak(app_payload_idx),
    /* K6   */  be_const_int(2147483647),
    /* K7   */  be_nested_str_weak(tohex),
    /* K8   */  be_const_int(2),
    }),
    be_str_weak(parse_MsgCounterSyncRsp),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x60140018,  //  0003  GETGBL	R5	G24
      0x58180003,  //  0004  LDCONST	R6	K3
      0x881C0305,  //  0005  GETMBR	R7	R1	K5
      0x401C0F06,  //  0006  CONNECT	R7	R7	K6
      0x88200304,  //  0007  GETMBR	R8	R1	K4
      0x941C1007,  //  0008  GETIDX	R7	R8	R7
      0x8C1C0F07,  //  0009  GETMET	R7	R7	K7
      0x7C1C0200,  //  000A  CALL	R7	1
      0x7C140400,  //  000B  CALL	R5	2
      0x58180008,  //  000C  LDCONST	R6	K8
      0x7C0C0600,  //  000D  CALL	R3	3
      0x500C0000,  //  000E  LDBOOL	R3	0	0
      0x80040600,  //  000F  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_MsgCounterSyncReq
********************************************************************/
be_local_closure(Matter_Control_Message_parse_MsgCounterSyncReq,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(session),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20_X3EMCSyncReq_X20_X2A_X20Not_X20implemented_X20_X25s),
    /* K4   */  be_nested_str_weak(raw),
    /* K5   */  be_nested_str_weak(app_payload_idx),
    /* K6   */  be_const_int(2147483647),
    /* K7   */  be_nested_str_weak(tohex),
    /* K8   */  be_const_int(2),
    }),
    be_str_weak(parse_MsgCounterSyncReq),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x8C0C0702,  //  0002  GETMET	R3	R3	K2
      0x60140018,  //  0003  GETGBL	R5	G24
      0x58180003,  //  0004  LDCONST	R6	K3
      0x881C0305,  //  0005  GETMBR	R7	R1	K5
      0x401C0F06,  //  0006  CONNECT	R7	R7	K6
      0x88200304,  //  0007  GETMBR	R8	R1	K4
      0x941C1007,  //  0008  GETIDX	R7	R8	R7
      0x8C1C0F07,  //  0009  GETMET	R7	R7	K7
      0x7C1C0200,  //  000A  CALL	R7	1
      0x7C140400,  //  000B  CALL	R5	2
      0x58180008,  //  000C  LDCONST	R6	K8
      0x7C0C0600,  //  000D  CALL	R3	3
      0x500C0000,  //  000E  LDBOOL	R3	0	0
      0x80040600,  //  000F  RET	1	R3
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
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(MTR_X3A_X20received_X20control_X20message_X20),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(inspect),
    /* K5   */  be_const_int(3),
    /* K6   */  be_nested_str_weak(opcode),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(parse_MsgCounterSyncReq),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(parse_MsgCounterSyncRsp),
    /* K11  */  be_nested_str_weak(MTR_X3A_X20_X3E_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X20Unknown_X20OpCode_X20_X28control_X20message_X29_X20_X2502X),
    /* K12  */  be_const_int(2),
    }),
    be_str_weak(process_incoming_control_message),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
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
      0x70020011,  //  0010  JMP		#0023
      0x88080306,  //  0011  GETMBR	R2	R1	K6
      0x1C080509,  //  0012  EQ	R2	R2	K9
      0x780A0004,  //  0013  JMPF	R2	#0019
      0x8C08010A,  //  0014  GETMET	R2	R0	K10
      0x5C100200,  //  0015  MOVE	R4	R1
      0x7C080400,  //  0016  CALL	R2	2
      0x80040400,  //  0017  RET	1	R2
      0x70020009,  //  0018  JMP		#0023
      0xB80A0000,  //  0019  GETNGBL	R2	K0
      0x8C080501,  //  001A  GETMET	R2	R2	K1
      0x60100018,  //  001B  GETGBL	R4	G24
      0x5814000B,  //  001C  LDCONST	R5	K11
      0x88180306,  //  001D  GETMBR	R6	R1	K6
      0x7C100400,  //  001E  CALL	R4	2
      0x5814000C,  //  001F  LDCONST	R5	K12
      0x7C080600,  //  0020  CALL	R2	3
      0x50080000,  //  0021  LDBOOL	R2	0	0
      0x80040400,  //  0022  RET	1	R2
      0x50080000,  //  0023  LDBOOL	R2	0	0
      0x80040400,  //  0024  RET	1	R2
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
