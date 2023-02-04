/* Solidification of Matter_IM.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Response_container;

/********************************************************************
** Solidified class: Matter_Response_container
********************************************************************/
be_local_class(Matter_Response_container,
    5,
    NULL,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(command, -1), be_const_var(3) },
        { be_const_key_weak(attribute, -1), be_const_var(2) },
        { be_const_key_weak(cluster, -1), be_const_var(1) },
        { be_const_key_weak(endpoint, 0), be_const_var(0) },
        { be_const_key_weak(status, -1), be_const_var(4) },
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
    15,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
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
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(StatusResponseMessage),
    /* K13  */  be_nested_str_weak(status),
    /* K14  */  be_nested_str_weak(SUCCESS),
    /* K15  */  be_nested_str_weak(build_response),
    /* K16  */  be_const_int(1),
    /* K17  */  be_nested_str_weak(encode),
    /* K18  */  be_nested_str_weak(to_TLV),
    /* K19  */  be_nested_str_weak(encrypt),
    /* K20  */  be_nested_str_weak(responder),
    /* K21  */  be_nested_str_weak(send_response),
    /* K22  */  be_nested_str_weak(raw),
    /* K23  */  be_nested_str_weak(message_counter),
    }),
    be_str_weak(process_timed_request),
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0xA4160000,  //  0000  IMPORT	R5	K0
      0xB81A0200,  //  0001  GETNGBL	R6	K1
      0x8C180D02,  //  0002  GETMET	R6	R6	K2
      0x7C180200,  //  0003  CALL	R6	1
      0x8C180D03,  //  0004  GETMET	R6	R6	K3
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C180400,  //  0006  CALL	R6	2
      0xB81E0800,  //  0007  GETNGBL	R7	K4
      0x8C1C0F05,  //  0008  GETMET	R7	R7	K5
      0x60240008,  //  0009  GETGBL	R9	G8
      0x5C280C00,  //  000A  MOVE	R10	R6
      0x7C240200,  //  000B  CALL	R9	1
      0x00260C09,  //  000C  ADD	R9	K6	R9
      0x58280007,  //  000D  LDCONST	R10	K7
      0x7C1C0600,  //  000E  CALL	R7	3
      0xB81E0800,  //  000F  GETNGBL	R7	K4
      0x8C1C0F05,  //  0010  GETMET	R7	R7	K5
      0x8C240B08,  //  0011  GETMET	R9	R5	K8
      0x582C0009,  //  0012  LDCONST	R11	K9
      0x88300D0A,  //  0013  GETMBR	R12	R6	K10
      0x5C340600,  //  0014  MOVE	R13	R3
      0x5C380800,  //  0015  MOVE	R14	R4
      0x7C240A00,  //  0016  CALL	R9	5
      0x5828000B,  //  0017  LDCONST	R10	K11
      0x7C1C0600,  //  0018  CALL	R7	3
      0xB81E0200,  //  0019  GETNGBL	R7	K1
      0x8C1C0F0C,  //  001A  GETMET	R7	R7	K12
      0x7C1C0200,  //  001B  CALL	R7	1
      0xB8220200,  //  001C  GETNGBL	R8	K1
      0x8820110E,  //  001D  GETMBR	R8	R8	K14
      0x901E1A08,  //  001E  SETMBR	R7	K13	R8
      0x8C20030F,  //  001F  GETMET	R8	R1	K15
      0x58280010,  //  0020  LDCONST	R10	K16
      0x502C0200,  //  0021  LDBOOL	R11	1	0
      0x7C200600,  //  0022  CALL	R8	3
      0x8C241111,  //  0023  GETMET	R9	R8	K17
      0x8C2C0F12,  //  0024  GETMET	R11	R7	K18
      0x7C2C0200,  //  0025  CALL	R11	1
      0x8C2C1711,  //  0026  GETMET	R11	R11	K17
      0x7C2C0200,  //  0027  CALL	R11	1
      0x7C240400,  //  0028  CALL	R9	2
      0x8C241113,  //  0029  GETMET	R9	R8	K19
      0x7C240200,  //  002A  CALL	R9	1
      0x88240114,  //  002B  GETMBR	R9	R0	K20
      0x8C241315,  //  002C  GETMET	R9	R9	K21
      0x882C1116,  //  002D  GETMBR	R11	R8	K22
      0x5C300600,  //  002E  MOVE	R12	R3
      0x5C340800,  //  002F  MOVE	R13	R4
      0x88381117,  //  0030  GETMBR	R14	R8	K23
      0x7C240A00,  //  0031  CALL	R9	5
      0x50240200,  //  0032  LDBOOL	R9	1	0
      0x80041200,  //  0033  RET	1	R9
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_status_response
********************************************************************/
be_local_closure(Matter_IM_process_status_response,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    5,                          /* argc */
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
      0xA4160000,  //  0000  IMPORT	R5	K0
      0x8C180501,  //  0001  GETMET	R6	R2	K1
      0x58200002,  //  0002  LDCONST	R8	K2
      0x542600FE,  //  0003  LDINT	R9	255
      0x7C180600,  //  0004  CALL	R6	3
      0xB81E0600,  //  0005  GETNGBL	R7	K3
      0x8C1C0F04,  //  0006  GETMET	R7	R7	K4
      0x8C240B05,  //  0007  GETMET	R9	R5	K5
      0x582C0006,  //  0008  LDCONST	R11	K6
      0x5C300C00,  //  0009  MOVE	R12	R6
      0x7C240600,  //  000A  CALL	R9	3
      0x58280007,  //  000B  LDCONST	R10	K7
      0x7C1C0600,  //  000C  CALL	R7	3
      0x501C0200,  //  000D  LDBOOL	R7	1	0
      0x80040E00,  //  000E  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_IM_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(responder),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
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
** Solidified function: process_write_response
********************************************************************/
be_local_closure(Matter_IM_process_write_response,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
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
      0xA4160000,  //  0000  IMPORT	R5	K0
      0xB81A0200,  //  0001  GETNGBL	R6	K1
      0x8C180D02,  //  0002  GETMET	R6	R6	K2
      0x7C180200,  //  0003  CALL	R6	1
      0x8C180D03,  //  0004  GETMET	R6	R6	K3
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C180400,  //  0006  CALL	R6	2
      0xB81E0800,  //  0007  GETNGBL	R7	K4
      0x8C1C0F05,  //  0008  GETMET	R7	R7	K5
      0x60240008,  //  0009  GETGBL	R9	G8
      0x5C280C00,  //  000A  MOVE	R10	R6
      0x7C240200,  //  000B  CALL	R9	1
      0x00260C09,  //  000C  ADD	R9	K6	R9
      0x58280007,  //  000D  LDCONST	R10	K7
      0x7C1C0600,  //  000E  CALL	R7	3
      0x501C0000,  //  000F  LDBOOL	R7	0	0
      0x80040E00,  //  0010  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_incoming
********************************************************************/
be_local_closure(Matter_IM_process_incoming,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
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
    ( &(const binstruction[148]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x8C100901,  //  0001  GETMET	R4	R4	K1
      0xB81A0600,  //  0002  GETNGBL	R6	K3
      0x8C180D04,  //  0003  GETMET	R6	R6	K4
      0x5C200200,  //  0004  MOVE	R8	R1
      0x7C180400,  //  0005  CALL	R6	2
      0x001A0406,  //  0006  ADD	R6	K2	R6
      0x581C0005,  //  0007  LDCONST	R7	K5
      0x7C100600,  //  0008  CALL	R4	3
      0xB8120600,  //  0009  GETNGBL	R4	K3
      0x88100906,  //  000A  GETMBR	R4	R4	K6
      0x8C100907,  //  000B  GETMET	R4	R4	K7
      0x88180308,  //  000C  GETMBR	R6	R1	K8
      0x881C0309,  //  000D  GETMBR	R7	R1	K9
      0x7C100600,  //  000E  CALL	R4	3
      0xB8160000,  //  000F  GETNGBL	R5	K0
      0x8C140B01,  //  0010  GETMET	R5	R5	K1
      0x601C0008,  //  0011  GETGBL	R7	G8
      0x5C200800,  //  0012  MOVE	R8	R4
      0x7C1C0200,  //  0013  CALL	R7	1
      0x001E1407,  //  0014  ADD	R7	K10	R7
      0x58200005,  //  0015  LDCONST	R8	K5
      0x7C140600,  //  0016  CALL	R5	3
      0x8C14090B,  //  0017  GETMET	R5	R4	K11
      0x541E00FE,  //  0018  LDINT	R7	255
      0x7C140400,  //  0019  CALL	R5	2
      0xB81A0000,  //  001A  GETNGBL	R6	K0
      0x8C180D01,  //  001B  GETMET	R6	R6	K1
      0x4C200000,  //  001C  LDNIL	R8
      0x20200A08,  //  001D  NE	R8	R5	R8
      0x78220003,  //  001E  JMPF	R8	#0023
      0x60200008,  //  001F  GETGBL	R8	G8
      0x5C240A00,  //  0020  MOVE	R9	R5
      0x7C200200,  //  0021  CALL	R8	1
      0x70020000,  //  0022  JMP		#0024
      0x5820000D,  //  0023  LDCONST	R8	K13
      0x00221808,  //  0024  ADD	R8	K12	R8
      0x58240005,  //  0025  LDCONST	R9	K5
      0x7C180600,  //  0026  CALL	R6	3
      0x8818030E,  //  0027  GETMBR	R6	R1	K14
      0x1C1C0D0F,  //  0028  EQ	R7	R6	K15
      0x781E0007,  //  0029  JMPF	R7	#0032
      0x8C1C0110,  //  002A  GETMET	R7	R0	K16
      0x5C240200,  //  002B  MOVE	R9	R1
      0x5C280800,  //  002C  MOVE	R10	R4
      0x5C2C0400,  //  002D  MOVE	R11	R2
      0x5C300600,  //  002E  MOVE	R12	R3
      0x7C1C0A00,  //  002F  CALL	R7	5
      0x80040E00,  //  0030  RET	1	R7
      0x7002005F,  //  0031  JMP		#0092
      0x1C1C0D11,  //  0032  EQ	R7	R6	K17
      0x781E0007,  //  0033  JMPF	R7	#003C
      0x8C1C0112,  //  0034  GETMET	R7	R0	K18
      0x5C240200,  //  0035  MOVE	R9	R1
      0x5C280800,  //  0036  MOVE	R10	R4
      0x5C2C0400,  //  0037  MOVE	R11	R2
      0x5C300600,  //  0038  MOVE	R12	R3
      0x7C1C0A00,  //  0039  CALL	R7	5
      0x80040E00,  //  003A  RET	1	R7
      0x70020055,  //  003B  JMP		#0092
      0x1C1C0D05,  //  003C  EQ	R7	R6	K5
      0x781E0007,  //  003D  JMPF	R7	#0046
      0x8C1C0113,  //  003E  GETMET	R7	R0	K19
      0x5C240200,  //  003F  MOVE	R9	R1
      0x5C280800,  //  0040  MOVE	R10	R4
      0x5C2C0400,  //  0041  MOVE	R11	R2
      0x5C300600,  //  0042  MOVE	R12	R3
      0x7C1C0A00,  //  0043  CALL	R7	5
      0x80040E00,  //  0044  RET	1	R7
      0x7002004B,  //  0045  JMP		#0092
      0x541E0003,  //  0046  LDINT	R7	4
      0x1C1C0C07,  //  0047  EQ	R7	R6	R7
      0x781E0007,  //  0048  JMPF	R7	#0051
      0x8C1C0114,  //  0049  GETMET	R7	R0	K20
      0x5C240200,  //  004A  MOVE	R9	R1
      0x5C280800,  //  004B  MOVE	R10	R4
      0x5C2C0400,  //  004C  MOVE	R11	R2
      0x5C300600,  //  004D  MOVE	R12	R3
      0x7C1C0A00,  //  004E  CALL	R7	5
      0x80040E00,  //  004F  RET	1	R7
      0x70020040,  //  0050  JMP		#0092
      0x541E0004,  //  0051  LDINT	R7	5
      0x1C1C0C07,  //  0052  EQ	R7	R6	R7
      0x781E0007,  //  0053  JMPF	R7	#005C
      0x8C1C0115,  //  0054  GETMET	R7	R0	K21
      0x5C240200,  //  0055  MOVE	R9	R1
      0x5C280800,  //  0056  MOVE	R10	R4
      0x5C2C0400,  //  0057  MOVE	R11	R2
      0x5C300600,  //  0058  MOVE	R12	R3
      0x7C1C0A00,  //  0059  CALL	R7	5
      0x80040E00,  //  005A  RET	1	R7
      0x70020035,  //  005B  JMP		#0092
      0x541E0005,  //  005C  LDINT	R7	6
      0x1C1C0C07,  //  005D  EQ	R7	R6	R7
      0x781E0007,  //  005E  JMPF	R7	#0067
      0x8C1C0116,  //  005F  GETMET	R7	R0	K22
      0x5C240200,  //  0060  MOVE	R9	R1
      0x5C280800,  //  0061  MOVE	R10	R4
      0x5C2C0400,  //  0062  MOVE	R11	R2
      0x5C300600,  //  0063  MOVE	R12	R3
      0x7C1C0A00,  //  0064  CALL	R7	5
      0x80040E00,  //  0065  RET	1	R7
      0x7002002A,  //  0066  JMP		#0092
      0x541E0006,  //  0067  LDINT	R7	7
      0x1C1C0C07,  //  0068  EQ	R7	R6	R7
      0x781E0007,  //  0069  JMPF	R7	#0072
      0x8C1C0117,  //  006A  GETMET	R7	R0	K23
      0x5C240200,  //  006B  MOVE	R9	R1
      0x5C280800,  //  006C  MOVE	R10	R4
      0x5C2C0400,  //  006D  MOVE	R11	R2
      0x5C300600,  //  006E  MOVE	R12	R3
      0x7C1C0A00,  //  006F  CALL	R7	5
      0x80040E00,  //  0070  RET	1	R7
      0x7002001F,  //  0071  JMP		#0092
      0x541E0007,  //  0072  LDINT	R7	8
      0x1C1C0C07,  //  0073  EQ	R7	R6	R7
      0x781E0007,  //  0074  JMPF	R7	#007D
      0x8C1C0118,  //  0075  GETMET	R7	R0	K24
      0x5C240200,  //  0076  MOVE	R9	R1
      0x5C280800,  //  0077  MOVE	R10	R4
      0x5C2C0400,  //  0078  MOVE	R11	R2
      0x5C300600,  //  0079  MOVE	R12	R3
      0x7C1C0A00,  //  007A  CALL	R7	5
      0x80040E00,  //  007B  RET	1	R7
      0x70020014,  //  007C  JMP		#0092
      0x541E0008,  //  007D  LDINT	R7	9
      0x1C1C0C07,  //  007E  EQ	R7	R6	R7
      0x781E0007,  //  007F  JMPF	R7	#0088
      0x8C1C0119,  //  0080  GETMET	R7	R0	K25
      0x5C240200,  //  0081  MOVE	R9	R1
      0x5C280800,  //  0082  MOVE	R10	R4
      0x5C2C0400,  //  0083  MOVE	R11	R2
      0x5C300600,  //  0084  MOVE	R12	R3
      0x7C1C0A00,  //  0085  CALL	R7	5
      0x80040E00,  //  0086  RET	1	R7
      0x70020009,  //  0087  JMP		#0092
      0x541E0009,  //  0088  LDINT	R7	10
      0x1C1C0C07,  //  0089  EQ	R7	R6	R7
      0x781E0006,  //  008A  JMPF	R7	#0092
      0x8C1C011A,  //  008B  GETMET	R7	R0	K26
      0x5C240200,  //  008C  MOVE	R9	R1
      0x5C280800,  //  008D  MOVE	R10	R4
      0x5C2C0400,  //  008E  MOVE	R11	R2
      0x5C300600,  //  008F  MOVE	R12	R3
      0x7C1C0A00,  //  0090  CALL	R7	5
      0x80040E00,  //  0091  RET	1	R7
      0x501C0000,  //  0092  LDBOOL	R7	0	0
      0x80040E00,  //  0093  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_write_request
********************************************************************/
be_local_closure(Matter_IM_process_write_request,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
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
      0xA4160000,  //  0000  IMPORT	R5	K0
      0xB81A0200,  //  0001  GETNGBL	R6	K1
      0x8C180D02,  //  0002  GETMET	R6	R6	K2
      0x7C180200,  //  0003  CALL	R6	1
      0x8C180D03,  //  0004  GETMET	R6	R6	K3
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C180400,  //  0006  CALL	R6	2
      0xB81E0800,  //  0007  GETNGBL	R7	K4
      0x8C1C0F05,  //  0008  GETMET	R7	R7	K5
      0x60240008,  //  0009  GETGBL	R9	G8
      0x5C280C00,  //  000A  MOVE	R10	R6
      0x7C240200,  //  000B  CALL	R9	1
      0x00260C09,  //  000C  ADD	R9	K6	R9
      0x58280007,  //  000D  LDCONST	R10	K7
      0x7C1C0600,  //  000E  CALL	R7	3
      0x501C0000,  //  000F  LDBOOL	R7	0	0
      0x80040E00,  //  0010  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_invoke_request
********************************************************************/
be_local_closure(Matter_IM_process_invoke_request,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[50]) {     /* constants */
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
    /* K23  */  be_const_int(2),
    /* K24  */  be_nested_str_weak(responder),
    /* K25  */  be_nested_str_weak(device),
    /* K26  */  be_nested_str_weak(invoke_request),
    /* K27  */  be_nested_str_weak(command_fields),
    /* K28  */  be_nested_str_weak(InvokeResponseIB),
    /* K29  */  be_nested_str_weak(CommandDataIB),
    /* K30  */  be_nested_str_weak(CommandPathIB),
    /* K31  */  be_nested_str_weak(push),
    /* K32  */  be_nested_str_weak(0x_X2504X_X2F0x_X2502X),
    /* K33  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_cmd_X20_X20_X20_X25s),
    /* K34  */  be_nested_str_weak(CommandStatusIB),
    /* K35  */  be_nested_str_weak(StatusIB),
    /* K36  */  be_nested_str_weak(stop_iteration),
    /* K37  */  be_nested_str_weak(MTR_X3A_X20invoke_responses_X3D),
    /* K38  */  be_const_int(0),
    /* K39  */  be_nested_str_weak(MTR_X3A_X20InvokeResponse_X3D),
    /* K40  */  be_nested_str_weak(MTR_X3A_X20InvokeResponseTLV_X3D),
    /* K41  */  be_nested_str_weak(to_TLV),
    /* K42  */  be_nested_str_weak(build_response),
    /* K43  */  be_nested_str_weak(encode),
    /* K44  */  be_nested_str_weak(encrypt),
    /* K45  */  be_nested_str_weak(send_response),
    /* K46  */  be_nested_str_weak(raw),
    /* K47  */  be_nested_str_weak(message_counter),
    /* K48  */  be_nested_str_weak(x_flag_r),
    /* K49  */  be_nested_str_weak(build_standalone_ack),
    }),
    be_str_weak(process_invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[242]) {  /* code */
      0xA4160000,  //  0000  IMPORT	R5	K0
      0xB81A0200,  //  0001  GETNGBL	R6	K1
      0x8C180D02,  //  0002  GETMET	R6	R6	K2
      0x58200003,  //  0003  LDCONST	R8	K3
      0x58240004,  //  0004  LDCONST	R9	K4
      0x7C180600,  //  0005  CALL	R6	3
      0xB81A0A00,  //  0006  GETNGBL	R6	K5
      0x8C180D06,  //  0007  GETMET	R6	R6	K6
      0x7C180200,  //  0008  CALL	R6	1
      0xB81E0A00,  //  0009  GETNGBL	R7	K5
      0x8C1C0F07,  //  000A  GETMET	R7	R7	K7
      0x7C1C0200,  //  000B  CALL	R7	1
      0x8C1C0F08,  //  000C  GETMET	R7	R7	K8
      0x5C240400,  //  000D  MOVE	R9	R2
      0x7C1C0400,  //  000E  CALL	R7	2
      0x88200F09,  //  000F  GETMBR	R8	R7	K9
      0x4C240000,  //  0010  LDNIL	R9
      0x20201009,  //  0011  NE	R8	R8	R9
      0x782200DD,  //  0012  JMPF	R8	#00F1
      0xB8220A00,  //  0013  GETNGBL	R8	K5
      0x8C20110A,  //  0014  GETMET	R8	R8	K10
      0x7C200200,  //  0015  CALL	R8	1
      0x50240000,  //  0016  LDBOOL	R9	0	0
      0x90221609,  //  0017  SETMBR	R8	K11	R9
      0x60240012,  //  0018  GETGBL	R9	G18
      0x7C240000,  //  0019  CALL	R9	0
      0x90221809,  //  001A  SETMBR	R8	K12	R9
      0x60240010,  //  001B  GETGBL	R9	G16
      0x88280F09,  //  001C  GETMBR	R10	R7	K9
      0x7C240200,  //  001D  CALL	R9	1
      0xA802008D,  //  001E  EXBLK	0	#00AD
      0x5C281200,  //  001F  MOVE	R10	R9
      0x7C280000,  //  0020  CALL	R10	0
      0x882C150E,  //  0021  GETMBR	R11	R10	K14
      0x882C170D,  //  0022  GETMBR	R11	R11	K13
      0x901A1A0B,  //  0023  SETMBR	R6	K13	R11
      0x882C150E,  //  0024  GETMBR	R11	R10	K14
      0x882C170F,  //  0025  GETMBR	R11	R11	K15
      0x901A1E0B,  //  0026  SETMBR	R6	K15	R11
      0x882C150E,  //  0027  GETMBR	R11	R10	K14
      0x882C1710,  //  0028  GETMBR	R11	R11	K16
      0x901A200B,  //  0029  SETMBR	R6	K16	R11
      0xB82E0A00,  //  002A  GETNGBL	R11	K5
      0x882C1712,  //  002B  GETMBR	R11	R11	K18
      0x901A220B,  //  002C  SETMBR	R6	K17	R11
      0xB82E0A00,  //  002D  GETNGBL	R11	K5
      0x8C2C1713,  //  002E  GETMET	R11	R11	K19
      0x88340D0F,  //  002F  GETMBR	R13	R6	K15
      0x88380D10,  //  0030  GETMBR	R14	R6	K16
      0x7C2C0600,  //  0031  CALL	R11	3
      0x4C300000,  //  0032  LDNIL	R12
      0x1C30160C,  //  0033  EQ	R12	R11	R12
      0x78320005,  //  0034  JMPF	R12	#003B
      0x8C300B14,  //  0035  GETMET	R12	R5	K20
      0x58380015,  //  0036  LDCONST	R14	K21
      0x883C0D0F,  //  0037  GETMBR	R15	R6	K15
      0x88400D10,  //  0038  GETMBR	R16	R6	K16
      0x7C300800,  //  0039  CALL	R12	4
      0x5C2C1800,  //  003A  MOVE	R11	R12
      0xB8320200,  //  003B  GETNGBL	R12	K1
      0x8C301902,  //  003C  GETMET	R12	R12	K2
      0x8C380B14,  //  003D  GETMET	R14	R5	K20
      0x58400016,  //  003E  LDCONST	R16	K22
      0x5C441600,  //  003F  MOVE	R17	R11
      0x5C480600,  //  0040  MOVE	R18	R3
      0x5C4C0800,  //  0041  MOVE	R19	R4
      0x7C380A00,  //  0042  CALL	R14	5
      0x583C0017,  //  0043  LDCONST	R15	K23
      0x7C300600,  //  0044  CALL	R12	3
      0x88300118,  //  0045  GETMBR	R12	R0	K24
      0x88301919,  //  0046  GETMBR	R12	R12	K25
      0x8C30191A,  //  0047  GETMET	R12	R12	K26
      0x5C380200,  //  0048  MOVE	R14	R1
      0x883C151B,  //  0049  GETMBR	R15	R10	K27
      0x5C400C00,  //  004A  MOVE	R16	R6
      0x7C300800,  //  004B  CALL	R12	4
      0xB8360A00,  //  004C  GETNGBL	R13	K5
      0x8C341B1C,  //  004D  GETMET	R13	R13	K28
      0x7C340200,  //  004E  CALL	R13	1
      0x4C380000,  //  004F  LDNIL	R14
      0x2038180E,  //  0050  NE	R14	R12	R14
      0x783A0032,  //  0051  JMPF	R14	#0085
      0xB83A0A00,  //  0052  GETNGBL	R14	K5
      0x8C381D1D,  //  0053  GETMET	R14	R14	K29
      0x7C380200,  //  0054  CALL	R14	1
      0x9036200E,  //  0055  SETMBR	R13	K16	R14
      0x88381B10,  //  0056  GETMBR	R14	R13	K16
      0xB83E0A00,  //  0057  GETNGBL	R15	K5
      0x8C3C1F1E,  //  0058  GETMET	R15	R15	K30
      0x7C3C0200,  //  0059  CALL	R15	1
      0x903A1C0F,  //  005A  SETMBR	R14	K14	R15
      0x88381B10,  //  005B  GETMBR	R14	R13	K16
      0x88381D0E,  //  005C  GETMBR	R14	R14	K14
      0x883C0D0D,  //  005D  GETMBR	R15	R6	K13
      0x903A1A0F,  //  005E  SETMBR	R14	K13	R15
      0x88381B10,  //  005F  GETMBR	R14	R13	K16
      0x88381D0E,  //  0060  GETMBR	R14	R14	K14
      0x883C0D0F,  //  0061  GETMBR	R15	R6	K15
      0x903A1E0F,  //  0062  SETMBR	R14	K15	R15
      0x88381B10,  //  0063  GETMBR	R14	R13	K16
      0x88381D0E,  //  0064  GETMBR	R14	R14	K14
      0x883C0D10,  //  0065  GETMBR	R15	R6	K16
      0x903A200F,  //  0066  SETMBR	R14	K16	R15
      0x88381B10,  //  0067  GETMBR	R14	R13	K16
      0x903A360C,  //  0068  SETMBR	R14	K27	R12
      0x8838110C,  //  0069  GETMBR	R14	R8	K12
      0x8C381D1F,  //  006A  GETMET	R14	R14	K31
      0x5C401A00,  //  006B  MOVE	R16	R13
      0x7C380400,  //  006C  CALL	R14	2
      0xB83A0A00,  //  006D  GETNGBL	R14	K5
      0x8C381D13,  //  006E  GETMET	R14	R14	K19
      0x88400D0F,  //  006F  GETMBR	R16	R6	K15
      0x88440D10,  //  0070  GETMBR	R17	R6	K16
      0x7C380600,  //  0071  CALL	R14	3
      0x5C2C1C00,  //  0072  MOVE	R11	R14
      0x4C380000,  //  0073  LDNIL	R14
      0x1C38160E,  //  0074  EQ	R14	R11	R14
      0x783A0005,  //  0075  JMPF	R14	#007C
      0x8C380B14,  //  0076  GETMET	R14	R5	K20
      0x58400020,  //  0077  LDCONST	R16	K32
      0x88440D0F,  //  0078  GETMBR	R17	R6	K15
      0x88480D10,  //  0079  GETMBR	R18	R6	K16
      0x7C380800,  //  007A  CALL	R14	4
      0x5C2C1C00,  //  007B  MOVE	R11	R14
      0xB83A0200,  //  007C  GETNGBL	R14	K1
      0x8C381D02,  //  007D  GETMET	R14	R14	K2
      0x8C400B14,  //  007E  GETMET	R16	R5	K20
      0x58480021,  //  007F  LDCONST	R18	K33
      0x5C4C1600,  //  0080  MOVE	R19	R11
      0x7C400600,  //  0081  CALL	R16	3
      0x58440017,  //  0082  LDCONST	R17	K23
      0x7C380600,  //  0083  CALL	R14	3
      0x70020026,  //  0084  JMP		#00AC
      0x88380D11,  //  0085  GETMBR	R14	R6	K17
      0x4C3C0000,  //  0086  LDNIL	R15
      0x20381C0F,  //  0087  NE	R14	R14	R15
      0x783A0022,  //  0088  JMPF	R14	#00AC
      0xB83A0A00,  //  0089  GETNGBL	R14	K5
      0x8C381D22,  //  008A  GETMET	R14	R14	K34
      0x7C380200,  //  008B  CALL	R14	1
      0x9036220E,  //  008C  SETMBR	R13	K17	R14
      0x88381B11,  //  008D  GETMBR	R14	R13	K17
      0xB83E0A00,  //  008E  GETNGBL	R15	K5
      0x8C3C1F1E,  //  008F  GETMET	R15	R15	K30
      0x7C3C0200,  //  0090  CALL	R15	1
      0x903A1C0F,  //  0091  SETMBR	R14	K14	R15
      0x88381B11,  //  0092  GETMBR	R14	R13	K17
      0x88381D0E,  //  0093  GETMBR	R14	R14	K14
      0x883C0D0D,  //  0094  GETMBR	R15	R6	K13
      0x903A1A0F,  //  0095  SETMBR	R14	K13	R15
      0x88381B11,  //  0096  GETMBR	R14	R13	K17
      0x88381D0E,  //  0097  GETMBR	R14	R14	K14
      0x883C0D0F,  //  0098  GETMBR	R15	R6	K15
      0x903A1E0F,  //  0099  SETMBR	R14	K15	R15
      0x88381B11,  //  009A  GETMBR	R14	R13	K17
      0x88381D0E,  //  009B  GETMBR	R14	R14	K14
      0x883C0D10,  //  009C  GETMBR	R15	R6	K16
      0x903A200F,  //  009D  SETMBR	R14	K16	R15
      0x88381B11,  //  009E  GETMBR	R14	R13	K17
      0xB83E0A00,  //  009F  GETNGBL	R15	K5
      0x8C3C1F23,  //  00A0  GETMET	R15	R15	K35
      0x7C3C0200,  //  00A1  CALL	R15	1
      0x903A220F,  //  00A2  SETMBR	R14	K17	R15
      0x88381B11,  //  00A3  GETMBR	R14	R13	K17
      0x88381D11,  //  00A4  GETMBR	R14	R14	K17
      0x883C0D11,  //  00A5  GETMBR	R15	R6	K17
      0x903A220F,  //  00A6  SETMBR	R14	K17	R15
      0x8838110C,  //  00A7  GETMBR	R14	R8	K12
      0x8C381D1F,  //  00A8  GETMET	R14	R14	K31
      0x5C401A00,  //  00A9  MOVE	R16	R13
      0x7C380400,  //  00AA  CALL	R14	2
      0x7001FFFF,  //  00AB  JMP		#00AC
      0x7001FF71,  //  00AC  JMP		#001F
      0x58240024,  //  00AD  LDCONST	R9	K36
      0xAC240200,  //  00AE  CATCH	R9	1	0
      0xB0080000,  //  00AF  RAISE	2	R0	R0
      0xB8260200,  //  00B0  GETNGBL	R9	K1
      0x8C241302,  //  00B1  GETMET	R9	R9	K2
      0x602C0008,  //  00B2  GETGBL	R11	G8
      0x8830110C,  //  00B3  GETMBR	R12	R8	K12
      0x7C2C0200,  //  00B4  CALL	R11	1
      0x002E4A0B,  //  00B5  ADD	R11	K37	R11
      0x58300004,  //  00B6  LDCONST	R12	K4
      0x7C240600,  //  00B7  CALL	R9	3
      0x6024000C,  //  00B8  GETGBL	R9	G12
      0x8828110C,  //  00B9  GETMBR	R10	R8	K12
      0x7C240200,  //  00BA  CALL	R9	1
      0x24241326,  //  00BB  GT	R9	R9	K38
      0x78260024,  //  00BC  JMPF	R9	#00E2
      0xB8260200,  //  00BD  GETNGBL	R9	K1
      0x8C241302,  //  00BE  GETMET	R9	R9	K2
      0x602C0008,  //  00BF  GETGBL	R11	G8
      0x5C301000,  //  00C0  MOVE	R12	R8
      0x7C2C0200,  //  00C1  CALL	R11	1
      0x002E4E0B,  //  00C2  ADD	R11	K39	R11
      0x58300004,  //  00C3  LDCONST	R12	K4
      0x7C240600,  //  00C4  CALL	R9	3
      0xB8260200,  //  00C5  GETNGBL	R9	K1
      0x8C241302,  //  00C6  GETMET	R9	R9	K2
      0x602C0008,  //  00C7  GETGBL	R11	G8
      0x8C301129,  //  00C8  GETMET	R12	R8	K41
      0x7C300200,  //  00C9  CALL	R12	1
      0x7C2C0200,  //  00CA  CALL	R11	1
      0x002E500B,  //  00CB  ADD	R11	K40	R11
      0x58300004,  //  00CC  LDCONST	R12	K4
      0x7C240600,  //  00CD  CALL	R9	3
      0x8C24032A,  //  00CE  GETMET	R9	R1	K42
      0x542E0008,  //  00CF  LDINT	R11	9
      0x50300200,  //  00D0  LDBOOL	R12	1	0
      0x7C240600,  //  00D1  CALL	R9	3
      0x8C28132B,  //  00D2  GETMET	R10	R9	K43
      0x8C301129,  //  00D3  GETMET	R12	R8	K41
      0x7C300200,  //  00D4  CALL	R12	1
      0x8C30192B,  //  00D5  GETMET	R12	R12	K43
      0x7C300200,  //  00D6  CALL	R12	1
      0x7C280400,  //  00D7  CALL	R10	2
      0x8C28132C,  //  00D8  GETMET	R10	R9	K44
      0x7C280200,  //  00D9  CALL	R10	1
      0x88280118,  //  00DA  GETMBR	R10	R0	K24
      0x8C28152D,  //  00DB  GETMET	R10	R10	K45
      0x8830132E,  //  00DC  GETMBR	R12	R9	K46
      0x5C340600,  //  00DD  MOVE	R13	R3
      0x5C380800,  //  00DE  MOVE	R14	R4
      0x883C132F,  //  00DF  GETMBR	R15	R9	K47
      0x7C280A00,  //  00E0  CALL	R10	5
      0x7002000E,  //  00E1  JMP		#00F1
      0x88240330,  //  00E2  GETMBR	R9	R1	K48
      0x7826000C,  //  00E3  JMPF	R9	#00F1
      0x8C240331,  //  00E4  GETMET	R9	R1	K49
      0x7C240200,  //  00E5  CALL	R9	1
      0x8C28132B,  //  00E6  GETMET	R10	R9	K43
      0x7C280200,  //  00E7  CALL	R10	1
      0x8C28132C,  //  00E8  GETMET	R10	R9	K44
      0x7C280200,  //  00E9  CALL	R10	1
      0x88280118,  //  00EA  GETMBR	R10	R0	K24
      0x8C28152D,  //  00EB  GETMET	R10	R10	K45
      0x8830132E,  //  00EC  GETMBR	R12	R9	K46
      0x5C340600,  //  00ED  MOVE	R13	R3
      0x5C380800,  //  00EE  MOVE	R14	R4
      0x883C132F,  //  00EF  GETMBR	R15	R9	K47
      0x7C280A00,  //  00F0  CALL	R10	5
      0x80000000,  //  00F1  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: subscribe_response
********************************************************************/
be_local_closure(Matter_IM_subscribe_response,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
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
      0xA4160000,  //  0000  IMPORT	R5	K0
      0xB81A0200,  //  0001  GETNGBL	R6	K1
      0x8C180D02,  //  0002  GETMET	R6	R6	K2
      0x7C180200,  //  0003  CALL	R6	1
      0x8C180D03,  //  0004  GETMET	R6	R6	K3
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C180400,  //  0006  CALL	R6	2
      0xB81E0800,  //  0007  GETNGBL	R7	K4
      0x8C1C0F05,  //  0008  GETMET	R7	R7	K5
      0x60240008,  //  0009  GETGBL	R9	G8
      0x5C280C00,  //  000A  MOVE	R10	R6
      0x7C240200,  //  000B  CALL	R9	1
      0x00260C09,  //  000C  ADD	R9	K6	R9
      0x58280007,  //  000D  LDCONST	R10	K7
      0x7C1C0600,  //  000E  CALL	R7	3
      0x501C0000,  //  000F  LDBOOL	R7	0	0
      0x80040E00,  //  0010  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: subscribe_request
********************************************************************/
be_local_closure(Matter_IM_subscribe_request,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
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
      0xA4160000,  //  0000  IMPORT	R5	K0
      0xB81A0200,  //  0001  GETNGBL	R6	K1
      0x8C180D02,  //  0002  GETMET	R6	R6	K2
      0x7C180200,  //  0003  CALL	R6	1
      0x8C180D03,  //  0004  GETMET	R6	R6	K3
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C180400,  //  0006  CALL	R6	2
      0xB81E0800,  //  0007  GETNGBL	R7	K4
      0x8C1C0F05,  //  0008  GETMET	R7	R7	K5
      0x60240008,  //  0009  GETGBL	R9	G8
      0x5C280C00,  //  000A  MOVE	R10	R6
      0x7C240200,  //  000B  CALL	R9	1
      0x00260C09,  //  000C  ADD	R9	K6	R9
      0x58280007,  //  000D  LDCONST	R10	K7
      0x7C1C0600,  //  000E  CALL	R7	3
      0x501C0000,  //  000F  LDBOOL	R7	0	0
      0x80040E00,  //  0010  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_read_request
********************************************************************/
be_local_closure(Matter_IM_process_read_request,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[43]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(MTR_X3A_X20IM_X3Aread_request_X20processing_X20start),
    /* K3   */  be_const_int(3),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(ReadRequestMessage),
    /* K6   */  be_nested_str_weak(from_TLV),
    /* K7   */  be_nested_str_weak(attributes_requests),
    /* K8   */  be_nested_str_weak(ReportDataMessage),
    /* K9   */  be_nested_str_weak(suppress_response),
    /* K10  */  be_nested_str_weak(attribute_reports),
    /* K11  */  be_nested_str_weak(get_attribute_name),
    /* K12  */  be_nested_str_weak(cluster),
    /* K13  */  be_nested_str_weak(attribute),
    /* K14  */  be_nested_str_weak(MTR_X3A_X20Read_X20Attribute_X20),
    /* K15  */  be_nested_str_weak(_X20_X28),
    /* K16  */  be_nested_str_weak(_X29),
    /* K17  */  be_nested_str_weak(),
    /* K18  */  be_const_int(2),
    /* K19  */  be_nested_str_weak(responder),
    /* K20  */  be_nested_str_weak(device),
    /* K21  */  be_nested_str_weak(read_attribute),
    /* K22  */  be_nested_str_weak(endpoint),
    /* K23  */  be_nested_str_weak(AttributeReportIB),
    /* K24  */  be_nested_str_weak(attribute_data),
    /* K25  */  be_nested_str_weak(AttributeDataIB),
    /* K26  */  be_nested_str_weak(data_version),
    /* K27  */  be_const_int(1),
    /* K28  */  be_nested_str_weak(path),
    /* K29  */  be_nested_str_weak(AttributePathIB),
    /* K30  */  be_const_int(0),
    /* K31  */  be_nested_str_weak(data),
    /* K32  */  be_nested_str_weak(push),
    /* K33  */  be_nested_str_weak(stop_iteration),
    /* K34  */  be_nested_str_weak(MTR_X3A_X20ReportDataMessage_X3D),
    /* K35  */  be_nested_str_weak(MTR_X3A_X20ReportDataMessageTLV_X3D),
    /* K36  */  be_nested_str_weak(to_TLV),
    /* K37  */  be_nested_str_weak(build_response),
    /* K38  */  be_nested_str_weak(encode),
    /* K39  */  be_nested_str_weak(encrypt),
    /* K40  */  be_nested_str_weak(send_response),
    /* K41  */  be_nested_str_weak(raw),
    /* K42  */  be_nested_str_weak(message_counter),
    }),
    be_str_weak(process_read_request),
    &be_const_str_solidified,
    ( &(const binstruction[132]) {  /* code */
      0xB8160000,  //  0000  GETNGBL	R5	K0
      0x8C140B01,  //  0001  GETMET	R5	R5	K1
      0x581C0002,  //  0002  LDCONST	R7	K2
      0x58200003,  //  0003  LDCONST	R8	K3
      0x7C140600,  //  0004  CALL	R5	3
      0xB8160800,  //  0005  GETNGBL	R5	K4
      0x8C140B05,  //  0006  GETMET	R5	R5	K5
      0x7C140200,  //  0007  CALL	R5	1
      0x8C140B06,  //  0008  GETMET	R5	R5	K6
      0x5C1C0400,  //  0009  MOVE	R7	R2
      0x7C140400,  //  000A  CALL	R5	2
      0x88180B07,  //  000B  GETMBR	R6	R5	K7
      0x4C1C0000,  //  000C  LDNIL	R7
      0x20180C07,  //  000D  NE	R6	R6	R7
      0x781A0072,  //  000E  JMPF	R6	#0082
      0xB81A0800,  //  000F  GETNGBL	R6	K4
      0x8C180D08,  //  0010  GETMET	R6	R6	K8
      0x7C180200,  //  0011  CALL	R6	1
      0x501C0200,  //  0012  LDBOOL	R7	1	0
      0x901A1207,  //  0013  SETMBR	R6	K9	R7
      0x601C0012,  //  0014  GETGBL	R7	G18
      0x7C1C0000,  //  0015  CALL	R7	0
      0x901A1407,  //  0016  SETMBR	R6	K10	R7
      0x601C0010,  //  0017  GETGBL	R7	G16
      0x88200B07,  //  0018  GETMBR	R8	R5	K7
      0x7C1C0200,  //  0019  CALL	R7	1
      0xA802003F,  //  001A  EXBLK	0	#005B
      0x5C200E00,  //  001B  MOVE	R8	R7
      0x7C200000,  //  001C  CALL	R8	0
      0xB8260800,  //  001D  GETNGBL	R9	K4
      0x8C24130B,  //  001E  GETMET	R9	R9	K11
      0x882C110C,  //  001F  GETMBR	R11	R8	K12
      0x8830110D,  //  0020  GETMBR	R12	R8	K13
      0x7C240600,  //  0021  CALL	R9	3
      0xB82A0000,  //  0022  GETNGBL	R10	K0
      0x8C281501,  //  0023  GETMET	R10	R10	K1
      0x60300008,  //  0024  GETGBL	R12	G8
      0x5C341000,  //  0025  MOVE	R13	R8
      0x7C300200,  //  0026  CALL	R12	1
      0x00321C0C,  //  0027  ADD	R12	K14	R12
      0x78260002,  //  0028  JMPF	R9	#002C
      0x00361E09,  //  0029  ADD	R13	K15	R9
      0x00341B10,  //  002A  ADD	R13	R13	K16
      0x70020000,  //  002B  JMP		#002D
      0x58340011,  //  002C  LDCONST	R13	K17
      0x0030180D,  //  002D  ADD	R12	R12	R13
      0x58340012,  //  002E  LDCONST	R13	K18
      0x7C280600,  //  002F  CALL	R10	3
      0x88280113,  //  0030  GETMBR	R10	R0	K19
      0x88281514,  //  0031  GETMBR	R10	R10	K20
      0x8C281515,  //  0032  GETMET	R10	R10	K21
      0x5C300200,  //  0033  MOVE	R12	R1
      0x88341116,  //  0034  GETMBR	R13	R8	K22
      0x8838110C,  //  0035  GETMBR	R14	R8	K12
      0x883C110D,  //  0036  GETMBR	R15	R8	K13
      0x7C280A00,  //  0037  CALL	R10	5
      0x4C2C0000,  //  0038  LDNIL	R11
      0x202C140B,  //  0039  NE	R11	R10	R11
      0x782E001E,  //  003A  JMPF	R11	#005A
      0xB82E0800,  //  003B  GETNGBL	R11	K4
      0x8C2C1717,  //  003C  GETMET	R11	R11	K23
      0x7C2C0200,  //  003D  CALL	R11	1
      0xB8320800,  //  003E  GETNGBL	R12	K4
      0x8C301919,  //  003F  GETMET	R12	R12	K25
      0x7C300200,  //  0040  CALL	R12	1
      0x902E300C,  //  0041  SETMBR	R11	K24	R12
      0x88301718,  //  0042  GETMBR	R12	R11	K24
      0x9032351B,  //  0043  SETMBR	R12	K26	K27
      0x88301718,  //  0044  GETMBR	R12	R11	K24
      0xB8360800,  //  0045  GETNGBL	R13	K4
      0x8C341B1D,  //  0046  GETMET	R13	R13	K29
      0x7C340200,  //  0047  CALL	R13	1
      0x9032380D,  //  0048  SETMBR	R12	K28	R13
      0x88301718,  //  0049  GETMBR	R12	R11	K24
      0x8830191C,  //  004A  GETMBR	R12	R12	K28
      0x90322D1E,  //  004B  SETMBR	R12	K22	K30
      0x88301718,  //  004C  GETMBR	R12	R11	K24
      0x8830191C,  //  004D  GETMBR	R12	R12	K28
      0x8834110C,  //  004E  GETMBR	R13	R8	K12
      0x9032180D,  //  004F  SETMBR	R12	K12	R13
      0x88301718,  //  0050  GETMBR	R12	R11	K24
      0x8830191C,  //  0051  GETMBR	R12	R12	K28
      0x8834110D,  //  0052  GETMBR	R13	R8	K13
      0x90321A0D,  //  0053  SETMBR	R12	K13	R13
      0x88301718,  //  0054  GETMBR	R12	R11	K24
      0x90323E0A,  //  0055  SETMBR	R12	K31	R10
      0x88300D0A,  //  0056  GETMBR	R12	R6	K10
      0x8C301920,  //  0057  GETMET	R12	R12	K32
      0x5C381600,  //  0058  MOVE	R14	R11
      0x7C300400,  //  0059  CALL	R12	2
      0x7001FFBF,  //  005A  JMP		#001B
      0x581C0021,  //  005B  LDCONST	R7	K33
      0xAC1C0200,  //  005C  CATCH	R7	1	0
      0xB0080000,  //  005D  RAISE	2	R0	R0
      0xB81E0000,  //  005E  GETNGBL	R7	K0
      0x8C1C0F01,  //  005F  GETMET	R7	R7	K1
      0x60240008,  //  0060  GETGBL	R9	G8
      0x5C280C00,  //  0061  MOVE	R10	R6
      0x7C240200,  //  0062  CALL	R9	1
      0x00264409,  //  0063  ADD	R9	K34	R9
      0x58280003,  //  0064  LDCONST	R10	K3
      0x7C1C0600,  //  0065  CALL	R7	3
      0xB81E0000,  //  0066  GETNGBL	R7	K0
      0x8C1C0F01,  //  0067  GETMET	R7	R7	K1
      0x60240008,  //  0068  GETGBL	R9	G8
      0x8C280D24,  //  0069  GETMET	R10	R6	K36
      0x7C280200,  //  006A  CALL	R10	1
      0x7C240200,  //  006B  CALL	R9	1
      0x00264609,  //  006C  ADD	R9	K35	R9
      0x58280003,  //  006D  LDCONST	R10	K3
      0x7C1C0600,  //  006E  CALL	R7	3
      0x8C1C0325,  //  006F  GETMET	R7	R1	K37
      0x54260004,  //  0070  LDINT	R9	5
      0x50280200,  //  0071  LDBOOL	R10	1	0
      0x7C1C0600,  //  0072  CALL	R7	3
      0x8C200F26,  //  0073  GETMET	R8	R7	K38
      0x8C280D24,  //  0074  GETMET	R10	R6	K36
      0x7C280200,  //  0075  CALL	R10	1
      0x8C281526,  //  0076  GETMET	R10	R10	K38
      0x7C280200,  //  0077  CALL	R10	1
      0x7C200400,  //  0078  CALL	R8	2
      0x8C200F27,  //  0079  GETMET	R8	R7	K39
      0x7C200200,  //  007A  CALL	R8	1
      0x88200113,  //  007B  GETMBR	R8	R0	K19
      0x8C201128,  //  007C  GETMET	R8	R8	K40
      0x88280F29,  //  007D  GETMBR	R10	R7	K41
      0x5C2C0600,  //  007E  MOVE	R11	R3
      0x5C300800,  //  007F  MOVE	R12	R4
      0x88340F2A,  //  0080  GETMBR	R13	R7	K42
      0x7C200A00,  //  0081  CALL	R8	5
      0x50180200,  //  0082  LDBOOL	R6	1	0
      0x80040C00,  //  0083  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: report_data
********************************************************************/
be_local_closure(Matter_IM_report_data,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
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
      0xA4160000,  //  0000  IMPORT	R5	K0
      0xB81A0200,  //  0001  GETNGBL	R6	K1
      0x8C180D02,  //  0002  GETMET	R6	R6	K2
      0x7C180200,  //  0003  CALL	R6	1
      0x8C180D03,  //  0004  GETMET	R6	R6	K3
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C180400,  //  0006  CALL	R6	2
      0xB81E0800,  //  0007  GETNGBL	R7	K4
      0x8C1C0F05,  //  0008  GETMET	R7	R7	K5
      0x60240008,  //  0009  GETGBL	R9	G8
      0x5C280C00,  //  000A  MOVE	R10	R6
      0x7C240200,  //  000B  CALL	R9	1
      0x00260C09,  //  000C  ADD	R9	K6	R9
      0x58280007,  //  000D  LDCONST	R10	K7
      0x7C1C0600,  //  000E  CALL	R7	3
      0x501C0000,  //  000F  LDBOOL	R7	0	0
      0x80040E00,  //  0010  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_invoke_response
********************************************************************/
be_local_closure(Matter_IM_process_invoke_response,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
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
      0xA4160000,  //  0000  IMPORT	R5	K0
      0xB81A0200,  //  0001  GETNGBL	R6	K1
      0x8C180D02,  //  0002  GETMET	R6	R6	K2
      0x7C180200,  //  0003  CALL	R6	1
      0x8C180D03,  //  0004  GETMET	R6	R6	K3
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C180400,  //  0006  CALL	R6	2
      0xB81E0800,  //  0007  GETNGBL	R7	K4
      0x8C1C0F05,  //  0008  GETMET	R7	R7	K5
      0x60240008,  //  0009  GETGBL	R9	G8
      0x5C280C00,  //  000A  MOVE	R10	R6
      0x7C240200,  //  000B  CALL	R9	1
      0x00260C09,  //  000C  ADD	R9	K6	R9
      0x58280007,  //  000D  LDCONST	R10	K7
      0x7C1C0600,  //  000E  CALL	R7	3
      0x501C0000,  //  000F  LDBOOL	R7	0	0
      0x80040E00,  //  0010  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_IM
********************************************************************/
be_local_class(Matter_IM,
    1,
    NULL,
    be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(process_timed_request, -1), be_const_closure(Matter_IM_process_timed_request_closure) },
        { be_const_key_weak(process_status_response, -1), be_const_closure(Matter_IM_process_status_response_closure) },
        { be_const_key_weak(process_incoming, -1), be_const_closure(Matter_IM_process_incoming_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_IM_every_second_closure) },
        { be_const_key_weak(process_write_response, -1), be_const_closure(Matter_IM_process_write_response_closure) },
        { be_const_key_weak(init, 10), be_const_closure(Matter_IM_init_closure) },
        { be_const_key_weak(process_write_request, -1), be_const_closure(Matter_IM_process_write_request_closure) },
        { be_const_key_weak(process_invoke_request, 2), be_const_closure(Matter_IM_process_invoke_request_closure) },
        { be_const_key_weak(process_read_request, -1), be_const_closure(Matter_IM_process_read_request_closure) },
        { be_const_key_weak(subscribe_response, -1), be_const_closure(Matter_IM_subscribe_response_closure) },
        { be_const_key_weak(subscribe_request, 7), be_const_closure(Matter_IM_subscribe_request_closure) },
        { be_const_key_weak(responder, 8), be_const_var(0) },
        { be_const_key_weak(report_data, -1), be_const_closure(Matter_IM_report_data_closure) },
        { be_const_key_weak(process_invoke_response, -1), be_const_closure(Matter_IM_process_invoke_response_closure) },
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
