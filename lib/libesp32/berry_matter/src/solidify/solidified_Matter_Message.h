/* Solidification of Matter_Message.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Frame;

/********************************************************************
** Solidified function: encode_frame
********************************************************************/
be_local_closure(Matter_Frame_encode_frame,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[29]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(flags),
    /* K2   */  be_nested_str_weak(flag_s),
    /* K3   */  be_nested_str_weak(flag_dsiz),
    /* K4   */  be_const_int(3),
    /* K5   */  be_nested_str_weak(add),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(local_session_id),
    /* K8   */  be_const_int(2),
    /* K9   */  be_nested_str_weak(sec_flags),
    /* K10  */  be_nested_str_weak(sec_p),
    /* K11  */  be_nested_str_weak(sec_c),
    /* K12  */  be_nested_str_weak(sec_sesstype),
    /* K13  */  be_nested_str_weak(message_counter),
    /* K14  */  be_nested_str_weak(source_node_id),
    /* K15  */  be_nested_str_weak(dest_node_id_8),
    /* K16  */  be_nested_str_weak(dest_node_id_2),
    /* K17  */  be_nested_str_weak(payload_idx),
    /* K18  */  be_nested_str_weak(x_flags),
    /* K19  */  be_nested_str_weak(x_flag_v),
    /* K20  */  be_nested_str_weak(x_flag_r),
    /* K21  */  be_nested_str_weak(x_flag_a),
    /* K22  */  be_nested_str_weak(x_flag_i),
    /* K23  */  be_nested_str_weak(opcode),
    /* K24  */  be_nested_str_weak(exchange_id),
    /* K25  */  be_nested_str_weak(protocol_id),
    /* K26  */  be_nested_str_weak(ack_message_counter),
    /* K27  */  be_nested_str_weak(app_payload_idx),
    /* K28  */  be_nested_str_weak(raw),
    }),
    be_str_weak(encode_frame),
    &be_const_str_solidified,
    ( &(const binstruction[155]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E000A,  //  0002  JMPF	R3	#000E
      0x600C0015,  //  0003  GETGBL	R3	G21
      0x5412000F,  //  0004  LDINT	R4	16
      0x78060003,  //  0005  JMPF	R1	#000A
      0x6014000C,  //  0006  GETGBL	R5	G12
      0x5C180200,  //  0007  MOVE	R6	R1
      0x7C140200,  //  0008  CALL	R5	1
      0x70020000,  //  0009  JMP		#000B
      0x58140000,  //  000A  LDCONST	R5	K0
      0x00100805,  //  000B  ADD	R4	R4	R5
      0x7C0C0200,  //  000C  CALL	R3	1
      0x5C080600,  //  000D  MOVE	R2	R3
      0x880C0101,  //  000E  GETMBR	R3	R0	K1
      0x4C100000,  //  000F  LDNIL	R4
      0x1C0C0604,  //  0010  EQ	R3	R3	R4
      0x780E000D,  //  0011  JMPF	R3	#0020
      0x90020300,  //  0012  SETMBR	R0	K1	K0
      0x880C0102,  //  0013  GETMBR	R3	R0	K2
      0x780E0003,  //  0014  JMPF	R3	#0019
      0x880C0101,  //  0015  GETMBR	R3	R0	K1
      0x54120003,  //  0016  LDINT	R4	4
      0x300C0604,  //  0017  OR	R3	R3	R4
      0x90020203,  //  0018  SETMBR	R0	K1	R3
      0x880C0103,  //  0019  GETMBR	R3	R0	K3
      0x780E0004,  //  001A  JMPF	R3	#0020
      0x880C0101,  //  001B  GETMBR	R3	R0	K1
      0x88100103,  //  001C  GETMBR	R4	R0	K3
      0x2C100904,  //  001D  AND	R4	R4	K4
      0x300C0604,  //  001E  OR	R3	R3	R4
      0x90020203,  //  001F  SETMBR	R0	K1	R3
      0x8C0C0505,  //  0020  GETMET	R3	R2	K5
      0x88140101,  //  0021  GETMBR	R5	R0	K1
      0x58180006,  //  0022  LDCONST	R6	K6
      0x7C0C0600,  //  0023  CALL	R3	3
      0x8C0C0505,  //  0024  GETMET	R3	R2	K5
      0x88140107,  //  0025  GETMBR	R5	R0	K7
      0x78160001,  //  0026  JMPF	R5	#0029
      0x88140107,  //  0027  GETMBR	R5	R0	K7
      0x70020000,  //  0028  JMP		#002A
      0x58140000,  //  0029  LDCONST	R5	K0
      0x58180008,  //  002A  LDCONST	R6	K8
      0x7C0C0600,  //  002B  CALL	R3	3
      0x880C0109,  //  002C  GETMBR	R3	R0	K9
      0x4C100000,  //  002D  LDNIL	R4
      0x1C0C0604,  //  002E  EQ	R3	R3	R4
      0x780E0013,  //  002F  JMPF	R3	#0044
      0x90021300,  //  0030  SETMBR	R0	K9	K0
      0x880C010A,  //  0031  GETMBR	R3	R0	K10
      0x780E0003,  //  0032  JMPF	R3	#0037
      0x880C0109,  //  0033  GETMBR	R3	R0	K9
      0x5412007F,  //  0034  LDINT	R4	128
      0x300C0604,  //  0035  OR	R3	R3	R4
      0x90021203,  //  0036  SETMBR	R0	K9	R3
      0x880C010B,  //  0037  GETMBR	R3	R0	K11
      0x780E0003,  //  0038  JMPF	R3	#003D
      0x880C0109,  //  0039  GETMBR	R3	R0	K9
      0x5412003F,  //  003A  LDINT	R4	64
      0x300C0604,  //  003B  OR	R3	R3	R4
      0x90021203,  //  003C  SETMBR	R0	K9	R3
      0x880C010C,  //  003D  GETMBR	R3	R0	K12
      0x780E0004,  //  003E  JMPF	R3	#0044
      0x880C0109,  //  003F  GETMBR	R3	R0	K9
      0x8810010C,  //  0040  GETMBR	R4	R0	K12
      0x2C100904,  //  0041  AND	R4	R4	K4
      0x300C0604,  //  0042  OR	R3	R3	R4
      0x90021203,  //  0043  SETMBR	R0	K9	R3
      0x8C0C0505,  //  0044  GETMET	R3	R2	K5
      0x88140109,  //  0045  GETMBR	R5	R0	K9
      0x58180006,  //  0046  LDCONST	R6	K6
      0x7C0C0600,  //  0047  CALL	R3	3
      0x8C0C0505,  //  0048  GETMET	R3	R2	K5
      0x8814010D,  //  0049  GETMBR	R5	R0	K13
      0x541A0003,  //  004A  LDINT	R6	4
      0x7C0C0600,  //  004B  CALL	R3	3
      0x880C0102,  //  004C  GETMBR	R3	R0	K2
      0x780E0001,  //  004D  JMPF	R3	#0050
      0x880C010E,  //  004E  GETMBR	R3	R0	K14
      0x400C0403,  //  004F  CONNECT	R3	R2	R3
      0x880C0103,  //  0050  GETMBR	R3	R0	K3
      0x1C0C0706,  //  0051  EQ	R3	R3	K6
      0x780E0001,  //  0052  JMPF	R3	#0055
      0x880C010F,  //  0053  GETMBR	R3	R0	K15
      0x400C0403,  //  0054  CONNECT	R3	R2	R3
      0x880C0103,  //  0055  GETMBR	R3	R0	K3
      0x1C0C0708,  //  0056  EQ	R3	R3	K8
      0x780E0003,  //  0057  JMPF	R3	#005C
      0x8C0C0505,  //  0058  GETMET	R3	R2	K5
      0x88140110,  //  0059  GETMBR	R5	R0	K16
      0x58180008,  //  005A  LDCONST	R6	K8
      0x7C0C0600,  //  005B  CALL	R3	3
      0x600C000C,  //  005C  GETGBL	R3	G12
      0x5C100400,  //  005D  MOVE	R4	R2
      0x7C0C0200,  //  005E  CALL	R3	1
      0x90022203,  //  005F  SETMBR	R0	K17	R3
      0x880C0112,  //  0060  GETMBR	R3	R0	K18
      0x4C100000,  //  0061  LDNIL	R4
      0x1C0C0604,  //  0062  EQ	R3	R3	R4
      0x780E0016,  //  0063  JMPF	R3	#007B
      0x90022500,  //  0064  SETMBR	R0	K18	K0
      0x880C0113,  //  0065  GETMBR	R3	R0	K19
      0x780E0003,  //  0066  JMPF	R3	#006B
      0x880C0112,  //  0067  GETMBR	R3	R0	K18
      0x5412000F,  //  0068  LDINT	R4	16
      0x300C0604,  //  0069  OR	R3	R3	R4
      0x90022403,  //  006A  SETMBR	R0	K18	R3
      0x880C0114,  //  006B  GETMBR	R3	R0	K20
      0x780E0003,  //  006C  JMPF	R3	#0071
      0x880C0112,  //  006D  GETMBR	R3	R0	K18
      0x54120003,  //  006E  LDINT	R4	4
      0x300C0604,  //  006F  OR	R3	R3	R4
      0x90022403,  //  0070  SETMBR	R0	K18	R3
      0x880C0115,  //  0071  GETMBR	R3	R0	K21
      0x780E0002,  //  0072  JMPF	R3	#0076
      0x880C0112,  //  0073  GETMBR	R3	R0	K18
      0x300C0708,  //  0074  OR	R3	R3	K8
      0x90022403,  //  0075  SETMBR	R0	K18	R3
      0x880C0116,  //  0076  GETMBR	R3	R0	K22
      0x780E0002,  //  0077  JMPF	R3	#007B
      0x880C0112,  //  0078  GETMBR	R3	R0	K18
      0x300C0706,  //  0079  OR	R3	R3	K6
      0x90022403,  //  007A  SETMBR	R0	K18	R3
      0x8C0C0505,  //  007B  GETMET	R3	R2	K5
      0x88140112,  //  007C  GETMBR	R5	R0	K18
      0x58180006,  //  007D  LDCONST	R6	K6
      0x7C0C0600,  //  007E  CALL	R3	3
      0x8C0C0505,  //  007F  GETMET	R3	R2	K5
      0x88140117,  //  0080  GETMBR	R5	R0	K23
      0x58180006,  //  0081  LDCONST	R6	K6
      0x7C0C0600,  //  0082  CALL	R3	3
      0x8C0C0505,  //  0083  GETMET	R3	R2	K5
      0x88140118,  //  0084  GETMBR	R5	R0	K24
      0x541AFFFE,  //  0085  LDINT	R6	65535
      0x2C140A06,  //  0086  AND	R5	R5	R6
      0x58180008,  //  0087  LDCONST	R6	K8
      0x7C0C0600,  //  0088  CALL	R3	3
      0x8C0C0505,  //  0089  GETMET	R3	R2	K5
      0x88140119,  //  008A  GETMBR	R5	R0	K25
      0x58180008,  //  008B  LDCONST	R6	K8
      0x7C0C0600,  //  008C  CALL	R3	3
      0x880C0115,  //  008D  GETMBR	R3	R0	K21
      0x780E0003,  //  008E  JMPF	R3	#0093
      0x8C0C0505,  //  008F  GETMET	R3	R2	K5
      0x8814011A,  //  0090  GETMBR	R5	R0	K26
      0x541A0003,  //  0091  LDINT	R6	4
      0x7C0C0600,  //  0092  CALL	R3	3
      0x600C000C,  //  0093  GETGBL	R3	G12
      0x5C100400,  //  0094  MOVE	R4	R2
      0x7C0C0200,  //  0095  CALL	R3	1
      0x90023603,  //  0096  SETMBR	R0	K27	R3
      0x78060000,  //  0097  JMPF	R1	#0099
      0x400C0401,  //  0098  CONNECT	R3	R2	R1
      0x90023802,  //  0099  SETMBR	R0	K28	R2
      0x80040400,  //  009A  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: encrypt
********************************************************************/
be_local_closure(Matter_Frame_encrypt,   /* name */
  be_nested_proto(
    23,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(raw),
    /* K2   */  be_nested_str_weak(session),
    /* K3   */  be_nested_str_weak(payload_idx),
    /* K4   */  be_nested_str_weak(get_r2i),
    /* K5   */  be_nested_str_weak(message_handler),
    /* K6   */  be_nested_str_weak(_n_bytes),
    /* K7   */  be_nested_str_weak(clear),
    /* K8   */  be_nested_str_weak(add),
    /* K9   */  be_nested_str_weak(flags),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_str_weak(message_counter),
    /* K12  */  be_nested_str_weak(is_CASE),
    /* K13  */  be_nested_str_weak(get_device_id),
    /* K14  */  be_nested_str_weak(resize),
    /* K15  */  be_nested_str_weak(AES_CCM),
    /* K16  */  be_nested_str_weak(encrypt1),
    /* K17  */  be_const_int(0),
    }),
    be_str_weak(encrypt),
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x88100103,  //  0003  GETMBR	R4	R0	K3
      0x5416000F,  //  0004  LDINT	R5	16
      0x8C180704,  //  0005  GETMET	R6	R3	K4
      0x7C180200,  //  0006  CALL	R6	1
      0x881C0105,  //  0007  GETMBR	R7	R0	K5
      0x881C0F06,  //  0008  GETMBR	R7	R7	K6
      0x8C200F07,  //  0009  GETMET	R8	R7	K7
      0x7C200200,  //  000A  CALL	R8	1
      0x8C200F08,  //  000B  GETMET	R8	R7	K8
      0x88280109,  //  000C  GETMBR	R10	R0	K9
      0x582C000A,  //  000D  LDCONST	R11	K10
      0x7C200600,  //  000E  CALL	R8	3
      0x8C200F08,  //  000F  GETMET	R8	R7	K8
      0x8828010B,  //  0010  GETMBR	R10	R0	K11
      0x542E0003,  //  0011  LDINT	R11	4
      0x7C200600,  //  0012  CALL	R8	3
      0x8C20070C,  //  0013  GETMET	R8	R3	K12
      0x7C200200,  //  0014  CALL	R8	1
      0x78220005,  //  0015  JMPF	R8	#001C
      0x8C20070D,  //  0016  GETMET	R8	R3	K13
      0x7C200200,  //  0017  CALL	R8	1
      0x78220002,  //  0018  JMPF	R8	#001C
      0x8C20070D,  //  0019  GETMET	R8	R3	K13
      0x7C200200,  //  001A  CALL	R8	1
      0x40200E08,  //  001B  CONNECT	R8	R7	R8
      0x8C200F0E,  //  001C  GETMET	R8	R7	K14
      0x542A000C,  //  001D  LDINT	R10	13
      0x7C200400,  //  001E  CALL	R8	2
      0x8C20050E,  //  001F  GETMET	R8	R2	K14
      0x6028000C,  //  0020  GETGBL	R10	G12
      0x5C2C0400,  //  0021  MOVE	R11	R2
      0x7C280200,  //  0022  CALL	R10	1
      0x00281405,  //  0023  ADD	R10	R10	R5
      0x7C200400,  //  0024  CALL	R8	2
      0x8820030F,  //  0025  GETMBR	R8	R1	K15
      0x8C201110,  //  0026  GETMET	R8	R8	K16
      0x5C280C00,  //  0027  MOVE	R10	R6
      0x5C2C0E00,  //  0028  MOVE	R11	R7
      0x58300011,  //  0029  LDCONST	R12	K17
      0x6034000C,  //  002A  GETGBL	R13	G12
      0x5C380E00,  //  002B  MOVE	R14	R7
      0x7C340200,  //  002C  CALL	R13	1
      0x5C380400,  //  002D  MOVE	R14	R2
      0x583C0011,  //  002E  LDCONST	R15	K17
      0x5C400800,  //  002F  MOVE	R16	R4
      0x5C440400,  //  0030  MOVE	R17	R2
      0x5C480800,  //  0031  MOVE	R18	R4
      0x604C000C,  //  0032  GETGBL	R19	G12
      0x5C500400,  //  0033  MOVE	R20	R2
      0x7C4C0200,  //  0034  CALL	R19	1
      0x044C2604,  //  0035  SUB	R19	R19	R4
      0x044C2605,  //  0036  SUB	R19	R19	R5
      0x5C500400,  //  0037  MOVE	R20	R2
      0x6054000C,  //  0038  GETGBL	R21	G12
      0x5C580400,  //  0039  MOVE	R22	R2
      0x7C540200,  //  003A  CALL	R21	1
      0x04542A05,  //  003B  SUB	R21	R21	R5
      0x5C580A00,  //  003C  MOVE	R22	R5
      0x7C201C00,  //  003D  CALL	R8	14
      0x80000000,  //  003E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: debug
********************************************************************/
be_local_closure(Matter_Frame_debug,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(Frame),
    /* K2   */  be_nested_str_weak(message_handler),
    /* K3   */  be_nested_str_weak(decode_header),
    /* K4   */  be_nested_str_weak(decode_payload),
    }),
    be_str_weak(debug),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x80000400,  //  0000  RET	0
      0xB80A0000,  //  0001  GETNGBL	R2	K0
      0x8C080501,  //  0002  GETMET	R2	R2	K1
      0x88100102,  //  0003  GETMBR	R4	R0	K2
      0x5C140200,  //  0004  MOVE	R5	R1
      0x7C080600,  //  0005  CALL	R2	3
      0x8C0C0503,  //  0006  GETMET	R3	R2	K3
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C0C0504,  //  0008  GETMET	R3	R2	K4
      0x7C0C0200,  //  0009  CALL	R3	1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: build_standalone_ack
********************************************************************/
be_local_closure(Matter_Frame_build_standalone_ack,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(message_handler),
    /* K1   */  be_nested_str_weak(remote_ip),
    /* K2   */  be_nested_str_weak(remote_port),
    /* K3   */  be_nested_str_weak(flag_s),
    /* K4   */  be_nested_str_weak(flag_dsiz),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_weak(dest_node_id_8),
    /* K7   */  be_nested_str_weak(source_node_id),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(session),
    /* K10  */  be_nested_str_weak(message_counter),
    /* K11  */  be_nested_str_weak(counter_snd_next),
    /* K12  */  be_nested_str_weak(local_session_id),
    /* K13  */  be_nested_str_weak(initiator_session_id),
    /* K14  */  be_nested_str_weak(x_flag_i),
    /* K15  */  be_nested_str_weak(opcode),
    /* K16  */  be_nested_str_weak(exchange_id),
    /* K17  */  be_nested_str_weak(protocol_id),
    /* K18  */  be_nested_str_weak(x_flag_a),
    /* K19  */  be_nested_str_weak(ack_message_counter),
    /* K20  */  be_nested_str_weak(x_flag_r),
    }),
    be_str_weak(build_standalone_ack),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x60080006,  //  0000  GETGBL	R2	G6
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x880C0100,  //  0003  GETMBR	R3	R0	K0
      0x7C080200,  //  0004  CALL	R2	1
      0x880C0101,  //  0005  GETMBR	R3	R0	K1
      0x900A0203,  //  0006  SETMBR	R2	K1	R3
      0x880C0102,  //  0007  GETMBR	R3	R0	K2
      0x900A0403,  //  0008  SETMBR	R2	K2	R3
      0x880C0103,  //  0009  GETMBR	R3	R0	K3
      0x780E0003,  //  000A  JMPF	R3	#000F
      0x900A0905,  //  000B  SETMBR	R2	K4	K5
      0x880C0107,  //  000C  GETMBR	R3	R0	K7
      0x900A0C03,  //  000D  SETMBR	R2	K6	R3
      0x70020000,  //  000E  JMP		#0010
      0x900A0908,  //  000F  SETMBR	R2	K4	K8
      0x880C0109,  //  0010  GETMBR	R3	R0	K9
      0x900A1203,  //  0011  SETMBR	R2	K9	R3
      0x880C0109,  //  0012  GETMBR	R3	R0	K9
      0x8C0C070B,  //  0013  GETMET	R3	R3	K11
      0x7C0C0200,  //  0014  CALL	R3	1
      0x900A1403,  //  0015  SETMBR	R2	K10	R3
      0x880C0109,  //  0016  GETMBR	R3	R0	K9
      0x880C070D,  //  0017  GETMBR	R3	R3	K13
      0x900A1803,  //  0018  SETMBR	R2	K12	R3
      0x880C010E,  //  0019  GETMBR	R3	R0	K14
      0x780E0001,  //  001A  JMPF	R3	#001D
      0x580C0008,  //  001B  LDCONST	R3	K8
      0x70020000,  //  001C  JMP		#001E
      0x580C0005,  //  001D  LDCONST	R3	K5
      0x900A1C03,  //  001E  SETMBR	R2	K14	R3
      0x540E000F,  //  001F  LDINT	R3	16
      0x900A1E03,  //  0020  SETMBR	R2	K15	R3
      0x880C0110,  //  0021  GETMBR	R3	R0	K16
      0x900A2003,  //  0022  SETMBR	R2	K16	R3
      0x900A2308,  //  0023  SETMBR	R2	K17	K8
      0x900A2505,  //  0024  SETMBR	R2	K18	K5
      0x880C010A,  //  0025  GETMBR	R3	R0	K10
      0x900A2603,  //  0026  SETMBR	R2	K19	R3
      0x78060001,  //  0027  JMPF	R1	#002A
      0x580C0005,  //  0028  LDCONST	R3	K5
      0x70020000,  //  0029  JMP		#002B
      0x580C0008,  //  002A  LDCONST	R3	K8
      0x900A2803,  //  002B  SETMBR	R2	K20	R3
      0x80040400,  //  002C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: build_response
