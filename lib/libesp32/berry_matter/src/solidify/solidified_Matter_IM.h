/* Solidification of Matter_IM.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Response_container;

/********************************************************************
** Solidified function: tostring
********************************************************************/
be_local_closure(Matter_Response_container_tostring,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
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
    /* K12  */  be_nested_str_weak(Exception_X3E_X20),
    /* K13  */  be_nested_str_weak(_X2C_X20),
    }),
    be_str_weak(tostring),
    &be_const_str_solidified,
    ( &(const binstruction[66]) {  /* code */
      0xA8020031,  //  0000  EXBLK	0	#0033
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
      0x88180109,  //  002A  GETMBR	R6	R0	K9
      0x7C0C0600,  //  002B  CALL	R3	3
      0x70020000,  //  002C  JMP		#002E
      0x580C0001,  //  002D  LDCONST	R3	K1
      0x00080403,  //  002E  ADD	R2	R2	R3
      0xA8040001,  //  002F  EXBLK	1	1
      0x80040400,  //  0030  RET	1	R2
      0xA8040001,  //  0031  EXBLK	1	1
      0x7002000D,  //  0032  JMP		#0041
      0xAC040002,  //  0033  CATCH	R1	0	2
      0x7002000A,  //  0034  JMP		#0040
      0x600C0008,  //  0035  GETGBL	R3	G8
      0x5C100200,  //  0036  MOVE	R4	R1
      0x7C0C0200,  //  0037  CALL	R3	1
      0x000E1803,  //  0038  ADD	R3	K12	R3
      0x000C070D,  //  0039  ADD	R3	R3	K13
      0x60100008,  //  003A  GETGBL	R4	G8
      0x5C140400,  //  003B  MOVE	R5	R2
      0x7C100200,  //  003C  CALL	R4	1
      0x000C0604,  //  003D  ADD	R3	R3	R4
      0x80040600,  //  003E  RET	1	R3
      0x70020000,  //  003F  JMP		#0041
      0xB0080000,  //  0040  RAISE	2	R0	R0
      0x80000000,  //  0041  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Response_container
********************************************************************/
be_local_class(Matter_Response_container,
    5,
    NULL,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(tostring, -1), be_const_closure(Matter_Response_container_tostring_closure) },
        { be_const_key_weak(cluster, 3), be_const_var(1) },
        { be_const_key_weak(command, -1), be_const_var(3) },
        { be_const_key_weak(status, 0), be_const_var(4) },
        { be_const_key_weak(endpoint, -1), be_const_var(0) },
        { be_const_key_weak(attribute, -1), be_const_var(2) },
    })),
    be_str_weak(Matter_Response_container)
);
/*******************************************************************/

void be_load_Matter_Response_container_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Response_container);
    be_setglobal(vm, "Matter_Response_container");
    be_pop(vm, 1);
}

extern const bclass be_class_Matter_IM;

/********************************************************************
** Solidified function: process_timed_request
********************************************************************/
be_local_closure(Matter_IM_process_timed_request,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[26]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TimedRequestMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20received_X20TimedRequestMessage_X3D),
    /* K7   */  be_const_int(3),
    /* K8   */  be_nested_str_weak(format),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_IM_X20_X20_X20TimedRequest_X3D_X25i_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K10  */  be_nested_str_weak(timeout),
    /* K11  */  be_nested_str_weak(remote_ip),
    /* K12  */  be_nested_str_weak(remote_port),
    /* K13  */  be_const_int(2),
    /* K14  */  be_nested_str_weak(StatusResponseMessage),
    /* K15  */  be_nested_str_weak(status),
    /* K16  */  be_nested_str_weak(SUCCESS),
    /* K17  */  be_nested_str_weak(build_response),
    /* K18  */  be_const_int(1),
    /* K19  */  be_nested_str_weak(encode),
    /* K20  */  be_nested_str_weak(to_TLV),
    /* K21  */  be_nested_str_weak(encrypt),
    /* K22  */  be_nested_str_weak(responder),
    /* K23  */  be_nested_str_weak(send_response),
    /* K24  */  be_nested_str_weak(raw),
    /* K25  */  be_nested_str_weak(message_counter),
    }),
    be_str_weak(process_timed_request),
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0xB8160800,  //  0007  GETNGBL	R5	K4
      0x8C140B05,  //  0008  GETMET	R5	R5	K5
      0x601C0008,  //  0009  GETGBL	R7	G8
      0x5C200800,  //  000A  MOVE	R8	R4
      0x7C1C0200,  //  000B  CALL	R7	1
      0x001E0C07,  //  000C  ADD	R7	K6	R7
      0x58200007,  //  000D  LDCONST	R8	K7
      0x7C140600,  //  000E  CALL	R5	3
      0xB8160800,  //  000F  GETNGBL	R5	K4
      0x8C140B05,  //  0010  GETMET	R5	R5	K5
      0x8C1C0708,  //  0011  GETMET	R7	R3	K8
      0x58240009,  //  0012  LDCONST	R9	K9
      0x8828090A,  //  0013  GETMBR	R10	R4	K10
      0x882C030B,  //  0014  GETMBR	R11	R1	K11
      0x8830030C,  //  0015  GETMBR	R12	R1	K12
      0x7C1C0A00,  //  0016  CALL	R7	5
      0x5820000D,  //  0017  LDCONST	R8	K13
      0x7C140600,  //  0018  CALL	R5	3
      0xB8160200,  //  0019  GETNGBL	R5	K1
      0x8C140B0E,  //  001A  GETMET	R5	R5	K14
      0x7C140200,  //  001B  CALL	R5	1
      0xB81A0200,  //  001C  GETNGBL	R6	K1
      0x88180D10,  //  001D  GETMBR	R6	R6	K16
      0x90161E06,  //  001E  SETMBR	R5	K15	R6
      0x8C180311,  //  001F  GETMET	R6	R1	K17
      0x58200012,  //  0020  LDCONST	R8	K18
      0x50240200,  //  0021  LDBOOL	R9	1	0
      0x7C180600,  //  0022  CALL	R6	3
      0x8C1C0D13,  //  0023  GETMET	R7	R6	K19
      0x8C240B14,  //  0024  GETMET	R9	R5	K20
      0x7C240200,  //  0025  CALL	R9	1
      0x8C241313,  //  0026  GETMET	R9	R9	K19
      0x7C240200,  //  0027  CALL	R9	1
      0x7C1C0400,  //  0028  CALL	R7	2
      0x8C1C0D15,  //  0029  GETMET	R7	R6	K21
      0x7C1C0200,  //  002A  CALL	R7	1
      0x881C0116,  //  002B  GETMBR	R7	R0	K22
      0x8C1C0F17,  //  002C  GETMET	R7	R7	K23
      0x88240D18,  //  002D  GETMBR	R9	R6	K24
      0x8828030B,  //  002E  GETMBR	R10	R1	K11
      0x882C030C,  //  002F  GETMBR	R11	R1	K12
      0x88300D19,  //  0030  GETMBR	R12	R6	K25
      0x7C1C0A00,  //  0031  CALL	R7	5
      0x501C0200,  //  0032  LDBOOL	R7	1	0
      0x80040E00,  //  0033  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_status_response
********************************************************************/
be_local_closure(Matter_IM_process_status_response,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(findsubval),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20Status_X20Response_X20_X3D_X200x_X2502X),
    /* K7   */  be_const_int(3),
    }),
    be_str_weak(process_status_response),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x8C100501,  //  0001  GETMET	R4	R2	K1
      0x58180002,  //  0002  LDCONST	R6	K2
      0x541E00FE,  //  0003  LDINT	R7	255
      0x7C100600,  //  0004  CALL	R4	3
      0xB8160600,  //  0005  GETNGBL	R5	K3
      0x8C140B04,  //  0006  GETMET	R5	R5	K4
      0x8C1C0705,  //  0007  GETMET	R7	R3	K5
      0x58240006,  //  0008  LDCONST	R9	K6
      0x5C280800,  //  0009  MOVE	R10	R4
      0x7C1C0600,  //  000A  CALL	R7	3
      0x58200007,  //  000B  LDCONST	R8	K7
      0x7C140600,  //  000C  CALL	R5	3
      0x50140200,  //  000D  LDBOOL	R5	1	0
      0x80040A00,  //  000E  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_IM_every_second,   /* name */
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
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: report_data
********************************************************************/
be_local_closure(Matter_IM_report_data,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(ReportDataMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20received_X20ReportDataMessage_X3D),
    /* K7   */  be_const_int(3),
    }),
    be_str_weak(report_data),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0xB8160800,  //  0007  GETNGBL	R5	K4
      0x8C140B05,  //  0008  GETMET	R5	R5	K5
      0x601C0008,  //  0009  GETGBL	R7	G8
      0x5C200800,  //  000A  MOVE	R8	R4
      0x7C1C0200,  //  000B  CALL	R7	1
      0x001E0C07,  //  000C  ADD	R7	K6	R7
      0x58200007,  //  000D  LDCONST	R8	K7
      0x7C140600,  //  000E  CALL	R5	3
      0x50140000,  //  000F  LDBOOL	R5	0	0
      0x80040A00,  //  0010  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_invoke_response
