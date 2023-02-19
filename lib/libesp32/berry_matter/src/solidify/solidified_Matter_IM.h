/* Solidification of Matter_IM.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_IM;

/********************************************************************
** Solidified function: send_subscribe_response
********************************************************************/
be_local_closure(Matter_IM_send_subscribe_response,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(send_queue),
    /* K1   */  be_nested_str_weak(push),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(IM_SubscribeResponse),
    }),
    be_str_weak(send_subscribe_response),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x8C100901,  //  0001  GETMET	R4	R4	K1
      0xB81A0400,  //  0002  GETNGBL	R6	K2
      0x8C180D03,  //  0003  GETMET	R6	R6	K3
      0x5C200200,  //  0004  MOVE	R8	R1
      0x5C240400,  //  0005  MOVE	R9	R2
      0x5C280600,  //  0006  MOVE	R10	R3
      0x7C180800,  //  0007  CALL	R6	4
      0x7C100400,  //  0008  CALL	R4	2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_write_request
********************************************************************/
be_local_closure(Matter_IM_process_write_request,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        19,                          /* nstack */
        5,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 1),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[25]) {     /* constants */
        /* K0   */  be_nested_str_weak(string),
        /* K1   */  be_nested_str_weak(matter),
        /* K2   */  be_nested_str_weak(get_attribute_name),
        /* K3   */  be_nested_str_weak(cluster),
        /* K4   */  be_nested_str_weak(attribute),
        /* K5   */  be_nested_str_weak(_X20_X28),
        /* K6   */  be_nested_str_weak(_X29),
        /* K7   */  be_nested_str_weak(),
        /* K8   */  be_nested_str_weak(status),
        /* K9   */  be_nested_str_weak(UNSUPPORTED_WRITE),
        /* K10  */  be_nested_str_weak(write_attribute),
        /* K11  */  be_nested_str_weak(SUCCESS),
        /* K12  */  be_nested_str_weak(AttributeStatusIB),
        /* K13  */  be_nested_str_weak(path),
        /* K14  */  be_nested_str_weak(AttributePathIB),
        /* K15  */  be_nested_str_weak(StatusIB),
        /* K16  */  be_nested_str_weak(endpoint),
        /* K17  */  be_nested_str_weak(write_responses),
        /* K18  */  be_nested_str_weak(push),
        /* K19  */  be_nested_str_weak(tasmota),
        /* K20  */  be_nested_str_weak(log),
        /* K21  */  be_nested_str_weak(format),
        /* K22  */  be_nested_str_weak(MTR_X3A_X20Write_Attr_X20_X25s_X25s_X20_X2D_X20STATUS_X3A_X200x_X2502X_X20_X25s),
        /* K23  */  be_const_int(2),
        /* K24  */  be_nested_str_weak(MTR_X3A_X20Write_Attr_X20_X25s_X25s_X20_X2D_X20IGNORED),
        }),
        be_str_weak(write_single_attribute),
        &be_const_str_solidified,
        ( &(const binstruction[96]) {  /* code */
          0xA4160000,  //  0000  IMPORT	R5	K0
          0xB81A0200,  //  0001  GETNGBL	R6	K1
          0x8C180D02,  //  0002  GETMET	R6	R6	K2
          0x88200503,  //  0003  GETMBR	R8	R2	K3
          0x88240504,  //  0004  GETMBR	R9	R2	K4
          0x7C180600,  //  0005  CALL	R6	3
          0x781A0002,  //  0006  JMPF	R6	#000A
          0x001E0A06,  //  0007  ADD	R7	K5	R6
          0x001C0F06,  //  0008  ADD	R7	R7	K6
          0x70020000,  //  0009  JMP		#000B
          0x581C0007,  //  000A  LDCONST	R7	K7
          0x5C180E00,  //  000B  MOVE	R6	R7
          0xB81E0200,  //  000C  GETNGBL	R7	K1
          0x881C0F09,  //  000D  GETMBR	R7	R7	K9
          0x900A1007,  //  000E  SETMBR	R2	K8	R7
          0x4C1C0000,  //  000F  LDNIL	R7
          0x201C0207,  //  0010  NE	R7	R1	R7
          0x781E0005,  //  0011  JMPF	R7	#0018
          0x8C1C030A,  //  0012  GETMET	R7	R1	K10
          0x68240000,  //  0013  GETUPV	R9	U0
          0x5C280400,  //  0014  MOVE	R10	R2
          0x5C2C0600,  //  0015  MOVE	R11	R3
          0x7C1C0800,  //  0016  CALL	R7	4
          0x70020000,  //  0017  JMP		#0019
          0x4C1C0000,  //  0018  LDNIL	R7
          0x781E0002,  //  0019  JMPF	R7	#001D
          0xB8220200,  //  001A  GETNGBL	R8	K1
          0x8820110B,  //  001B  GETMBR	R8	R8	K11
          0x900A1008,  //  001C  SETMBR	R2	K8	R8
          0x88200508,  //  001D  GETMBR	R8	R2	K8
          0x4C240000,  //  001E  LDNIL	R9
          0x20201009,  //  001F  NE	R8	R8	R9
          0x78220032,  //  0020  JMPF	R8	#0054
          0x78120030,  //  0021  JMPF	R4	#0053
          0xB8220200,  //  0022  GETNGBL	R8	K1
          0x8C20110C,  //  0023  GETMET	R8	R8	K12
          0x7C200200,  //  0024  CALL	R8	1
          0xB8260200,  //  0025  GETNGBL	R9	K1
          0x8C24130E,  //  0026  GETMET	R9	R9	K14
          0x7C240200,  //  0027  CALL	R9	1
          0x90221A09,  //  0028  SETMBR	R8	K13	R9
          0xB8260200,  //  0029  GETNGBL	R9	K1
          0x8C24130F,  //  002A  GETMET	R9	R9	K15
          0x7C240200,  //  002B  CALL	R9	1
          0x90221009,  //  002C  SETMBR	R8	K8	R9
          0x8824110D,  //  002D  GETMBR	R9	R8	K13
          0x88280510,  //  002E  GETMBR	R10	R2	K16
          0x9026200A,  //  002F  SETMBR	R9	K16	R10
          0x8824110D,  //  0030  GETMBR	R9	R8	K13
          0x88280503,  //  0031  GETMBR	R10	R2	K3
          0x9026060A,  //  0032  SETMBR	R9	K3	R10
          0x8824110D,  //  0033  GETMBR	R9	R8	K13
          0x88280504,  //  0034  GETMBR	R10	R2	K4
          0x9026080A,  //  0035  SETMBR	R9	K4	R10
          0x88241108,  //  0036  GETMBR	R9	R8	K8
          0x88280508,  //  0037  GETMBR	R10	R2	K8
          0x9026100A,  //  0038  SETMBR	R9	K8	R10
          0x88240111,  //  0039  GETMBR	R9	R0	K17
          0x8C241312,  //  003A  GETMET	R9	R9	K18
          0x5C2C1000,  //  003B  MOVE	R11	R8
          0x7C240400,  //  003C  CALL	R9	2
          0xB8262600,  //  003D  GETNGBL	R9	K19
          0x8C241314,  //  003E  GETMET	R9	R9	K20
          0x8C2C0B15,  //  003F  GETMET	R11	R5	K21
          0x58340016,  //  0040  LDCONST	R13	K22
          0x60380008,  //  0041  GETGBL	R14	G8
          0x5C3C0400,  //  0042  MOVE	R15	R2
          0x7C380200,  //  0043  CALL	R14	1
          0x5C3C0C00,  //  0044  MOVE	R15	R6
          0x88400508,  //  0045  GETMBR	R16	R2	K8
          0x88440508,  //  0046  GETMBR	R17	R2	K8
          0xB84A0200,  //  0047  GETNGBL	R18	K1
          0x8848250B,  //  0048  GETMBR	R18	R18	K11
          0x1C442212,  //  0049  EQ	R17	R17	R18
          0x78460001,  //  004A  JMPF	R17	#004D
          0x5844000B,  //  004B  LDCONST	R17	K11
          0x70020000,  //  004C  JMP		#004E
          0x58440007,  //  004D  LDCONST	R17	K7
          0x7C2C0C00,  //  004E  CALL	R11	6
          0x58300017,  //  004F  LDCONST	R12	K23
          0x7C240600,  //  0050  CALL	R9	3
          0x50240200,  //  0051  LDBOOL	R9	1	0
          0x80041200,  //  0052  RET	1	R9
          0x7002000A,  //  0053  JMP		#005F
          0xB8222600,  //  0054  GETNGBL	R8	K19
          0x8C201114,  //  0055  GETMET	R8	R8	K20
          0x8C280B15,  //  0056  GETMET	R10	R5	K21
          0x58300018,  //  0057  LDCONST	R12	K24
          0x60340008,  //  0058  GETGBL	R13	G8
          0x5C380400,  //  0059  MOVE	R14	R2
          0x7C340200,  //  005A  CALL	R13	1
          0x5C380C00,  //  005B  MOVE	R14	R6
          0x7C280800,  //  005C  CALL	R10	4
          0x582C0017,  //  005D  LDCONST	R11	K23
          0x7C200600,  //  005E  CALL	R8	3
          0x80000000,  //  005F  RET	0
        })
      ),
      be_nested_proto(
        9,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 3]) {  /* upvals */
          be_local_const_upval(1, 7),
          be_local_const_upval(1, 9),
          be_local_const_upval(1, 13),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        0,                          /* has constants */
        NULL,                       /* no const */
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 8]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x68100001,  //  0001  GETUPV	R4	U1
          0x5C140000,  //  0002  MOVE	R5	R0
          0x5C180200,  //  0003  MOVE	R6	R1
          0x681C0002,  //  0004  GETUPV	R7	U2
          0x5C200400,  //  0005  MOVE	R8	R2
          0x7C0C0A00,  //  0006  CALL	R3	5
          0x80040600,  //  0007  RET	1	R3
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[36]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(WriteRequestMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20received_X20WriteRequestMessage_X3D),
    /* K7   */  be_const_int(3),
    /* K8   */  be_nested_str_weak(suppress_response),
    /* K9   */  be_nested_str_weak(device),
    /* K10  */  be_nested_str_weak(get_active_endpoints),
    /* K11  */  be_nested_str_weak(MTR_X3A_X20IM_X3Awrite_request_X20processing_X20start),
    /* K12  */  be_nested_str_weak(Path),
    /* K13  */  be_nested_str_weak(write_requests),
    /* K14  */  be_nested_str_weak(WriteResponseMessage),
    /* K15  */  be_nested_str_weak(write_responses),
    /* K16  */  be_nested_str_weak(path),
    /* K17  */  be_nested_str_weak(data),
    /* K18  */  be_nested_str_weak(endpoint),
    /* K19  */  be_nested_str_weak(cluster),
    /* K20  */  be_nested_str_weak(attribute),
    /* K21  */  be_nested_str_weak(status),
    /* K22  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K23  */  be_nested_str_weak(INVALID_ACTION),
    /* K24  */  be_nested_str_weak(get_attribute_name),
    /* K25  */  be_nested_str_weak(MTR_X3A_X20Write_Attr_X20),
    /* K26  */  be_nested_str_weak(_X20_X28),
    /* K27  */  be_nested_str_weak(_X29),
    /* K28  */  be_nested_str_weak(),
    /* K29  */  be_const_int(2),
    /* K30  */  be_nested_str_weak(process_attribute_expansion),
    /* K31  */  be_nested_str_weak(stop_iteration),
    /* K32  */  be_nested_str_weak(MTR_X3A_X20ReportWriteMessage_X3D),
    /* K33  */  be_nested_str_weak(MTR_X3A_X20ReportWriteMessageTLV_X3D),
    /* K34  */  be_nested_str_weak(to_TLV),
    /* K35  */  be_nested_str_weak(send_write_response),
    }),
    be_str_weak(process_write_request),
    &be_const_str_solidified,
    ( &(const binstruction[134]) {  /* code */
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
      0x88140908,  //  000F  GETMBR	R5	R4	K8
      0x88180109,  //  0010  GETMBR	R6	R0	K9
      0x8C180D0A,  //  0011  GETMET	R6	R6	K10
      0x7C180200,  //  0012  CALL	R6	1
      0x841C0000,  //  0013  CLOSURE	R7	P0
      0xB8220800,  //  0014  GETNGBL	R8	K4
      0x8C201105,  //  0015  GETMET	R8	R8	K5
      0x5828000B,  //  0016  LDCONST	R10	K11
      0x542E0003,  //  0017  LDINT	R11	4
      0x7C200600,  //  0018  CALL	R8	3
      0xB8220200,  //  0019  GETNGBL	R8	K1
      0x8C20110C,  //  001A  GETMET	R8	R8	K12
      0x7C200200,  //  001B  CALL	R8	1
      0x8824090D,  //  001C  GETMBR	R9	R4	K13
      0x4C280000,  //  001D  LDNIL	R10
      0x2024120A,  //  001E  NE	R9	R9	R10
      0x78260062,  //  001F  JMPF	R9	#0083
      0xB8260200,  //  0020  GETNGBL	R9	K1
      0x8C24130E,  //  0021  GETMET	R9	R9	K14
      0x7C240200,  //  0022  CALL	R9	1
      0x60280012,  //  0023  GETGBL	R10	G18
      0x7C280000,  //  0024  CALL	R10	0
      0x90261E0A,  //  0025  SETMBR	R9	K15	R10
      0x60280010,  //  0026  GETGBL	R10	G16
      0x882C090D,  //  0027  GETMBR	R11	R4	K13
      0x7C280200,  //  0028  CALL	R10	1
      0xA802003D,  //  0029  EXBLK	0	#0068
      0x5C2C1400,  //  002A  MOVE	R11	R10
      0x7C2C0000,  //  002B  CALL	R11	0
      0x88301710,  //  002C  GETMBR	R12	R11	K16
      0x88341711,  //  002D  GETMBR	R13	R11	K17
      0x88381912,  //  002E  GETMBR	R14	R12	K18
      0x9022240E,  //  002F  SETMBR	R8	K18	R14
      0x88381913,  //  0030  GETMBR	R14	R12	K19
      0x9022260E,  //  0031  SETMBR	R8	K19	R14
      0x88381914,  //  0032  GETMBR	R14	R12	K20
      0x9022280E,  //  0033  SETMBR	R8	K20	R14
      0xB83A0200,  //  0034  GETNGBL	R14	K1
      0x88381D16,  //  0035  GETMBR	R14	R14	K22
      0x90222A0E,  //  0036  SETMBR	R8	K21	R14
      0x88381113,  //  0037  GETMBR	R14	R8	K19
      0x4C3C0000,  //  0038  LDNIL	R15
      0x1C381C0F,  //  0039  EQ	R14	R14	R15
      0x743A0003,  //  003A  JMPT	R14	#003F
      0x88381114,  //  003B  GETMBR	R14	R8	K20
      0x4C3C0000,  //  003C  LDNIL	R15
      0x1C381C0F,  //  003D  EQ	R14	R14	R15
      0x783A000A,  //  003E  JMPF	R14	#004A
      0xB83A0200,  //  003F  GETNGBL	R14	K1
      0x88381D17,  //  0040  GETMBR	R14	R14	K23
      0x90222A0E,  //  0041  SETMBR	R8	K21	R14
      0x5C380E00,  //  0042  MOVE	R14	R7
      0x5C3C1200,  //  0043  MOVE	R15	R9
      0x4C400000,  //  0044  LDNIL	R16
      0x5C441000,  //  0045  MOVE	R17	R8
      0x4C480000,  //  0046  LDNIL	R18
      0x504C0200,  //  0047  LDBOOL	R19	1	0
      0x7C380A00,  //  0048  CALL	R14	5
      0x7001FFDF,  //  0049  JMP		#002A
      0x88381112,  //  004A  GETMBR	R14	R8	K18
      0x4C3C0000,  //  004B  LDNIL	R15
      0x1C381C0F,  //  004C  EQ	R14	R14	R15
      0x783A0012,  //  004D  JMPF	R14	#0061
      0xB83A0200,  //  004E  GETNGBL	R14	K1
      0x8C381D18,  //  004F  GETMET	R14	R14	K24
      0x88401113,  //  0050  GETMBR	R16	R8	K19
      0x88441114,  //  0051  GETMBR	R17	R8	K20
      0x7C380600,  //  0052  CALL	R14	3
      0xB83E0800,  //  0053  GETNGBL	R15	K4
      0x8C3C1F05,  //  0054  GETMET	R15	R15	K5
      0x60440008,  //  0055  GETGBL	R17	G8
      0x5C481000,  //  0056  MOVE	R18	R8
      0x7C440200,  //  0057  CALL	R17	1
      0x00463211,  //  0058  ADD	R17	K25	R17
      0x783A0002,  //  0059  JMPF	R14	#005D
      0x004A340E,  //  005A  ADD	R18	K26	R14
      0x0048251B,  //  005B  ADD	R18	R18	K27
      0x70020000,  //  005C  JMP		#005E
      0x5848001C,  //  005D  LDCONST	R18	K28
      0x00442212,  //  005E  ADD	R17	R17	R18
      0x5848001D,  //  005F  LDCONST	R18	K29
      0x7C3C0600,  //  0060  CALL	R15	3
      0x88380109,  //  0061  GETMBR	R14	R0	K9
      0x8C381D1E,  //  0062  GETMET	R14	R14	K30
      0x5C401000,  //  0063  MOVE	R16	R8
      0x84440001,  //  0064  CLOSURE	R17	P1
      0x7C380600,  //  0065  CALL	R14	3
      0xA0280000,  //  0066  CLOSE	R10
      0x7001FFC1,  //  0067  JMP		#002A
      0x5828001F,  //  0068  LDCONST	R10	K31
      0xAC280200,  //  0069  CATCH	R10	1	0
      0xB0080000,  //  006A  RAISE	2	R0	R0
      0xB82A0800,  //  006B  GETNGBL	R10	K4
      0x8C281505,  //  006C  GETMET	R10	R10	K5
      0x60300008,  //  006D  GETGBL	R12	G8
      0x5C341200,  //  006E  MOVE	R13	R9
      0x7C300200,  //  006F  CALL	R12	1
      0x0032400C,  //  0070  ADD	R12	K32	R12
      0x54360003,  //  0071  LDINT	R13	4
      0x7C280600,  //  0072  CALL	R10	3
      0xB82A0800,  //  0073  GETNGBL	R10	K4
      0x8C281505,  //  0074  GETMET	R10	R10	K5
      0x60300008,  //  0075  GETGBL	R12	G8
      0x8C341322,  //  0076  GETMET	R13	R9	K34
      0x7C340200,  //  0077  CALL	R13	1
      0x7C300200,  //  0078  CALL	R12	1
      0x0032420C,  //  0079  ADD	R12	K33	R12
      0x58340007,  //  007A  LDCONST	R13	K7
      0x7C280600,  //  007B  CALL	R10	3
      0x5C280A00,  //  007C  MOVE	R10	R5
      0x742A0003,  //  007D  JMPT	R10	#0082
      0x8C280123,  //  007E  GETMET	R10	R0	K35
      0x5C300200,  //  007F  MOVE	R12	R1
      0x5C341200,  //  0080  MOVE	R13	R9
      0x7C280600,  //  0081  CALL	R10	3
      0xA0240000,  //  0082  CLOSE	R9
      0x50240200,  //  0083  LDBOOL	R9	1	0
      0xA0000000,  //  0084  CLOSE	R0
      0x80041200,  //  0085  RET	1	R9
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _inner_process_read_request
********************************************************************/
be_local_closure(Matter_IM__inner_process_read_request,   /* name */
  be_nested_proto(
    14,                          /* nstack */
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
        /* K23  */  be_nested_str_weak(MTR_X3A_X20Read_Attr_X20_X20_X25s_X25s_X20_X2D_X20_X25s),
        /* K24  */  be_const_int(2),
        /* K25  */  be_nested_str_weak(status),
        /* K26  */  be_nested_str_weak(attribute_status),
        /* K27  */  be_nested_str_weak(AttributeStatusIB),
        /* K28  */  be_nested_str_weak(StatusIB),
        /* K29  */  be_nested_str_weak(MTR_X3A_X20Read_Attr_X20_X20_X25s_X25s_X20_X2D_X20STATUS_X3A_X200x_X2502X_X20_X25s),
        /* K30  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
        /* K31  */  be_nested_str_weak(MTR_X3A_X20Read_Attr_X20_X20_X25s_X25s_X20_X2D_X20IGNORED),
        }),
        be_str_weak(read_single_attribute),
        &be_const_str_solidified,
        ( &(const binstruction[152]) {  /* code */
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
          0x7002004D,  //  0048  JMP		#0097
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
          0x7002000C,  //  0089  JMP		#0097
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
          0x501C0000,  //  0095  LDBOOL	R7	0	0
          0x80040E00,  //  0096  RET	1	R7
          0x80000000,  //  0097  RET	0
        })
      ),
      be_nested_proto(
        8,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 4),
          be_local_const_upval(1, 6),
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
    ( &(const bvalue[26]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(get_active_endpoints),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(Path),
    /* K4   */  be_nested_str_weak(ReportDataMessage),
    /* K5   */  be_nested_str_weak(attribute_reports),
    /* K6   */  be_nested_str_weak(attributes_requests),
    /* K7   */  be_nested_str_weak(endpoint),
    /* K8   */  be_nested_str_weak(cluster),
    /* K9   */  be_nested_str_weak(attribute),
    /* K10  */  be_nested_str_weak(status),
    /* K11  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K12  */  be_nested_str_weak(get_attribute_name),
    /* K13  */  be_nested_str_weak(tasmota),
    /* K14  */  be_nested_str_weak(log),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20Read_Attr_X20_X20),
    /* K16  */  be_nested_str_weak(_X20_X28),
    /* K17  */  be_nested_str_weak(_X29),
    /* K18  */  be_nested_str_weak(),
    /* K19  */  be_const_int(2),
    /* K20  */  be_nested_str_weak(process_attribute_expansion),
    /* K21  */  be_nested_str_weak(stop_iteration),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20ReportDataMessage_X3D),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20ReportDataMessageTLV_X3D),
    /* K24  */  be_nested_str_weak(to_TLV),
    /* K25  */  be_const_int(3),
    }),
    be_str_weak(_inner_process_read_request),
    &be_const_str_solidified,
    ( &(const binstruction[104]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x84100000,  //  0003  CLOSURE	R4	P0
      0xB8160400,  //  0004  GETNGBL	R5	K2
      0x8C140B03,  //  0005  GETMET	R5	R5	K3
      0x7C140200,  //  0006  CALL	R5	1
      0xB81A0400,  //  0007  GETNGBL	R6	K2
      0x8C180D04,  //  0008  GETMET	R6	R6	K4
      0x7C180200,  //  0009  CALL	R6	1
      0x601C0012,  //  000A  GETGBL	R7	G18
      0x7C1C0000,  //  000B  CALL	R7	0
      0x901A0A07,  //  000C  SETMBR	R6	K5	R7
      0x601C0010,  //  000D  GETGBL	R7	G16
      0x88200506,  //  000E  GETMBR	R8	R2	K6
      0x7C1C0200,  //  000F  CALL	R7	1
      0xA8020040,  //  0010  EXBLK	0	#0052
      0x5C200E00,  //  0011  MOVE	R8	R7
      0x7C200000,  //  0012  CALL	R8	0
      0x88241107,  //  0013  GETMBR	R9	R8	K7
      0x90160E09,  //  0014  SETMBR	R5	K7	R9
      0x88241108,  //  0015  GETMBR	R9	R8	K8
      0x90161009,  //  0016  SETMBR	R5	K8	R9
      0x88241109,  //  0017  GETMBR	R9	R8	K9
      0x90161209,  //  0018  SETMBR	R5	K9	R9
      0xB8260400,  //  0019  GETNGBL	R9	K2
      0x8824130B,  //  001A  GETMBR	R9	R9	K11
      0x90161409,  //  001B  SETMBR	R5	K10	R9
      0x88240B07,  //  001C  GETMBR	R9	R5	K7
      0x4C280000,  //  001D  LDNIL	R10
      0x1C24120A,  //  001E  EQ	R9	R9	R10
      0x74260007,  //  001F  JMPT	R9	#0028
      0x88240B08,  //  0020  GETMBR	R9	R5	K8
      0x4C280000,  //  0021  LDNIL	R10
      0x1C24120A,  //  0022  EQ	R9	R9	R10
      0x74260003,  //  0023  JMPT	R9	#0028
      0x88240B09,  //  0024  GETMBR	R9	R5	K9
      0x4C280000,  //  0025  LDNIL	R10
      0x1C24120A,  //  0026  EQ	R9	R9	R10
      0x78260023,  //  0027  JMPF	R9	#004C
      0x88240B08,  //  0028  GETMBR	R9	R5	K8
      0x4C280000,  //  0029  LDNIL	R10
      0x2024120A,  //  002A  NE	R9	R9	R10
      0x78260017,  //  002B  JMPF	R9	#0044
      0x88240B09,  //  002C  GETMBR	R9	R5	K9
      0x4C280000,  //  002D  LDNIL	R10
      0x2024120A,  //  002E  NE	R9	R9	R10
      0x78260013,  //  002F  JMPF	R9	#0044
      0xB8260400,  //  0030  GETNGBL	R9	K2
      0x8C24130C,  //  0031  GETMET	R9	R9	K12
      0x882C0B08,  //  0032  GETMBR	R11	R5	K8
      0x88300B09,  //  0033  GETMBR	R12	R5	K9
      0x7C240600,  //  0034  CALL	R9	3
      0xB82A1A00,  //  0035  GETNGBL	R10	K13
      0x8C28150E,  //  0036  GETMET	R10	R10	K14
      0x60300008,  //  0037  GETGBL	R12	G8
      0x5C340A00,  //  0038  MOVE	R13	R5
      0x7C300200,  //  0039  CALL	R12	1
      0x00321E0C,  //  003A  ADD	R12	K15	R12
      0x78260002,  //  003B  JMPF	R9	#003F
      0x00362009,  //  003C  ADD	R13	K16	R9
      0x00341B11,  //  003D  ADD	R13	R13	K17
      0x70020000,  //  003E  JMP		#0040
      0x58340012,  //  003F  LDCONST	R13	K18
      0x0030180D,  //  0040  ADD	R12	R12	R13
      0x58340013,  //  0041  LDCONST	R13	K19
      0x7C280600,  //  0042  CALL	R10	3
      0x70020007,  //  0043  JMP		#004C
      0xB8261A00,  //  0044  GETNGBL	R9	K13
      0x8C24130E,  //  0045  GETMET	R9	R9	K14
      0x602C0008,  //  0046  GETGBL	R11	G8
      0x5C300A00,  //  0047  MOVE	R12	R5
      0x7C2C0200,  //  0048  CALL	R11	1
      0x002E1E0B,  //  0049  ADD	R11	K15	R11
      0x58300013,  //  004A  LDCONST	R12	K19
      0x7C240600,  //  004B  CALL	R9	3
      0x88240100,  //  004C  GETMBR	R9	R0	K0
      0x8C241314,  //  004D  GETMET	R9	R9	K20
      0x5C2C0A00,  //  004E  MOVE	R11	R5
      0x84300001,  //  004F  CLOSURE	R12	P1
      0x7C240600,  //  0050  CALL	R9	3
      0x7001FFBE,  //  0051  JMP		#0011
      0x581C0015,  //  0052  LDCONST	R7	K21
      0xAC1C0200,  //  0053  CATCH	R7	1	0
      0xB0080000,  //  0054  RAISE	2	R0	R0
      0xB81E1A00,  //  0055  GETNGBL	R7	K13
      0x8C1C0F0E,  //  0056  GETMET	R7	R7	K14
      0x60240008,  //  0057  GETGBL	R9	G8
      0x5C280C00,  //  0058  MOVE	R10	R6
      0x7C240200,  //  0059  CALL	R9	1
      0x00262C09,  //  005A  ADD	R9	K22	R9
      0x542A0003,  //  005B  LDINT	R10	4
      0x7C1C0600,  //  005C  CALL	R7	3
      0xB81E1A00,  //  005D  GETNGBL	R7	K13
      0x8C1C0F0E,  //  005E  GETMET	R7	R7	K14
      0x60240008,  //  005F  GETGBL	R9	G8
      0x8C280D18,  //  0060  GETMET	R10	R6	K24
      0x7C280200,  //  0061  CALL	R10	1
      0x7C240200,  //  0062  CALL	R9	1
      0x00262E09,  //  0063  ADD	R9	K23	R9
      0x58280019,  //  0064  LDCONST	R10	K25
      0x7C1C0600,  //  0065  CALL	R7	3
      0xA0000000,  //  0066  CLOSE	R0
      0x80040C00,  //  0067  RET	1	R6
    })
  )
);
/*******************************************************************/


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
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(TimedRequestMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20received_X20TimedRequestMessage_X3D),
    /* K7   */  be_const_int(3),
    /* K8   */  be_nested_str_weak(format),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_X20_X20TimedRequest_X3D_X25i_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K10  */  be_nested_str_weak(timeout),
    /* K11  */  be_nested_str_weak(remote_ip),
    /* K12  */  be_nested_str_weak(remote_port),
    /* K13  */  be_const_int(2),
    /* K14  */  be_nested_str_weak(send_status),
    /* K15  */  be_nested_str_weak(SUCCESS),
    }),
    be_str_weak(process_timed_request),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
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
      0x8C14010E,  //  0019  GETMET	R5	R0	K14
      0x5C1C0200,  //  001A  MOVE	R7	R1
      0xB8220200,  //  001B  GETNGBL	R8	K1
      0x8820110F,  //  001C  GETMBR	R8	R8	K15
      0x7C140600,  //  001D  CALL	R5	3
      0x50140200,  //  001E  LDBOOL	R5	1	0
      0x80040A00,  //  001F  RET	1	R5
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
    /* K7   */  be_const_int(2),
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
** Solidified function: send_write_response
********************************************************************/
be_local_closure(Matter_IM_send_write_response,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(send_queue),
    /* K1   */  be_nested_str_weak(push),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(IM_WriteResponse),
    }),
    be_str_weak(send_write_response),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0xB8160400,  //  0002  GETNGBL	R5	K2
      0x8C140B03,  //  0003  GETMET	R5	R5	K3
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C140600,  //  0006  CALL	R5	3
      0x7C0C0400,  //  0007  CALL	R3	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_status
