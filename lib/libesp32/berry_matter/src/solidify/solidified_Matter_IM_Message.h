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
    6,
    NULL,
    be_nested_map(7,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(log, 4), be_const_var(5) },
        { be_const_key_weak(command, 2), be_const_var(3) },
        { be_const_key_weak(status, -1), be_const_var(4) },
        { be_const_key_weak(tostring, -1), be_const_closure(Matter_Path_tostring_closure) },
        { be_const_key_weak(cluster, 5), be_const_var(1) },
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
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(resp),
    /* K1   */  be_nested_str_weak(build_response),
    /* K2   */  be_nested_str_weak(ready),
    /* K3   */  be_nested_str_weak(expiration),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(millis),
    /* K6   */  be_nested_str_weak(MSG_TIMEOUT),
    /* K7   */  be_nested_str_weak(last_counter),
    /* K8   */  be_const_int(0),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
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
      0x80000000,  //  000E  RET	0
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
    ( &(const bvalue[23]) {     /* constants */
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
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
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
      0x80040800,  //  002F  RET	1	R4
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
    5,
    NULL,
    be_nested_map(13,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(status_ok_received, -1), be_const_closure(Matter_IM_Message_status_ok_received_closure) },
        { be_const_key_weak(data, 0), be_const_var(3) },
        { be_const_key_weak(resp, 11), be_const_var(1) },
        { be_const_key_weak(ack_received, 9), be_const_closure(Matter_IM_Message_ack_received_closure) },
        { be_const_key_weak(MSG_TIMEOUT, 12), be_const_int(5000) },
        { be_const_key_weak(get_exchangeid, -1), be_const_closure(Matter_IM_Message_get_exchangeid_closure) },
        { be_const_key_weak(init, 8), be_const_closure(Matter_IM_Message_init_closure) },
        { be_const_key_weak(send_im, -1), be_const_closure(Matter_IM_Message_send_im_closure) },
        { be_const_key_weak(expiration, -1), be_const_var(0) },
        { be_const_key_weak(reached_timeout, -1), be_const_closure(Matter_IM_Message_reached_timeout_closure) },
        { be_const_key_weak(ready, -1), be_const_var(2) },
        { be_const_key_weak(status_error_received, -1), be_const_closure(Matter_IM_Message_status_error_received_closure) },
        { be_const_key_weak(last_counter, -1), be_const_var(4) },
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
    20,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[33]) {     /* constants */
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
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[165]) {  /* code */
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
      0x6020000C,  //  0018  GETGBL	R8	G12
      0x8824090D,  //  0019  GETMBR	R9	R4	K13
      0x7C200200,  //  001A  CALL	R8	1
      0x24201109,  //  001B  GT	R8	R8	K9
      0x78220007,  //  001C  JMPF	R8	#0025
      0x8820090D,  //  001D  GETMBR	R8	R4	K13
      0x94201109,  //  001E  GETIDX	R8	R8	K9
      0x8C20110E,  //  001F  GETMET	R8	R8	K14
      0x7C200200,  //  0020  CALL	R8	1
      0x8C20110F,  //  0021  GETMET	R8	R8	K15
      0x7C200200,  //  0022  CALL	R8	1
      0x5C181000,  //  0023  MOVE	R6	R8
      0x581C0008,  //  0024  LDCONST	R7	K8
      0x88200110,  //  0025  GETMBR	R8	R0	K16
      0x14200C08,  //  0026  LT	R8	R6	R8
      0x78220013,  //  0027  JMPF	R8	#003C
      0x6020000C,  //  0028  GETGBL	R8	G12
      0x8824090D,  //  0029  GETMBR	R9	R4	K13
      0x7C200200,  //  002A  CALL	R8	1
      0x14200E08,  //  002B  LT	R8	R7	R8
      0x7822000E,  //  002C  JMPF	R8	#003C
      0x8820090D,  //  002D  GETMBR	R8	R4	K13
      0x94201007,  //  002E  GETIDX	R8	R8	R7
      0x8C20110E,  //  002F  GETMET	R8	R8	K14
      0x7C200200,  //  0030  CALL	R8	1
      0x8C20110F,  //  0031  GETMET	R8	R8	K15
      0x7C200200,  //  0032  CALL	R8	1
      0x00240C08,  //  0033  ADD	R9	R6	R8
      0x88280110,  //  0034  GETMBR	R10	R0	K16
      0x1424120A,  //  0035  LT	R9	R9	R10
      0x78260002,  //  0036  JMPF	R9	#003A
      0x00180C08,  //  0037  ADD	R6	R6	R8
      0x001C0F08,  //  0038  ADD	R7	R7	K8
      0x70020000,  //  0039  JMP		#003B
      0x70020000,  //  003A  JMP		#003C
      0x7001FFE8,  //  003B  JMP		#0025
      0xB8220200,  //  003C  GETNGBL	R8	K1
      0x8C201102,  //  003D  GETMET	R8	R8	K2
      0x8C280503,  //  003E  GETMET	R10	R2	K3
      0x58300011,  //  003F  LDCONST	R12	K17
      0x8C340112,  //  0040  GETMET	R13	R0	K18
      0x7C340200,  //  0041  CALL	R13	1
      0x5C380E00,  //  0042  MOVE	R14	R7
      0x5C3C0C00,  //  0043  MOVE	R15	R6
      0x6040000C,  //  0044  GETGBL	R16	G12
      0x8844090D,  //  0045  GETMBR	R17	R4	K13
      0x7C400200,  //  0046  CALL	R16	1
      0x7C280C00,  //  0047  CALL	R10	6
      0x582C000A,  //  0048  LDCONST	R11	K10
      0x7C200600,  //  0049  CALL	R8	3
      0x40200F13,  //  004A  CONNECT	R8	R7	K19
      0x8824090D,  //  004B  GETMBR	R9	R4	K13
      0x94201208,  //  004C  GETIDX	R8	R9	R8
      0x04280F08,  //  004D  SUB	R10	R7	K8
      0x402A120A,  //  004E  CONNECT	R10	K9	R10
      0x882C090D,  //  004F  GETMBR	R11	R4	K13
      0x9428160A,  //  0050  GETIDX	R10	R11	R10
      0x90121A0A,  //  0051  SETMBR	R4	K13	R10
      0x6028000C,  //  0052  GETGBL	R10	G12
      0x5C2C1000,  //  0053  MOVE	R11	R8
      0x7C280200,  //  0054  CALL	R10	1
      0x24281509,  //  0055  GT	R10	R10	K9
      0x9012180A,  //  0056  SETMBR	R4	K12	R10
      0x78160008,  //  0057  JMPF	R5	#0061
      0xB82A0200,  //  0058  GETNGBL	R10	K1
      0x8C281502,  //  0059  GETMET	R10	R10	K2
      0x8C300503,  //  005A  GETMET	R12	R2	K3
      0x58380014,  //  005B  LDCONST	R14	K20
      0x8C3C0112,  //  005C  GETMET	R15	R0	K18
      0x7C3C0200,  //  005D  CALL	R15	1
      0x7C300600,  //  005E  CALL	R12	3
      0x5834000A,  //  005F  LDCONST	R13	K10
      0x7C280600,  //  0060  CALL	R10	3
      0x8824090C,  //  0061  GETMBR	R9	R4	K12
      0x7826000A,  //  0062  JMPF	R9	#006E
      0x5C240A00,  //  0063  MOVE	R9	R5
      0x74260008,  //  0064  JMPT	R9	#006E
      0xB8260200,  //  0065  GETNGBL	R9	K1
      0x8C241302,  //  0066  GETMET	R9	R9	K2
      0x8C2C0503,  //  0067  GETMET	R11	R2	K3
      0x58340015,  //  0068  LDCONST	R13	K21
      0x8C380112,  //  0069  GETMET	R14	R0	K18
      0x7C380200,  //  006A  CALL	R14	1
      0x7C2C0600,  //  006B  CALL	R11	3
      0x5830000A,  //  006C  LDCONST	R12	K10
      0x7C240600,  //  006D  CALL	R9	3
      0x8824010B,  //  006E  GETMBR	R9	R0	K11
      0x8C24130E,  //  006F  GETMET	R9	R9	K14
      0x7C240200,  //  0070  CALL	R9	1
      0x8C281316,  //  0071  GETMET	R10	R9	K22
      0x60300015,  //  0072  GETGBL	R12	G21
      0x88340110,  //  0073  GETMBR	R13	R0	K16
      0x7C300200,  //  0074  CALL	R12	1
      0x7C280400,  //  0075  CALL	R10	2
      0x8C2C0717,  //  0076  GETMET	R11	R3	K23
      0x5C341400,  //  0077  MOVE	R13	R10
      0x7C2C0400,  //  0078  CALL	R11	2
      0x8C2C0718,  //  0079  GETMET	R11	R3	K24
      0x7C2C0200,  //  007A  CALL	R11	1
      0xB82E0200,  //  007B  GETNGBL	R11	K1
      0x8C2C1702,  //  007C  GETMET	R11	R11	K2
      0x8C340503,  //  007D  GETMET	R13	R2	K3
      0x583C0019,  //  007E  LDCONST	R15	K25
      0x8840071A,  //  007F  GETMBR	R16	R3	K26
      0x8840211B,  //  0080  GETMBR	R16	R16	K27
      0x8844071C,  //  0081  GETMBR	R17	R3	K28
      0x88480706,  //  0082  GETMBR	R18	R3	K6
      0x884C071D,  //  0083  GETMBR	R19	R3	K29
      0x7C340C00,  //  0084  CALL	R13	6
      0x5838000A,  //  0085  LDCONST	R14	K10
      0x7C2C0600,  //  0086  CALL	R11	3
      0x8C2C031E,  //  0087  GETMET	R11	R1	K30
      0x5C340600,  //  0088  MOVE	R13	R3
      0x7C2C0400,  //  0089  CALL	R11	2
      0x882C071C,  //  008A  GETMBR	R11	R3	K28
      0x90023E0B,  //  008B  SETMBR	R0	K31	R11
      0x602C000C,  //  008C  GETGBL	R11	G12
      0x5C301000,  //  008D  MOVE	R12	R8
      0x7C2C0200,  //  008E  CALL	R11	1
      0x242C1709,  //  008F  GT	R11	R11	K9
      0x782E0010,  //  0090  JMPF	R11	#00A2
      0x90121A08,  //  0091  SETMBR	R4	K13	R8
      0xB82E0200,  //  0092  GETNGBL	R11	K1
      0x8C2C1702,  //  0093  GETMET	R11	R11	K2
      0x8C340503,  //  0094  GETMET	R13	R2	K3
      0x583C0020,  //  0095  LDCONST	R15	K32
      0x6040000C,  //  0096  GETGBL	R16	G12
      0x8844090D,  //  0097  GETMBR	R17	R4	K13
      0x7C400200,  //  0098  CALL	R16	1
      0x88440706,  //  0099  GETMBR	R17	R3	K6
      0x7C340800,  //  009A  CALL	R13	4
      0x5838000A,  //  009B  LDCONST	R14	K10
      0x7C2C0600,  //  009C  CALL	R11	3
      0x502C0000,  //  009D  LDBOOL	R11	0	0
      0x90020E0B,  //  009E  SETMBR	R0	K7	R11
      0x502C0000,  //  009F  LDBOOL	R11	0	0
      0x80041600,  //  00A0  RET	1	R11
      0x70020001,  //  00A1  JMP		#00A4
      0x502C0200,  //  00A2  LDBOOL	R11	1	0
      0x80041600,  //  00A3  RET	1	R11
      0x80000000,  //  00A4  RET	0
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
    ( &(const bvalue[27]) {     /* constants */
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
    /* K13  */  be_nested_str_weak(data),
    /* K14  */  be_nested_str_weak(attribute_reports),
    /* K15  */  be_nested_str_weak(report_data_phase),
    /* K16  */  be_nested_str_weak(send_im),
    /* K17  */  be_nested_str_weak(build_standalone_ack),
    /* K18  */  be_nested_str_weak(encode_frame),
    /* K19  */  be_nested_str_weak(encrypt),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20_X3CAck_X20_X20_X20_X20_X20_X20_X20_X28_X256i_X29_X20ack_X3D_X25i_X20id_X3D_X25i),
    /* K21  */  be_nested_str_weak(session),
    /* K22  */  be_nested_str_weak(local_session_id),
    /* K23  */  be_nested_str_weak(ack_message_counter),
    /* K24  */  be_nested_str_weak(message_counter),
    /* K25  */  be_nested_str_weak(send_response_frame),
    /* K26  */  be_nested_str_weak(last_counter),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[87]) {  /* code */
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
      0x600C000C,  //  0015  GETGBL	R3	G12
      0x8810010D,  //  0016  GETMBR	R4	R0	K13
      0x8810090E,  //  0017  GETMBR	R4	R4	K14
      0x7C0C0200,  //  0018  CALL	R3	1
      0x240C070B,  //  0019  GT	R3	R3	K11
      0x780E002B,  //  001A  JMPF	R3	#0047
      0x880C010F,  //  001B  GETMBR	R3	R0	K15
      0x780E000E,  //  001C  JMPF	R3	#002C
      0x600C0003,  //  001D  GETGBL	R3	G3
      0x5C100000,  //  001E  MOVE	R4	R0
      0x7C0C0200,  //  001F  CALL	R3	1
      0x8C0C0710,  //  0020  GETMET	R3	R3	K16
      0x5C140200,  //  0021  MOVE	R5	R1
      0x7C0C0400,  //  0022  CALL	R3	2
      0x5C100600,  //  0023  MOVE	R4	R3
      0x74120001,  //  0024  JMPT	R4	#0027
      0x50100000,  //  0025  LDBOOL	R4	0	0
      0x80040800,  //  0026  RET	1	R4
      0x50100000,  //  0027  LDBOOL	R4	0	0
      0x90021E04,  //  0028  SETMBR	R0	K15	R4
      0x50100000,  //  0029  LDBOOL	R4	0	0
      0x80040800,  //  002A  RET	1	R4
      0x70020019,  //  002B  JMP		#0046
      0x880C0107,  //  002C  GETMBR	R3	R0	K7
      0x8C0C0711,  //  002D  GETMET	R3	R3	K17
      0x50140000,  //  002E  LDBOOL	R5	0	0
      0x7C0C0400,  //  002F  CALL	R3	2
      0x8C100712,  //  0030  GETMET	R4	R3	K18
      0x7C100200,  //  0031  CALL	R4	1
      0x8C100713,  //  0032  GETMET	R4	R3	K19
      0x7C100200,  //  0033  CALL	R4	1
      0xB8120200,  //  0034  GETNGBL	R4	K1
      0x8C100902,  //  0035  GETMET	R4	R4	K2
      0x8C180503,  //  0036  GETMET	R6	R2	K3
      0x58200014,  //  0037  LDCONST	R8	K20
      0x88240715,  //  0038  GETMBR	R9	R3	K21
      0x88241316,  //  0039  GETMBR	R9	R9	K22
      0x88280717,  //  003A  GETMBR	R10	R3	K23
      0x882C0718,  //  003B  GETMBR	R11	R3	K24
      0x7C180A00,  //  003C  CALL	R6	5
      0x581C000C,  //  003D  LDCONST	R7	K12
      0x7C100600,  //  003E  CALL	R4	3
      0x8C100319,  //  003F  GETMET	R4	R1	K25
      0x5C180600,  //  0040  MOVE	R6	R3
      0x7C100400,  //  0041  CALL	R4	2
      0x88100718,  //  0042  GETMBR	R4	R3	K24
      0x90023404,  //  0043  SETMBR	R0	K26	R4
      0x50100200,  //  0044  LDBOOL	R4	1	0
      0x80040800,  //  0045  RET	1	R4
      0x7002000E,  //  0046  JMP		#0056
      0x880C010F,  //  0047  GETMBR	R3	R0	K15
      0x780E000A,  //  0048  JMPF	R3	#0054
      0x600C0003,  //  0049  GETGBL	R3	G3
      0x5C100000,  //  004A  MOVE	R4	R0
      0x7C0C0200,  //  004B  CALL	R3	1
      0x8C0C0710,  //  004C  GETMET	R3	R3	K16
      0x5C140200,  //  004D  MOVE	R5	R1
      0x7C0C0400,  //  004E  CALL	R3	2
      0x500C0000,  //  004F  LDBOOL	R3	0	0
      0x90021E03,  //  0050  SETMBR	R0	K15	R3
      0x500C0000,  //  0051  LDBOOL	R3	0	0
      0x80040600,  //  0052  RET	1	R3
      0x70020001,  //  0053  JMP		#0056
      0x500C0200,  //  0054  LDBOOL	R3	1	0
      0x80040600,  //  0055  RET	1	R3
      0x80000000,  //  0056  RET	0
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
    ( &(const bvalue[27]) {     /* constants */
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
    /* K13  */  be_nested_str_weak(resp),
    /* K14  */  be_nested_str_weak(matter),
    /* K15  */  be_nested_str_weak(SubscribeResponseMessage),
    /* K16  */  be_nested_str_weak(max_interval),
    /* K17  */  be_nested_str_weak(opcode),
    /* K18  */  be_nested_str_weak(encode_frame),
    /* K19  */  be_nested_str_weak(to_TLV),
    /* K20  */  be_nested_str_weak(tlv2raw),
    /* K21  */  be_nested_str_weak(encrypt),
    /* K22  */  be_nested_str_weak(send_response_frame),
    /* K23  */  be_nested_str_weak(last_counter),
    /* K24  */  be_nested_str_weak(message_counter),
    /* K25  */  be_nested_str_weak(MTR_X3A_X20Send_X20SubscribeResponseMessage_X20sub_X3D_X25i_X20id_X3D_X25i),
    /* K26  */  be_nested_str_weak(re_arm),
    }),
    be_str_weak(send_im),
    &be_const_str_solidified,
    ( &(const binstruction[77]) {  /* code */
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
      0x780E000D,  //  0014  JMPF	R3	#0023
      0x600C0003,  //  0015  GETGBL	R3	G3
      0x5C100000,  //  0016  MOVE	R4	R0
      0x7C0C0200,  //  0017  CALL	R3	1
      0x8C0C070C,  //  0018  GETMET	R3	R3	K12
      0x5C140200,  //  0019  MOVE	R5	R1
      0x7C0C0400,  //  001A  CALL	R3	2
      0x780E0001,  //  001B  JMPF	R3	#001E
      0x50100000,  //  001C  LDBOOL	R4	0	0
      0x90021604,  //  001D  SETMBR	R0	K11	R4
      0x50100000,  //  001E  LDBOOL	R4	0	0
      0x90020E04,  //  001F  SETMBR	R0	K7	R4
      0x50100000,  //  0020  LDBOOL	R4	0	0
      0x80040800,  //  0021  RET	1	R4
      0x70020028,  //  0022  JMP		#004C
      0x880C010D,  //  0023  GETMBR	R3	R0	K13
      0xB8121C00,  //  0024  GETNGBL	R4	K14
      0x8C10090F,  //  0025  GETMET	R4	R4	K15
      0x7C100200,  //  0026  CALL	R4	1
      0x88140105,  //  0027  GETMBR	R5	R0	K5
      0x88140B06,  //  0028  GETMBR	R5	R5	K6
      0x90120C05,  //  0029  SETMBR	R4	K6	R5
      0x88140105,  //  002A  GETMBR	R5	R0	K5
      0x88140B10,  //  002B  GETMBR	R5	R5	K16
      0x90122005,  //  002C  SETMBR	R4	K16	R5
      0x8814010D,  //  002D  GETMBR	R5	R0	K13
      0x541A0003,  //  002E  LDINT	R6	4
      0x90162206,  //  002F  SETMBR	R5	K17	R6
      0x8C140712,  //  0030  GETMET	R5	R3	K18
      0x8C1C0913,  //  0031  GETMET	R7	R4	K19
      0x7C1C0200,  //  0032  CALL	R7	1
      0x8C1C0F14,  //  0033  GETMET	R7	R7	K20
      0x7C1C0200,  //  0034  CALL	R7	1
      0x7C140400,  //  0035  CALL	R5	2
      0x8C140715,  //  0036  GETMET	R5	R3	K21
      0x7C140200,  //  0037  CALL	R5	1
      0x8C140316,  //  0038  GETMET	R5	R1	K22
      0x5C1C0600,  //  0039  MOVE	R7	R3
      0x7C140400,  //  003A  CALL	R5	2
      0x88140718,  //  003B  GETMBR	R5	R3	K24
      0x90022E05,  //  003C  SETMBR	R0	K23	R5
      0xB8160200,  //  003D  GETNGBL	R5	K1
      0x8C140B02,  //  003E  GETMET	R5	R5	K2
      0x8C1C0503,  //  003F  GETMET	R7	R2	K3
      0x58240019,  //  0040  LDCONST	R9	K25
      0x88280105,  //  0041  GETMBR	R10	R0	K5
      0x88281506,  //  0042  GETMBR	R10	R10	K6
      0x882C0718,  //  0043  GETMBR	R11	R3	K24
      0x7C1C0800,  //  0044  CALL	R7	4
      0x5820000A,  //  0045  LDCONST	R8	K10
      0x7C140600,  //  0046  CALL	R5	3
      0x88140105,  //  0047  GETMBR	R5	R0	K5
      0x8C140B1A,  //  0048  GETMET	R5	R5	K26
      0x7C140200,  //  0049  CALL	R5	1
      0x50140200,  //  004A  LDBOOL	R5	1	0
      0x80040A00,  //  004B  RET	1	R5
      0x80000000,  //  004C  RET	0
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