********************************************************************/
be_local_closure(Matter_IM_process_invoke_response,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(InvokeResponseMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20received_X20InvokeResponseMessage_X3D),
    /* K7   */  be_const_int(3),
    }),
    be_str_weak(process_invoke_response),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0xB8160800,  //  0007  GETNGBL	R5	K4
      0x8C140B05,  //  0008  GETMET	R5	R5	K5
      0x601C0008,  //  0009  GETGBL	R7	G8
      0x5C200800,  //  000A  MOVE	R8	R4
      0x7C1C0200,  //  000B  CALL	R7	1
      0x001E0C07,  //  000C  ADD	R7	K6	R7
      0x58200007,  //  000D  LDCONST	R8	K7
      0x7C140600,  //  000E  CALL	R5	3
      0x50140000,  //  000F  LDBOOL	R5	0	0
      0x80040A00,  //  0010  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_read_request
********************************************************************/
be_local_closure(Matter_IM_process_read_request,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        18,                          /* nstack */
        4,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 1),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[32]) {     /* constants */
        /* K0   */  be_nested_str_weak(string),
        /* K1   */  be_nested_str_weak(matter),
        /* K2   */  be_nested_str_weak(get_attribute_name),
        /* K3   */  be_nested_str_weak(cluster),
        /* K4   */  be_nested_str_weak(attribute),
        /* K5   */  be_nested_str_weak(_X20_X28),
        /* K6   */  be_nested_str_weak(_X29),
        /* K7   */  be_nested_str_weak(),
        /* K8   */  be_nested_str_weak(read_attribute),
        /* K9   */  be_nested_str_weak(AttributeReportIB),
        /* K10  */  be_nested_str_weak(attribute_data),
        /* K11  */  be_nested_str_weak(AttributeDataIB),
        /* K12  */  be_nested_str_weak(data_version),
        /* K13  */  be_const_int(1),
        /* K14  */  be_nested_str_weak(path),
        /* K15  */  be_nested_str_weak(AttributePathIB),
        /* K16  */  be_nested_str_weak(endpoint),
        /* K17  */  be_nested_str_weak(data),
        /* K18  */  be_nested_str_weak(attribute_reports),
        /* K19  */  be_nested_str_weak(push),
        /* K20  */  be_nested_str_weak(tasmota),
        /* K21  */  be_nested_str_weak(log),
        /* K22  */  be_nested_str_weak(format),
        /* K23  */  be_nested_str_weak(MTR_X3A_X20Read_Attr_X20_X25s_X25s_X20_X2D_X20_X25s),
        /* K24  */  be_const_int(2),
        /* K25  */  be_nested_str_weak(status),
        /* K26  */  be_nested_str_weak(attribute_status),
        /* K27  */  be_nested_str_weak(AttributeStatusIB),
        /* K28  */  be_nested_str_weak(StatusIB),
        /* K29  */  be_nested_str_weak(MTR_X3A_X20Read_Attr_X20_X25s_X25s_X20_X2D_X20STATUS_X3A_X200x_X2502X_X20_X25s),
        /* K30  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
        /* K31  */  be_nested_str_weak(MTR_X3A_X20Read_Attr_X20_X25s_X25s_X20_X2D_X20IGNORED),
        }),
        be_str_weak(read_single_attribute),
        &be_const_str_solidified,
        ( &(const binstruction[150]) {  /* code */
          0xA4120000,  //  0000  IMPORT	R4	K0
          0xB8160200,  //  0001  GETNGBL	R5	K1
          0x8C140B02,  //  0002  GETMET	R5	R5	K2
          0x881C0503,  //  0003  GETMBR	R7	R2	K3
          0x88200504,  //  0004  GETMBR	R8	R2	K4
          0x7C140600,  //  0005  CALL	R5	3
          0x78160002,  //  0006  JMPF	R5	#000A
          0x001A0A05,  //  0007  ADD	R6	K5	R5
          0x00180D06,  //  0008  ADD	R6	R6	K6
          0x70020000,  //  0009  JMP		#000B
          0x58180007,  //  000A  LDCONST	R6	K7
          0x5C140C00,  //  000B  MOVE	R5	R6
          0x4C180000,  //  000C  LDNIL	R6
          0x20180206,  //  000D  NE	R6	R1	R6
          0x781A0004,  //  000E  JMPF	R6	#0014
          0x8C180308,  //  000F  GETMET	R6	R1	K8
          0x68200000,  //  0010  GETUPV	R8	U0
          0x5C240400,  //  0011  MOVE	R9	R2
          0x7C180600,  //  0012  CALL	R6	3
          0x70020000,  //  0013  JMP		#0015
          0x4C180000,  //  0014  LDNIL	R6
          0x4C1C0000,  //  0015  LDNIL	R7
          0x201C0C07,  //  0016  NE	R7	R6	R7
          0x781E0030,  //  0017  JMPF	R7	#0049
          0xB81E0200,  //  0018  GETNGBL	R7	K1
          0x8C1C0F09,  //  0019  GETMET	R7	R7	K9
          0x7C1C0200,  //  001A  CALL	R7	1
          0xB8220200,  //  001B  GETNGBL	R8	K1
          0x8C20110B,  //  001C  GETMET	R8	R8	K11
          0x7C200200,  //  001D  CALL	R8	1
          0x901E1408,  //  001E  SETMBR	R7	K10	R8
          0x88200F0A,  //  001F  GETMBR	R8	R7	K10
          0x9022190D,  //  0020  SETMBR	R8	K12	K13
          0x88200F0A,  //  0021  GETMBR	R8	R7	K10
          0xB8260200,  //  0022  GETNGBL	R9	K1
          0x8C24130F,  //  0023  GETMET	R9	R9	K15
          0x7C240200,  //  0024  CALL	R9	1
          0x90221C09,  //  0025  SETMBR	R8	K14	R9
          0x88200F0A,  //  0026  GETMBR	R8	R7	K10
          0x8820110E,  //  0027  GETMBR	R8	R8	K14
          0x88240510,  //  0028  GETMBR	R9	R2	K16
          0x90222009,  //  0029  SETMBR	R8	K16	R9
          0x88200F0A,  //  002A  GETMBR	R8	R7	K10
          0x8820110E,  //  002B  GETMBR	R8	R8	K14
          0x88240503,  //  002C  GETMBR	R9	R2	K3
          0x90220609,  //  002D  SETMBR	R8	K3	R9
          0x88200F0A,  //  002E  GETMBR	R8	R7	K10
          0x8820110E,  //  002F  GETMBR	R8	R8	K14
          0x88240504,  //  0030  GETMBR	R9	R2	K4
          0x90220809,  //  0031  SETMBR	R8	K4	R9
          0x88200F0A,  //  0032  GETMBR	R8	R7	K10
          0x90222206,  //  0033  SETMBR	R8	K17	R6
          0x88200112,  //  0034  GETMBR	R8	R0	K18
          0x8C201113,  //  0035  GETMET	R8	R8	K19
          0x5C280E00,  //  0036  MOVE	R10	R7
          0x7C200400,  //  0037  CALL	R8	2
          0xB8222800,  //  0038  GETNGBL	R8	K20
          0x8C201115,  //  0039  GETMET	R8	R8	K21
          0x8C280916,  //  003A  GETMET	R10	R4	K22
          0x58300017,  //  003B  LDCONST	R12	K23
          0x60340008,  //  003C  GETGBL	R13	G8
          0x5C380400,  //  003D  MOVE	R14	R2
          0x7C340200,  //  003E  CALL	R13	1
          0x5C380A00,  //  003F  MOVE	R14	R5
          0x603C0008,  //  0040  GETGBL	R15	G8
          0x5C400C00,  //  0041  MOVE	R16	R6
          0x7C3C0200,  //  0042  CALL	R15	1
          0x7C280A00,  //  0043  CALL	R10	5
          0x582C0018,  //  0044  LDCONST	R11	K24
          0x7C200600,  //  0045  CALL	R8	3
          0x50200200,  //  0046  LDBOOL	R8	1	0
          0x80041000,  //  0047  RET	1	R8
          0x7002004B,  //  0048  JMP		#0095
          0x881C0519,  //  0049  GETMBR	R7	R2	K25
          0x4C200000,  //  004A  LDNIL	R8
          0x201C0E08,  //  004B  NE	R7	R7	R8
          0x781E003C,  //  004C  JMPF	R7	#008A
          0x780E003A,  //  004D  JMPF	R3	#0089
          0xB81E0200,  //  004E  GETNGBL	R7	K1
          0x8C1C0F09,  //  004F  GETMET	R7	R7	K9
          0x7C1C0200,  //  0050  CALL	R7	1
          0xB8220200,  //  0051  GETNGBL	R8	K1
          0x8C20111B,  //  0052  GETMET	R8	R8	K27
          0x7C200200,  //  0053  CALL	R8	1
          0x901E3408,  //  0054  SETMBR	R7	K26	R8
          0x88200F1A,  //  0055  GETMBR	R8	R7	K26
          0xB8260200,  //  0056  GETNGBL	R9	K1
          0x8C24130F,  //  0057  GETMET	R9	R9	K15
          0x7C240200,  //  0058  CALL	R9	1
          0x90221C09,  //  0059  SETMBR	R8	K14	R9
          0x88200F1A,  //  005A  GETMBR	R8	R7	K26
          0xB8260200,  //  005B  GETNGBL	R9	K1
          0x8C24131C,  //  005C  GETMET	R9	R9	K28
          0x7C240200,  //  005D  CALL	R9	1
          0x90223209,  //  005E  SETMBR	R8	K25	R9
          0x88200F1A,  //  005F  GETMBR	R8	R7	K26
          0x8820110E,  //  0060  GETMBR	R8	R8	K14
          0x88240510,  //  0061  GETMBR	R9	R2	K16
          0x90222009,  //  0062  SETMBR	R8	K16	R9
          0x88200F1A,  //  0063  GETMBR	R8	R7	K26
          0x8820110E,  //  0064  GETMBR	R8	R8	K14
          0x88240503,  //  0065  GETMBR	R9	R2	K3
          0x90220609,  //  0066  SETMBR	R8	K3	R9
          0x88200F1A,  //  0067  GETMBR	R8	R7	K26
          0x8820110E,  //  0068  GETMBR	R8	R8	K14
          0x88240504,  //  0069  GETMBR	R9	R2	K4
          0x90220809,  //  006A  SETMBR	R8	K4	R9
          0x88200F1A,  //  006B  GETMBR	R8	R7	K26
          0x88201119,  //  006C  GETMBR	R8	R8	K25
          0x88240519,  //  006D  GETMBR	R9	R2	K25
          0x90223209,  //  006E  SETMBR	R8	K25	R9
          0x88200112,  //  006F  GETMBR	R8	R0	K18
          0x8C201113,  //  0070  GETMET	R8	R8	K19
          0x5C280E00,  //  0071  MOVE	R10	R7
          0x7C200400,  //  0072  CALL	R8	2
          0xB8222800,  //  0073  GETNGBL	R8	K20
          0x8C201115,  //  0074  GETMET	R8	R8	K21
          0x8C280916,  //  0075  GETMET	R10	R4	K22
          0x5830001D,  //  0076  LDCONST	R12	K29
          0x60340008,  //  0077  GETGBL	R13	G8
          0x5C380400,  //  0078  MOVE	R14	R2
          0x7C340200,  //  0079  CALL	R13	1
          0x5C380A00,  //  007A  MOVE	R14	R5
          0x883C0519,  //  007B  GETMBR	R15	R2	K25
          0x88400519,  //  007C  GETMBR	R16	R2	K25
          0xB8460200,  //  007D  GETNGBL	R17	K1
          0x8844231E,  //  007E  GETMBR	R17	R17	K30
          0x1C402011,  //  007F  EQ	R16	R16	R17
          0x78420001,  //  0080  JMPF	R16	#0083
          0x5840001E,  //  0081  LDCONST	R16	K30
          0x70020000,  //  0082  JMP		#0084
          0x58400007,  //  0083  LDCONST	R16	K7
          0x7C280C00,  //  0084  CALL	R10	6
          0x582C0018,  //  0085  LDCONST	R11	K24
          0x7C200600,  //  0086  CALL	R8	3
          0x50200200,  //  0087  LDBOOL	R8	1	0
          0x80041000,  //  0088  RET	1	R8
          0x7002000A,  //  0089  JMP		#0095
          0xB81E2800,  //  008A  GETNGBL	R7	K20
          0x8C1C0F15,  //  008B  GETMET	R7	R7	K21
          0x8C240916,  //  008C  GETMET	R9	R4	K22
          0x582C001F,  //  008D  LDCONST	R11	K31
          0x60300008,  //  008E  GETGBL	R12	G8
          0x5C340400,  //  008F  MOVE	R13	R2
          0x7C300200,  //  0090  CALL	R12	1
          0x5C340A00,  //  0091  MOVE	R13	R5
          0x7C240800,  //  0092  CALL	R9	4
          0x58280018,  //  0093  LDCONST	R10	K24
          0x7C1C0600,  //  0094  CALL	R7	3
          0x80000000,  //  0095  RET	0
        })
      ),
      be_nested_proto(
        8,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 4),
          be_local_const_upval(1, 7),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 7]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x68100001,  //  0001  GETUPV	R4	U1
          0x5C140000,  //  0002  MOVE	R5	R0
          0x5C180200,  //  0003  MOVE	R6	R1
          0x5C1C0400,  //  0004  MOVE	R7	R2
          0x7C0C0800,  //  0005  CALL	R3	4
          0x80040600,  //  0006  RET	1	R3
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(get_active_endpoints),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(log),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20IM_X3Aread_request_X20processing_X20start),
    /* K5   */  be_const_int(3),
    /* K6   */  be_nested_str_weak(matter),
    /* K7   */  be_nested_str_weak(Response_container),
    /* K8   */  be_nested_str_weak(ReadRequestMessage),
    /* K9   */  be_nested_str_weak(from_TLV),
    /* K10  */  be_nested_str_weak(attributes_requests),
    /* K11  */  be_nested_str_weak(ReportDataMessage),
    /* K12  */  be_nested_str_weak(attribute_reports),
    /* K13  */  be_nested_str_weak(endpoint),
    /* K14  */  be_nested_str_weak(cluster),
    /* K15  */  be_nested_str_weak(attribute),
    /* K16  */  be_nested_str_weak(status),
    /* K17  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K18  */  be_nested_str_weak(get_attribute_name),
    /* K19  */  be_nested_str_weak(MTR_X3A_X20Read_Attr_X20),
    /* K20  */  be_nested_str_weak(_X20_X28),
    /* K21  */  be_nested_str_weak(_X29),
    /* K22  */  be_nested_str_weak(),
    /* K23  */  be_const_int(2),
    /* K24  */  be_nested_str_weak(process_attribute_expansion),
    /* K25  */  be_nested_str_weak(stop_iteration),
    /* K26  */  be_nested_str_weak(MTR_X3A_X20ReportDataMessage_X3D),
    /* K27  */  be_nested_str_weak(MTR_X3A_X20ReportDataMessageTLV_X3D),
    /* K28  */  be_nested_str_weak(to_TLV),
    /* K29  */  be_nested_str_weak(send_attr_report),
    }),
    be_str_weak(process_read_request),
    &be_const_str_solidified,
    ( &(const binstruction[125]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x84100000,  //  0003  CLOSURE	R4	P0
      0xB8160400,  //  0004  GETNGBL	R5	K2
      0x8C140B03,  //  0005  GETMET	R5	R5	K3
      0x581C0004,  //  0006  LDCONST	R7	K4
      0x58200005,  //  0007  LDCONST	R8	K5
      0x7C140600,  //  0008  CALL	R5	3
      0xB8160C00,  //  0009  GETNGBL	R5	K6
      0x8C140B07,  //  000A  GETMET	R5	R5	K7
      0x7C140200,  //  000B  CALL	R5	1
      0xB81A0C00,  //  000C  GETNGBL	R6	K6
      0x8C180D08,  //  000D  GETMET	R6	R6	K8
      0x7C180200,  //  000E  CALL	R6	1
      0x8C180D09,  //  000F  GETMET	R6	R6	K9
      0x5C200400,  //  0010  MOVE	R8	R2
      0x7C180400,  //  0011  CALL	R6	2
      0x881C0D0A,  //  0012  GETMBR	R7	R6	K10
      0x4C200000,  //  0013  LDNIL	R8
      0x201C0E08,  //  0014  NE	R7	R7	R8
      0x781E0063,  //  0015  JMPF	R7	#007A
      0xB81E0C00,  //  0016  GETNGBL	R7	K6
      0x8C1C0F0B,  //  0017  GETMET	R7	R7	K11
      0x7C1C0200,  //  0018  CALL	R7	1
      0x60200012,  //  0019  GETGBL	R8	G18
      0x7C200000,  //  001A  CALL	R8	0
      0x901E1808,  //  001B  SETMBR	R7	K12	R8
      0x60200010,  //  001C  GETGBL	R8	G16
      0x88240D0A,  //  001D  GETMBR	R9	R6	K10
      0x7C200200,  //  001E  CALL	R8	1
      0xA8020040,  //  001F  EXBLK	0	#0061
      0x5C241000,  //  0020  MOVE	R9	R8
      0x7C240000,  //  0021  CALL	R9	0
      0x8828130D,  //  0022  GETMBR	R10	R9	K13
      0x90161A0A,  //  0023  SETMBR	R5	K13	R10
      0x8828130E,  //  0024  GETMBR	R10	R9	K14
      0x90161C0A,  //  0025  SETMBR	R5	K14	R10
      0x8828130F,  //  0026  GETMBR	R10	R9	K15
      0x90161E0A,  //  0027  SETMBR	R5	K15	R10
      0xB82A0C00,  //  0028  GETNGBL	R10	K6
      0x88281511,  //  0029  GETMBR	R10	R10	K17
      0x9016200A,  //  002A  SETMBR	R5	K16	R10
      0x88280B0D,  //  002B  GETMBR	R10	R5	K13
      0x4C2C0000,  //  002C  LDNIL	R11
      0x1C28140B,  //  002D  EQ	R10	R10	R11
      0x742A0007,  //  002E  JMPT	R10	#0037
      0x88280B0E,  //  002F  GETMBR	R10	R5	K14
      0x4C2C0000,  //  0030  LDNIL	R11
      0x1C28140B,  //  0031  EQ	R10	R10	R11
      0x742A0003,  //  0032  JMPT	R10	#0037
      0x88280B0F,  //  0033  GETMBR	R10	R5	K15
      0x4C2C0000,  //  0034  LDNIL	R11
      0x1C28140B,  //  0035  EQ	R10	R10	R11
      0x782A0023,  //  0036  JMPF	R10	#005B
      0x88280B0E,  //  0037  GETMBR	R10	R5	K14
      0x4C2C0000,  //  0038  LDNIL	R11
      0x2028140B,  //  0039  NE	R10	R10	R11
      0x782A0017,  //  003A  JMPF	R10	#0053
      0x88280B0F,  //  003B  GETMBR	R10	R5	K15
      0x4C2C0000,  //  003C  LDNIL	R11
      0x2028140B,  //  003D  NE	R10	R10	R11
      0x782A0013,  //  003E  JMPF	R10	#0053
      0xB82A0C00,  //  003F  GETNGBL	R10	K6
      0x8C281512,  //  0040  GETMET	R10	R10	K18
      0x88300B0E,  //  0041  GETMBR	R12	R5	K14
      0x88340B0F,  //  0042  GETMBR	R13	R5	K15
      0x7C280600,  //  0043  CALL	R10	3
      0xB82E0400,  //  0044  GETNGBL	R11	K2
      0x8C2C1703,  //  0045  GETMET	R11	R11	K3
      0x60340008,  //  0046  GETGBL	R13	G8
      0x5C380A00,  //  0047  MOVE	R14	R5
      0x7C340200,  //  0048  CALL	R13	1
      0x0036260D,  //  0049  ADD	R13	K19	R13
      0x782A0002,  //  004A  JMPF	R10	#004E
      0x003A280A,  //  004B  ADD	R14	K20	R10
      0x00381D15,  //  004C  ADD	R14	R14	K21
      0x70020000,  //  004D  JMP		#004F
      0x58380016,  //  004E  LDCONST	R14	K22
      0x00341A0E,  //  004F  ADD	R13	R13	R14
      0x58380017,  //  0050  LDCONST	R14	K23
      0x7C2C0600,  //  0051  CALL	R11	3
      0x70020007,  //  0052  JMP		#005B
      0xB82A0400,  //  0053  GETNGBL	R10	K2
      0x8C281503,  //  0054  GETMET	R10	R10	K3
      0x60300008,  //  0055  GETGBL	R12	G8
      0x5C340A00,  //  0056  MOVE	R13	R5
      0x7C300200,  //  0057  CALL	R12	1
      0x0032260C,  //  0058  ADD	R12	K19	R12
      0x58340017,  //  0059  LDCONST	R13	K23
      0x7C280600,  //  005A  CALL	R10	3
      0x88280100,  //  005B  GETMBR	R10	R0	K0
      0x8C281518,  //  005C  GETMET	R10	R10	K24
      0x5C300A00,  //  005D  MOVE	R12	R5
      0x84340001,  //  005E  CLOSURE	R13	P1
      0x7C280600,  //  005F  CALL	R10	3
      0x7001FFBE,  //  0060  JMP		#0020
      0x58200019,  //  0061  LDCONST	R8	K25
      0xAC200200,  //  0062  CATCH	R8	1	0
      0xB0080000,  //  0063  RAISE	2	R0	R0
      0xB8220400,  //  0064  GETNGBL	R8	K2
      0x8C201103,  //  0065  GETMET	R8	R8	K3
      0x60280008,  //  0066  GETGBL	R10	G8
      0x5C2C0E00,  //  0067  MOVE	R11	R7
      0x7C280200,  //  0068  CALL	R10	1
      0x002A340A,  //  0069  ADD	R10	K26	R10
      0x582C0005,  //  006A  LDCONST	R11	K5
      0x7C200600,  //  006B  CALL	R8	3
      0xB8220400,  //  006C  GETNGBL	R8	K2
      0x8C201103,  //  006D  GETMET	R8	R8	K3
      0x60280008,  //  006E  GETGBL	R10	G8
      0x8C2C0F1C,  //  006F  GETMET	R11	R7	K28
      0x7C2C0200,  //  0070  CALL	R11	1
      0x7C280200,  //  0071  CALL	R10	1
      0x002A360A,  //  0072  ADD	R10	K27	R10
      0x582C0005,  //  0073  LDCONST	R11	K5
      0x7C200600,  //  0074  CALL	R8	3
      0x8C20011D,  //  0075  GETMET	R8	R0	K29
      0x5C280200,  //  0076  MOVE	R10	R1
      0x5C2C0E00,  //  0077  MOVE	R11	R7
      0x7C200600,  //  0078  CALL	R8	3
      0xA01C0000,  //  0079  CLOSE	R7
      0x501C0200,  //  007A  LDBOOL	R7	1	0
      0xA0000000,  //  007B  CLOSE	R0
      0x80040E00,  //  007C  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_write_request
********************************************************************/
be_local_closure(Matter_IM_process_write_request,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(WriteRequestMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20received_X20WriteRequestMessage_X3D),
    /* K7   */  be_const_int(3),
    }),
    be_str_weak(process_write_request),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0xB8160800,  //  0007  GETNGBL	R5	K4
      0x8C140B05,  //  0008  GETMET	R5	R5	K5
      0x601C0008,  //  0009  GETGBL	R7	G8
      0x5C200800,  //  000A  MOVE	R8	R4
      0x7C1C0200,  //  000B  CALL	R7	1
      0x001E0C07,  //  000C  ADD	R7	K6	R7
      0x58200007,  //  000D  LDCONST	R8	K7
      0x7C140600,  //  000E  CALL	R5	3
      0x50140000,  //  000F  LDBOOL	R5	0	0
      0x80040A00,  //  0010  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_invoke_request
********************************************************************/
be_local_closure(Matter_IM_process_invoke_request,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[52]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20IM_X3Ainvoke_request_X20processing_X20start),
    /* K4   */  be_const_int(3),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(Response_container),
    /* K7   */  be_nested_str_weak(InvokeRequestMessage),
    /* K8   */  be_nested_str_weak(from_TLV),
    /* K9   */  be_nested_str_weak(invoke_requests),
    /* K10  */  be_nested_str_weak(InvokeResponseMessage),
    /* K11  */  be_nested_str_weak(suppress_response),
    /* K12  */  be_nested_str_weak(invoke_responses),
    /* K13  */  be_nested_str_weak(endpoint),
    /* K14  */  be_nested_str_weak(command_path),
    /* K15  */  be_nested_str_weak(cluster),
    /* K16  */  be_nested_str_weak(command),
    /* K17  */  be_nested_str_weak(status),
    /* K18  */  be_nested_str_weak(UNSUPPORTED_COMMAND),
    /* K19  */  be_nested_str_weak(get_command_name),
    /* K20  */  be_nested_str_weak(format),
    /* K21  */  be_nested_str_weak(0x_X2504X_X2F0x02X),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_cmd_X20_X20_X25s_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K23  */  be_nested_str_weak(remote_ip),
    /* K24  */  be_nested_str_weak(remote_port),
    /* K25  */  be_const_int(2),
    /* K26  */  be_nested_str_weak(responder),
    /* K27  */  be_nested_str_weak(device),
    /* K28  */  be_nested_str_weak(invoke_request),
    /* K29  */  be_nested_str_weak(command_fields),
    /* K30  */  be_nested_str_weak(InvokeResponseIB),
    /* K31  */  be_nested_str_weak(CommandDataIB),
    /* K32  */  be_nested_str_weak(CommandPathIB),
    /* K33  */  be_nested_str_weak(push),
    /* K34  */  be_nested_str_weak(0x_X2504X_X2F0x_X2502X),
    /* K35  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_cmd_X20_X20_X20_X25s),
    /* K36  */  be_nested_str_weak(CommandStatusIB),
    /* K37  */  be_nested_str_weak(StatusIB),
    /* K38  */  be_nested_str_weak(stop_iteration),
    /* K39  */  be_nested_str_weak(MTR_X3A_X20invoke_responses_X3D),
    /* K40  */  be_const_int(0),
    /* K41  */  be_nested_str_weak(MTR_X3A_X20InvokeResponse_X3D),
    /* K42  */  be_nested_str_weak(MTR_X3A_X20InvokeResponseTLV_X3D),
    /* K43  */  be_nested_str_weak(to_TLV),
    /* K44  */  be_nested_str_weak(build_response),
    /* K45  */  be_nested_str_weak(encode),
    /* K46  */  be_nested_str_weak(encrypt),
    /* K47  */  be_nested_str_weak(send_response),
    /* K48  */  be_nested_str_weak(raw),
    /* K49  */  be_nested_str_weak(message_counter),
    /* K50  */  be_nested_str_weak(x_flag_r),
    /* K51  */  be_nested_str_weak(build_standalone_ack),
    }),
    be_str_weak(process_invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[242]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x58180003,  //  0003  LDCONST	R6	K3
      0x581C0004,  //  0004  LDCONST	R7	K4
      0x7C100600,  //  0005  CALL	R4	3
      0xB8120A00,  //  0006  GETNGBL	R4	K5
      0x8C100906,  //  0007  GETMET	R4	R4	K6
      0x7C100200,  //  0008  CALL	R4	1
      0xB8160A00,  //  0009  GETNGBL	R5	K5
      0x8C140B07,  //  000A  GETMET	R5	R5	K7
      0x7C140200,  //  000B  CALL	R5	1
      0x8C140B08,  //  000C  GETMET	R5	R5	K8
      0x5C1C0400,  //  000D  MOVE	R7	R2
      0x7C140400,  //  000E  CALL	R5	2
      0x88180B09,  //  000F  GETMBR	R6	R5	K9
      0x4C1C0000,  //  0010  LDNIL	R7
      0x20180C07,  //  0011  NE	R6	R6	R7
      0x781A00DD,  //  0012  JMPF	R6	#00F1
      0xB81A0A00,  //  0013  GETNGBL	R6	K5
      0x8C180D0A,  //  0014  GETMET	R6	R6	K10
      0x7C180200,  //  0015  CALL	R6	1
      0x501C0000,  //  0016  LDBOOL	R7	0	0
      0x901A1607,  //  0017  SETMBR	R6	K11	R7
      0x601C0012,  //  0018  GETGBL	R7	G18
      0x7C1C0000,  //  0019  CALL	R7	0
      0x901A1807,  //  001A  SETMBR	R6	K12	R7
      0x601C0010,  //  001B  GETGBL	R7	G16
      0x88200B09,  //  001C  GETMBR	R8	R5	K9
      0x7C1C0200,  //  001D  CALL	R7	1
      0xA802008D,  //  001E  EXBLK	0	#00AD
      0x5C200E00,  //  001F  MOVE	R8	R7
      0x7C200000,  //  0020  CALL	R8	0
      0x8824110E,  //  0021  GETMBR	R9	R8	K14
      0x8824130D,  //  0022  GETMBR	R9	R9	K13
      0x90121A09,  //  0023  SETMBR	R4	K13	R9
      0x8824110E,  //  0024  GETMBR	R9	R8	K14
      0x8824130F,  //  0025  GETMBR	R9	R9	K15
      0x90121E09,  //  0026  SETMBR	R4	K15	R9
      0x8824110E,  //  0027  GETMBR	R9	R8	K14
      0x88241310,  //  0028  GETMBR	R9	R9	K16
      0x90122009,  //  0029  SETMBR	R4	K16	R9
      0xB8260A00,  //  002A  GETNGBL	R9	K5
      0x88241312,  //  002B  GETMBR	R9	R9	K18
      0x90122209,  //  002C  SETMBR	R4	K17	R9
      0xB8260A00,  //  002D  GETNGBL	R9	K5
      0x8C241313,  //  002E  GETMET	R9	R9	K19
      0x882C090F,  //  002F  GETMBR	R11	R4	K15
      0x88300910,  //  0030  GETMBR	R12	R4	K16
      0x7C240600,  //  0031  CALL	R9	3
      0x4C280000,  //  0032  LDNIL	R10
      0x1C28120A,  //  0033  EQ	R10	R9	R10
      0x782A0005,  //  0034  JMPF	R10	#003B
      0x8C280714,  //  0035  GETMET	R10	R3	K20
      0x58300015,  //  0036  LDCONST	R12	K21
      0x8834090F,  //  0037  GETMBR	R13	R4	K15
      0x88380910,  //  0038  GETMBR	R14	R4	K16
      0x7C280800,  //  0039  CALL	R10	4
      0x5C241400,  //  003A  MOVE	R9	R10
      0xB82A0200,  //  003B  GETNGBL	R10	K1
      0x8C281502,  //  003C  GETMET	R10	R10	K2
      0x8C300714,  //  003D  GETMET	R12	R3	K20
      0x58380016,  //  003E  LDCONST	R14	K22
      0x5C3C1200,  //  003F  MOVE	R15	R9
      0x88400317,  //  0040  GETMBR	R16	R1	K23
      0x88440318,  //  0041  GETMBR	R17	R1	K24
      0x7C300A00,  //  0042  CALL	R12	5
      0x58340019,  //  0043  LDCONST	R13	K25
      0x7C280600,  //  0044  CALL	R10	3
      0x8828011A,  //  0045  GETMBR	R10	R0	K26
      0x8828151B,  //  0046  GETMBR	R10	R10	K27
      0x8C28151C,  //  0047  GETMET	R10	R10	K28
      0x5C300200,  //  0048  MOVE	R12	R1
      0x8834111D,  //  0049  GETMBR	R13	R8	K29
      0x5C380800,  //  004A  MOVE	R14	R4
      0x7C280800,  //  004B  CALL	R10	4
      0xB82E0A00,  //  004C  GETNGBL	R11	K5
      0x8C2C171E,  //  004D  GETMET	R11	R11	K30
      0x7C2C0200,  //  004E  CALL	R11	1
      0x4C300000,  //  004F  LDNIL	R12
      0x2030140C,  //  0050  NE	R12	R10	R12
      0x78320032,  //  0051  JMPF	R12	#0085
      0xB8320A00,  //  0052  GETNGBL	R12	K5
      0x8C30191F,  //  0053  GETMET	R12	R12	K31
      0x7C300200,  //  0054  CALL	R12	1
      0x902E200C,  //  0055  SETMBR	R11	K16	R12
      0x88301710,  //  0056  GETMBR	R12	R11	K16
      0xB8360A00,  //  0057  GETNGBL	R13	K5
      0x8C341B20,  //  0058  GETMET	R13	R13	K32
      0x7C340200,  //  0059  CALL	R13	1
      0x90321C0D,  //  005A  SETMBR	R12	K14	R13
      0x88301710,  //  005B  GETMBR	R12	R11	K16
      0x8830190E,  //  005C  GETMBR	R12	R12	K14
      0x8834090D,  //  005D  GETMBR	R13	R4	K13
      0x90321A0D,  //  005E  SETMBR	R12	K13	R13
      0x88301710,  //  005F  GETMBR	R12	R11	K16
      0x8830190E,  //  0060  GETMBR	R12	R12	K14
      0x8834090F,  //  0061  GETMBR	R13	R4	K15
      0x90321E0D,  //  0062  SETMBR	R12	K15	R13
      0x88301710,  //  0063  GETMBR	R12	R11	K16
      0x8830190E,  //  0064  GETMBR	R12	R12	K14
      0x88340910,  //  0065  GETMBR	R13	R4	K16
      0x9032200D,  //  0066  SETMBR	R12	K16	R13
      0x88301710,  //  0067  GETMBR	R12	R11	K16
      0x90323A0A,  //  0068  SETMBR	R12	K29	R10
      0x88300D0C,  //  0069  GETMBR	R12	R6	K12
      0x8C301921,  //  006A  GETMET	R12	R12	K33
      0x5C381600,  //  006B  MOVE	R14	R11
      0x7C300400,  //  006C  CALL	R12	2
      0xB8320A00,  //  006D  GETNGBL	R12	K5
      0x8C301913,  //  006E  GETMET	R12	R12	K19
      0x8838090F,  //  006F  GETMBR	R14	R4	K15
      0x883C0910,  //  0070  GETMBR	R15	R4	K16
      0x7C300600,  //  0071  CALL	R12	3
      0x5C241800,  //  0072  MOVE	R9	R12
      0x4C300000,  //  0073  LDNIL	R12
      0x1C30120C,  //  0074  EQ	R12	R9	R12
      0x78320005,  //  0075  JMPF	R12	#007C
      0x8C300714,  //  0076  GETMET	R12	R3	K20
      0x58380022,  //  0077  LDCONST	R14	K34
      0x883C090F,  //  0078  GETMBR	R15	R4	K15
      0x88400910,  //  0079  GETMBR	R16	R4	K16
      0x7C300800,  //  007A  CALL	R12	4
      0x5C241800,  //  007B  MOVE	R9	R12
      0xB8320200,  //  007C  GETNGBL	R12	K1
      0x8C301902,  //  007D  GETMET	R12	R12	K2
      0x8C380714,  //  007E  GETMET	R14	R3	K20
      0x58400023,  //  007F  LDCONST	R16	K35
      0x5C441200,  //  0080  MOVE	R17	R9
      0x7C380600,  //  0081  CALL	R14	3
      0x583C0019,  //  0082  LDCONST	R15	K25
      0x7C300600,  //  0083  CALL	R12	3
      0x70020026,  //  0084  JMP		#00AC
      0x88300911,  //  0085  GETMBR	R12	R4	K17
      0x4C340000,  //  0086  LDNIL	R13
      0x2030180D,  //  0087  NE	R12	R12	R13
      0x78320022,  //  0088  JMPF	R12	#00AC
      0xB8320A00,  //  0089  GETNGBL	R12	K5
      0x8C301924,  //  008A  GETMET	R12	R12	K36
      0x7C300200,  //  008B  CALL	R12	1
      0x902E220C,  //  008C  SETMBR	R11	K17	R12
      0x88301711,  //  008D  GETMBR	R12	R11	K17
      0xB8360A00,  //  008E  GETNGBL	R13	K5
      0x8C341B20,  //  008F  GETMET	R13	R13	K32
      0x7C340200,  //  0090  CALL	R13	1
      0x90321C0D,  //  0091  SETMBR	R12	K14	R13
      0x88301711,  //  0092  GETMBR	R12	R11	K17
      0x8830190E,  //  0093  GETMBR	R12	R12	K14
      0x8834090D,  //  0094  GETMBR	R13	R4	K13
      0x90321A0D,  //  0095  SETMBR	R12	K13	R13
      0x88301711,  //  0096  GETMBR	R12	R11	K17
      0x8830190E,  //  0097  GETMBR	R12	R12	K14
      0x8834090F,  //  0098  GETMBR	R13	R4	K15
      0x90321E0D,  //  0099  SETMBR	R12	K15	R13
      0x88301711,  //  009A  GETMBR	R12	R11	K17
      0x8830190E,  //  009B  GETMBR	R12	R12	K14
      0x88340910,  //  009C  GETMBR	R13	R4	K16
      0x9032200D,  //  009D  SETMBR	R12	K16	R13
      0x88301711,  //  009E  GETMBR	R12	R11	K17
      0xB8360A00,  //  009F  GETNGBL	R13	K5
      0x8C341B25,  //  00A0  GETMET	R13	R13	K37
      0x7C340200,  //  00A1  CALL	R13	1
      0x9032220D,  //  00A2  SETMBR	R12	K17	R13
      0x88301711,  //  00A3  GETMBR	R12	R11	K17
      0x88301911,  //  00A4  GETMBR	R12	R12	K17
      0x88340911,  //  00A5  GETMBR	R13	R4	K17
      0x9032220D,  //  00A6  SETMBR	R12	K17	R13
      0x88300D0C,  //  00A7  GETMBR	R12	R6	K12
      0x8C301921,  //  00A8  GETMET	R12	R12	K33
      0x5C381600,  //  00A9  MOVE	R14	R11
      0x7C300400,  //  00AA  CALL	R12	2
      0x7001FFFF,  //  00AB  JMP		#00AC
      0x7001FF71,  //  00AC  JMP		#001F
      0x581C0026,  //  00AD  LDCONST	R7	K38
      0xAC1C0200,  //  00AE  CATCH	R7	1	0
      0xB0080000,  //  00AF  RAISE	2	R0	R0
      0xB81E0200,  //  00B0  GETNGBL	R7	K1
      0x8C1C0F02,  //  00B1  GETMET	R7	R7	K2
      0x60240008,  //  00B2  GETGBL	R9	G8
      0x88280D0C,  //  00B3  GETMBR	R10	R6	K12
      0x7C240200,  //  00B4  CALL	R9	1
      0x00264E09,  //  00B5  ADD	R9	K39	R9
      0x58280004,  //  00B6  LDCONST	R10	K4
      0x7C1C0600,  //  00B7  CALL	R7	3
      0x601C000C,  //  00B8  GETGBL	R7	G12
      0x88200D0C,  //  00B9  GETMBR	R8	R6	K12
      0x7C1C0200,  //  00BA  CALL	R7	1
      0x241C0F28,  //  00BB  GT	R7	R7	K40
      0x781E0024,  //  00BC  JMPF	R7	#00E2
      0xB81E0200,  //  00BD  GETNGBL	R7	K1
      0x8C1C0F02,  //  00BE  GETMET	R7	R7	K2
      0x60240008,  //  00BF  GETGBL	R9	G8
      0x5C280C00,  //  00C0  MOVE	R10	R6
      0x7C240200,  //  00C1  CALL	R9	1
      0x00265209,  //  00C2  ADD	R9	K41	R9
      0x58280004,  //  00C3  LDCONST	R10	K4
      0x7C1C0600,  //  00C4  CALL	R7	3
      0xB81E0200,  //  00C5  GETNGBL	R7	K1
      0x8C1C0F02,  //  00C6  GETMET	R7	R7	K2
      0x60240008,  //  00C7  GETGBL	R9	G8
      0x8C280D2B,  //  00C8  GETMET	R10	R6	K43
      0x7C280200,  //  00C9  CALL	R10	1
      0x7C240200,  //  00CA  CALL	R9	1
      0x00265409,  //  00CB  ADD	R9	K42	R9
      0x58280004,  //  00CC  LDCONST	R10	K4
      0x7C1C0600,  //  00CD  CALL	R7	3
      0x8C1C032C,  //  00CE  GETMET	R7	R1	K44
      0x54260008,  //  00CF  LDINT	R9	9
      0x50280200,  //  00D0  LDBOOL	R10	1	0
      0x7C1C0600,  //  00D1  CALL	R7	3
      0x8C200F2D,  //  00D2  GETMET	R8	R7	K45
      0x8C280D2B,  //  00D3  GETMET	R10	R6	K43
      0x7C280200,  //  00D4  CALL	R10	1
      0x8C28152D,  //  00D5  GETMET	R10	R10	K45
      0x7C280200,  //  00D6  CALL	R10	1
      0x7C200400,  //  00D7  CALL	R8	2
      0x8C200F2E,  //  00D8  GETMET	R8	R7	K46
      0x7C200200,  //  00D9  CALL	R8	1
      0x8820011A,  //  00DA  GETMBR	R8	R0	K26
      0x8C20112F,  //  00DB  GETMET	R8	R8	K47
      0x88280F30,  //  00DC  GETMBR	R10	R7	K48
      0x882C0317,  //  00DD  GETMBR	R11	R1	K23
      0x88300318,  //  00DE  GETMBR	R12	R1	K24
      0x88340F31,  //  00DF  GETMBR	R13	R7	K49
      0x7C200A00,  //  00E0  CALL	R8	5
      0x7002000E,  //  00E1  JMP		#00F1
      0x881C0332,  //  00E2  GETMBR	R7	R1	K50
      0x781E000C,  //  00E3  JMPF	R7	#00F1
      0x8C1C0333,  //  00E4  GETMET	R7	R1	K51
      0x7C1C0200,  //  00E5  CALL	R7	1
      0x8C200F2D,  //  00E6  GETMET	R8	R7	K45
      0x7C200200,  //  00E7  CALL	R8	1
      0x8C200F2E,  //  00E8  GETMET	R8	R7	K46
      0x7C200200,  //  00E9  CALL	R8	1
      0x8820011A,  //  00EA  GETMBR	R8	R0	K26
      0x8C20112F,  //  00EB  GETMET	R8	R8	K47
      0x88280F30,  //  00EC  GETMBR	R10	R7	K48
      0x882C0317,  //  00ED  GETMBR	R11	R1	K23
      0x88300318,  //  00EE  GETMBR	R12	R1	K24
      0x88340F31,  //  00EF  GETMBR	R13	R7	K49
      0x7C200A00,  //  00F0  CALL	R8	5
      0x80000000,  //  00F1  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_IM_init,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(responder),
    /* K1   */  be_nested_str_weak(device),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


extern const bclass be_class_Matter_Attr_Report;

/********************************************************************
** Solidified class: Matter_Attr_Report
********************************************************************/
be_local_class(Matter_Attr_Report,
    3,
    NULL,
    be_nested_map(3,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ret, 1), be_const_var(0) },
        { be_const_key_weak(expiration, 2), be_const_var(2) },
        { be_const_key_weak(resp, -1), be_const_var(1) },
    })),
    be_str_weak(Matter_Attr_Report)
);

