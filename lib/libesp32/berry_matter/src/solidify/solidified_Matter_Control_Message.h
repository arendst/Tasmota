/* Solidification of Matter_Control_Message.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Control_Message' ktab size: 22, total: 31 (saved 72 bytes)
static const bvalue be_ktab_class_Matter_Control_Message[22] = {
  /* K0   */  be_nested_str_weak(session),
  /* K1   */  be_nested_str_weak(log),
  /* K2   */  be_nested_str_weak(MTR_X3A_X20_X3EMCSyncRsp_X20_X2A_X20Not_X20implemented_X20_X25s),
  /* K3   */  be_nested_str_weak(raw),
  /* K4   */  be_nested_str_weak(app_payload_idx),
  /* K5   */  be_const_int(2147483647),
  /* K6   */  be_nested_str_weak(tohex),
  /* K7   */  be_const_int(2),
  /* K8   */  be_nested_str_weak(MTR_X3A_X20_X3EMCSyncReq_X20_X2A_X20Not_X20implemented_X20_X25s),
  /* K9   */  be_nested_str_weak(crypto),
  /* K10  */  be_nested_str_weak(responder),
  /* K11  */  be_nested_str_weak(device),
  /* K12  */  be_nested_str_weak(MTR_X3A_X20received_X20control_X20message_X20),
  /* K13  */  be_nested_str_weak(matter),
  /* K14  */  be_nested_str_weak(inspect),
  /* K15  */  be_const_int(3),
  /* K16  */  be_nested_str_weak(opcode),
  /* K17  */  be_const_int(0),
  /* K18  */  be_nested_str_weak(parse_MsgCounterSyncReq),
  /* K19  */  be_const_int(1),
  /* K20  */  be_nested_str_weak(parse_MsgCounterSyncRsp),
  /* K21  */  be_nested_str_weak(MTR_X3A_X20_X3E_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X3F_X20Unknown_X20OpCode_X20_X28control_X20message_X29_X20_X2502X),
};


extern const bclass be_class_Matter_Control_Message;