********************************************************************/
be_local_closure(Matter_IM_send_status,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(send_queue),
    /* K1   */  be_nested_str_weak(push),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(IM_Status),
    }),
    be_str_weak(send_status),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0xB8160400,  //  0002  GETNGBL	R5	K2
      0x8C140B03,  //  0003  GETMET	R5	R5	K3
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C140600,  //  0006  CALL	R5	3
      0x7C0C0400,  //  0007  CALL	R3	2
      0x80000000,  //  0008  RET	0
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
    /* K7   */  be_const_int(2),
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
** Solidified function: process_read_request
********************************************************************/
be_local_closure(Matter_IM_process_read_request,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(ReadRequestMessage),
    /* K2   */  be_nested_str_weak(from_TLV),
    /* K3   */  be_nested_str_weak(attributes_requests),
    /* K4   */  be_nested_str_weak(_inner_process_read_request),
    /* K5   */  be_nested_str_weak(send_report_data),
    }),
    be_str_weak(process_read_request),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0xB80E0000,  //  0000  GETNGBL	R3	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0702,  //  0003  GETMET	R3	R3	K2
      0x5C140400,  //  0004  MOVE	R5	R2
      0x7C0C0400,  //  0005  CALL	R3	2
      0x88100703,  //  0006  GETMBR	R4	R3	K3
      0x4C140000,  //  0007  LDNIL	R5
      0x20100805,  //  0008  NE	R4	R4	R5
      0x78120007,  //  0009  JMPF	R4	#0012
      0x8C100104,  //  000A  GETMET	R4	R0	K4
      0x5C180200,  //  000B  MOVE	R6	R1
      0x5C1C0600,  //  000C  MOVE	R7	R3
      0x7C100600,  //  000D  CALL	R4	3
      0x8C140105,  //  000E  GETMET	R5	R0	K5
      0x5C1C0200,  //  000F  MOVE	R7	R1
      0x5C200800,  //  0010  MOVE	R8	R4
      0x7C140600,  //  0011  CALL	R5	3
      0x50100200,  //  0012  LDBOOL	R4	1	0
      0x80040800,  //  0013  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_report_data