/********************************************************************
** Solidified function: send_attr_report
********************************************************************/
be_local_closure(Matter_IM_send_attr_report,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Attr_Report),
    /* K1   */  be_const_int(0),
    /* K2   */  be_nested_str_weak(attribute_reports),
    /* K3   */  be_nested_str_weak(to_TLV),
    /* K4   */  be_nested_str_weak(encode),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_weak(MAX_MESSAGE),
    /* K7   */  be_const_int(2147483647),
    /* K8   */  be_nested_str_weak(more_chunked_messages),
    /* K9   */  be_nested_str_weak(build_response),
    /* K10  */  be_nested_str_weak(encrypt),
    /* K11  */  be_nested_str_weak(responder),
    /* K12  */  be_nested_str_weak(send_response),
    /* K13  */  be_nested_str_weak(raw),
    /* K14  */  be_nested_str_weak(remote_ip),
    /* K15  */  be_nested_str_weak(remote_port),
    /* K16  */  be_nested_str_weak(message_counter),
    /* K17  */  be_nested_str_weak(ret),
    /* K18  */  be_nested_str_weak(resp),
    /* K19  */  be_nested_str_weak(expiration),
    /* K20  */  be_nested_str_weak(tasmota),
    /* K21  */  be_nested_str_weak(millis),
    /* K22  */  be_nested_str_weak(MSG_TIMEOUT),
    }),
    be_str_weak(send_attr_report),
    &be_const_str_solidified,
    ( &(const binstruction[93]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0xB4000000,  //  0001  CLASS	K0
      0x58100001,  //  0002  LDCONST	R4	K1
      0x58140001,  //  0003  LDCONST	R5	K1
      0x6018000C,  //  0004  GETGBL	R6	G12
      0x881C0502,  //  0005  GETMBR	R7	R2	K2
      0x7C180200,  //  0006  CALL	R6	1
      0x24180D01,  //  0007  GT	R6	R6	K1
      0x781A0009,  //  0008  JMPF	R6	#0013
      0x6018000C,  //  0009  GETGBL	R6	G12
      0x881C0502,  //  000A  GETMBR	R7	R2	K2
      0x941C0F01,  //  000B  GETIDX	R7	R7	K1
      0x8C1C0F03,  //  000C  GETMET	R7	R7	K3
      0x7C1C0200,  //  000D  CALL	R7	1
      0x8C1C0F04,  //  000E  GETMET	R7	R7	K4
      0x7C1C0200,  //  000F  CALL	R7	1
      0x7C180200,  //  0010  CALL	R6	1
      0x5C100C00,  //  0011  MOVE	R4	R6
      0x58140005,  //  0012  LDCONST	R5	K5
      0x88180106,  //  0013  GETMBR	R6	R0	K6
      0x14180806,  //  0014  LT	R6	R4	R6
      0x781A0013,  //  0015  JMPF	R6	#002A
      0x6018000C,  //  0016  GETGBL	R6	G12
      0x881C0502,  //  0017  GETMBR	R7	R2	K2
      0x7C180200,  //  0018  CALL	R6	1
      0x14180A06,  //  0019  LT	R6	R5	R6
      0x781A000E,  //  001A  JMPF	R6	#002A
      0x6018000C,  //  001B  GETGBL	R6	G12
      0x881C0502,  //  001C  GETMBR	R7	R2	K2
      0x941C0E05,  //  001D  GETIDX	R7	R7	R5
      0x8C1C0F03,  //  001E  GETMET	R7	R7	K3
      0x7C1C0200,  //  001F  CALL	R7	1
      0x8C1C0F04,  //  0020  GETMET	R7	R7	K4
      0x7C1C0200,  //  0021  CALL	R7	1
      0x7C180200,  //  0022  CALL	R6	1
      0x001C0806,  //  0023  ADD	R7	R4	R6
      0x88200106,  //  0024  GETMBR	R8	R0	K6
      0x141C0E08,  //  0025  LT	R7	R7	R8
      0x781E0001,  //  0026  JMPF	R7	#0029
      0x00100806,  //  0027  ADD	R4	R4	R6
      0x00140B05,  //  0028  ADD	R5	R5	K5
      0x7001FFE8,  //  0029  JMP		#0013
      0x40180B07,  //  002A  CONNECT	R6	R5	K7
      0x881C0502,  //  002B  GETMBR	R7	R2	K2
      0x94180E06,  //  002C  GETIDX	R6	R7	R6
      0x04200B05,  //  002D  SUB	R8	R5	K5
      0x40220208,  //  002E  CONNECT	R8	K1	R8
      0x88240502,  //  002F  GETMBR	R9	R2	K2
      0x94201208,  //  0030  GETIDX	R8	R9	R8
      0x900A0408,  //  0031  SETMBR	R2	K2	R8
      0x6020000C,  //  0032  GETGBL	R8	G12
      0x5C240C00,  //  0033  MOVE	R9	R6
      0x7C200200,  //  0034  CALL	R8	1
      0x24201101,  //  0035  GT	R8	R8	K1
      0x78220001,  //  0036  JMPF	R8	#0039
      0x50200200,  //  0037  LDBOOL	R8	1	0
      0x900A1008,  //  0038  SETMBR	R2	K8	R8
      0x8C1C0309,  //  0039  GETMET	R7	R1	K9
      0x54260004,  //  003A  LDINT	R9	5
      0x50280200,  //  003B  LDBOOL	R10	1	0
      0x7C1C0600,  //  003C  CALL	R7	3
      0x8C200F04,  //  003D  GETMET	R8	R7	K4
      0x8C280503,  //  003E  GETMET	R10	R2	K3
      0x7C280200,  //  003F  CALL	R10	1
      0x8C281504,  //  0040  GETMET	R10	R10	K4
      0x7C280200,  //  0041  CALL	R10	1
      0x7C200400,  //  0042  CALL	R8	2
      0x8C200F0A,  //  0043  GETMET	R8	R7	K10
      0x7C200200,  //  0044  CALL	R8	1
      0x8820010B,  //  0045  GETMBR	R8	R0	K11
      0x8C20110C,  //  0046  GETMET	R8	R8	K12
      0x88280F0D,  //  0047  GETMBR	R10	R7	K13
      0x882C030E,  //  0048  GETMBR	R11	R1	K14
      0x8830030F,  //  0049  GETMBR	R12	R1	K15
      0x88340F10,  //  004A  GETMBR	R13	R7	K16
      0x7C200A00,  //  004B  CALL	R8	5
      0x6020000C,  //  004C  GETGBL	R8	G12
      0x5C240C00,  //  004D  MOVE	R9	R6
      0x7C200200,  //  004E  CALL	R8	1
      0x24201101,  //  004F  GT	R8	R8	K1
      0x7822000A,  //  0050  JMPF	R8	#005C
      0x900A0406,  //  0051  SETMBR	R2	K2	R6
      0x5C200600,  //  0052  MOVE	R8	R3
      0x7C200000,  //  0053  CALL	R8	0
      0x90222202,  //  0054  SETMBR	R8	K17	R2
      0x90222407,  //  0055  SETMBR	R8	K18	R7
      0xB8262800,  //  0056  GETNGBL	R9	K20
      0x8C241315,  //  0057  GETMET	R9	R9	K21
      0x7C240200,  //  0058  CALL	R9	1
      0x88280116,  //  0059  GETMBR	R10	R0	K22
      0x0024120A,  //  005A  ADD	R9	R9	R10
      0x90222609,  //  005B  SETMBR	R8	K19	R9
      0x80000000,  //  005C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: subscribe_response
********************************************************************/
be_local_closure(Matter_IM_subscribe_response,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(SubscribeResponseMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20received_X20SubscribeResponsetMessage_X3D),
    /* K7   */  be_const_int(3),
    }),
    be_str_weak(subscribe_response),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0xB8160800,  //  0007  GETNGBL	R5	K4
      0x8C140B05,  //  0008  GETMET	R5	R5	K5
      0x601C0008,  //  0009  GETGBL	R7	G8
      0x5C200800,  //  000A  MOVE	R8	R4
      0x7C1C0200,  //  000B  CALL	R7	1
      0x001E0C07,  //  000C  ADD	R7	K6	R7
      0x58200007,  //  000D  LDCONST	R8	K7
      0x7C140600,  //  000E  CALL	R5	3
      0x50140000,  //  000F  LDBOOL	R5	0	0
      0x80040A00,  //  0010  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: subscribe_request
********************************************************************/
be_local_closure(Matter_IM_subscribe_request,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(SubscribeRequestMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20received_X20SubscribeRequestMessage_X3D),
    /* K7   */  be_const_int(3),
    }),
    be_str_weak(subscribe_request),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0xB8160800,  //  0007  GETNGBL	R5	K4
      0x8C140B05,  //  0008  GETMET	R5	R5	K5
      0x601C0008,  //  0009  GETGBL	R7	G8
      0x5C200800,  //  000A  MOVE	R8	R4
      0x7C1C0200,  //  000B  CALL	R7	1
      0x001E0C07,  //  000C  ADD	R7	K6	R7
      0x58200007,  //  000D  LDCONST	R8	K7
      0x7C140600,  //  000E  CALL	R5	3
      0x50140000,  //  000F  LDBOOL	R5	0	0
      0x80040A00,  //  0010  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_write_response
