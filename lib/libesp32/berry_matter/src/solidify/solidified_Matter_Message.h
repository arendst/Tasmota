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
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(flags),
    /* K1   */  be_const_int(0),
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
    /* K28  */  be_nested_str_weak(debug),
    /* K29  */  be_nested_str_weak(raw),
    }),
    be_str_weak(encode_frame),
    &be_const_str_solidified,
    ( &(const binstruction[146]) {  /* code */
      0x60080015,  //  0000  GETGBL	R2	G21
      0x7C080000,  //  0001  CALL	R2	0
      0x880C0100,  //  0002  GETMBR	R3	R0	K0
      0x4C100000,  //  0003  LDNIL	R4
      0x1C0C0604,  //  0004  EQ	R3	R3	R4
      0x780E000D,  //  0005  JMPF	R3	#0014
      0x90020101,  //  0006  SETMBR	R0	K0	K1
      0x880C0102,  //  0007  GETMBR	R3	R0	K2
      0x780E0003,  //  0008  JMPF	R3	#000D
      0x880C0100,  //  0009  GETMBR	R3	R0	K0
      0x54120003,  //  000A  LDINT	R4	4
      0x300C0604,  //  000B  OR	R3	R3	R4
      0x90020003,  //  000C  SETMBR	R0	K0	R3
      0x880C0103,  //  000D  GETMBR	R3	R0	K3
      0x780E0004,  //  000E  JMPF	R3	#0014
      0x880C0100,  //  000F  GETMBR	R3	R0	K0
      0x88100103,  //  0010  GETMBR	R4	R0	K3
      0x2C100904,  //  0011  AND	R4	R4	K4
      0x300C0604,  //  0012  OR	R3	R3	R4
      0x90020003,  //  0013  SETMBR	R0	K0	R3
      0x8C0C0505,  //  0014  GETMET	R3	R2	K5
      0x88140100,  //  0015  GETMBR	R5	R0	K0
      0x58180006,  //  0016  LDCONST	R6	K6
      0x7C0C0600,  //  0017  CALL	R3	3
      0x8C0C0505,  //  0018  GETMET	R3	R2	K5
      0x88140107,  //  0019  GETMBR	R5	R0	K7
      0x78160001,  //  001A  JMPF	R5	#001D
      0x88140107,  //  001B  GETMBR	R5	R0	K7
      0x70020000,  //  001C  JMP		#001E
      0x58140001,  //  001D  LDCONST	R5	K1
      0x58180008,  //  001E  LDCONST	R6	K8
      0x7C0C0600,  //  001F  CALL	R3	3
      0x880C0109,  //  0020  GETMBR	R3	R0	K9
      0x4C100000,  //  0021  LDNIL	R4
      0x1C0C0604,  //  0022  EQ	R3	R3	R4
      0x780E0013,  //  0023  JMPF	R3	#0038
      0x90021301,  //  0024  SETMBR	R0	K9	K1
      0x880C010A,  //  0025  GETMBR	R3	R0	K10
      0x780E0003,  //  0026  JMPF	R3	#002B
      0x880C0109,  //  0027  GETMBR	R3	R0	K9
      0x5412007F,  //  0028  LDINT	R4	128
      0x300C0604,  //  0029  OR	R3	R3	R4
      0x90021203,  //  002A  SETMBR	R0	K9	R3
      0x880C010B,  //  002B  GETMBR	R3	R0	K11
      0x780E0003,  //  002C  JMPF	R3	#0031
      0x880C0109,  //  002D  GETMBR	R3	R0	K9
      0x5412003F,  //  002E  LDINT	R4	64
      0x300C0604,  //  002F  OR	R3	R3	R4
      0x90021203,  //  0030  SETMBR	R0	K9	R3
      0x880C010C,  //  0031  GETMBR	R3	R0	K12
      0x780E0004,  //  0032  JMPF	R3	#0038
      0x880C0109,  //  0033  GETMBR	R3	R0	K9
      0x8810010C,  //  0034  GETMBR	R4	R0	K12
      0x2C100904,  //  0035  AND	R4	R4	K4
      0x300C0604,  //  0036  OR	R3	R3	R4
      0x90021203,  //  0037  SETMBR	R0	K9	R3
      0x8C0C0505,  //  0038  GETMET	R3	R2	K5
      0x88140109,  //  0039  GETMBR	R5	R0	K9
      0x58180006,  //  003A  LDCONST	R6	K6
      0x7C0C0600,  //  003B  CALL	R3	3
      0x8C0C0505,  //  003C  GETMET	R3	R2	K5
      0x8814010D,  //  003D  GETMBR	R5	R0	K13
      0x541A0003,  //  003E  LDINT	R6	4
      0x7C0C0600,  //  003F  CALL	R3	3
      0x880C0102,  //  0040  GETMBR	R3	R0	K2
      0x780E0001,  //  0041  JMPF	R3	#0044
      0x880C010E,  //  0042  GETMBR	R3	R0	K14
      0x400C0403,  //  0043  CONNECT	R3	R2	R3
      0x880C0103,  //  0044  GETMBR	R3	R0	K3
      0x1C0C0706,  //  0045  EQ	R3	R3	K6
      0x780E0001,  //  0046  JMPF	R3	#0049
      0x880C010F,  //  0047  GETMBR	R3	R0	K15
      0x400C0403,  //  0048  CONNECT	R3	R2	R3
      0x880C0103,  //  0049  GETMBR	R3	R0	K3
      0x1C0C0708,  //  004A  EQ	R3	R3	K8
      0x780E0003,  //  004B  JMPF	R3	#0050
      0x8C0C0505,  //  004C  GETMET	R3	R2	K5
      0x88140110,  //  004D  GETMBR	R5	R0	K16
      0x58180008,  //  004E  LDCONST	R6	K8
      0x7C0C0600,  //  004F  CALL	R3	3
      0x600C000C,  //  0050  GETGBL	R3	G12
      0x5C100400,  //  0051  MOVE	R4	R2
      0x7C0C0200,  //  0052  CALL	R3	1
      0x90022203,  //  0053  SETMBR	R0	K17	R3
      0x880C0112,  //  0054  GETMBR	R3	R0	K18
      0x4C100000,  //  0055  LDNIL	R4
      0x1C0C0604,  //  0056  EQ	R3	R3	R4
      0x780E0016,  //  0057  JMPF	R3	#006F
      0x90022501,  //  0058  SETMBR	R0	K18	K1
      0x880C0113,  //  0059  GETMBR	R3	R0	K19
      0x780E0003,  //  005A  JMPF	R3	#005F
      0x880C0112,  //  005B  GETMBR	R3	R0	K18
      0x5412000F,  //  005C  LDINT	R4	16
      0x300C0604,  //  005D  OR	R3	R3	R4
      0x90022403,  //  005E  SETMBR	R0	K18	R3
      0x880C0114,  //  005F  GETMBR	R3	R0	K20
      0x780E0003,  //  0060  JMPF	R3	#0065
      0x880C0112,  //  0061  GETMBR	R3	R0	K18
      0x54120003,  //  0062  LDINT	R4	4
      0x300C0604,  //  0063  OR	R3	R3	R4
      0x90022403,  //  0064  SETMBR	R0	K18	R3
      0x880C0115,  //  0065  GETMBR	R3	R0	K21
      0x780E0002,  //  0066  JMPF	R3	#006A
      0x880C0112,  //  0067  GETMBR	R3	R0	K18
      0x300C0708,  //  0068  OR	R3	R3	K8
      0x90022403,  //  0069  SETMBR	R0	K18	R3
      0x880C0116,  //  006A  GETMBR	R3	R0	K22
      0x780E0002,  //  006B  JMPF	R3	#006F
      0x880C0112,  //  006C  GETMBR	R3	R0	K18
      0x300C0706,  //  006D  OR	R3	R3	K6
      0x90022403,  //  006E  SETMBR	R0	K18	R3
      0x8C0C0505,  //  006F  GETMET	R3	R2	K5
      0x88140112,  //  0070  GETMBR	R5	R0	K18
      0x58180006,  //  0071  LDCONST	R6	K6
      0x7C0C0600,  //  0072  CALL	R3	3
      0x8C0C0505,  //  0073  GETMET	R3	R2	K5
      0x88140117,  //  0074  GETMBR	R5	R0	K23
      0x58180006,  //  0075  LDCONST	R6	K6
      0x7C0C0600,  //  0076  CALL	R3	3
      0x8C0C0505,  //  0077  GETMET	R3	R2	K5
      0x88140118,  //  0078  GETMBR	R5	R0	K24
      0x541AFFFE,  //  0079  LDINT	R6	65535
      0x2C140A06,  //  007A  AND	R5	R5	R6
      0x58180008,  //  007B  LDCONST	R6	K8
      0x7C0C0600,  //  007C  CALL	R3	3
      0x8C0C0505,  //  007D  GETMET	R3	R2	K5
      0x88140119,  //  007E  GETMBR	R5	R0	K25
      0x58180008,  //  007F  LDCONST	R6	K8
      0x7C0C0600,  //  0080  CALL	R3	3
      0x880C0115,  //  0081  GETMBR	R3	R0	K21
      0x780E0003,  //  0082  JMPF	R3	#0087
      0x8C0C0505,  //  0083  GETMET	R3	R2	K5
      0x8814011A,  //  0084  GETMBR	R5	R0	K26
      0x541A0003,  //  0085  LDINT	R6	4
      0x7C0C0600,  //  0086  CALL	R3	3
      0x600C000C,  //  0087  GETGBL	R3	G12
      0x5C100400,  //  0088  MOVE	R4	R2
      0x7C0C0200,  //  0089  CALL	R3	1
      0x90023603,  //  008A  SETMBR	R0	K27	R3
      0x78060000,  //  008B  JMPF	R1	#008D
      0x400C0401,  //  008C  CONNECT	R3	R2	R1
      0x8C0C011C,  //  008D  GETMET	R3	R0	K28
      0x5C140400,  //  008E  MOVE	R5	R2
      0x7C0C0400,  //  008F  CALL	R3	2
      0x90023A02,  //  0090  SETMBR	R0	K29	R2
      0x80040400,  //  0091  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: encrypt
********************************************************************/
be_local_closure(Matter_Frame_encrypt,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[28]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(raw),
    /* K2   */  be_nested_str_weak(session),
    /* K3   */  be_nested_str_weak(get_r2i),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(payload_idx),
    /* K6   */  be_const_int(1),
    /* K7   */  be_const_int(2147483647),
    /* K8   */  be_nested_str_weak(add),
    /* K9   */  be_nested_str_weak(flags),
    /* K10  */  be_nested_str_weak(message_counter),
    /* K11  */  be_nested_str_weak(is_CASE),
    /* K12  */  be_nested_str_weak(get_device_id),
    /* K13  */  be_nested_str_weak(resize),
    /* K14  */  be_nested_str_weak(tasmota),
    /* K15  */  be_nested_str_weak(log),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20cleartext_X3A_X20),
    /* K17  */  be_nested_str_weak(tohex),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K19  */  be_nested_str_weak(MTR_X3A_X20r2i_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20p_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20a_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20n_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K23  */  be_nested_str_weak(AES_CCM),
    /* K24  */  be_nested_str_weak(encrypt),
    /* K25  */  be_nested_str_weak(tag),
    /* K26  */  be_nested_str_weak(MTR_X3A_X20ciphertext_X20_X20_X3D),
    /* K27  */  be_nested_str_weak(MTR_X3A_X20tag_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    }),
    be_str_weak(encrypt),
    &be_const_str_solidified,
    ( &(const binstruction[122]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x8C100703,  //  0003  GETMET	R4	R3	K3
      0x7C100200,  //  0004  CALL	R4	1
      0x88140105,  //  0005  GETMBR	R5	R0	K5
      0x04140B06,  //  0006  SUB	R5	R5	K6
      0x40160805,  //  0007  CONNECT	R5	K4	R5
      0x94140405,  //  0008  GETIDX	R5	R2	R5
      0x88180105,  //  0009  GETMBR	R6	R0	K5
      0x40180D07,  //  000A  CONNECT	R6	R6	K7
      0x94180406,  //  000B  GETIDX	R6	R2	R6
      0x601C0015,  //  000C  GETGBL	R7	G21
      0x7C1C0000,  //  000D  CALL	R7	0
      0x8C200F08,  //  000E  GETMET	R8	R7	K8
      0x88280109,  //  000F  GETMBR	R10	R0	K9
      0x582C0006,  //  0010  LDCONST	R11	K6
      0x7C200600,  //  0011  CALL	R8	3
      0x8C200F08,  //  0012  GETMET	R8	R7	K8
      0x8828010A,  //  0013  GETMBR	R10	R0	K10
      0x542E0003,  //  0014  LDINT	R11	4
      0x7C200600,  //  0015  CALL	R8	3
      0x8C20070B,  //  0016  GETMET	R8	R3	K11
      0x7C200200,  //  0017  CALL	R8	1
      0x78220005,  //  0018  JMPF	R8	#001F
      0x8C20070C,  //  0019  GETMET	R8	R3	K12
      0x7C200200,  //  001A  CALL	R8	1
      0x78220002,  //  001B  JMPF	R8	#001F
      0x8C20070C,  //  001C  GETMET	R8	R3	K12
      0x7C200200,  //  001D  CALL	R8	1
      0x40200E08,  //  001E  CONNECT	R8	R7	R8
      0x8C200F0D,  //  001F  GETMET	R8	R7	K13
      0x542A000C,  //  0020  LDINT	R10	13
      0x7C200400,  //  0021  CALL	R8	2
      0xB8221C00,  //  0022  GETNGBL	R8	K14
      0x8C20110F,  //  0023  GETMET	R8	R8	K15
      0x88280101,  //  0024  GETMBR	R10	R0	K1
      0x8C281511,  //  0025  GETMET	R10	R10	K17
      0x7C280200,  //  0026  CALL	R10	1
      0x002A200A,  //  0027  ADD	R10	K16	R10
      0x542E0003,  //  0028  LDINT	R11	4
      0x7C200600,  //  0029  CALL	R8	3
      0xB8221C00,  //  002A  GETNGBL	R8	K14
      0x8C20110F,  //  002B  GETMET	R8	R8	K15
      0x58280012,  //  002C  LDCONST	R10	K18
      0x542E0003,  //  002D  LDINT	R11	4
      0x7C200600,  //  002E  CALL	R8	3
      0xB8221C00,  //  002F  GETNGBL	R8	K14
      0x8C20110F,  //  0030  GETMET	R8	R8	K15
      0x8C280911,  //  0031  GETMET	R10	R4	K17
      0x7C280200,  //  0032  CALL	R10	1
      0x002A260A,  //  0033  ADD	R10	K19	R10
      0x542E0003,  //  0034  LDINT	R11	4
      0x7C200600,  //  0035  CALL	R8	3
      0xB8221C00,  //  0036  GETNGBL	R8	K14
      0x8C20110F,  //  0037  GETMET	R8	R8	K15
      0x8C280D11,  //  0038  GETMET	R10	R6	K17
      0x7C280200,  //  0039  CALL	R10	1
      0x002A280A,  //  003A  ADD	R10	K20	R10
      0x542E0003,  //  003B  LDINT	R11	4
      0x7C200600,  //  003C  CALL	R8	3
      0xB8221C00,  //  003D  GETNGBL	R8	K14
      0x8C20110F,  //  003E  GETMET	R8	R8	K15
      0x8C280B11,  //  003F  GETMET	R10	R5	K17
      0x7C280200,  //  0040  CALL	R10	1
      0x002A2A0A,  //  0041  ADD	R10	K21	R10
      0x542E0003,  //  0042  LDINT	R11	4
      0x7C200600,  //  0043  CALL	R8	3
      0xB8221C00,  //  0044  GETNGBL	R8	K14
      0x8C20110F,  //  0045  GETMET	R8	R8	K15
      0x8C280F11,  //  0046  GETMET	R10	R7	K17
      0x7C280200,  //  0047  CALL	R10	1
      0x002A2C0A,  //  0048  ADD	R10	K22	R10
      0x542E0003,  //  0049  LDINT	R11	4
      0x7C200600,  //  004A  CALL	R8	3
      0x8C200317,  //  004B  GETMET	R8	R1	K23
      0x5C280800,  //  004C  MOVE	R10	R4
      0x5C2C0E00,  //  004D  MOVE	R11	R7
      0x5C300A00,  //  004E  MOVE	R12	R5
      0x6034000C,  //  004F  GETGBL	R13	G12
      0x5C380C00,  //  0050  MOVE	R14	R6
      0x7C340200,  //  0051  CALL	R13	1
      0x543A000F,  //  0052  LDINT	R14	16
      0x7C200C00,  //  0053  CALL	R8	6
      0x8C241118,  //  0054  GETMET	R9	R8	K24
      0x5C2C0C00,  //  0055  MOVE	R11	R6
      0x7C240400,  //  0056  CALL	R9	2
      0x8C281119,  //  0057  GETMET	R10	R8	K25
      0x7C280200,  //  0058  CALL	R10	1
      0xB82E1C00,  //  0059  GETNGBL	R11	K14
      0x8C2C170F,  //  005A  GETMET	R11	R11	K15
      0x58340012,  //  005B  LDCONST	R13	K18
      0x543A0003,  //  005C  LDINT	R14	4
      0x7C2C0600,  //  005D  CALL	R11	3
      0xB82E1C00,  //  005E  GETNGBL	R11	K14
      0x8C2C170F,  //  005F  GETMET	R11	R11	K15
      0x8C341311,  //  0060  GETMET	R13	R9	K17
      0x7C340200,  //  0061  CALL	R13	1
      0x0036340D,  //  0062  ADD	R13	K26	R13
      0x543A0003,  //  0063  LDINT	R14	4
      0x7C2C0600,  //  0064  CALL	R11	3
      0xB82E1C00,  //  0065  GETNGBL	R11	K14
      0x8C2C170F,  //  0066  GETMET	R11	R11	K15
      0x8C341511,  //  0067  GETMET	R13	R10	K17
      0x7C340200,  //  0068  CALL	R13	1
      0x0036360D,  //  0069  ADD	R13	K27	R13
      0x543A0003,  //  006A  LDINT	R14	4
      0x7C2C0600,  //  006B  CALL	R11	3
      0xB82E1C00,  //  006C  GETNGBL	R11	K14
      0x8C2C170F,  //  006D  GETMET	R11	R11	K15
      0x58340012,  //  006E  LDCONST	R13	K18
      0x543A0003,  //  006F  LDINT	R14	4
      0x7C2C0600,  //  0070  CALL	R11	3
      0x882C0101,  //  0071  GETMBR	R11	R0	K1
      0x8C2C170D,  //  0072  GETMET	R11	R11	K13
      0x88340105,  //  0073  GETMBR	R13	R0	K5
      0x7C2C0400,  //  0074  CALL	R11	2
      0x882C0101,  //  0075  GETMBR	R11	R0	K1
      0x402C1609,  //  0076  CONNECT	R11	R11	R9
      0x882C0101,  //  0077  GETMBR	R11	R0	K1
      0x402C160A,  //  0078  CONNECT	R11	R11	R10
      0x80000000,  //  0079  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: debug
********************************************************************/
be_local_closure(Matter_Frame_debug,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(Frame),
    /* K2   */  be_nested_str_weak(message_handler),
    /* K3   */  be_nested_str_weak(decode_header),
    /* K4   */  be_nested_str_weak(decode_payload),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20sending_X20decode_X3A_X20),
    /* K8   */  be_nested_str_weak(inspect),
    /* K9   */  be_const_int(3),
    }),
    be_str_weak(debug),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x88100102,  //  0002  GETMBR	R4	R0	K2
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C080600,  //  0004  CALL	R2	3
      0x8C0C0503,  //  0005  GETMET	R3	R2	K3
      0x7C0C0200,  //  0006  CALL	R3	1
      0x8C0C0504,  //  0007  GETMET	R3	R2	K4
      0x7C0C0200,  //  0008  CALL	R3	1
      0xB80E0A00,  //  0009  GETNGBL	R3	K5
      0x8C0C0706,  //  000A  GETMET	R3	R3	K6
      0xB8160000,  //  000B  GETNGBL	R5	K0
      0x8C140B08,  //  000C  GETMET	R5	R5	K8
      0x5C1C0400,  //  000D  MOVE	R7	R2
      0x7C140400,  //  000E  CALL	R5	2
      0x00160E05,  //  000F  ADD	R5	K7	R5
      0x58180009,  //  0010  LDCONST	R6	K9
      0x7C0C0600,  //  0011  CALL	R3	3
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: build_standalone_ack
********************************************************************/
be_local_closure(Matter_Frame_build_standalone_ack,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(message_handler),
    /* K2   */  be_nested_str_weak(remote_ip),
    /* K3   */  be_nested_str_weak(remote_port),
    /* K4   */  be_nested_str_weak(flag_s),
    /* K5   */  be_nested_str_weak(flag_dsiz),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(dest_node_id_8),
    /* K8   */  be_nested_str_weak(source_node_id),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(session),
    /* K11  */  be_nested_str_weak(message_counter),
    /* K12  */  be_nested_str_weak(counter_snd),
    /* K13  */  be_nested_str_weak(next),
    /* K14  */  be_nested_str_weak(local_session_id),
    /* K15  */  be_nested_str_weak(initiator_session_id),
    /* K16  */  be_nested_str_weak(x_flag_i),
    /* K17  */  be_nested_str_weak(opcode),
    /* K18  */  be_nested_str_weak(exchange_id),
    /* K19  */  be_nested_str_weak(protocol_id),
    /* K20  */  be_nested_str_weak(x_flag_a),
    /* K21  */  be_nested_str_weak(ack_message_counter),
    /* K22  */  be_nested_str_weak(x_flag_r),
    /* K23  */  be_nested_str_weak(tasmota),
    /* K24  */  be_nested_str_weak(log),
    /* K25  */  be_nested_str_weak(format),
    /* K26  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X25s),
    /* K27  */  be_nested_str_weak(matter),
    /* K28  */  be_nested_str_weak(get_opcode_name),
    /* K29  */  be_const_int(3),
    }),
    be_str_weak(build_standalone_ack),
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x60080006,  //  0001  GETGBL	R2	G6
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C080200,  //  0003  CALL	R2	1
      0x880C0101,  //  0004  GETMBR	R3	R0	K1
      0x7C080200,  //  0005  CALL	R2	1
      0x880C0102,  //  0006  GETMBR	R3	R0	K2
      0x900A0403,  //  0007  SETMBR	R2	K2	R3
      0x880C0103,  //  0008  GETMBR	R3	R0	K3
      0x900A0603,  //  0009  SETMBR	R2	K3	R3
      0x880C0104,  //  000A  GETMBR	R3	R0	K4
      0x780E0003,  //  000B  JMPF	R3	#0010
      0x900A0B06,  //  000C  SETMBR	R2	K5	K6
      0x880C0108,  //  000D  GETMBR	R3	R0	K8
      0x900A0E03,  //  000E  SETMBR	R2	K7	R3
      0x70020000,  //  000F  JMP		#0011
      0x900A0B09,  //  0010  SETMBR	R2	K5	K9
      0x880C010A,  //  0011  GETMBR	R3	R0	K10
      0x900A1403,  //  0012  SETMBR	R2	K10	R3
      0x880C010A,  //  0013  GETMBR	R3	R0	K10
      0x880C070C,  //  0014  GETMBR	R3	R3	K12
      0x8C0C070D,  //  0015  GETMET	R3	R3	K13
      0x7C0C0200,  //  0016  CALL	R3	1
      0x900A1603,  //  0017  SETMBR	R2	K11	R3
      0x880C010A,  //  0018  GETMBR	R3	R0	K10
      0x880C070F,  //  0019  GETMBR	R3	R3	K15
      0x900A1C03,  //  001A  SETMBR	R2	K14	R3
      0x880C0110,  //  001B  GETMBR	R3	R0	K16
      0x780E0001,  //  001C  JMPF	R3	#001F
      0x580C0009,  //  001D  LDCONST	R3	K9
      0x70020000,  //  001E  JMP		#0020
      0x580C0006,  //  001F  LDCONST	R3	K6
      0x900A2003,  //  0020  SETMBR	R2	K16	R3
      0x540E000F,  //  0021  LDINT	R3	16
      0x900A2203,  //  0022  SETMBR	R2	K17	R3
      0x880C0112,  //  0023  GETMBR	R3	R0	K18
      0x900A2403,  //  0024  SETMBR	R2	K18	R3
      0x900A2709,  //  0025  SETMBR	R2	K19	K9
      0x900A2906,  //  0026  SETMBR	R2	K20	K6
      0x880C010B,  //  0027  GETMBR	R3	R0	K11
      0x900A2A03,  //  0028  SETMBR	R2	K21	R3
      0x900A2D06,  //  0029  SETMBR	R2	K22	K6
      0xB80E2E00,  //  002A  GETNGBL	R3	K23
      0x8C0C0718,  //  002B  GETMET	R3	R3	K24
      0x8C140319,  //  002C  GETMET	R5	R1	K25
      0x581C001A,  //  002D  LDCONST	R7	K26
      0xB8223600,  //  002E  GETNGBL	R8	K27
      0x8C20111C,  //  002F  GETMET	R8	R8	K28
      0x88280511,  //  0030  GETMBR	R10	R2	K17
      0x7C200400,  //  0031  CALL	R8	2
      0x7C140600,  //  0032  CALL	R5	3
      0x5818001D,  //  0033  LDCONST	R6	K29
      0x7C0C0600,  //  0034  CALL	R3	3
      0x80040400,  //  0035  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: build_response
********************************************************************/
be_local_closure(Matter_Frame_build_response,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[32]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(message_handler),
    /* K2   */  be_nested_str_weak(remote_ip),
    /* K3   */  be_nested_str_weak(remote_port),
    /* K4   */  be_nested_str_weak(flag_s),
    /* K5   */  be_nested_str_weak(flag_dsiz),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(dest_node_id_8),
    /* K8   */  be_nested_str_weak(source_node_id),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(session),
    /* K11  */  be_nested_str_weak(local_session_id),
    /* K12  */  be_nested_str_weak(initiator_session_id),
    /* K13  */  be_nested_str_weak(message_counter),
    /* K14  */  be_nested_str_weak(counter_snd),
    /* K15  */  be_nested_str_weak(next),
    /* K16  */  be_nested_str_weak(_counter_insecure_snd),
    /* K17  */  be_nested_str_weak(x_flag_i),
    /* K18  */  be_nested_str_weak(opcode),
    /* K19  */  be_nested_str_weak(exchange_id),
    /* K20  */  be_nested_str_weak(protocol_id),
    /* K21  */  be_nested_str_weak(x_flag_r),
    /* K22  */  be_nested_str_weak(x_flag_a),
    /* K23  */  be_nested_str_weak(ack_message_counter),
    /* K24  */  be_nested_str_weak(matter),
    /* K25  */  be_nested_str_weak(get_opcode_name),
    /* K26  */  be_nested_str_weak(format),
    /* K27  */  be_nested_str_weak(0x_X2502X),
    /* K28  */  be_nested_str_weak(tasmota),
    /* K29  */  be_nested_str_weak(log),
    /* K30  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X25s),
    /* K31  */  be_const_int(2),
    }),
    be_str_weak(build_response),
    &be_const_str_solidified,
    ( &(const binstruction[91]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x4C140000,  //  0001  LDNIL	R5
      0x1C140605,  //  0002  EQ	R5	R3	R5
      0x78160005,  //  0003  JMPF	R5	#000A
      0x60140006,  //  0004  GETGBL	R5	G6
      0x5C180000,  //  0005  MOVE	R6	R0
      0x7C140200,  //  0006  CALL	R5	1
      0x88180101,  //  0007  GETMBR	R6	R0	K1
      0x7C140200,  //  0008  CALL	R5	1
      0x5C0C0A00,  //  0009  MOVE	R3	R5
      0x88140102,  //  000A  GETMBR	R5	R0	K2
      0x900E0405,  //  000B  SETMBR	R3	K2	R5
      0x88140103,  //  000C  GETMBR	R5	R0	K3
      0x900E0605,  //  000D  SETMBR	R3	K3	R5
      0x88140104,  //  000E  GETMBR	R5	R0	K4
      0x78160003,  //  000F  JMPF	R5	#0014
      0x900E0B06,  //  0010  SETMBR	R3	K5	K6
      0x88140108,  //  0011  GETMBR	R5	R0	K8
      0x900E0E05,  //  0012  SETMBR	R3	K7	R5
      0x70020000,  //  0013  JMP		#0015
      0x900E0B09,  //  0014  SETMBR	R3	K5	K9
      0x8814010A,  //  0015  GETMBR	R5	R0	K10
      0x900E1405,  //  0016  SETMBR	R3	K10	R5
      0x8814010B,  //  0017  GETMBR	R5	R0	K11
      0x20140B09,  //  0018  NE	R5	R5	K9
      0x7816000E,  //  0019  JMPF	R5	#0029
      0x8814010A,  //  001A  GETMBR	R5	R0	K10
      0x7816000C,  //  001B  JMPF	R5	#0029
      0x8814010A,  //  001C  GETMBR	R5	R0	K10
      0x88140B0C,  //  001D  GETMBR	R5	R5	K12
      0x20140B09,  //  001E  NE	R5	R5	K9
      0x78160008,  //  001F  JMPF	R5	#0029
      0x8814010A,  //  0020  GETMBR	R5	R0	K10
      0x88140B0E,  //  0021  GETMBR	R5	R5	K14
      0x8C140B0F,  //  0022  GETMET	R5	R5	K15
      0x7C140200,  //  0023  CALL	R5	1
      0x900E1A05,  //  0024  SETMBR	R3	K13	R5
      0x8814010A,  //  0025  GETMBR	R5	R0	K10
      0x88140B0C,  //  0026  GETMBR	R5	R5	K12
      0x900E1605,  //  0027  SETMBR	R3	K11	R5
      0x70020005,  //  0028  JMP		#002F
      0x8814010A,  //  0029  GETMBR	R5	R0	K10
      0x88140B10,  //  002A  GETMBR	R5	R5	K16
      0x8C140B0F,  //  002B  GETMET	R5	R5	K15
      0x7C140200,  //  002C  CALL	R5	1
      0x900E1A05,  //  002D  SETMBR	R3	K13	R5
      0x900E1709,  //  002E  SETMBR	R3	K11	K9
      0x88140111,  //  002F  GETMBR	R5	R0	K17
      0x78160001,  //  0030  JMPF	R5	#0033
      0x58140009,  //  0031  LDCONST	R5	K9
      0x70020000,  //  0032  JMP		#0034
      0x58140006,  //  0033  LDCONST	R5	K6
      0x900E2205,  //  0034  SETMBR	R3	K17	R5
      0x900E2401,  //  0035  SETMBR	R3	K18	R1
      0x88140113,  //  0036  GETMBR	R5	R0	K19
      0x900E2605,  //  0037  SETMBR	R3	K19	R5
      0x88140114,  //  0038  GETMBR	R5	R0	K20
      0x900E2805,  //  0039  SETMBR	R3	K20	R5
      0x88140115,  //  003A  GETMBR	R5	R0	K21
      0x78160002,  //  003B  JMPF	R5	#003F
      0x900E2D06,  //  003C  SETMBR	R3	K22	K6
      0x8814010D,  //  003D  GETMBR	R5	R0	K13
      0x900E2E05,  //  003E  SETMBR	R3	K23	R5
      0x780A0001,  //  003F  JMPF	R2	#0042
      0x58140006,  //  0040  LDCONST	R5	K6
      0x70020000,  //  0041  JMP		#0043
      0x58140009,  //  0042  LDCONST	R5	K9
      0x900E2A05,  //  0043  SETMBR	R3	K21	R5
      0x8814070B,  //  0044  GETMBR	R5	R3	K11
      0x1C140B09,  //  0045  EQ	R5	R5	K9
      0x78160012,  //  0046  JMPF	R5	#005A
      0xB8163000,  //  0047  GETNGBL	R5	K24
      0x8C140B19,  //  0048  GETMET	R5	R5	K25
      0x881C0712,  //  0049  GETMBR	R7	R3	K18
      0x7C140400,  //  004A  CALL	R5	2
      0x5C180A00,  //  004B  MOVE	R6	R5
      0x741A0004,  //  004C  JMPT	R6	#0052
      0x8C18091A,  //  004D  GETMET	R6	R4	K26
      0x5820001B,  //  004E  LDCONST	R8	K27
      0x88240712,  //  004F  GETMBR	R9	R3	K18
      0x7C180600,  //  0050  CALL	R6	3
      0x5C140C00,  //  0051  MOVE	R5	R6
      0xB81A3800,  //  0052  GETNGBL	R6	K28
      0x8C180D1D,  //  0053  GETMET	R6	R6	K29
      0x8C20091A,  //  0054  GETMET	R8	R4	K26
      0x5828001E,  //  0055  LDCONST	R10	K30
      0x5C2C0A00,  //  0056  MOVE	R11	R5
      0x7C200600,  //  0057  CALL	R8	3
      0x5824001F,  //  0058  LDCONST	R9	K31
      0x7C180600,  //  0059  CALL	R6	3
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
    10,                          /* nstack */
    5,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Frame),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(matter),
    /* K3   */  be_nested_str_weak(Frame),
    /* K4   */  be_nested_str_weak(remote_ip),
    /* K5   */  be_nested_str_weak(_ip),
    /* K6   */  be_nested_str_weak(remote_port),
    /* K7   */  be_nested_str_weak(_port),
    /* K8   */  be_nested_str_weak(flag_dsiz),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(session),
    /* K11  */  be_nested_str_weak(initiator_session_id),
    /* K12  */  be_nested_str_weak(message_counter),
    /* K13  */  be_nested_str_weak(counter_snd),
    /* K14  */  be_nested_str_weak(next),
    /* K15  */  be_nested_str_weak(local_session_id),
    /* K16  */  be_nested_str_weak(_counter_insecure_snd),
    /* K17  */  be_nested_str_weak(x_flag_i),
    /* K18  */  be_const_int(1),
    /* K19  */  be_nested_str_weak(opcode),
    /* K20  */  be_nested_str_weak(_exchange_id),
    /* K21  */  be_nested_str_weak(exchange_id),
    /* K22  */  be_nested_str_weak(protocol_id),
    /* K23  */  be_nested_str_weak(x_flag_r),
    }),
    be_str_weak(initiate_response),
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0x58140000,  //  0000  LDCONST	R5	K0
      0xA41A0200,  //  0001  IMPORT	R6	K1
      0x4C1C0000,  //  0002  LDNIL	R7
      0x1C1C0807,  //  0003  EQ	R7	R4	R7
      0x781E0004,  //  0004  JMPF	R7	#000A
      0xB81E0400,  //  0005  GETNGBL	R7	K2
      0x8C1C0F03,  //  0006  GETMET	R7	R7	K3
      0x5C240000,  //  0007  MOVE	R9	R0
      0x7C1C0400,  //  0008  CALL	R7	2
      0x5C100E00,  //  0009  MOVE	R4	R7
      0x881C0305,  //  000A  GETMBR	R7	R1	K5
      0x90120807,  //  000B  SETMBR	R4	K4	R7
      0x881C0307,  //  000C  GETMBR	R7	R1	K7
      0x90120C07,  //  000D  SETMBR	R4	K6	R7
      0x90121109,  //  000E  SETMBR	R4	K8	K9
      0x90121401,  //  000F  SETMBR	R4	K10	R1
      0x78060009,  //  0010  JMPF	R1	#001B
      0x881C030B,  //  0011  GETMBR	R7	R1	K11
      0x201C0F09,  //  0012  NE	R7	R7	K9
      0x781E0006,  //  0013  JMPF	R7	#001B
      0x881C030D,  //  0014  GETMBR	R7	R1	K13
      0x8C1C0F0E,  //  0015  GETMET	R7	R7	K14
      0x7C1C0200,  //  0016  CALL	R7	1
      0x90121807,  //  0017  SETMBR	R4	K12	R7
      0x881C030B,  //  0018  GETMBR	R7	R1	K11
      0x90121E07,  //  0019  SETMBR	R4	K15	R7
      0x70020004,  //  001A  JMP		#0020
      0x881C0310,  //  001B  GETMBR	R7	R1	K16
      0x8C1C0F0E,  //  001C  GETMET	R7	R7	K14
      0x7C1C0200,  //  001D  CALL	R7	1
      0x90121807,  //  001E  SETMBR	R4	K12	R7
      0x90121F09,  //  001F  SETMBR	R4	K15	K9
      0x90122312,  //  0020  SETMBR	R4	K17	K18
      0x90122602,  //  0021  SETMBR	R4	K19	R2
      0x881C0314,  //  0022  GETMBR	R7	R1	K20
      0x001C0F12,  //  0023  ADD	R7	R7	K18
      0x90062807,  //  0024  SETMBR	R1	K20	R7
      0x881C0314,  //  0025  GETMBR	R7	R1	K20
      0x5422FFFF,  //  0026  LDINT	R8	65536
      0x301C0E08,  //  0027  OR	R7	R7	R8
      0x90122A07,  //  0028  SETMBR	R4	K21	R7
      0x90122D12,  //  0029  SETMBR	R4	K22	K18
      0x780E0001,  //  002A  JMPF	R3	#002D
      0x581C0012,  //  002B  LDCONST	R7	K18
      0x70020000,  //  002C  JMP		#002E
      0x581C0009,  //  002D  LDCONST	R7	K9
      0x90122E07,  //  002E  SETMBR	R4	K23	R7
      0x80040800,  //  002F  RET	1	R4
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
    16,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[36]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(session),
    /* K2   */  be_nested_str_weak(raw),
    /* K3   */  be_const_int(2147483647),
    /* K4   */  be_nested_str_weak(get_i2r),
    /* K5   */  be_nested_str_weak(sec_p),
    /* K6   */  be_nested_str_weak(get_i2r_privacy),
    /* K7   */  be_nested_str_weak(add),
    /* K8   */  be_nested_str_weak(local_session_id),
    /* K9   */  be_nested_str_weak(payload_idx),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_str_weak(AES_CTR),
    /* K12  */  be_nested_str_weak(decrypt),
    /* K13  */  be_const_int(2),
    /* K14  */  be_const_int(0),
    /* K15  */  be_const_int(3),
    /* K16  */  be_nested_str_weak(self),
    /* K17  */  be_nested_str_weak(flags),
    /* K18  */  be_nested_str_weak(message_counter),
    /* K19  */  be_nested_str_weak(source_node_id),
    /* K20  */  be_nested_str_weak(peer_node_id),
    /* K21  */  be_nested_str_weak(resize),
    /* K22  */  be_nested_str_weak(tasmota),
    /* K23  */  be_nested_str_weak(log),
    /* K24  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K25  */  be_nested_str_weak(MTR_X3A_X20i2r_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K26  */  be_nested_str_weak(tohex),
    /* K27  */  be_nested_str_weak(MTR_X3A_X20p_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K28  */  be_nested_str_weak(MTR_X3A_X20a_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K29  */  be_nested_str_weak(MTR_X3A_X20n_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K30  */  be_nested_str_weak(MTR_X3A_X20mic_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K31  */  be_nested_str_weak(AES_CCM),
    /* K32  */  be_nested_str_weak(tag),
    /* K33  */  be_nested_str_weak(MTR_X3A_X20cleartext_X20_X20_X20_X3D),
    /* K34  */  be_nested_str_weak(MTR_X3A_X20tag_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D),
    /* K35  */  be_nested_str_weak(MTR_X3A_X20rejected_X20packet_X20due_X20to_X20invalid_X20MIC),
    }),
    be_str_weak(decrypt),
    &be_const_str_solidified,
    ( &(const binstruction[165]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x5411FFEF,  //  0003  LDINT	R4	-16
      0x40100903,  //  0004  CONNECT	R4	R4	K3
      0x94100604,  //  0005  GETIDX	R4	R3	R4
      0x8C140504,  //  0006  GETMET	R5	R2	K4
      0x7C140200,  //  0007  CALL	R5	1
      0x88180105,  //  0008  GETMBR	R6	R0	K5
      0x781A0025,  //  0009  JMPF	R6	#0030
      0x8C180506,  //  000A  GETMET	R6	R2	K6
      0x7C180200,  //  000B  CALL	R6	1
      0x601C0015,  //  000C  GETGBL	R7	G21
      0x7C1C0000,  //  000D  CALL	R7	0
      0x8C1C0F07,  //  000E  GETMET	R7	R7	K7
      0x88240108,  //  000F  GETMBR	R9	R0	K8
      0x5429FFFD,  //  0010  LDINT	R10	-2
      0x7C1C0600,  //  0011  CALL	R7	3
      0x54220004,  //  0012  LDINT	R8	5
      0x5426000E,  //  0013  LDINT	R9	15
      0x40201009,  //  0014  CONNECT	R8	R8	R9
      0x94200808,  //  0015  GETIDX	R8	R4	R8
      0x001C0E08,  //  0016  ADD	R7	R7	R8
      0x54220003,  //  0017  LDINT	R8	4
      0x88240109,  //  0018  GETMBR	R9	R0	K9
      0x0424130A,  //  0019  SUB	R9	R9	K10
      0x40201009,  //  001A  CONNECT	R8	R8	R9
      0x88240102,  //  001B  GETMBR	R9	R0	K2
      0x94201208,  //  001C  GETIDX	R8	R9	R8
      0x8C28030B,  //  001D  GETMET	R10	R1	K11
      0x5C300C00,  //  001E  MOVE	R12	R6
      0x7C280400,  //  001F  CALL	R10	2
      0x8C28150C,  //  0020  GETMET	R10	R10	K12
      0x5C301000,  //  0021  MOVE	R12	R8
      0x5C340E00,  //  0022  MOVE	R13	R7
      0x5838000D,  //  0023  LDCONST	R14	K13
      0x7C280800,  //  0024  CALL	R10	4
      0x5C241400,  //  0025  MOVE	R9	R10
      0x402A1D0F,  //  0026  CONNECT	R10	K14	K15
      0x882C0102,  //  0027  GETMBR	R11	R0	K2
      0x9428160A,  //  0028  GETIDX	R10	R11	R10
      0x00281409,  //  0029  ADD	R10	R10	R9
      0x882C0110,  //  002A  GETMBR	R11	R0	K16
      0x882C1709,  //  002B  GETMBR	R11	R11	K9
      0x402C1703,  //  002C  CONNECT	R11	R11	K3
      0x942C100B,  //  002D  GETIDX	R11	R8	R11
      0x0028140B,  //  002E  ADD	R10	R10	R11
      0x9002040A,  //  002F  SETMBR	R0	K2	R10
      0x88180109,  //  0030  GETMBR	R6	R0	K9
      0x04180D0A,  //  0031  SUB	R6	R6	K10
      0x401A1C06,  //  0032  CONNECT	R6	K14	R6
      0x94180606,  //  0033  GETIDX	R6	R3	R6
      0x881C0109,  //  0034  GETMBR	R7	R0	K9
      0x5421FFEE,  //  0035  LDINT	R8	-17
      0x401C0E08,  //  0036  CONNECT	R7	R7	R8
      0x941C0607,  //  0037  GETIDX	R7	R3	R7
      0x60200015,  //  0038  GETGBL	R8	G21
      0x7C200000,  //  0039  CALL	R8	0
      0x8C241107,  //  003A  GETMET	R9	R8	K7
      0x882C0111,  //  003B  GETMBR	R11	R0	K17
      0x5830000A,  //  003C  LDCONST	R12	K10
      0x7C240600,  //  003D  CALL	R9	3
      0x8C241107,  //  003E  GETMET	R9	R8	K7
      0x882C0112,  //  003F  GETMBR	R11	R0	K18
      0x54320003,  //  0040  LDINT	R12	4
      0x7C240600,  //  0041  CALL	R9	3
      0x88240113,  //  0042  GETMBR	R9	R0	K19
      0x78260001,  //  0043  JMPF	R9	#0046
      0x40241103,  //  0044  CONNECT	R9	R8	K3
      0x70020006,  //  0045  JMP		#004D
      0x88240514,  //  0046  GETMBR	R9	R2	K20
      0x78260001,  //  0047  JMPF	R9	#004A
      0x88240514,  //  0048  GETMBR	R9	R2	K20
      0x40241009,  //  0049  CONNECT	R9	R8	R9
      0x8C241115,  //  004A  GETMET	R9	R8	K21
      0x542E000C,  //  004B  LDINT	R11	13
      0x7C240400,  //  004C  CALL	R9	2
      0xB8262C00,  //  004D  GETNGBL	R9	K22
      0x8C241317,  //  004E  GETMET	R9	R9	K23
      0x582C0018,  //  004F  LDCONST	R11	K24
      0x54320003,  //  0050  LDINT	R12	4
      0x7C240600,  //  0051  CALL	R9	3
      0xB8262C00,  //  0052  GETNGBL	R9	K22
      0x8C241317,  //  0053  GETMET	R9	R9	K23
      0x8C2C0B1A,  //  0054  GETMET	R11	R5	K26
      0x7C2C0200,  //  0055  CALL	R11	1
      0x002E320B,  //  0056  ADD	R11	K25	R11
      0x54320003,  //  0057  LDINT	R12	4
      0x7C240600,  //  0058  CALL	R9	3
      0xB8262C00,  //  0059  GETNGBL	R9	K22
      0x8C241317,  //  005A  GETMET	R9	R9	K23
      0x8C2C0F1A,  //  005B  GETMET	R11	R7	K26
      0x7C2C0200,  //  005C  CALL	R11	1
      0x002E360B,  //  005D  ADD	R11	K27	R11
      0x54320003,  //  005E  LDINT	R12	4
      0x7C240600,  //  005F  CALL	R9	3
      0xB8262C00,  //  0060  GETNGBL	R9	K22
      0x8C241317,  //  0061  GETMET	R9	R9	K23
      0x8C2C0D1A,  //  0062  GETMET	R11	R6	K26
      0x7C2C0200,  //  0063  CALL	R11	1
      0x002E380B,  //  0064  ADD	R11	K28	R11
      0x54320003,  //  0065  LDINT	R12	4
      0x7C240600,  //  0066  CALL	R9	3
      0xB8262C00,  //  0067  GETNGBL	R9	K22
      0x8C241317,  //  0068  GETMET	R9	R9	K23
      0x8C2C111A,  //  0069  GETMET	R11	R8	K26
      0x7C2C0200,  //  006A  CALL	R11	1
      0x002E3A0B,  //  006B  ADD	R11	K29	R11
      0x54320003,  //  006C  LDINT	R12	4
      0x7C240600,  //  006D  CALL	R9	3
      0xB8262C00,  //  006E  GETNGBL	R9	K22
      0x8C241317,  //  006F  GETMET	R9	R9	K23
      0x8C2C091A,  //  0070  GETMET	R11	R4	K26
      0x7C2C0200,  //  0071  CALL	R11	1
      0x002E3C0B,  //  0072  ADD	R11	K30	R11
      0x54320003,  //  0073  LDINT	R12	4
      0x7C240600,  //  0074  CALL	R9	3
      0x8C24031F,  //  0075  GETMET	R9	R1	K31
      0x5C2C0A00,  //  0076  MOVE	R11	R5
      0x5C301000,  //  0077  MOVE	R12	R8
      0x5C340C00,  //  0078  MOVE	R13	R6
      0x6038000C,  //  0079  GETGBL	R14	G12
      0x5C3C0E00,  //  007A  MOVE	R15	R7
      0x7C380200,  //  007B  CALL	R14	1
      0x543E000F,  //  007C  LDINT	R15	16
      0x7C240C00,  //  007D  CALL	R9	6
      0x8C28130C,  //  007E  GETMET	R10	R9	K12
      0x5C300E00,  //  007F  MOVE	R12	R7
      0x7C280400,  //  0080  CALL	R10	2
      0x8C2C1320,  //  0081  GETMET	R11	R9	K32
      0x7C2C0200,  //  0082  CALL	R11	1
      0xB8322C00,  //  0083  GETNGBL	R12	K22
      0x8C301917,  //  0084  GETMET	R12	R12	K23
      0x58380018,  //  0085  LDCONST	R14	K24
      0x543E0003,  //  0086  LDINT	R15	4
      0x7C300600,  //  0087  CALL	R12	3
      0xB8322C00,  //  0088  GETNGBL	R12	K22
      0x8C301917,  //  0089  GETMET	R12	R12	K23
      0x8C38151A,  //  008A  GETMET	R14	R10	K26
      0x7C380200,  //  008B  CALL	R14	1
      0x003A420E,  //  008C  ADD	R14	K33	R14
      0x543E0003,  //  008D  LDINT	R15	4
      0x7C300600,  //  008E  CALL	R12	3
      0xB8322C00,  //  008F  GETNGBL	R12	K22
      0x8C301917,  //  0090  GETMET	R12	R12	K23
      0x8C38171A,  //  0091  GETMET	R14	R11	K26
      0x7C380200,  //  0092  CALL	R14	1
      0x003A440E,  //  0093  ADD	R14	K34	R14
      0x543E0003,  //  0094  LDINT	R15	4
      0x7C300600,  //  0095  CALL	R12	3
      0xB8322C00,  //  0096  GETNGBL	R12	K22
      0x8C301917,  //  0097  GETMET	R12	R12	K23
      0x58380018,  //  0098  LDCONST	R14	K24
      0x543E0003,  //  0099  LDINT	R15	4
      0x7C300600,  //  009A  CALL	R12	3
      0x20301604,  //  009B  NE	R12	R11	R4
      0x78320006,  //  009C  JMPF	R12	#00A4
      0xB8322C00,  //  009D  GETNGBL	R12	K22
      0x8C301917,  //  009E  GETMET	R12	R12	K23
      0x58380023,  //  009F  LDCONST	R14	K35
      0x583C000F,  //  00A0  LDCONST	R15	K15
      0x7C300600,  //  00A1  CALL	R12	3
      0x4C300000,  //  00A2  LDNIL	R12
      0x80041800,  //  00A3  RET	1	R12
      0x80041400,  //  00A4  RET	1	R10
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