********************************************************************/
be_local_closure(Matter_IM_send_report_data,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(send_queue),
    /* K1   */  be_nested_str_weak(push),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(IM_ReportData),
    }),
    be_str_weak(send_report_data),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0xB8160400,  //  0002  GETNGBL	R5	K2
      0x8C140B03,  //  0003  GETMET	R5	R5	K3
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C140600,  //  0006  CALL	R5	3
      0x7C0C0400,  //  0007  CALL	R3	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_IM_init,   /* name */
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
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(send_queue),
    /* K2   */  be_nested_str_weak(subs),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(IM_Subscription_Shop),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x90020202,  //  0003  SETMBR	R0	K1	R2
      0xB80A0600,  //  0004  GETNGBL	R2	K3
      0x8C080504,  //  0005  GETMET	R2	R2	K4
      0x5C100000,  //  0006  MOVE	R4	R0
      0x7C080400,  //  0007  CALL	R2	2
      0x90020402,  //  0008  SETMBR	R0	K2	R2
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: subscribe_request
********************************************************************/
be_local_closure(Matter_IM_subscribe_request,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(SubscribeRequestMessage),
    /* K3   */  be_nested_str_weak(from_TLV),
    /* K4   */  be_nested_str_weak(keep_subscriptions),
    /* K5   */  be_nested_str_weak(subs),
    /* K6   */  be_nested_str_weak(remove_by_session),
    /* K7   */  be_nested_str_weak(session),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20received_X20SubscribeRequestMessage_X3D),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str_weak(new_subscription),
    /* K13  */  be_nested_str_weak(_inner_process_read_request),
    /* K14  */  be_nested_str_weak(subscription_id),
    /* K15  */  be_nested_str_weak(send_subscribe_response),
    }),
    be_str_weak(subscribe_request),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100903,  //  0004  GETMET	R4	R4	K3
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C100400,  //  0006  CALL	R4	2
      0x88140904,  //  0007  GETMBR	R5	R4	K4
      0x74160003,  //  0008  JMPT	R5	#000D
      0x88140105,  //  0009  GETMBR	R5	R0	K5
      0x8C140B06,  //  000A  GETMET	R5	R5	K6
      0x881C0307,  //  000B  GETMBR	R7	R1	K7
      0x7C140400,  //  000C  CALL	R5	2
      0xB8161000,  //  000D  GETNGBL	R5	K8
      0x8C140B09,  //  000E  GETMET	R5	R5	K9
      0x601C0008,  //  000F  GETGBL	R7	G8
      0x5C200800,  //  0010  MOVE	R8	R4
      0x7C1C0200,  //  0011  CALL	R7	1
      0x001E1407,  //  0012  ADD	R7	K10	R7
      0x5820000B,  //  0013  LDCONST	R8	K11
      0x7C140600,  //  0014  CALL	R5	3
      0x88140105,  //  0015  GETMBR	R5	R0	K5
      0x8C140B0C,  //  0016  GETMET	R5	R5	K12
      0x881C0307,  //  0017  GETMBR	R7	R1	K7
      0x5C200800,  //  0018  MOVE	R8	R4
      0x7C140600,  //  0019  CALL	R5	3
      0x8C18010D,  //  001A  GETMET	R6	R0	K13
      0x5C200200,  //  001B  MOVE	R8	R1
      0x5C240800,  //  001C  MOVE	R9	R4
      0x7C180600,  //  001D  CALL	R6	3
      0x881C0B0E,  //  001E  GETMBR	R7	R5	K14
      0x901A1C07,  //  001F  SETMBR	R6	K14	R7
      0x8C1C010F,  //  0020  GETMET	R7	R0	K15
      0x5C240200,  //  0021  MOVE	R9	R1
      0x5C280C00,  //  0022  MOVE	R10	R6
      0x5C2C0A00,  //  0023  MOVE	R11	R5
      0x7C1C0800,  //  0024  CALL	R7	4
      0x501C0200,  //  0025  LDBOOL	R7	1	0
      0x80040E00,  //  0026  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_enqueued