********************************************************************/
be_local_closure(Matter_IM_process_write_response,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(WriteResponseMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20received_X20WriteResponseMessage_X3D),
    /* K7   */  be_const_int(3),
    }),
    be_str_weak(process_write_response),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0xB8160800,  //  0007  GETNGBL	R5	K4
      0x8C140B05,  //  0008  GETMET	R5	R5	K5
      0x601C0008,  //  0009  GETGBL	R7	G8
      0x5C200800,  //  000A  MOVE	R8	R4
      0x7C1C0200,  //  000B  CALL	R7	1
      0x001E0C07,  //  000C  ADD	R7	K6	R7
      0x58200007,  //  000D  LDCONST	R8	K7
      0x7C140600,  //  000E  CALL	R5	3
      0x50140000,  //  000F  LDBOOL	R5	0	0
      0x80040A00,  //  0010  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_incoming
********************************************************************/
be_local_closure(Matter_IM_process_incoming,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[27]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(MTR_X3A_X20received_X20IM_X20message_X20),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(inspect),
    /* K5   */  be_const_int(3),
    /* K6   */  be_nested_str_weak(TLV),
    /* K7   */  be_nested_str_weak(parse),
    /* K8   */  be_nested_str_weak(raw),
    /* K9   */  be_nested_str_weak(app_payload_idx),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20IM_X20TLV_X3A_X20),
    /* K11  */  be_nested_str_weak(findsubval),
    /* K12  */  be_nested_str_weak(MTR_X3A_X20InteractionModelRevision_X3D),
    /* K13  */  be_nested_str_weak(nil),
    /* K14  */  be_nested_str_weak(opcode),
    /* K15  */  be_const_int(1),
    /* K16  */  be_nested_str_weak(process_status_response),
    /* K17  */  be_const_int(2),
    /* K18  */  be_nested_str_weak(process_read_request),
    /* K19  */  be_nested_str_weak(subscribe_request),
    /* K20  */  be_nested_str_weak(subscribe_response),
    /* K21  */  be_nested_str_weak(report_data),
    /* K22  */  be_nested_str_weak(process_write_request),
    /* K23  */  be_nested_str_weak(process_write_response),
    /* K24  */  be_nested_str_weak(process_invoke_request),
    /* K25  */  be_nested_str_weak(process_invoke_response),
    /* K26  */  be_nested_str_weak(process_timed_request),
    }),
    be_str_weak(process_incoming),
    &be_const_str_solidified,
    ( &(const binstruction[128]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0xB8120600,  //  0002  GETNGBL	R4	K3
      0x8C100904,  //  0003  GETMET	R4	R4	K4
      0x5C180200,  //  0004  MOVE	R6	R1
      0x7C100400,  //  0005  CALL	R4	2
      0x00120404,  //  0006  ADD	R4	K2	R4
      0x58140005,  //  0007  LDCONST	R5	K5
      0x7C080600,  //  0008  CALL	R2	3
      0xB80A0600,  //  0009  GETNGBL	R2	K3
      0x88080506,  //  000A  GETMBR	R2	R2	K6
      0x8C080507,  //  000B  GETMET	R2	R2	K7
      0x88100308,  //  000C  GETMBR	R4	R1	K8
      0x88140309,  //  000D  GETMBR	R5	R1	K9
      0x7C080600,  //  000E  CALL	R2	3
      0xB80E0000,  //  000F  GETNGBL	R3	K0
      0x8C0C0701,  //  0010  GETMET	R3	R3	K1
      0x60140008,  //  0011  GETGBL	R5	G8
      0x5C180400,  //  0012  MOVE	R6	R2
      0x7C140200,  //  0013  CALL	R5	1
      0x00161405,  //  0014  ADD	R5	K10	R5
      0x58180005,  //  0015  LDCONST	R6	K5
      0x7C0C0600,  //  0016  CALL	R3	3
      0x8C0C050B,  //  0017  GETMET	R3	R2	K11
      0x541600FE,  //  0018  LDINT	R5	255
      0x7C0C0400,  //  0019  CALL	R3	2
      0xB8120000,  //  001A  GETNGBL	R4	K0
      0x8C100901,  //  001B  GETMET	R4	R4	K1
      0x4C180000,  //  001C  LDNIL	R6
      0x20180606,  //  001D  NE	R6	R3	R6
      0x781A0003,  //  001E  JMPF	R6	#0023
      0x60180008,  //  001F  GETGBL	R6	G8
      0x5C1C0600,  //  0020  MOVE	R7	R3
      0x7C180200,  //  0021  CALL	R6	1
      0x70020000,  //  0022  JMP		#0024
      0x5818000D,  //  0023  LDCONST	R6	K13
      0x001A1806,  //  0024  ADD	R6	K12	R6
      0x581C0005,  //  0025  LDCONST	R7	K5
      0x7C100600,  //  0026  CALL	R4	3
      0x8810030E,  //  0027  GETMBR	R4	R1	K14
      0x1C14090F,  //  0028  EQ	R5	R4	K15
      0x78160005,  //  0029  JMPF	R5	#0030
      0x8C140110,  //  002A  GETMET	R5	R0	K16
      0x5C1C0200,  //  002B  MOVE	R7	R1
      0x5C200400,  //  002C  MOVE	R8	R2
      0x7C140600,  //  002D  CALL	R5	3
      0x80040A00,  //  002E  RET	1	R5
      0x7002004D,  //  002F  JMP		#007E
      0x1C140911,  //  0030  EQ	R5	R4	K17
      0x78160005,  //  0031  JMPF	R5	#0038
      0x8C140112,  //  0032  GETMET	R5	R0	K18
      0x5C1C0200,  //  0033  MOVE	R7	R1
      0x5C200400,  //  0034  MOVE	R8	R2
      0x7C140600,  //  0035  CALL	R5	3
      0x80040A00,  //  0036  RET	1	R5
      0x70020045,  //  0037  JMP		#007E
      0x1C140905,  //  0038  EQ	R5	R4	K5
      0x78160005,  //  0039  JMPF	R5	#0040
      0x8C140113,  //  003A  GETMET	R5	R0	K19
      0x5C1C0200,  //  003B  MOVE	R7	R1
      0x5C200400,  //  003C  MOVE	R8	R2
      0x7C140600,  //  003D  CALL	R5	3
      0x80040A00,  //  003E  RET	1	R5
      0x7002003D,  //  003F  JMP		#007E
      0x54160003,  //  0040  LDINT	R5	4
      0x1C140805,  //  0041  EQ	R5	R4	R5
      0x78160005,  //  0042  JMPF	R5	#0049
      0x8C140114,  //  0043  GETMET	R5	R0	K20
      0x5C1C0200,  //  0044  MOVE	R7	R1
      0x5C200400,  //  0045  MOVE	R8	R2
      0x7C140600,  //  0046  CALL	R5	3
      0x80040A00,  //  0047  RET	1	R5
      0x70020034,  //  0048  JMP		#007E
      0x54160004,  //  0049  LDINT	R5	5
      0x1C140805,  //  004A  EQ	R5	R4	R5
      0x78160005,  //  004B  JMPF	R5	#0052
      0x8C140115,  //  004C  GETMET	R5	R0	K21
      0x5C1C0200,  //  004D  MOVE	R7	R1
      0x5C200400,  //  004E  MOVE	R8	R2
      0x7C140600,  //  004F  CALL	R5	3
      0x80040A00,  //  0050  RET	1	R5
      0x7002002B,  //  0051  JMP		#007E
      0x54160005,  //  0052  LDINT	R5	6
      0x1C140805,  //  0053  EQ	R5	R4	R5
      0x78160005,  //  0054  JMPF	R5	#005B
      0x8C140116,  //  0055  GETMET	R5	R0	K22
      0x5C1C0200,  //  0056  MOVE	R7	R1
      0x5C200400,  //  0057  MOVE	R8	R2
      0x7C140600,  //  0058  CALL	R5	3
      0x80040A00,  //  0059  RET	1	R5
      0x70020022,  //  005A  JMP		#007E
      0x54160006,  //  005B  LDINT	R5	7
      0x1C140805,  //  005C  EQ	R5	R4	R5
      0x78160005,  //  005D  JMPF	R5	#0064
      0x8C140117,  //  005E  GETMET	R5	R0	K23
      0x5C1C0200,  //  005F  MOVE	R7	R1
      0x5C200400,  //  0060  MOVE	R8	R2
      0x7C140600,  //  0061  CALL	R5	3
      0x80040A00,  //  0062  RET	1	R5
      0x70020019,  //  0063  JMP		#007E
      0x54160007,  //  0064  LDINT	R5	8
      0x1C140805,  //  0065  EQ	R5	R4	R5
      0x78160005,  //  0066  JMPF	R5	#006D
      0x8C140118,  //  0067  GETMET	R5	R0	K24
      0x5C1C0200,  //  0068  MOVE	R7	R1
      0x5C200400,  //  0069  MOVE	R8	R2
      0x7C140600,  //  006A  CALL	R5	3
      0x80040A00,  //  006B  RET	1	R5
      0x70020010,  //  006C  JMP		#007E
      0x54160008,  //  006D  LDINT	R5	9
      0x1C140805,  //  006E  EQ	R5	R4	R5
      0x78160005,  //  006F  JMPF	R5	#0076
      0x8C140119,  //  0070  GETMET	R5	R0	K25
      0x5C1C0200,  //  0071  MOVE	R7	R1
      0x5C200400,  //  0072  MOVE	R8	R2
      0x7C140600,  //  0073  CALL	R5	3
      0x80040A00,  //  0074  RET	1	R5
      0x70020007,  //  0075  JMP		#007E
      0x54160009,  //  0076  LDINT	R5	10
      0x1C140805,  //  0077  EQ	R5	R4	R5
      0x78160004,  //  0078  JMPF	R5	#007E
      0x8C14011A,  //  0079  GETMET	R5	R0	K26
      0x5C1C0200,  //  007A  MOVE	R7	R1
      0x5C200400,  //  007B  MOVE	R8	R2
      0x7C140600,  //  007C  CALL	R5	3
      0x80040A00,  //  007D  RET	1	R5
      0x50140000,  //  007E  LDBOOL	R5	0	0
      0x80040A00,  //  007F  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM
********************************************************************/
be_local_class(Matter_IM,
    2,
    NULL,
    be_nested_map(18,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(process_timed_request, 14), be_const_closure(Matter_IM_process_timed_request_closure) },
        { be_const_key_weak(responder, -1), be_const_var(0) },
        { be_const_key_weak(process_read_request, -1), be_const_closure(Matter_IM_process_read_request_closure) },
        { be_const_key_weak(every_second, 2), be_const_closure(Matter_IM_every_second_closure) },
        { be_const_key_weak(report_data, -1), be_const_closure(Matter_IM_report_data_closure) },
        { be_const_key_weak(process_invoke_response, -1), be_const_closure(Matter_IM_process_invoke_response_closure) },
        { be_const_key_weak(process_write_response, -1), be_const_closure(Matter_IM_process_write_response_closure) },
        { be_const_key_weak(device, -1), be_const_var(1) },
        { be_const_key_weak(subscribe_request, -1), be_const_closure(Matter_IM_subscribe_request_closure) },
        { be_const_key_weak(process_invoke_request, -1), be_const_closure(Matter_IM_process_invoke_request_closure) },
        { be_const_key_weak(MAX_MESSAGE, -1), be_const_int(1200) },
        { be_const_key_weak(process_status_response, 10), be_const_closure(Matter_IM_process_status_response_closure) },
        { be_const_key_weak(send_attr_report, -1), be_const_closure(Matter_IM_send_attr_report_closure) },
        { be_const_key_weak(subscribe_response, 8), be_const_closure(Matter_IM_subscribe_response_closure) },
        { be_const_key_weak(process_write_request, -1), be_const_closure(Matter_IM_process_write_request_closure) },
        { be_const_key_weak(init, 7), be_const_closure(Matter_IM_init_closure) },
        { be_const_key_weak(MSG_TIMEOUT, 6), be_const_int(10000) },
        { be_const_key_weak(process_incoming, -1), be_const_closure(Matter_IM_process_incoming_closure) },
    })),
    be_str_weak(Matter_IM)
);
/*******************************************************************/

void be_load_Matter_IM_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_IM);
    be_setglobal(vm, "Matter_IM");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