/********************************************************************
** Solidified function: parse_MsgCounterSyncRsp
********************************************************************/
be_local_closure(class_Matter_Control_Message_parse_MsgCounterSyncRsp,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Control_Message,     /* shared constants */
    be_str_weak(parse_MsgCounterSyncRsp),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x60100018,  //  0002  GETGBL	R4	G24
      0x58140002,  //  0003  LDCONST	R5	K2
      0x88180304,  //  0004  GETMBR	R6	R1	K4
      0x40180D05,  //  0005  CONNECT	R6	R6	K5
      0x881C0303,  //  0006  GETMBR	R7	R1	K3
      0x94180E06,  //  0007  GETIDX	R6	R7	R6
      0x8C180D06,  //  0008  GETMET	R6	R6	K6
      0x7C180200,  //  0009  CALL	R6	1
      0x7C100400,  //  000A  CALL	R4	2
      0x58140007,  //  000B  LDCONST	R5	K7
      0x7C0C0400,  //  000C  CALL	R3	2
      0x500C0000,  //  000D  LDBOOL	R3	0	0
      0x80040600,  //  000E  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_MsgCounterSyncReq
********************************************************************/
be_local_closure(class_Matter_Control_Message_parse_MsgCounterSyncReq,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Control_Message,     /* shared constants */
    be_str_weak(parse_MsgCounterSyncReq),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0xB80E0200,  //  0001  GETNGBL	R3	K1
      0x60100018,  //  0002  GETGBL	R4	G24
      0x58140008,  //  0003  LDCONST	R5	K8
      0x88180304,  //  0004  GETMBR	R6	R1	K4
      0x40180D05,  //  0005  CONNECT	R6	R6	K5
      0x881C0303,  //  0006  GETMBR	R7	R1	K3
      0x94180E06,  //  0007  GETIDX	R6	R7	R6
      0x8C180D06,  //  0008  GETMET	R6	R6	K6
      0x7C180200,  //  0009  CALL	R6	1
      0x7C100400,  //  000A  CALL	R4	2
      0x58140007,  //  000B  LDCONST	R5	K7
      0x7C0C0400,  //  000C  CALL	R3	2
      0x500C0000,  //  000D  LDBOOL	R3	0	0
      0x80040600,  //  000E  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Control_Message_init,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Control_Message,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0xA40A1200,  //  0000  IMPORT	R2	K9
      0x90021401,  //  0001  SETMBR	R0	K10	R1
      0x880C030B,  //  0002  GETMBR	R3	R1	K11
      0x90021603,  //  0003  SETMBR	R0	K11	R3
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_incoming_control_message
********************************************************************/
be_local_closure(class_Matter_Control_Message_process_incoming_control_message,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Control_Message,     /* shared constants */
    be_str_weak(process_incoming_control_message),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0xB80A0200,  //  0000  GETNGBL	R2	K1
      0xB80E1A00,  //  0001  GETNGBL	R3	K13
      0x8C0C070E,  //  0002  GETMET	R3	R3	K14
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C0C0400,  //  0004  CALL	R3	2
      0x000E1803,  //  0005  ADD	R3	K12	R3
      0x5810000F,  //  0006  LDCONST	R4	K15
      0x7C080400,  //  0007  CALL	R2	2
      0x88080310,  //  0008  GETMBR	R2	R1	K16
      0x1C080511,  //  0009  EQ	R2	R2	K17
      0x780A0004,  //  000A  JMPF	R2	#0010
      0x8C080112,  //  000B  GETMET	R2	R0	K18
      0x5C100200,  //  000C  MOVE	R4	R1
      0x7C080400,  //  000D  CALL	R2	2
      0x80040400,  //  000E  RET	1	R2
      0x70020010,  //  000F  JMP		#0021
      0x88080310,  //  0010  GETMBR	R2	R1	K16
      0x1C080513,  //  0011  EQ	R2	R2	K19
      0x780A0004,  //  0012  JMPF	R2	#0018
      0x8C080114,  //  0013  GETMET	R2	R0	K20
      0x5C100200,  //  0014  MOVE	R4	R1
      0x7C080400,  //  0015  CALL	R2	2
      0x80040400,  //  0016  RET	1	R2
      0x70020008,  //  0017  JMP		#0021
      0xB80A0200,  //  0018  GETNGBL	R2	K1
      0x600C0018,  //  0019  GETGBL	R3	G24
      0x58100015,  //  001A  LDCONST	R4	K21
      0x88140310,  //  001B  GETMBR	R5	R1	K16
      0x7C0C0400,  //  001C  CALL	R3	2
      0x58100007,  //  001D  LDCONST	R4	K7
      0x7C080400,  //  001E  CALL	R2	2
      0x50080000,  //  001F  LDBOOL	R2	0	0
      0x80040400,  //  0020  RET	1	R2
      0x50080000,  //  0021  LDBOOL	R2	0	0
      0x80040400,  //  0022  RET	1	R2
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
        { be_const_key_weak(parse_MsgCounterSyncRsp, -1), be_const_closure(class_Matter_Control_Message_parse_MsgCounterSyncRsp_closure) },
        { be_const_key_weak(responder, 2), be_const_var(0) },
        { be_const_key_weak(parse_MsgCounterSyncReq, -1), be_const_closure(class_Matter_Control_Message_parse_MsgCounterSyncReq_closure) },
        { be_const_key_weak(init, 4), be_const_closure(class_Matter_Control_Message_init_closure) },
        { be_const_key_weak(device, -1), be_const_var(1) },
        { be_const_key_weak(process_incoming_control_message, -1), be_const_closure(class_Matter_Control_Message_process_incoming_control_message_closure) },
    })),
    be_str_weak(Matter_Control_Message)
);
/********************************************************************/
/* End of solidification */