********************************************************************/
be_local_closure(Matter_IM_send_enqueued,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(send_queue),
    /* K2   */  be_nested_str_weak(ready),
    /* K3   */  be_nested_str_weak(send),
    /* K4   */  be_nested_str_weak(remove),
    /* K5   */  be_const_int(1),
    }),
    be_str_weak(send_enqueued),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E0011,  //  0005  JMPF	R3	#0018
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x88100702,  //  0008  GETMBR	R4	R3	K2
      0x7812000B,  //  0009  JMPF	R4	#0016
      0x8C100703,  //  000A  GETMET	R4	R3	K3
      0x5C180200,  //  000B  MOVE	R6	R1
      0x7C100400,  //  000C  CALL	R4	2
      0x78120005,  //  000D  JMPF	R4	#0014
      0x88140101,  //  000E  GETMBR	R5	R0	K1
      0x8C140B04,  //  000F  GETMET	R5	R5	K4
      0x5C1C0400,  //  0010  MOVE	R7	R2
      0x7C140400,  //  0011  CALL	R5	2
      0x04080505,  //  0012  SUB	R2	R2	K5
      0x70020001,  //  0013  JMP		#0016
      0x50140000,  //  0014  LDBOOL	R5	0	0
      0x900E0405,  //  0015  SETMBR	R3	K2	R5
      0x00080505,  //  0016  ADD	R2	R2	K5
      0x7001FFE8,  //  0017  JMP		#0001
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_invoke_request
********************************************************************/
be_local_closure(Matter_IM_process_invoke_request,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[44]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20IM_X3Ainvoke_request_X20processing_X20start),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(Path),
    /* K6   */  be_nested_str_weak(InvokeRequestMessage),
    /* K7   */  be_nested_str_weak(from_TLV),
    /* K8   */  be_nested_str_weak(invoke_requests),
    /* K9   */  be_nested_str_weak(InvokeResponseMessage),
    /* K10  */  be_nested_str_weak(suppress_response),
    /* K11  */  be_nested_str_weak(invoke_responses),
    /* K12  */  be_nested_str_weak(endpoint),
    /* K13  */  be_nested_str_weak(command_path),
    /* K14  */  be_nested_str_weak(cluster),
    /* K15  */  be_nested_str_weak(command),
    /* K16  */  be_nested_str_weak(status),
    /* K17  */  be_nested_str_weak(UNSUPPORTED_COMMAND),
    /* K18  */  be_nested_str_weak(get_command_name),
    /* K19  */  be_nested_str_weak(format),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20_X3EReceived_X20_X20_X25s_X20_X25s_X20from_X20_X5B_X25s_X5D_X3A_X25i),
    /* K21  */  be_nested_str_weak(),
    /* K22  */  be_nested_str_weak(remote_ip),
    /* K23  */  be_nested_str_weak(remote_port),
    /* K24  */  be_const_int(2),
    /* K25  */  be_nested_str_weak(device),
    /* K26  */  be_nested_str_weak(invoke_request),
    /* K27  */  be_nested_str_weak(command_fields),
    /* K28  */  be_nested_str_weak(InvokeResponseIB),
    /* K29  */  be_nested_str_weak(CommandStatusIB),
    /* K30  */  be_nested_str_weak(CommandPathIB),
    /* K31  */  be_nested_str_weak(StatusIB),
    /* K32  */  be_nested_str_weak(SUCCESS),
    /* K33  */  be_nested_str_weak(push),
    /* K34  */  be_nested_str_weak(CommandDataIB),
    /* K35  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X25s_X20_X25s),
    /* K36  */  be_nested_str_weak(stop_iteration),
    /* K37  */  be_nested_str_weak(MTR_X3A_X20invoke_responses_X3D),
    /* K38  */  be_const_int(0),
    /* K39  */  be_nested_str_weak(MTR_X3A_X20InvokeResponse_X3D),
    /* K40  */  be_nested_str_weak(MTR_X3A_X20InvokeResponseTLV_X3D),
    /* K41  */  be_nested_str_weak(to_TLV),
    /* K42  */  be_const_int(3),
    /* K43  */  be_nested_str_weak(send_invoke_response),
    }),
    be_str_weak(process_invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[248]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xB8120200,  //  0001  GETNGBL	R4	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x58180003,  //  0003  LDCONST	R6	K3
      0x541E0003,  //  0004  LDINT	R7	4
      0x7C100600,  //  0005  CALL	R4	3
      0xB8120800,  //  0006  GETNGBL	R4	K4
      0x8C100905,  //  0007  GETMET	R4	R4	K5
      0x7C100200,  //  0008  CALL	R4	1
      0xB8160800,  //  0009  GETNGBL	R5	K4
      0x8C140B06,  //  000A  GETMET	R5	R5	K6
      0x7C140200,  //  000B  CALL	R5	1
      0x8C140B07,  //  000C  GETMET	R5	R5	K7
      0x5C1C0400,  //  000D  MOVE	R7	R2
      0x7C140400,  //  000E  CALL	R5	2
      0x88180B08,  //  000F  GETMBR	R6	R5	K8
      0x4C1C0000,  //  0010  LDNIL	R7
      0x20180C07,  //  0011  NE	R6	R6	R7
      0x781A00E3,  //  0012  JMPF	R6	#00F7
      0xB81A0800,  //  0013  GETNGBL	R6	K4
      0x8C180D09,  //  0014  GETMET	R6	R6	K9
      0x7C180200,  //  0015  CALL	R6	1
      0x501C0000,  //  0016  LDBOOL	R7	0	0
      0x901A1407,  //  0017  SETMBR	R6	K10	R7
      0x601C0012,  //  0018  GETGBL	R7	G18
      0x7C1C0000,  //  0019  CALL	R7	0
      0x901A1607,  //  001A  SETMBR	R6	K11	R7
      0x601C0010,  //  001B  GETGBL	R7	G16
      0x88200B08,  //  001C  GETMBR	R8	R5	K8
      0x7C1C0200,  //  001D  CALL	R7	1
      0xA80200AD,  //  001E  EXBLK	0	#00CD
      0x5C200E00,  //  001F  MOVE	R8	R7
      0x7C200000,  //  0020  CALL	R8	0
      0x8824110D,  //  0021  GETMBR	R9	R8	K13
      0x8824130C,  //  0022  GETMBR	R9	R9	K12
      0x90121809,  //  0023  SETMBR	R4	K12	R9
      0x8824110D,  //  0024  GETMBR	R9	R8	K13
      0x8824130E,  //  0025  GETMBR	R9	R9	K14
      0x90121C09,  //  0026  SETMBR	R4	K14	R9
      0x8824110D,  //  0027  GETMBR	R9	R8	K13
      0x8824130F,  //  0028  GETMBR	R9	R9	K15
      0x90121E09,  //  0029  SETMBR	R4	K15	R9
      0xB8260800,  //  002A  GETNGBL	R9	K4
      0x88241311,  //  002B  GETMBR	R9	R9	K17
      0x90122009,  //  002C  SETMBR	R4	K16	R9
      0xB8260800,  //  002D  GETNGBL	R9	K4
      0x8C241312,  //  002E  GETMET	R9	R9	K18
      0x882C090E,  //  002F  GETMBR	R11	R4	K14
      0x8830090F,  //  0030  GETMBR	R12	R4	K15
      0x7C240600,  //  0031  CALL	R9	3
      0xB82A0200,  //  0032  GETNGBL	R10	K1
      0x8C281502,  //  0033  GETMET	R10	R10	K2
      0x8C300713,  //  0034  GETMET	R12	R3	K19
      0x58380014,  //  0035  LDCONST	R14	K20
      0x603C0008,  //  0036  GETGBL	R15	G8
      0x5C400800,  //  0037  MOVE	R16	R4
      0x7C3C0200,  //  0038  CALL	R15	1
      0x78260001,  //  0039  JMPF	R9	#003C
      0x5C401200,  //  003A  MOVE	R16	R9
      0x70020000,  //  003B  JMP		#003D
      0x58400015,  //  003C  LDCONST	R16	K21
      0x88440316,  //  003D  GETMBR	R17	R1	K22
      0x88480317,  //  003E  GETMBR	R18	R1	K23
      0x7C300C00,  //  003F  CALL	R12	6
      0x58340018,  //  0040  LDCONST	R13	K24
      0x7C280600,  //  0041  CALL	R10	3
      0x88280119,  //  0042  GETMBR	R10	R0	K25
      0x8C28151A,  //  0043  GETMET	R10	R10	K26
      0x5C300200,  //  0044  MOVE	R12	R1
      0x8834111B,  //  0045  GETMBR	R13	R8	K27
      0x5C380800,  //  0046  MOVE	R14	R4
      0x7C280800,  //  0047  CALL	R10	4
      0xB82E0800,  //  0048  GETNGBL	R11	K4
      0x8C2C171C,  //  0049  GETMET	R11	R11	K28
      0x7C2C0200,  //  004A  CALL	R11	1
      0x50300200,  //  004B  LDBOOL	R12	1	0
      0x1C30140C,  //  004C  EQ	R12	R10	R12
      0x78320023,  //  004D  JMPF	R12	#0072
      0xB8320800,  //  004E  GETNGBL	R12	K4
      0x8C30191D,  //  004F  GETMET	R12	R12	K29
      0x7C300200,  //  0050  CALL	R12	1
      0x902E200C,  //  0051  SETMBR	R11	K16	R12
      0x88301710,  //  0052  GETMBR	R12	R11	K16
      0xB8360800,  //  0053  GETNGBL	R13	K4
      0x8C341B1E,  //  0054  GETMET	R13	R13	K30
      0x7C340200,  //  0055  CALL	R13	1
      0x90321A0D,  //  0056  SETMBR	R12	K13	R13
      0x88301710,  //  0057  GETMBR	R12	R11	K16
      0x8830190D,  //  0058  GETMBR	R12	R12	K13
      0x8834090C,  //  0059  GETMBR	R13	R4	K12
      0x9032180D,  //  005A  SETMBR	R12	K12	R13
      0x88301710,  //  005B  GETMBR	R12	R11	K16
      0x8830190D,  //  005C  GETMBR	R12	R12	K13
      0x8834090E,  //  005D  GETMBR	R13	R4	K14
      0x90321C0D,  //  005E  SETMBR	R12	K14	R13
      0x88301710,  //  005F  GETMBR	R12	R11	K16
      0x8830190D,  //  0060  GETMBR	R12	R12	K13
      0x8834090F,  //  0061  GETMBR	R13	R4	K15
      0x90321E0D,  //  0062  SETMBR	R12	K15	R13
      0x88301710,  //  0063  GETMBR	R12	R11	K16
      0xB8360800,  //  0064  GETNGBL	R13	K4
      0x8C341B1F,  //  0065  GETMET	R13	R13	K31
      0x7C340200,  //  0066  CALL	R13	1
      0x9032200D,  //  0067  SETMBR	R12	K16	R13
      0x88301710,  //  0068  GETMBR	R12	R11	K16
      0x88301910,  //  0069  GETMBR	R12	R12	K16
      0xB8360800,  //  006A  GETNGBL	R13	K4
      0x88341B20,  //  006B  GETMBR	R13	R13	K32
      0x9032200D,  //  006C  SETMBR	R12	K16	R13
      0x88300D0B,  //  006D  GETMBR	R12	R6	K11
      0x8C301921,  //  006E  GETMET	R12	R12	K33
      0x5C381600,  //  006F  MOVE	R14	R11
      0x7C300400,  //  0070  CALL	R12	2
      0x70020059,  //  0071  JMP		#00CC
      0x4C300000,  //  0072  LDNIL	R12
      0x2030140C,  //  0073  NE	R12	R10	R12
      0x7832002F,  //  0074  JMPF	R12	#00A5
      0xB8320800,  //  0075  GETNGBL	R12	K4
      0x8C301922,  //  0076  GETMET	R12	R12	K34
      0x7C300200,  //  0077  CALL	R12	1
      0x902E1E0C,  //  0078  SETMBR	R11	K15	R12
      0x8830170F,  //  0079  GETMBR	R12	R11	K15
      0xB8360800,  //  007A  GETNGBL	R13	K4
      0x8C341B1E,  //  007B  GETMET	R13	R13	K30
      0x7C340200,  //  007C  CALL	R13	1
      0x90321A0D,  //  007D  SETMBR	R12	K13	R13
      0x8830170F,  //  007E  GETMBR	R12	R11	K15
      0x8830190D,  //  007F  GETMBR	R12	R12	K13
      0x8834090C,  //  0080  GETMBR	R13	R4	K12
      0x9032180D,  //  0081  SETMBR	R12	K12	R13
      0x8830170F,  //  0082  GETMBR	R12	R11	K15
      0x8830190D,  //  0083  GETMBR	R12	R12	K13
      0x8834090E,  //  0084  GETMBR	R13	R4	K14
      0x90321C0D,  //  0085  SETMBR	R12	K14	R13
      0x8830170F,  //  0086  GETMBR	R12	R11	K15
      0x8830190D,  //  0087  GETMBR	R12	R12	K13
      0x8834090F,  //  0088  GETMBR	R13	R4	K15
      0x90321E0D,  //  0089  SETMBR	R12	K15	R13
      0x8830170F,  //  008A  GETMBR	R12	R11	K15
      0x9032360A,  //  008B  SETMBR	R12	K27	R10
      0x88300D0B,  //  008C  GETMBR	R12	R6	K11
      0x8C301921,  //  008D  GETMET	R12	R12	K33
      0x5C381600,  //  008E  MOVE	R14	R11
      0x7C300400,  //  008F  CALL	R12	2
      0xB8320800,  //  0090  GETNGBL	R12	K4
      0x8C301912,  //  0091  GETMET	R12	R12	K18
      0x8838090E,  //  0092  GETMBR	R14	R4	K14
      0x883C090F,  //  0093  GETMBR	R15	R4	K15
      0x7C300600,  //  0094  CALL	R12	3
      0x5C241800,  //  0095  MOVE	R9	R12
      0xB8320200,  //  0096  GETNGBL	R12	K1
      0x8C301902,  //  0097  GETMET	R12	R12	K2
      0x8C380713,  //  0098  GETMET	R14	R3	K19
      0x58400023,  //  0099  LDCONST	R16	K35
      0x60440008,  //  009A  GETGBL	R17	G8
      0x5C480800,  //  009B  MOVE	R18	R4
      0x7C440200,  //  009C  CALL	R17	1
      0x78260001,  //  009D  JMPF	R9	#00A0
      0x5C481200,  //  009E  MOVE	R18	R9
      0x70020000,  //  009F  JMP		#00A1
      0x58480015,  //  00A0  LDCONST	R18	K21
      0x7C380800,  //  00A1  CALL	R14	4
      0x583C0018,  //  00A2  LDCONST	R15	K24
      0x7C300600,  //  00A3  CALL	R12	3
      0x70020026,  //  00A4  JMP		#00CC
      0x88300910,  //  00A5  GETMBR	R12	R4	K16
      0x4C340000,  //  00A6  LDNIL	R13
      0x2030180D,  //  00A7  NE	R12	R12	R13
      0x78320022,  //  00A8  JMPF	R12	#00CC
      0xB8320800,  //  00A9  GETNGBL	R12	K4
      0x8C30191D,  //  00AA  GETMET	R12	R12	K29
      0x7C300200,  //  00AB  CALL	R12	1
      0x902E200C,  //  00AC  SETMBR	R11	K16	R12
      0x88301710,  //  00AD  GETMBR	R12	R11	K16
      0xB8360800,  //  00AE  GETNGBL	R13	K4
      0x8C341B1E,  //  00AF  GETMET	R13	R13	K30
      0x7C340200,  //  00B0  CALL	R13	1
      0x90321A0D,  //  00B1  SETMBR	R12	K13	R13
      0x88301710,  //  00B2  GETMBR	R12	R11	K16
      0x8830190D,  //  00B3  GETMBR	R12	R12	K13
      0x8834090C,  //  00B4  GETMBR	R13	R4	K12
      0x9032180D,  //  00B5  SETMBR	R12	K12	R13
      0x88301710,  //  00B6  GETMBR	R12	R11	K16
      0x8830190D,  //  00B7  GETMBR	R12	R12	K13
      0x8834090E,  //  00B8  GETMBR	R13	R4	K14
      0x90321C0D,  //  00B9  SETMBR	R12	K14	R13
      0x88301710,  //  00BA  GETMBR	R12	R11	K16
      0x8830190D,  //  00BB  GETMBR	R12	R12	K13
      0x8834090F,  //  00BC  GETMBR	R13	R4	K15
      0x90321E0D,  //  00BD  SETMBR	R12	K15	R13
      0x88301710,  //  00BE  GETMBR	R12	R11	K16
      0xB8360800,  //  00BF  GETNGBL	R13	K4
      0x8C341B1F,  //  00C0  GETMET	R13	R13	K31
      0x7C340200,  //  00C1  CALL	R13	1
      0x9032200D,  //  00C2  SETMBR	R12	K16	R13
      0x88301710,  //  00C3  GETMBR	R12	R11	K16
      0x88301910,  //  00C4  GETMBR	R12	R12	K16
      0x88340910,  //  00C5  GETMBR	R13	R4	K16
      0x9032200D,  //  00C6  SETMBR	R12	K16	R13
      0x88300D0B,  //  00C7  GETMBR	R12	R6	K11
      0x8C301921,  //  00C8  GETMET	R12	R12	K33
      0x5C381600,  //  00C9  MOVE	R14	R11
      0x7C300400,  //  00CA  CALL	R12	2
      0x7001FFFF,  //  00CB  JMP		#00CC
      0x7001FF51,  //  00CC  JMP		#001F
      0x581C0024,  //  00CD  LDCONST	R7	K36
      0xAC1C0200,  //  00CE  CATCH	R7	1	0
      0xB0080000,  //  00CF  RAISE	2	R0	R0
      0xB81E0200,  //  00D0  GETNGBL	R7	K1
      0x8C1C0F02,  //  00D1  GETMET	R7	R7	K2
      0x60240008,  //  00D2  GETGBL	R9	G8
      0x88280D0B,  //  00D3  GETMBR	R10	R6	K11
      0x7C240200,  //  00D4  CALL	R9	1
      0x00264A09,  //  00D5  ADD	R9	K37	R9
      0x542A0003,  //  00D6  LDINT	R10	4
      0x7C1C0600,  //  00D7  CALL	R7	3
      0x601C000C,  //  00D8  GETGBL	R7	G12
      0x88200D0B,  //  00D9  GETMBR	R8	R6	K11
      0x7C1C0200,  //  00DA  CALL	R7	1
      0x241C0F26,  //  00DB  GT	R7	R7	K38
      0x781E0015,  //  00DC  JMPF	R7	#00F3
      0xB81E0200,  //  00DD  GETNGBL	R7	K1
      0x8C1C0F02,  //  00DE  GETMET	R7	R7	K2
      0x60240008,  //  00DF  GETGBL	R9	G8
      0x5C280C00,  //  00E0  MOVE	R10	R6
      0x7C240200,  //  00E1  CALL	R9	1
      0x00264E09,  //  00E2  ADD	R9	K39	R9
      0x542A0003,  //  00E3  LDINT	R10	4
      0x7C1C0600,  //  00E4  CALL	R7	3
      0xB81E0200,  //  00E5  GETNGBL	R7	K1
      0x8C1C0F02,  //  00E6  GETMET	R7	R7	K2
      0x60240008,  //  00E7  GETGBL	R9	G8
      0x8C280D29,  //  00E8  GETMET	R10	R6	K41
      0x7C280200,  //  00E9  CALL	R10	1
      0x7C240200,  //  00EA  CALL	R9	1
      0x00265009,  //  00EB  ADD	R9	K40	R9
      0x5828002A,  //  00EC  LDCONST	R10	K42
      0x7C1C0600,  //  00ED  CALL	R7	3
      0x8C1C012B,  //  00EE  GETMET	R7	R0	K43
      0x5C240200,  //  00EF  MOVE	R9	R1
      0x5C280C00,  //  00F0  MOVE	R10	R6
      0x7C1C0600,  //  00F1  CALL	R7	3
      0x70020001,  //  00F2  JMP		#00F5
      0x501C0000,  //  00F3  LDBOOL	R7	0	0
      0x80040E00,  //  00F4  RET	1	R7
      0x501C0200,  //  00F5  LDBOOL	R7	1	0
      0x80040E00,  //  00F6  RET	1	R7
      0x80000000,  //  00F7  RET	0
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
    /* K7   */  be_const_int(2),
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
** Solidified function: find_sendqueue_by_exchangeid
********************************************************************/
be_local_closure(Matter_IM_find_sendqueue_by_exchangeid,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(send_queue),
    /* K2   */  be_nested_str_weak(get_exchangeid),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(find_sendqueue_by_exchangeid),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0001,  //  0002  JMPF	R2	#0005
      0x4C080000,  //  0003  LDNIL	R2
      0x80040400,  //  0004  RET	1	R2
      0x58080000,  //  0005  LDCONST	R2	K0
      0x600C000C,  //  0006  GETGBL	R3	G12
      0x88100101,  //  0007  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0008  CALL	R3	1
      0x140C0403,  //  0009  LT	R3	R2	R3
      0x780E0008,  //  000A  JMPF	R3	#0014
      0x880C0101,  //  000B  GETMBR	R3	R0	K1
      0x940C0602,  //  000C  GETIDX	R3	R3	R2
      0x8C100702,  //  000D  GETMET	R4	R3	K2
      0x7C100200,  //  000E  CALL	R4	1
      0x1C100801,  //  000F  EQ	R4	R4	R1
      0x78120000,  //  0010  JMPF	R4	#0012
      0x80040600,  //  0011  RET	1	R3
      0x00080503,  //  0012  ADD	R2	R2	K3
      0x7001FFF1,  //  0013  JMP		#0006
      0x4C0C0000,  //  0014  LDNIL	R3
      0x80040600,  //  0015  RET	1	R3
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
    /* K7   */  be_const_int(2),
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
** Solidified function: send_invoke_response
********************************************************************/
be_local_closure(Matter_IM_send_invoke_response,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(send_queue),
    /* K1   */  be_nested_str_weak(push),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(IM_InvokeResponse),
    }),
    be_str_weak(send_invoke_response),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0xB8160400,  //  0002  GETNGBL	R5	K2
      0x8C140B03,  //  0003  GETMET	R5	R5	K3
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x5C200400,  //  0005  MOVE	R8	R2
      0x7C140600,  //  0006  CALL	R5	3
      0x7C0C0400,  //  0007  CALL	R3	2
      0x80000000,  //  0008  RET	0
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
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(findsubval),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(SUCCESS),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20_X3EStatus),
    /* K8   */  be_const_int(2),
    /* K9   */  be_nested_str_weak(find_sendqueue_by_exchangeid),
    /* K10  */  be_nested_str_weak(exchange_id),
    /* K11  */  be_nested_str_weak(ack_received),
    /* K12  */  be_nested_str_weak(format),
    /* K13  */  be_nested_str_weak(MTR_X3A_X20_X3EStatus_X20_X20_X20_X20ERROR_X20_X3D_X200x_X2502X),
    }),
    be_str_weak(process_status_response),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x8C100501,  //  0001  GETMET	R4	R2	K1
      0x58180002,  //  0002  LDCONST	R6	K2
      0x541E00FE,  //  0003  LDINT	R7	255
      0x7C100600,  //  0004  CALL	R4	3
      0xB8160600,  //  0005  GETNGBL	R5	K3
      0x88140B04,  //  0006  GETMBR	R5	R5	K4
      0x1C140805,  //  0007  EQ	R5	R4	R5
      0x7816000E,  //  0008  JMPF	R5	#0018
      0xB8160A00,  //  0009  GETNGBL	R5	K5
      0x8C140B06,  //  000A  GETMET	R5	R5	K6
      0x581C0007,  //  000B  LDCONST	R7	K7
      0x58200008,  //  000C  LDCONST	R8	K8
      0x7C140600,  //  000D  CALL	R5	3
      0x8C140109,  //  000E  GETMET	R5	R0	K9
      0x881C030A,  //  000F  GETMBR	R7	R1	K10
      0x7C140400,  //  0010  CALL	R5	2
      0x78160004,  //  0011  JMPF	R5	#0017
      0x8C180B0B,  //  0012  GETMET	R6	R5	K11
      0x5C200200,  //  0013  MOVE	R8	R1
      0x7C180400,  //  0014  CALL	R6	2
      0x50180200,  //  0015  LDBOOL	R6	1	0
      0x80040C00,  //  0016  RET	1	R6
      0x70020007,  //  0017  JMP		#0020
      0xB8160A00,  //  0018  GETNGBL	R5	K5
      0x8C140B06,  //  0019  GETMET	R5	R5	K6
      0x8C1C070C,  //  001A  GETMET	R7	R3	K12
      0x5824000D,  //  001B  LDCONST	R9	K13
      0x5C280800,  //  001C  MOVE	R10	R4
      0x7C1C0600,  //  001D  CALL	R7	3
      0x58200008,  //  001E  LDCONST	R8	K8
      0x7C140600,  //  001F  CALL	R5	3
      0x50140000,  //  0020  LDBOOL	R5	0	0
      0x80040A00,  //  0021  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: send_subscribe_update