********************************************************************/
be_local_closure(Matter_Frame_build_response,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(message_handler),
    /* K1   */  be_nested_str_weak(remote_ip),
    /* K2   */  be_nested_str_weak(remote_port),
    /* K3   */  be_nested_str_weak(flag_s),
    /* K4   */  be_nested_str_weak(flag_dsiz),
    /* K5   */  be_const_int(1),
    /* K6   */  be_nested_str_weak(dest_node_id_8),
    /* K7   */  be_nested_str_weak(source_node_id),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(session),
    /* K10  */  be_nested_str_weak(local_session_id),
    /* K11  */  be_nested_str_weak(initiator_session_id),
    /* K12  */  be_nested_str_weak(message_counter),
    /* K13  */  be_nested_str_weak(counter_snd_next),
    /* K14  */  be_nested_str_weak(_counter_insecure_snd),
    /* K15  */  be_nested_str_weak(next),
    /* K16  */  be_nested_str_weak(x_flag_i),
    /* K17  */  be_nested_str_weak(opcode),
    /* K18  */  be_nested_str_weak(exchange_id),
    /* K19  */  be_nested_str_weak(protocol_id),
    /* K20  */  be_nested_str_weak(x_flag_r),
    /* K21  */  be_nested_str_weak(x_flag_a),
    /* K22  */  be_nested_str_weak(ack_message_counter),
    /* K23  */  be_nested_str_weak(matter),
    /* K24  */  be_nested_str_weak(get_opcode_name),
    /* K25  */  be_nested_str_weak(0x_X2502X),
    /* K26  */  be_nested_str_weak(tasmota),
    /* K27  */  be_nested_str_weak(log),
    /* K28  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20_X25s),
    /* K29  */  be_const_int(3),
    }),
    be_str_weak(build_response),
    &be_const_str_solidified,
    ( &(const binstruction[91]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x1C100604,  //  0001  EQ	R4	R3	R4
      0x78120005,  //  0002  JMPF	R4	#0009
      0x60100006,  //  0003  GETGBL	R4	G6
      0x5C140000,  //  0004  MOVE	R5	R0
      0x7C100200,  //  0005  CALL	R4	1
      0x88140100,  //  0006  GETMBR	R5	R0	K0
      0x7C100200,  //  0007  CALL	R4	1
      0x5C0C0800,  //  0008  MOVE	R3	R4
      0x88100101,  //  0009  GETMBR	R4	R0	K1
      0x900E0204,  //  000A  SETMBR	R3	K1	R4
      0x88100102,  //  000B  GETMBR	R4	R0	K2
      0x900E0404,  //  000C  SETMBR	R3	K2	R4
      0x88100103,  //  000D  GETMBR	R4	R0	K3
      0x78120003,  //  000E  JMPF	R4	#0013
      0x900E0905,  //  000F  SETMBR	R3	K4	K5
      0x88100107,  //  0010  GETMBR	R4	R0	K7
      0x900E0C04,  //  0011  SETMBR	R3	K6	R4
      0x70020000,  //  0012  JMP		#0014
      0x900E0908,  //  0013  SETMBR	R3	K4	K8
      0x88100109,  //  0014  GETMBR	R4	R0	K9
      0x900E1204,  //  0015  SETMBR	R3	K9	R4
      0x8810010A,  //  0016  GETMBR	R4	R0	K10
      0x20100908,  //  0017  NE	R4	R4	K8
      0x7812000D,  //  0018  JMPF	R4	#0027
      0x88100109,  //  0019  GETMBR	R4	R0	K9
      0x7812000B,  //  001A  JMPF	R4	#0027
      0x88100109,  //  001B  GETMBR	R4	R0	K9
      0x8810090B,  //  001C  GETMBR	R4	R4	K11
      0x20100908,  //  001D  NE	R4	R4	K8
      0x78120007,  //  001E  JMPF	R4	#0027
      0x88100109,  //  001F  GETMBR	R4	R0	K9
      0x8C10090D,  //  0020  GETMET	R4	R4	K13
      0x7C100200,  //  0021  CALL	R4	1
      0x900E1804,  //  0022  SETMBR	R3	K12	R4
      0x88100109,  //  0023  GETMBR	R4	R0	K9
      0x8810090B,  //  0024  GETMBR	R4	R4	K11
      0x900E1404,  //  0025  SETMBR	R3	K10	R4
      0x70020005,  //  0026  JMP		#002D
      0x88100109,  //  0027  GETMBR	R4	R0	K9
      0x8810090E,  //  0028  GETMBR	R4	R4	K14
      0x8C10090F,  //  0029  GETMET	R4	R4	K15
      0x7C100200,  //  002A  CALL	R4	1
      0x900E1804,  //  002B  SETMBR	R3	K12	R4
      0x900E1508,  //  002C  SETMBR	R3	K10	K8
      0x88100110,  //  002D  GETMBR	R4	R0	K16
      0x78120001,  //  002E  JMPF	R4	#0031
      0x58100008,  //  002F  LDCONST	R4	K8
      0x70020000,  //  0030  JMP		#0032
      0x58100005,  //  0031  LDCONST	R4	K5
      0x900E2004,  //  0032  SETMBR	R3	K16	R4
      0x900E2201,  //  0033  SETMBR	R3	K17	R1
      0x88100112,  //  0034  GETMBR	R4	R0	K18
      0x900E2404,  //  0035  SETMBR	R3	K18	R4
      0x88100113,  //  0036  GETMBR	R4	R0	K19
      0x900E2604,  //  0037  SETMBR	R3	K19	R4
      0x88100114,  //  0038  GETMBR	R4	R0	K20
      0x78120002,  //  0039  JMPF	R4	#003D
      0x900E2B05,  //  003A  SETMBR	R3	K21	K5
      0x8810010C,  //  003B  GETMBR	R4	R0	K12
      0x900E2C04,  //  003C  SETMBR	R3	K22	R4
      0x780A0001,  //  003D  JMPF	R2	#0040
      0x58100005,  //  003E  LDCONST	R4	K5
      0x70020000,  //  003F  JMP		#0041
      0x58100008,  //  0040  LDCONST	R4	K8
      0x900E2804,  //  0041  SETMBR	R3	K20	R4
      0x8810070A,  //  0042  GETMBR	R4	R3	K10
      0x1C100908,  //  0043  EQ	R4	R4	K8
      0x78120014,  //  0044  JMPF	R4	#005A
      0xB8122E00,  //  0045  GETNGBL	R4	K23
      0x8C100918,  //  0046  GETMET	R4	R4	K24
      0x88180711,  //  0047  GETMBR	R6	R3	K17
      0x7C100400,  //  0048  CALL	R4	2
      0x5C140800,  //  0049  MOVE	R5	R4
      0x74160004,  //  004A  JMPT	R5	#0050
      0x60140018,  //  004B  GETGBL	R5	G24
      0x58180019,  //  004C  LDCONST	R6	K25
      0x881C0711,  //  004D  GETMBR	R7	R3	K17
      0x7C140400,  //  004E  CALL	R5	2
      0x5C100A00,  //  004F  MOVE	R4	R5
      0xB8163400,  //  0050  GETNGBL	R5	K26
      0x8C140B1B,  //  0051  GETMET	R5	R5	K27
      0x601C0018,  //  0052  GETGBL	R7	G24
      0x5820001C,  //  0053  LDCONST	R8	K28
      0x88240709,  //  0054  GETMBR	R9	R3	K9
      0x8824130A,  //  0055  GETMBR	R9	R9	K10
      0x5C280800,  //  0056  MOVE	R10	R4
      0x7C1C0600,  //  0057  CALL	R7	3
      0x5820001D,  //  0058  LDCONST	R8	K29
      0x7C140600,  //  0059  CALL	R5	3
      0x80040600,  //  005A  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: initiate_response
********************************************************************/
be_local_closure(Matter_Frame_initiate_response,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    5,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Frame),
    /* K1   */  be_nested_str_weak(matter),
    /* K2   */  be_nested_str_weak(Frame),
    /* K3   */  be_nested_str_weak(remote_ip),
    /* K4   */  be_nested_str_weak(_ip),
    /* K5   */  be_nested_str_weak(remote_port),
    /* K6   */  be_nested_str_weak(_port),
    /* K7   */  be_nested_str_weak(flag_dsiz),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(session),
    /* K10  */  be_nested_str_weak(initiator_session_id),
    /* K11  */  be_nested_str_weak(message_counter),
    /* K12  */  be_nested_str_weak(counter_snd_next),
    /* K13  */  be_nested_str_weak(local_session_id),
    /* K14  */  be_nested_str_weak(_counter_insecure_snd),
    /* K15  */  be_nested_str_weak(next),
    /* K16  */  be_nested_str_weak(x_flag_i),
    /* K17  */  be_const_int(1),
    /* K18  */  be_nested_str_weak(opcode),
    /* K19  */  be_nested_str_weak(_exchange_id),
    /* K20  */  be_nested_str_weak(exchange_id),
    /* K21  */  be_nested_str_weak(protocol_id),
    /* K22  */  be_nested_str_weak(x_flag_r),
    }),
    be_str_weak(initiate_response),
    &be_const_str_solidified,
    ( &(const binstruction[46]) {  /* code */
      0x58140000,  //  0000  LDCONST	R5	K0
      0x4C180000,  //  0001  LDNIL	R6
      0x1C180806,  //  0002  EQ	R6	R4	R6
      0x781A0004,  //  0003  JMPF	R6	#0009
      0xB81A0200,  //  0004  GETNGBL	R6	K1
      0x8C180D02,  //  0005  GETMET	R6	R6	K2
      0x5C200000,  //  0006  MOVE	R8	R0
      0x7C180400,  //  0007  CALL	R6	2
      0x5C100C00,  //  0008  MOVE	R4	R6
      0x88180304,  //  0009  GETMBR	R6	R1	K4
      0x90120606,  //  000A  SETMBR	R4	K3	R6
      0x88180306,  //  000B  GETMBR	R6	R1	K6
      0x90120A06,  //  000C  SETMBR	R4	K5	R6
      0x90120F08,  //  000D  SETMBR	R4	K7	K8
      0x90121201,  //  000E  SETMBR	R4	K9	R1
      0x78060008,  //  000F  JMPF	R1	#0019
      0x8818030A,  //  0010  GETMBR	R6	R1	K10
      0x20180D08,  //  0011  NE	R6	R6	K8
      0x781A0005,  //  0012  JMPF	R6	#0019
      0x8C18030C,  //  0013  GETMET	R6	R1	K12
      0x7C180200,  //  0014  CALL	R6	1
      0x90121606,  //  0015  SETMBR	R4	K11	R6
      0x8818030A,  //  0016  GETMBR	R6	R1	K10
      0x90121A06,  //  0017  SETMBR	R4	K13	R6
      0x70020004,  //  0018  JMP		#001E
      0x8818030E,  //  0019  GETMBR	R6	R1	K14
      0x8C180D0F,  //  001A  GETMET	R6	R6	K15
      0x7C180200,  //  001B  CALL	R6	1
      0x90121606,  //  001C  SETMBR	R4	K11	R6
      0x90121B08,  //  001D  SETMBR	R4	K13	K8
      0x90122111,  //  001E  SETMBR	R4	K16	K17
      0x90122402,  //  001F  SETMBR	R4	K18	R2
      0x88180313,  //  0020  GETMBR	R6	R1	K19
      0x00180D11,  //  0021  ADD	R6	R6	K17
      0x90062606,  //  0022  SETMBR	R1	K19	R6
      0x88180313,  //  0023  GETMBR	R6	R1	K19
      0x541EFFFF,  //  0024  LDINT	R7	65536
      0x30180C07,  //  0025  OR	R6	R6	R7
      0x90122806,  //  0026  SETMBR	R4	K20	R6
      0x90122B11,  //  0027  SETMBR	R4	K21	K17
      0x780E0001,  //  0028  JMPF	R3	#002B
      0x58180011,  //  0029  LDCONST	R6	K17
      0x70020000,  //  002A  JMP		#002C
      0x58180008,  //  002B  LDCONST	R6	K8
      0x90122C06,  //  002C  SETMBR	R4	K22	R6
      0x80040800,  //  002D  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: decode_header
********************************************************************/
be_local_closure(Matter_Frame_decode_header,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(raw),
    /* K2   */  be_nested_str_weak(flags),
    /* K3   */  be_nested_str_weak(get),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(flag_s),
    /* K6   */  be_nested_str_weak(getbits),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(flag_dsiz),
    /* K9   */  be_const_int(3),
    /* K10  */  be_nested_str_weak(sec_flags),
    /* K11  */  be_nested_str_weak(sec_p),
    /* K12  */  be_nested_str_weak(sec_c),
    /* K13  */  be_nested_str_weak(sec_mx),
    /* K14  */  be_nested_str_weak(sec_sesstype),
    /* K15  */  be_nested_str_weak(local_session_id),
    /* K16  */  be_nested_str_weak(message_counter),
    /* K17  */  be_nested_str_weak(source_node_id),
    /* K18  */  be_nested_str_weak(dest_node_id_8),
    /* K19  */  be_nested_str_weak(dest_node_id_2),
    /* K20  */  be_nested_str_weak(payload_idx),
    }),
    be_str_weak(decode_header),
    &be_const_str_solidified,
    ( &(const binstruction[121]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C0C0503,  //  0002  GETMET	R3	R2	K3
      0x58140000,  //  0003  LDCONST	R5	K0
      0x58180004,  //  0004  LDCONST	R6	K4
      0x7C0C0600,  //  0005  CALL	R3	3
      0x90020403,  //  0006  SETMBR	R0	K2	R3
      0x880C0102,  //  0007  GETMBR	R3	R0	K2
      0x541200F7,  //  0008  LDINT	R4	248
      0x2C0C0604,  //  0009  AND	R3	R3	R4
      0x200C0700,  //  000A  NE	R3	R3	K0
      0x780E0001,  //  000B  JMPF	R3	#000E
      0x500C0000,  //  000C  LDBOOL	R3	0	0
      0x80040600,  //  000D  RET	1	R3
      0x8C0C0506,  //  000E  GETMET	R3	R2	K6
      0x58140007,  //  000F  LDCONST	R5	K7
      0x58180004,  //  0010  LDCONST	R6	K4
      0x7C0C0600,  //  0011  CALL	R3	3
      0x90020A03,  //  0012  SETMBR	R0	K5	R3
      0x8C0C0506,  //  0013  GETMET	R3	R2	K6
      0x58140000,  //  0014  LDCONST	R5	K0
      0x58180007,  //  0015  LDCONST	R6	K7
      0x7C0C0600,  //  0016  CALL	R3	3
      0x90021003,  //  0017  SETMBR	R0	K8	R3
      0x880C0108,  //  0018  GETMBR	R3	R0	K8
      0x1C0C0709,  //  0019  EQ	R3	R3	K9
      0x780E0001,  //  001A  JMPF	R3	#001D
      0x500C0000,  //  001B  LDBOOL	R3	0	0
      0x80040600,  //  001C  RET	1	R3
      0x8C0C0503,  //  001D  GETMET	R3	R2	K3
      0x58140009,  //  001E  LDCONST	R5	K9
      0x58180004,  //  001F  LDCONST	R6	K4
      0x7C0C0600,  //  0020  CALL	R3	3
      0x90021403,  //  0021  SETMBR	R0	K10	R3
      0x8C0C0506,  //  0022  GETMET	R3	R2	K6
      0x54160007,  //  0023  LDINT	R5	8
      0x08161205,  //  0024  MUL	R5	K9	R5
      0x541A0006,  //  0025  LDINT	R6	7
      0x00140A06,  //  0026  ADD	R5	R5	R6
      0x58180004,  //  0027  LDCONST	R6	K4
      0x7C0C0600,  //  0028  CALL	R3	3
      0x90021603,  //  0029  SETMBR	R0	K11	R3
      0x8C0C0506,  //  002A  GETMET	R3	R2	K6
      0x54160007,  //  002B  LDINT	R5	8
      0x08161205,  //  002C  MUL	R5	K9	R5
      0x541A0005,  //  002D  LDINT	R6	6
      0x00140A06,  //  002E  ADD	R5	R5	R6
      0x58180004,  //  002F  LDCONST	R6	K4
      0x7C0C0600,  //  0030  CALL	R3	3
      0x90021803,  //  0031  SETMBR	R0	K12	R3
      0x8C0C0506,  //  0032  GETMET	R3	R2	K6
      0x54160007,  //  0033  LDINT	R5	8
      0x08161205,  //  0034  MUL	R5	K9	R5
      0x541A0004,  //  0035  LDINT	R6	5
      0x00140A06,  //  0036  ADD	R5	R5	R6
      0x58180004,  //  0037  LDCONST	R6	K4
      0x7C0C0600,  //  0038  CALL	R3	3
      0x90021A03,  //  0039  SETMBR	R0	K13	R3
      0x8C0C0506,  //  003A  GETMET	R3	R2	K6
      0x54160007,  //  003B  LDINT	R5	8
      0x08161205,  //  003C  MUL	R5	K9	R5
      0x58180007,  //  003D  LDCONST	R6	K7
      0x7C0C0600,  //  003E  CALL	R3	3
      0x90021C03,  //  003F  SETMBR	R0	K14	R3
      0x880C010E,  //  0040  GETMBR	R3	R0	K14
      0x240C0704,  //  0041  GT	R3	R3	K4
      0x780E0001,  //  0042  JMPF	R3	#0045
      0x500C0000,  //  0043  LDBOOL	R3	0	0
      0x80040600,  //  0044  RET	1	R3
      0x8C0C0503,  //  0045  GETMET	R3	R2	K3
      0x58140004,  //  0046  LDCONST	R5	K4
      0x58180007,  //  0047  LDCONST	R6	K7
      0x7C0C0600,  //  0048  CALL	R3	3
      0x90021E03,  //  0049  SETMBR	R0	K15	R3
      0x8C0C0503,  //  004A  GETMET	R3	R2	K3
      0x54160003,  //  004B  LDINT	R5	4
      0x541A0003,  //  004C  LDINT	R6	4
      0x7C0C0600,  //  004D  CALL	R3	3
      0x90022003,  //  004E  SETMBR	R0	K16	R3
      0x540E0007,  //  004F  LDINT	R3	8
      0x00040203,  //  0050  ADD	R1	R1	R3
      0x880C0105,  //  0051  GETMBR	R3	R0	K5
      0x780E0006,  //  0052  JMPF	R3	#005A
      0x540E0006,  //  0053  LDINT	R3	7
      0x000C0203,  //  0054  ADD	R3	R1	R3
      0x400C0203,  //  0055  CONNECT	R3	R1	R3
      0x940C0403,  //  0056  GETIDX	R3	R2	R3
      0x90022203,  //  0057  SETMBR	R0	K17	R3
      0x540E0007,  //  0058  LDINT	R3	8
      0x00040203,  //  0059  ADD	R1	R1	R3
      0x880C0108,  //  005A  GETMBR	R3	R0	K8
      0x1C0C0704,  //  005B  EQ	R3	R3	K4
      0x780E0007,  //  005C  JMPF	R3	#0065
      0x540E0006,  //  005D  LDINT	R3	7
      0x000C0203,  //  005E  ADD	R3	R1	R3
      0x400C0203,  //  005F  CONNECT	R3	R1	R3
      0x940C0403,  //  0060  GETIDX	R3	R2	R3
      0x90022403,  //  0061  SETMBR	R0	K18	R3
      0x540E0007,  //  0062  LDINT	R3	8
      0x00040203,  //  0063  ADD	R1	R1	R3
      0x70020008,  //  0064  JMP		#006E
      0x880C0108,  //  0065  GETMBR	R3	R0	K8
      0x1C0C0707,  //  0066  EQ	R3	R3	K7
      0x780E0005,  //  0067  JMPF	R3	#006E
      0x8C0C0503,  //  0068  GETMET	R3	R2	K3
      0x5C140200,  //  0069  MOVE	R5	R1
      0x58180007,  //  006A  LDCONST	R6	K7
      0x7C0C0600,  //  006B  CALL	R3	3
      0x90022603,  //  006C  SETMBR	R0	K19	R3
      0x00040307,  //  006D  ADD	R1	R1	K7
      0x880C010D,  //  006E  GETMBR	R3	R0	K13
      0x780E0005,  //  006F  JMPF	R3	#0076
      0x8C0C0503,  //  0070  GETMET	R3	R2	K3
      0x5C140200,  //  0071  MOVE	R5	R1
      0x58180007,  //  0072  LDCONST	R6	K7
      0x7C0C0600,  //  0073  CALL	R3	3
      0x00100707,  //  0074  ADD	R4	R3	K7
      0x00040204,  //  0075  ADD	R1	R1	R4
      0x90022801,  //  0076  SETMBR	R0	K20	R1
      0x500C0200,  //  0077  LDBOOL	R3	1	0
      0x80040600,  //  0078  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Frame_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(message_handler),
    /* K1   */  be_nested_str_weak(raw),
    /* K2   */  be_nested_str_weak(remote_ip),
    /* K3   */  be_nested_str_weak(remote_port),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x90020202,  //  0001  SETMBR	R0	K1	R2
      0x90020403,  //  0002  SETMBR	R0	K2	R3
      0x90020604,  //  0003  SETMBR	R0	K3	R4
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: decode_payload
********************************************************************/
be_local_closure(Matter_Frame_decode_payload,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[19]) {     /* constants */
    /* K0   */  be_nested_str_weak(payload_idx),
    /* K1   */  be_nested_str_weak(raw),
    /* K2   */  be_nested_str_weak(x_flags),
    /* K3   */  be_nested_str_weak(get),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(x_flag_v),
    /* K6   */  be_nested_str_weak(getbits),
    /* K7   */  be_nested_str_weak(x_flag_sx),
    /* K8   */  be_const_int(3),
    /* K9   */  be_nested_str_weak(x_flag_r),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(x_flag_a),
    /* K12  */  be_nested_str_weak(x_flag_i),
    /* K13  */  be_nested_str_weak(opcode),
    /* K14  */  be_nested_str_weak(exchange_id),
    /* K15  */  be_nested_str_weak(protocol_id),
    /* K16  */  be_nested_str_weak(vendor_id),
    /* K17  */  be_nested_str_weak(ack_message_counter),
    /* K18  */  be_nested_str_weak(app_payload_idx),
    }),
    be_str_weak(decode_payload),
    &be_const_str_solidified,
    ( &(const binstruction[93]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x8C0C0503,  //  0002  GETMET	R3	R2	K3
      0x5C140200,  //  0003  MOVE	R5	R1
      0x58180004,  //  0004  LDCONST	R6	K4
      0x7C0C0600,  //  0005  CALL	R3	3
      0x90020403,  //  0006  SETMBR	R0	K2	R3
      0x8C0C0506,  //  0007  GETMET	R3	R2	K6
      0x54160007,  //  0008  LDINT	R5	8
      0x08140205,  //  0009  MUL	R5	R1	R5
      0x541A0003,  //  000A  LDINT	R6	4
      0x00140A06,  //  000B  ADD	R5	R5	R6
      0x58180004,  //  000C  LDCONST	R6	K4
      0x7C0C0600,  //  000D  CALL	R3	3
      0x90020A03,  //  000E  SETMBR	R0	K5	R3
      0x8C0C0506,  //  000F  GETMET	R3	R2	K6
      0x54160007,  //  0010  LDINT	R5	8
      0x08140205,  //  0011  MUL	R5	R1	R5
      0x00140B08,  //  0012  ADD	R5	R5	K8
      0x58180004,  //  0013  LDCONST	R6	K4
      0x7C0C0600,  //  0014  CALL	R3	3
      0x90020E03,  //  0015  SETMBR	R0	K7	R3
      0x8C0C0506,  //  0016  GETMET	R3	R2	K6
      0x54160007,  //  0017  LDINT	R5	8
      0x08140205,  //  0018  MUL	R5	R1	R5
      0x00140B0A,  //  0019  ADD	R5	R5	K10
      0x58180004,  //  001A  LDCONST	R6	K4
      0x7C0C0600,  //  001B  CALL	R3	3
      0x90021203,  //  001C  SETMBR	R0	K9	R3
      0x8C0C0506,  //  001D  GETMET	R3	R2	K6
      0x54160007,  //  001E  LDINT	R5	8
      0x08140205,  //  001F  MUL	R5	R1	R5
      0x00140B04,  //  0020  ADD	R5	R5	K4
      0x58180004,  //  0021  LDCONST	R6	K4
      0x7C0C0600,  //  0022  CALL	R3	3
      0x90021603,  //  0023  SETMBR	R0	K11	R3
      0x8C0C0506,  //  0024  GETMET	R3	R2	K6
      0x54160007,  //  0025  LDINT	R5	8
      0x08140205,  //  0026  MUL	R5	R1	R5
      0x58180004,  //  0027  LDCONST	R6	K4
      0x7C0C0600,  //  0028  CALL	R3	3
      0x90021803,  //  0029  SETMBR	R0	K12	R3
      0x8C0C0503,  //  002A  GETMET	R3	R2	K3
      0x00140304,  //  002B  ADD	R5	R1	K4
      0x58180004,  //  002C  LDCONST	R6	K4
      0x7C0C0600,  //  002D  CALL	R3	3
      0x90021A03,  //  002E  SETMBR	R0	K13	R3
      0x8C0C0503,  //  002F  GETMET	R3	R2	K3
      0x0014030A,  //  0030  ADD	R5	R1	K10
      0x5818000A,  //  0031  LDCONST	R6	K10
      0x7C0C0600,  //  0032  CALL	R3	3
      0x90021C03,  //  0033  SETMBR	R0	K14	R3
      0x880C010C,  //  0034  GETMBR	R3	R0	K12
      0x740E0003,  //  0035  JMPT	R3	#003A
      0x880C010E,  //  0036  GETMBR	R3	R0	K14
      0x5412FFFF,  //  0037  LDINT	R4	65536
      0x300C0604,  //  0038  OR	R3	R3	R4
      0x90021C03,  //  0039  SETMBR	R0	K14	R3
      0x8C0C0503,  //  003A  GETMET	R3	R2	K3
      0x54160003,  //  003B  LDINT	R5	4
      0x00140205,  //  003C  ADD	R5	R1	R5
      0x5818000A,  //  003D  LDCONST	R6	K10
      0x7C0C0600,  //  003E  CALL	R3	3
      0x90021E03,  //  003F  SETMBR	R0	K15	R3
      0x540E0005,  //  0040  LDINT	R3	6
      0x00040203,  //  0041  ADD	R1	R1	R3
      0x880C0105,  //  0042  GETMBR	R3	R0	K5
      0x780E0005,  //  0043  JMPF	R3	#004A
      0x8C0C0503,  //  0044  GETMET	R3	R2	K3
      0x5C140200,  //  0045  MOVE	R5	R1
      0x5818000A,  //  0046  LDCONST	R6	K10
      0x7C0C0600,  //  0047  CALL	R3	3
      0x90022003,  //  0048  SETMBR	R0	K16	R3
      0x0004030A,  //  0049  ADD	R1	R1	K10
      0x880C010B,  //  004A  GETMBR	R3	R0	K11
      0x780E0006,  //  004B  JMPF	R3	#0053
      0x8C0C0503,  //  004C  GETMET	R3	R2	K3
      0x5C140200,  //  004D  MOVE	R5	R1
      0x541A0003,  //  004E  LDINT	R6	4
      0x7C0C0600,  //  004F  CALL	R3	3
      0x90022203,  //  0050  SETMBR	R0	K17	R3
      0x540E0003,  //  0051  LDINT	R3	4
      0x00040203,  //  0052  ADD	R1	R1	R3
      0x880C0107,  //  0053  GETMBR	R3	R0	K7
      0x780E0005,  //  0054  JMPF	R3	#005B
      0x8C0C0503,  //  0055  GETMET	R3	R2	K3
      0x5C140200,  //  0056  MOVE	R5	R1
      0x5818000A,  //  0057  LDCONST	R6	K10
      0x7C0C0600,  //  0058  CALL	R3	3
      0x0010070A,  //  0059  ADD	R4	R3	K10
      0x00040204,  //  005A  ADD	R1	R1	R4
      0x90022401,  //  005B  SETMBR	R0	K18	R1
      0x80040000,  //  005C  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: decrypt
********************************************************************/
be_local_closure(Matter_Frame_decrypt,   /* name */
  be_nested_proto(
    23,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(raw),
    /* K3   */  be_nested_str_weak(payload_idx),
    /* K4   */  be_nested_str_weak(get_i2r),
    /* K5   */  be_nested_str_weak(sec_p),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(log),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X20Compute_X20Privacy_X20TODO),
    /* K9   */  be_const_int(2),
    /* K10  */  be_nested_str_weak(get_i2r_privacy),
    /* K11  */  be_const_int(2147483647),
    /* K12  */  be_nested_str_weak(add),
    /* K13  */  be_nested_str_weak(local_session_id),
    /* K14  */  be_const_int(1),
    /* K15  */  be_nested_str_weak(AES_CTR),
    /* K16  */  be_nested_str_weak(decrypt),
    /* K17  */  be_const_int(0),
    /* K18  */  be_const_int(3),
    /* K19  */  be_nested_str_weak(message_handler),
    /* K20  */  be_nested_str_weak(_n_bytes),
    /* K21  */  be_nested_str_weak(clear),
    /* K22  */  be_nested_str_weak(flags),
    /* K23  */  be_nested_str_weak(message_counter),
    /* K24  */  be_nested_str_weak(source_node_id),
    /* K25  */  be_nested_str_weak(peer_node_id),
    /* K26  */  be_nested_str_weak(resize),
    /* K27  */  be_nested_str_weak(AES_CCM),
    /* K28  */  be_nested_str_weak(decrypt1),
    /* K29  */  be_nested_str_weak(MTR_X3A_X20rejected_X20packet_X20due_X20to_X20invalid_X20MIC),
    }),
    be_str_weak(decrypt),
    &be_const_str_solidified,
    ( &(const binstruction[116]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x88100103,  //  0003  GETMBR	R4	R0	K3
      0x5416000F,  //  0004  LDINT	R5	16
      0x8C180504,  //  0005  GETMET	R6	R2	K4
      0x7C180200,  //  0006  CALL	R6	1
      0x881C0105,  //  0007  GETMBR	R7	R0	K5
      0x781E002C,  //  0008  JMPF	R7	#0036
      0xB81E0C00,  //  0009  GETNGBL	R7	K6
      0x8C1C0F07,  //  000A  GETMET	R7	R7	K7
      0x58240008,  //  000B  LDCONST	R9	K8
      0x58280009,  //  000C  LDCONST	R10	K9
      0x7C1C0600,  //  000D  CALL	R7	3
      0x8C1C050A,  //  000E  GETMET	R7	R2	K10
      0x7C1C0200,  //  000F  CALL	R7	1
      0x5421FFEF,  //  0010  LDINT	R8	-16
      0x4020110B,  //  0011  CONNECT	R8	R8	K11
      0x94200608,  //  0012  GETIDX	R8	R3	R8
      0x60240015,  //  0013  GETGBL	R9	G21
      0x7C240000,  //  0014  CALL	R9	0
      0x8C24130C,  //  0015  GETMET	R9	R9	K12
      0x882C010D,  //  0016  GETMBR	R11	R0	K13
      0x5431FFFD,  //  0017  LDINT	R12	-2
      0x7C240600,  //  0018  CALL	R9	3
      0x542A0004,  //  0019  LDINT	R10	5
      0x542E000E,  //  001A  LDINT	R11	15
      0x4028140B,  //  001B  CONNECT	R10	R10	R11
      0x9428100A,  //  001C  GETIDX	R10	R8	R10
      0x0024120A,  //  001D  ADD	R9	R9	R10
      0x542A0003,  //  001E  LDINT	R10	4
      0x882C0103,  //  001F  GETMBR	R11	R0	K3
      0x042C170E,  //  0020  SUB	R11	R11	K14
      0x4028140B,  //  0021  CONNECT	R10	R10	R11
      0x882C0102,  //  0022  GETMBR	R11	R0	K2
      0x9428160A,  //  0023  GETIDX	R10	R11	R10
      0x8C30030F,  //  0024  GETMET	R12	R1	K15
      0x5C380E00,  //  0025  MOVE	R14	R7
      0x7C300400,  //  0026  CALL	R12	2
      0x8C301910,  //  0027  GETMET	R12	R12	K16
      0x5C381400,  //  0028  MOVE	R14	R10
      0x5C3C1200,  //  0029  MOVE	R15	R9
      0x58400009,  //  002A  LDCONST	R16	K9
      0x7C300800,  //  002B  CALL	R12	4
      0x5C2C1800,  //  002C  MOVE	R11	R12
      0x40322312,  //  002D  CONNECT	R12	K17	K18
      0x88340102,  //  002E  GETMBR	R13	R0	K2
      0x94301A0C,  //  002F  GETIDX	R12	R13	R12
      0x0030180B,  //  0030  ADD	R12	R12	R11
      0x88340103,  //  0031  GETMBR	R13	R0	K3
      0x40341B0B,  //  0032  CONNECT	R13	R13	K11
      0x9434140D,  //  0033  GETIDX	R13	R10	R13
      0x0030180D,  //  0034  ADD	R12	R12	R13
      0x9002040C,  //  0035  SETMBR	R0	K2	R12
      0x881C0113,  //  0036  GETMBR	R7	R0	K19
      0x881C0F14,  //  0037  GETMBR	R7	R7	K20
      0x8C200F15,  //  0038  GETMET	R8	R7	K21
      0x7C200200,  //  0039  CALL	R8	1
      0x8C200F0C,  //  003A  GETMET	R8	R7	K12
      0x88280116,  //  003B  GETMBR	R10	R0	K22
      0x582C000E,  //  003C  LDCONST	R11	K14
      0x7C200600,  //  003D  CALL	R8	3
      0x8C200F0C,  //  003E  GETMET	R8	R7	K12
      0x88280117,  //  003F  GETMBR	R10	R0	K23
      0x542E0003,  //  0040  LDINT	R11	4
      0x7C200600,  //  0041  CALL	R8	3
      0x88200118,  //  0042  GETMBR	R8	R0	K24
      0x78220001,  //  0043  JMPF	R8	#0046
      0x40200F0B,  //  0044  CONNECT	R8	R7	K11
      0x70020006,  //  0045  JMP		#004D
      0x88200519,  //  0046  GETMBR	R8	R2	K25
      0x78220001,  //  0047  JMPF	R8	#004A
      0x88200519,  //  0048  GETMBR	R8	R2	K25
      0x40200E08,  //  0049  CONNECT	R8	R7	R8
      0x8C200F1A,  //  004A  GETMET	R8	R7	K26
      0x542A000C,  //  004B  LDINT	R10	13
      0x7C200400,  //  004C  CALL	R8	2
      0x8820031B,  //  004D  GETMBR	R8	R1	K27
      0x8C20111C,  //  004E  GETMET	R8	R8	K28
      0x5C280C00,  //  004F  MOVE	R10	R6
      0x5C2C0E00,  //  0050  MOVE	R11	R7
      0x58300011,  //  0051  LDCONST	R12	K17
      0x6034000C,  //  0052  GETGBL	R13	G12
      0x5C380E00,  //  0053  MOVE	R14	R7
      0x7C340200,  //  0054  CALL	R13	1
      0x5C380600,  //  0055  MOVE	R14	R3
      0x583C0011,  //  0056  LDCONST	R15	K17
      0x5C400800,  //  0057  MOVE	R16	R4
      0x5C440600,  //  0058  MOVE	R17	R3
      0x5C480800,  //  0059  MOVE	R18	R4
      0x604C000C,  //  005A  GETGBL	R19	G12
      0x5C500600,  //  005B  MOVE	R20	R3
      0x7C4C0200,  //  005C  CALL	R19	1
      0x044C2604,  //  005D  SUB	R19	R19	R4
      0x044C2605,  //  005E  SUB	R19	R19	R5
      0x5C500600,  //  005F  MOVE	R20	R3
      0x6054000C,  //  0060  GETGBL	R21	G12
      0x5C580600,  //  0061  MOVE	R22	R3
      0x7C540200,  //  0062  CALL	R21	1
      0x04542A05,  //  0063  SUB	R21	R21	R5
      0x5C580A00,  //  0064  MOVE	R22	R5
      0x7C201C00,  //  0065  CALL	R8	14
      0x78220006,  //  0066  JMPF	R8	#006E
      0x8C24071A,  //  0067  GETMET	R9	R3	K26
      0x602C000C,  //  0068  GETGBL	R11	G12
      0x5C300600,  //  0069  MOVE	R12	R3
      0x7C2C0200,  //  006A  CALL	R11	1
      0x042C1605,  //  006B  SUB	R11	R11	R5
      0x7C240400,  //  006C  CALL	R9	2
      0x70020004,  //  006D  JMP		#0073
      0xB8260C00,  //  006E  GETNGBL	R9	K6
      0x8C241307,  //  006F  GETMET	R9	R9	K7
      0x582C001D,  //  0070  LDCONST	R11	K29
      0x58300012,  //  0071  LDCONST	R12	K18
      0x7C240600,  //  0072  CALL	R9	3
      0x80041000,  //  0073  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Frame
********************************************************************/
be_local_class(Matter_Frame,
    32,
    NULL,
    be_nested_map(42,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(x_flag_i, -1), be_const_var(22) },
        { be_const_key_weak(dest_node_id_8, 7), be_const_var(16) },
        { be_const_key_weak(x_flags, -1), be_const_var(17) },
        { be_const_key_weak(x_flag_a, -1), be_const_var(21) },
        { be_const_key_weak(exchange_id, -1), be_const_var(24) },
        { be_const_key_weak(opcode, -1), be_const_var(23) },
        { be_const_key_weak(encode_frame, -1), be_const_closure(Matter_Frame_encode_frame_closure) },
        { be_const_key_weak(app_payload_idx, -1), be_const_var(29) },
        { be_const_key_weak(payload_idx, -1), be_const_var(3) },
        { be_const_key_weak(ack_message_counter, 24), be_const_var(27) },
        { be_const_key_weak(build_standalone_ack, -1), be_const_closure(Matter_Frame_build_standalone_ack_closure) },
        { be_const_key_weak(x_flag_v, 6), be_const_var(18) },
        { be_const_key_weak(sec_c, -1), be_const_var(10) },
        { be_const_key_weak(vendor_id, 32), be_const_var(26) },
        { be_const_key_weak(local_session_id, -1), be_const_var(7) },
        { be_const_key_weak(flag_s, -1), be_const_var(5) },
        { be_const_key_weak(debug, -1), be_const_closure(Matter_Frame_debug_closure) },
        { be_const_key_weak(message_handler, 10), be_const_var(0) },
        { be_const_key_weak(encrypt, 34), be_const_closure(Matter_Frame_encrypt_closure) },
        { be_const_key_weak(session, -1), be_const_var(1) },
        { be_const_key_weak(sec_flags, -1), be_const_var(8) },
        { be_const_key_weak(build_response, -1), be_const_closure(Matter_Frame_build_response_closure) },
        { be_const_key_weak(initiate_response, -1), be_const_static_closure(Matter_Frame_initiate_response_closure) },
        { be_const_key_weak(remote_port, -1), be_const_var(31) },
        { be_const_key_weak(sec_sesstype, -1), be_const_var(12) },
        { be_const_key_weak(decode_header, 23), be_const_closure(Matter_Frame_decode_header_closure) },
        { be_const_key_weak(flags, -1), be_const_var(4) },
        { be_const_key_weak(protocol_id, 13), be_const_var(25) },
        { be_const_key_weak(raw, -1), be_const_var(2) },
        { be_const_key_weak(source_node_id, 28), be_const_var(14) },
        { be_const_key_weak(flag_dsiz, -1), be_const_var(6) },
        { be_const_key_weak(x_flag_r, -1), be_const_var(20) },
        { be_const_key_weak(message_counter, -1), be_const_var(13) },
        { be_const_key_weak(init, 14), be_const_closure(Matter_Frame_init_closure) },
        { be_const_key_weak(x_flag_sx, 12), be_const_var(19) },
        { be_const_key_weak(dest_node_id_2, -1), be_const_var(15) },
        { be_const_key_weak(decode_payload, -1), be_const_closure(Matter_Frame_decode_payload_closure) },
        { be_const_key_weak(sec_p, 8), be_const_var(9) },
        { be_const_key_weak(decrypt, -1), be_const_closure(Matter_Frame_decrypt_closure) },
        { be_const_key_weak(sec_extensions, -1), be_const_var(28) },
        { be_const_key_weak(sec_mx, 3), be_const_var(11) },
        { be_const_key_weak(remote_ip, 2), be_const_var(30) },
    })),
    be_str_weak(Matter_Frame)
);
/*******************************************************************/

void be_load_Matter_Frame_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Frame);
    be_setglobal(vm, "Matter_Frame");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