********************************************************************/
be_local_closure(Matter_IM_send_subscribe_update,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(ReportDataMessage),
    /* K4   */  be_nested_str_weak(subscription_id),
    /* K5   */  be_nested_str_weak(attribute_reports),
    /* K6   */  be_nested_str_weak(suppress_response),
    /* K7   */  be_nested_str_weak(send_queue),
    /* K8   */  be_nested_str_weak(push),
    /* K9   */  be_nested_str_weak(IM_ReportDataSubscribed),
    /* K10  */  be_nested_str_weak(device),
    /* K11  */  be_nested_str_weak(message_handler),
    /* K12  */  be_nested_str_weak(send_enqueued),
    }),
    be_str_weak(send_subscribe_update),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x880C0301,  //  0001  GETMBR	R3	R1	K1
      0xB8120400,  //  0002  GETNGBL	R4	K2
      0x8C100903,  //  0003  GETMET	R4	R4	K3
      0x7C100200,  //  0004  CALL	R4	1
      0x88140304,  //  0005  GETMBR	R5	R1	K4
      0x90120805,  //  0006  SETMBR	R4	K4	R5
      0x60140012,  //  0007  GETGBL	R5	G18
      0x7C140000,  //  0008  CALL	R5	0
      0x90120A05,  //  0009  SETMBR	R4	K5	R5
      0x50140200,  //  000A  LDBOOL	R5	1	0
      0x90120C05,  //  000B  SETMBR	R4	K6	R5
      0x88140107,  //  000C  GETMBR	R5	R0	K7
      0x8C140B08,  //  000D  GETMET	R5	R5	K8
      0xB81E0400,  //  000E  GETNGBL	R7	K2
      0x8C1C0F09,  //  000F  GETMET	R7	R7	K9
      0x8824010A,  //  0010  GETMBR	R9	R0	K10
      0x8824130B,  //  0011  GETMBR	R9	R9	K11
      0x5C280600,  //  0012  MOVE	R10	R3
      0x5C2C0800,  //  0013  MOVE	R11	R4
      0x7C1C0800,  //  0014  CALL	R7	4
      0x7C140400,  //  0015  CALL	R5	2
      0x8C14010C,  //  0016  GETMET	R5	R0	K12
      0x881C010A,  //  0017  GETMBR	R7	R0	K10
      0x881C0F0B,  //  0018  GETMBR	R7	R7	K11
      0x7C140400,  //  0019  CALL	R5	2
      0x80000000,  //  001A  RET	0
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
      0x541E0003,  //  0025  LDINT	R7	4
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
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_IM_every_second,   /* name */
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
    /* K0   */  be_nested_str_weak(subs),
    /* K1   */  be_nested_str_weak(every_second),
    }),
    be_str_weak(every_second),
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
** Solidified class: Matter_IM
********************************************************************/
be_local_class(Matter_IM,
    3,
    NULL,
    be_nested_map(25,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(send_subscribe_response, -1), be_const_closure(Matter_IM_send_subscribe_response_closure) },
        { be_const_key_weak(process_write_request, -1), be_const_closure(Matter_IM_process_write_request_closure) },
        { be_const_key_weak(_inner_process_read_request, -1), be_const_closure(Matter_IM__inner_process_read_request_closure) },
        { be_const_key_weak(process_timed_request, 10), be_const_closure(Matter_IM_process_timed_request_closure) },
        { be_const_key_weak(process_write_response, -1), be_const_closure(Matter_IM_process_write_response_closure) },
        { be_const_key_weak(send_write_response, 12), be_const_closure(Matter_IM_send_write_response_closure) },
        { be_const_key_weak(send_status, 9), be_const_closure(Matter_IM_send_status_closure) },
        { be_const_key_weak(subscribe_response, -1), be_const_closure(Matter_IM_subscribe_response_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_IM_every_second_closure) },
        { be_const_key_weak(send_report_data, -1), be_const_closure(Matter_IM_send_report_data_closure) },
        { be_const_key_weak(send_subscribe_update, 23), be_const_closure(Matter_IM_send_subscribe_update_closure) },
        { be_const_key_weak(device, 21), be_const_var(0) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_IM_init_closure) },
        { be_const_key_weak(subscribe_request, -1), be_const_closure(Matter_IM_subscribe_request_closure) },
        { be_const_key_weak(send_enqueued, -1), be_const_closure(Matter_IM_send_enqueued_closure) },
        { be_const_key_weak(process_invoke_request, 8), be_const_closure(Matter_IM_process_invoke_request_closure) },
        { be_const_key_weak(process_invoke_response, -1), be_const_closure(Matter_IM_process_invoke_response_closure) },
        { be_const_key_weak(send_invoke_response, -1), be_const_closure(Matter_IM_send_invoke_response_closure) },
        { be_const_key_weak(report_data, -1), be_const_closure(Matter_IM_report_data_closure) },
        { be_const_key_weak(find_sendqueue_by_exchangeid, 17), be_const_closure(Matter_IM_find_sendqueue_by_exchangeid_closure) },
        { be_const_key_weak(process_status_response, -1), be_const_closure(Matter_IM_process_status_response_closure) },
        { be_const_key_weak(subs, -1), be_const_var(1) },
        { be_const_key_weak(process_incoming, -1), be_const_closure(Matter_IM_process_incoming_closure) },
        { be_const_key_weak(process_read_request, 7), be_const_closure(Matter_IM_process_read_request_closure) },
        { be_const_key_weak(send_queue, -1), be_const_var(2) },
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
