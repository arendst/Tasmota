/* Solidification of Matter_Message.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_Matter_Frame;
// compact class 'Matter_Frame' ktab size: 73, total: 200 (saved 1016 bytes)
static const bvalue be_ktab_class_Matter_Frame[73] = {
  /* K0   */  be_nested_str_weak(session),
  /* K1   */  be_nested_str_weak(get_node_id),
  /* K2   */  be_nested_str_weak(crypto),
  /* K3   */  be_nested_str_weak(raw),
  /* K4   */  be_nested_str_weak(payload_idx),
  /* K5   */  be_nested_str_weak(get_r2i),
  /* K6   */  be_nested_str_weak(message_handler),
  /* K7   */  be_nested_str_weak(_n_bytes),
  /* K8   */  be_nested_str_weak(clear),
  /* K9   */  be_nested_str_weak(add),
  /* K10  */  be_nested_str_weak(sec_flags),
  /* K11  */  be_const_int(1),
  /* K12  */  be_nested_str_weak(message_counter),
  /* K13  */  be_nested_str_weak(is_CASE),
  /* K14  */  be_nested_str_weak(get_device_id),
  /* K15  */  be_nested_str_weak(resize),
  /* K16  */  be_nested_str_weak(AES_CCM),
  /* K17  */  be_nested_str_weak(encrypt1),
  /* K18  */  be_const_int(0),
  /* K19  */  be_nested_str_weak(remote_ip),
  /* K20  */  be_nested_str_weak(remote_port),
  /* K21  */  be_nested_str_weak(flags),
  /* K22  */  be_nested_str_weak(flag_s),
  /* K23  */  be_nested_str_weak(flag_dsiz),
  /* K24  */  be_const_int(3),
  /* K25  */  be_nested_str_weak(local_session_id),
  /* K26  */  be_const_int(2),
  /* K27  */  be_nested_str_weak(sec_p),
  /* K28  */  be_nested_str_weak(sec_c),
  /* K29  */  be_nested_str_weak(sec_sesstype),
  /* K30  */  be_nested_str_weak(source_node_id),
  /* K31  */  be_nested_str_weak(dest_node_id_8),
  /* K32  */  be_nested_str_weak(dest_node_id_2),
  /* K33  */  be_nested_str_weak(x_flags),
  /* K34  */  be_nested_str_weak(x_flag_v),
  /* K35  */  be_nested_str_weak(x_flag_r),
  /* K36  */  be_nested_str_weak(x_flag_a),
  /* K37  */  be_nested_str_weak(x_flag_i),
  /* K38  */  be_nested_str_weak(opcode),
  /* K39  */  be_nested_str_weak(exchange_id),
  /* K40  */  be_nested_str_weak(protocol_id),
  /* K41  */  be_nested_str_weak(ack_message_counter),
  /* K42  */  be_nested_str_weak(app_payload_idx),
  /* K43  */  be_nested_str_weak(get),
  /* K44  */  be_nested_str_weak(getbits),
  /* K45  */  be_nested_str_weak(sec_mx),
  /* K46  */  be_nested_str_weak(matter),
  /* K47  */  be_nested_str_weak(Frame),
  /* K48  */  be_nested_str_weak(decode_header),
  /* K49  */  be_nested_str_weak(decode_payload),
  /* K50  */  be_nested_str_weak(initiator_session_id),
  /* K51  */  be_nested_str_weak(counter_snd_next),
  /* K52  */  be_nested_str_weak(_counter_insecure_snd),
  /* K53  */  be_nested_str_weak(next),
  /* K54  */  be_nested_str_weak(get_opcode_name),
  /* K55  */  be_nested_str_weak(0x_X2502X),
  /* K56  */  be_nested_str_weak(log),
  /* K57  */  be_nested_str_weak(MTR_X3A_X20_X3CReplied_X20_X20_X20_X28_X256i_X29_X20_X25s),
  /* K58  */  be_nested_str_weak(get_i2r),
  /* K59  */  be_nested_str_weak(MTR_X3A_X20_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X3E_X20Compute_X20Privacy_X20TODO),
  /* K60  */  be_nested_str_weak(get_i2r_privacy),
  /* K61  */  be_const_int(2147483647),
  /* K62  */  be_nested_str_weak(AES_CTR),
  /* K63  */  be_nested_str_weak(decrypt),
  /* K64  */  be_nested_str_weak(peer_node_id),
  /* K65  */  be_nested_str_weak(decrypt1),
  /* K66  */  be_nested_str_weak(MTR_X3A_X20rejected_X20packet_X20due_X20to_X20invalid_X20MIC),
  /* K67  */  be_const_class(be_class_Matter_Frame),
  /* K68  */  be_nested_str_weak(_ip),
  /* K69  */  be_nested_str_weak(_port),
  /* K70  */  be_nested_str_weak(_exchange_id),
  /* K71  */  be_nested_str_weak(x_flag_sx),
  /* K72  */  be_nested_str_weak(vendor_id),
};


extern const bclass be_class_Matter_Frame;

/********************************************************************
** Solidified function: get_node_id
********************************************************************/
be_local_closure(class_Matter_Frame_get_node_id,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Frame,     /* shared constants */
    be_str_weak(get_node_id),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060003,  //  0001  JMPF	R1	#0006
      0x88040100,  //  0002  GETMBR	R1	R0	K0
      0x8C040301,  //  0003  GETMET	R1	R1	K1
      0x7C040200,  //  0004  CALL	R1	1
      0x70020000,  //  0005  JMP		#0007
      0x4C040000,  //  0006  LDNIL	R1
      0x80040200,  //  0007  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: encrypt
********************************************************************/
be_local_closure(class_Matter_Frame_encrypt,   /* name */
  be_nested_proto(
    23,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Frame,     /* shared constants */
    be_str_weak(encrypt),
    &be_const_str_solidified,
    ( &(const binstruction[63]) {  /* code */
      0xA4060400,  //  0000  IMPORT	R1	K2
      0x88080103,  //  0001  GETMBR	R2	R0	K3
      0x880C0100,  //  0002  GETMBR	R3	R0	K0
      0x88100104,  //  0003  GETMBR	R4	R0	K4
      0x5416000F,  //  0004  LDINT	R5	16
      0x8C180705,  //  0005  GETMET	R6	R3	K5
      0x7C180200,  //  0006  CALL	R6	1
      0x881C0106,  //  0007  GETMBR	R7	R0	K6
      0x881C0F07,  //  0008  GETMBR	R7	R7	K7
      0x8C200F08,  //  0009  GETMET	R8	R7	K8
      0x7C200200,  //  000A  CALL	R8	1
      0x8C200F09,  //  000B  GETMET	R8	R7	K9
      0x8828010A,  //  000C  GETMBR	R10	R0	K10
      0x582C000B,  //  000D  LDCONST	R11	K11
      0x7C200600,  //  000E  CALL	R8	3
      0x8C200F09,  //  000F  GETMET	R8	R7	K9
      0x8828010C,  //  0010  GETMBR	R10	R0	K12
      0x542E0003,  //  0011  LDINT	R11	4
      0x7C200600,  //  0012  CALL	R8	3
      0x8C20070D,  //  0013  GETMET	R8	R3	K13
      0x7C200200,  //  0014  CALL	R8	1
      0x78220005,  //  0015  JMPF	R8	#001C
      0x8C20070E,  //  0016  GETMET	R8	R3	K14
      0x7C200200,  //  0017  CALL	R8	1
      0x78220002,  //  0018  JMPF	R8	#001C
      0x8C20070E,  //  0019  GETMET	R8	R3	K14
      0x7C200200,  //  001A  CALL	R8	1
      0x40200E08,  //  001B  CONNECT	R8	R7	R8
      0x8C200F0F,  //  001C  GETMET	R8	R7	K15
      0x542A000C,  //  001D  LDINT	R10	13
      0x7C200400,  //  001E  CALL	R8	2
      0x8C20050F,  //  001F  GETMET	R8	R2	K15
      0x6028000C,  //  0020  GETGBL	R10	G12
      0x5C2C0400,  //  0021  MOVE	R11	R2
      0x7C280200,  //  0022  CALL	R10	1
      0x00281405,  //  0023  ADD	R10	R10	R5
      0x7C200400,  //  0024  CALL	R8	2
      0x88200310,  //  0025  GETMBR	R8	R1	K16
      0x8C201111,  //  0026  GETMET	R8	R8	K17
      0x5C280C00,  //  0027  MOVE	R10	R6
      0x5C2C0E00,  //  0028  MOVE	R11	R7
      0x58300012,  //  0029  LDCONST	R12	K18
      0x6034000C,  //  002A  GETGBL	R13	G12
      0x5C380E00,  //  002B  MOVE	R14	R7
      0x7C340200,  //  002C  CALL	R13	1
      0x5C380400,  //  002D  MOVE	R14	R2
      0x583C0012,  //  002E  LDCONST	R15	K18
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
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Frame_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Frame,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x90020C01,  //  0000  SETMBR	R0	K6	R1
      0x90020602,  //  0001  SETMBR	R0	K3	R2
      0x90022603,  //  0002  SETMBR	R0	K19	R3
      0x90022804,  //  0003  SETMBR	R0	K20	R4
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: encode_frame
********************************************************************/
be_local_closure(class_Matter_Frame_encode_frame,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Frame,     /* shared constants */
    be_str_weak(encode_frame),
    &be_const_str_solidified,
    ( &(const binstruction[161]) {  /* code */
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
      0x58140012,  //  000A  LDCONST	R5	K18
      0x00100805,  //  000B  ADD	R4	R4	R5
      0x7C0C0200,  //  000C  CALL	R3	1
      0x5C080600,  //  000D  MOVE	R2	R3
      0x880C0115,  //  000E  GETMBR	R3	R0	K21
      0x4C100000,  //  000F  LDNIL	R4
      0x1C0C0604,  //  0010  EQ	R3	R3	R4
      0x780E000D,  //  0011  JMPF	R3	#0020
      0x90022B12,  //  0012  SETMBR	R0	K21	K18
      0x880C0116,  //  0013  GETMBR	R3	R0	K22
      0x780E0003,  //  0014  JMPF	R3	#0019
      0x880C0115,  //  0015  GETMBR	R3	R0	K21
      0x54120003,  //  0016  LDINT	R4	4
      0x300C0604,  //  0017  OR	R3	R3	R4
      0x90022A03,  //  0018  SETMBR	R0	K21	R3
      0x880C0117,  //  0019  GETMBR	R3	R0	K23
      0x780E0004,  //  001A  JMPF	R3	#0020
      0x880C0115,  //  001B  GETMBR	R3	R0	K21
      0x88100117,  //  001C  GETMBR	R4	R0	K23
      0x2C100918,  //  001D  AND	R4	R4	K24
      0x300C0604,  //  001E  OR	R3	R3	R4
      0x90022A03,  //  001F  SETMBR	R0	K21	R3
      0x8C0C0509,  //  0020  GETMET	R3	R2	K9
      0x88140115,  //  0021  GETMBR	R5	R0	K21
      0x5818000B,  //  0022  LDCONST	R6	K11
      0x7C0C0600,  //  0023  CALL	R3	3
      0x8C0C0509,  //  0024  GETMET	R3	R2	K9
      0x88140119,  //  0025  GETMBR	R5	R0	K25
      0x78160001,  //  0026  JMPF	R5	#0029
      0x88140119,  //  0027  GETMBR	R5	R0	K25
      0x70020000,  //  0028  JMP		#002A
      0x58140012,  //  0029  LDCONST	R5	K18
      0x5818001A,  //  002A  LDCONST	R6	K26
      0x7C0C0600,  //  002B  CALL	R3	3
      0x880C010A,  //  002C  GETMBR	R3	R0	K10
      0x4C100000,  //  002D  LDNIL	R4
      0x1C0C0604,  //  002E  EQ	R3	R3	R4
      0x780E0013,  //  002F  JMPF	R3	#0044
      0x90021512,  //  0030  SETMBR	R0	K10	K18
      0x880C011B,  //  0031  GETMBR	R3	R0	K27
      0x780E0003,  //  0032  JMPF	R3	#0037
      0x880C010A,  //  0033  GETMBR	R3	R0	K10
      0x5412007F,  //  0034  LDINT	R4	128
      0x300C0604,  //  0035  OR	R3	R3	R4
      0x90021403,  //  0036  SETMBR	R0	K10	R3
      0x880C011C,  //  0037  GETMBR	R3	R0	K28
      0x780E0003,  //  0038  JMPF	R3	#003D
      0x880C010A,  //  0039  GETMBR	R3	R0	K10
      0x5412003F,  //  003A  LDINT	R4	64
      0x300C0604,  //  003B  OR	R3	R3	R4
      0x90021403,  //  003C  SETMBR	R0	K10	R3
      0x880C011D,  //  003D  GETMBR	R3	R0	K29
      0x780E0004,  //  003E  JMPF	R3	#0044
      0x880C010A,  //  003F  GETMBR	R3	R0	K10
      0x8810011D,  //  0040  GETMBR	R4	R0	K29
      0x2C100918,  //  0041  AND	R4	R4	K24
      0x300C0604,  //  0042  OR	R3	R3	R4
      0x90021403,  //  0043  SETMBR	R0	K10	R3
      0x8C0C0509,  //  0044  GETMET	R3	R2	K9
      0x8814010A,  //  0045  GETMBR	R5	R0	K10
      0x5818000B,  //  0046  LDCONST	R6	K11
      0x7C0C0600,  //  0047  CALL	R3	3
      0x8C0C0509,  //  0048  GETMET	R3	R2	K9
      0x8814010C,  //  0049  GETMBR	R5	R0	K12
      0x541A0003,  //  004A  LDINT	R6	4
      0x7C0C0600,  //  004B  CALL	R3	3
      0x880C0116,  //  004C  GETMBR	R3	R0	K22
      0x780E0001,  //  004D  JMPF	R3	#0050
      0x880C011E,  //  004E  GETMBR	R3	R0	K30
      0x400C0403,  //  004F  CONNECT	R3	R2	R3
      0x880C0117,  //  0050  GETMBR	R3	R0	K23
      0x1C0C070B,  //  0051  EQ	R3	R3	K11
      0x780E0001,  //  0052  JMPF	R3	#0055
      0x880C011F,  //  0053  GETMBR	R3	R0	K31
      0x400C0403,  //  0054  CONNECT	R3	R2	R3
      0x880C0117,  //  0055  GETMBR	R3	R0	K23
      0x1C0C071A,  //  0056  EQ	R3	R3	K26
      0x780E0003,  //  0057  JMPF	R3	#005C
      0x8C0C0509,  //  0058  GETMET	R3	R2	K9
      0x88140120,  //  0059  GETMBR	R5	R0	K32
      0x5818001A,  //  005A  LDCONST	R6	K26
      0x7C0C0600,  //  005B  CALL	R3	3
      0x600C000C,  //  005C  GETGBL	R3	G12
      0x5C100400,  //  005D  MOVE	R4	R2
      0x7C0C0200,  //  005E  CALL	R3	1
      0x90020803,  //  005F  SETMBR	R0	K4	R3
      0x880C0121,  //  0060  GETMBR	R3	R0	K33
      0x4C100000,  //  0061  LDNIL	R4
      0x1C0C0604,  //  0062  EQ	R3	R3	R4
      0x780E0016,  //  0063  JMPF	R3	#007B
      0x90024312,  //  0064  SETMBR	R0	K33	K18
      0x880C0122,  //  0065  GETMBR	R3	R0	K34
      0x780E0003,  //  0066  JMPF	R3	#006B
      0x880C0121,  //  0067  GETMBR	R3	R0	K33
      0x5412000F,  //  0068  LDINT	R4	16
      0x300C0604,  //  0069  OR	R3	R3	R4
      0x90024203,  //  006A  SETMBR	R0	K33	R3
      0x880C0123,  //  006B  GETMBR	R3	R0	K35
      0x780E0003,  //  006C  JMPF	R3	#0071
      0x880C0121,  //  006D  GETMBR	R3	R0	K33
      0x54120003,  //  006E  LDINT	R4	4
      0x300C0604,  //  006F  OR	R3	R3	R4
      0x90024203,  //  0070  SETMBR	R0	K33	R3
      0x880C0124,  //  0071  GETMBR	R3	R0	K36
      0x780E0002,  //  0072  JMPF	R3	#0076
      0x880C0121,  //  0073  GETMBR	R3	R0	K33
      0x300C071A,  //  0074  OR	R3	R3	K26
      0x90024203,  //  0075  SETMBR	R0	K33	R3
      0x880C0125,  //  0076  GETMBR	R3	R0	K37
      0x780E0002,  //  0077  JMPF	R3	#007B
      0x880C0121,  //  0078  GETMBR	R3	R0	K33
      0x300C070B,  //  0079  OR	R3	R3	K11
      0x90024203,  //  007A  SETMBR	R0	K33	R3
      0x8C0C0509,  //  007B  GETMET	R3	R2	K9
      0x88140121,  //  007C  GETMBR	R5	R0	K33
      0x5818000B,  //  007D  LDCONST	R6	K11
      0x7C0C0600,  //  007E  CALL	R3	3
      0x8C0C0509,  //  007F  GETMET	R3	R2	K9
      0x88140126,  //  0080  GETMBR	R5	R0	K38
      0x5818000B,  //  0081  LDCONST	R6	K11
      0x7C0C0600,  //  0082  CALL	R3	3
      0x8C0C0509,  //  0083  GETMET	R3	R2	K9
      0x88140127,  //  0084  GETMBR	R5	R0	K39
      0x4C180000,  //  0085  LDNIL	R6
      0x20140A06,  //  0086  NE	R5	R5	R6
      0x78160003,  //  0087  JMPF	R5	#008C
      0x88140127,  //  0088  GETMBR	R5	R0	K39
      0x541AFFFE,  //  0089  LDINT	R6	65535
      0x2C140A06,  //  008A  AND	R5	R5	R6
      0x70020000,  //  008B  JMP		#008D
      0x58140012,  //  008C  LDCONST	R5	K18
      0x5818001A,  //  008D  LDCONST	R6	K26
      0x7C0C0600,  //  008E  CALL	R3	3
      0x8C0C0509,  //  008F  GETMET	R3	R2	K9
      0x88140128,  //  0090  GETMBR	R5	R0	K40
      0x5818001A,  //  0091  LDCONST	R6	K26
      0x7C0C0600,  //  0092  CALL	R3	3
      0x880C0124,  //  0093  GETMBR	R3	R0	K36
      0x780E0003,  //  0094  JMPF	R3	#0099
      0x8C0C0509,  //  0095  GETMET	R3	R2	K9
      0x88140129,  //  0096  GETMBR	R5	R0	K41
      0x541A0003,  //  0097  LDINT	R6	4
      0x7C0C0600,  //  0098  CALL	R3	3
      0x600C000C,  //  0099  GETGBL	R3	G12
      0x5C100400,  //  009A  MOVE	R4	R2
      0x7C0C0200,  //  009B  CALL	R3	1
      0x90025403,  //  009C  SETMBR	R0	K42	R3
      0x78060000,  //  009D  JMPF	R1	#009F
      0x400C0401,  //  009E  CONNECT	R3	R2	R1
      0x90020602,  //  009F  SETMBR	R0	K3	R2
      0x80040400,  //  00A0  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: decode_header
********************************************************************/
be_local_closure(class_Matter_Frame_decode_header,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Frame,     /* shared constants */
    be_str_weak(decode_header),
    &be_const_str_solidified,
    ( &(const binstruction[121]) {  /* code */
      0x58040012,  //  0000  LDCONST	R1	K18
      0x88080103,  //  0001  GETMBR	R2	R0	K3
      0x8C0C052B,  //  0002  GETMET	R3	R2	K43
      0x58140012,  //  0003  LDCONST	R5	K18
      0x5818000B,  //  0004  LDCONST	R6	K11
      0x7C0C0600,  //  0005  CALL	R3	3
      0x90022A03,  //  0006  SETMBR	R0	K21	R3
      0x880C0115,  //  0007  GETMBR	R3	R0	K21
      0x541200F7,  //  0008  LDINT	R4	248
      0x2C0C0604,  //  0009  AND	R3	R3	R4
      0x200C0712,  //  000A  NE	R3	R3	K18
      0x780E0001,  //  000B  JMPF	R3	#000E
      0x500C0000,  //  000C  LDBOOL	R3	0	0
      0x80040600,  //  000D  RET	1	R3
      0x8C0C052C,  //  000E  GETMET	R3	R2	K44
      0x5814001A,  //  000F  LDCONST	R5	K26
      0x5818000B,  //  0010  LDCONST	R6	K11
      0x7C0C0600,  //  0011  CALL	R3	3
      0x90022C03,  //  0012  SETMBR	R0	K22	R3
      0x8C0C052C,  //  0013  GETMET	R3	R2	K44
      0x58140012,  //  0014  LDCONST	R5	K18
      0x5818001A,  //  0015  LDCONST	R6	K26
      0x7C0C0600,  //  0016  CALL	R3	3
      0x90022E03,  //  0017  SETMBR	R0	K23	R3
      0x880C0117,  //  0018  GETMBR	R3	R0	K23
      0x1C0C0718,  //  0019  EQ	R3	R3	K24
      0x780E0001,  //  001A  JMPF	R3	#001D
      0x500C0000,  //  001B  LDBOOL	R3	0	0
      0x80040600,  //  001C  RET	1	R3
      0x8C0C052B,  //  001D  GETMET	R3	R2	K43
      0x58140018,  //  001E  LDCONST	R5	K24
      0x5818000B,  //  001F  LDCONST	R6	K11
      0x7C0C0600,  //  0020  CALL	R3	3
      0x90021403,  //  0021  SETMBR	R0	K10	R3
      0x8C0C052C,  //  0022  GETMET	R3	R2	K44
      0x54160007,  //  0023  LDINT	R5	8
      0x08163005,  //  0024  MUL	R5	K24	R5
      0x541A0006,  //  0025  LDINT	R6	7
      0x00140A06,  //  0026  ADD	R5	R5	R6
      0x5818000B,  //  0027  LDCONST	R6	K11
      0x7C0C0600,  //  0028  CALL	R3	3
      0x90023603,  //  0029  SETMBR	R0	K27	R3
      0x8C0C052C,  //  002A  GETMET	R3	R2	K44
      0x54160007,  //  002B  LDINT	R5	8
      0x08163005,  //  002C  MUL	R5	K24	R5
      0x541A0005,  //  002D  LDINT	R6	6
      0x00140A06,  //  002E  ADD	R5	R5	R6
      0x5818000B,  //  002F  LDCONST	R6	K11
      0x7C0C0600,  //  0030  CALL	R3	3
      0x90023803,  //  0031  SETMBR	R0	K28	R3
      0x8C0C052C,  //  0032  GETMET	R3	R2	K44
      0x54160007,  //  0033  LDINT	R5	8
      0x08163005,  //  0034  MUL	R5	K24	R5
      0x541A0004,  //  0035  LDINT	R6	5
      0x00140A06,  //  0036  ADD	R5	R5	R6
      0x5818000B,  //  0037  LDCONST	R6	K11
      0x7C0C0600,  //  0038  CALL	R3	3
      0x90025A03,  //  0039  SETMBR	R0	K45	R3
      0x8C0C052C,  //  003A  GETMET	R3	R2	K44
      0x54160007,  //  003B  LDINT	R5	8
      0x08163005,  //  003C  MUL	R5	K24	R5
      0x5818001A,  //  003D  LDCONST	R6	K26
      0x7C0C0600,  //  003E  CALL	R3	3
      0x90023A03,  //  003F  SETMBR	R0	K29	R3
      0x880C011D,  //  0040  GETMBR	R3	R0	K29
      0x240C070B,  //  0041  GT	R3	R3	K11
      0x780E0001,  //  0042  JMPF	R3	#0045
      0x500C0000,  //  0043  LDBOOL	R3	0	0
      0x80040600,  //  0044  RET	1	R3
      0x8C0C052B,  //  0045  GETMET	R3	R2	K43
      0x5814000B,  //  0046  LDCONST	R5	K11
      0x5818001A,  //  0047  LDCONST	R6	K26
      0x7C0C0600,  //  0048  CALL	R3	3
      0x90023203,  //  0049  SETMBR	R0	K25	R3
      0x8C0C052B,  //  004A  GETMET	R3	R2	K43
      0x54160003,  //  004B  LDINT	R5	4
      0x541A0003,  //  004C  LDINT	R6	4
      0x7C0C0600,  //  004D  CALL	R3	3
      0x90021803,  //  004E  SETMBR	R0	K12	R3
      0x540E0007,  //  004F  LDINT	R3	8
      0x00040203,  //  0050  ADD	R1	R1	R3
      0x880C0116,  //  0051  GETMBR	R3	R0	K22
      0x780E0006,  //  0052  JMPF	R3	#005A
      0x540E0006,  //  0053  LDINT	R3	7
      0x000C0203,  //  0054  ADD	R3	R1	R3
      0x400C0203,  //  0055  CONNECT	R3	R1	R3
      0x940C0403,  //  0056  GETIDX	R3	R2	R3
      0x90023C03,  //  0057  SETMBR	R0	K30	R3
      0x540E0007,  //  0058  LDINT	R3	8
      0x00040203,  //  0059  ADD	R1	R1	R3
      0x880C0117,  //  005A  GETMBR	R3	R0	K23
      0x1C0C070B,  //  005B  EQ	R3	R3	K11
      0x780E0007,  //  005C  JMPF	R3	#0065
      0x540E0006,  //  005D  LDINT	R3	7
      0x000C0203,  //  005E  ADD	R3	R1	R3
      0x400C0203,  //  005F  CONNECT	R3	R1	R3
      0x940C0403,  //  0060  GETIDX	R3	R2	R3
      0x90023E03,  //  0061  SETMBR	R0	K31	R3
      0x540E0007,  //  0062  LDINT	R3	8
      0x00040203,  //  0063  ADD	R1	R1	R3
      0x70020008,  //  0064  JMP		#006E
      0x880C0117,  //  0065  GETMBR	R3	R0	K23
      0x1C0C071A,  //  0066  EQ	R3	R3	K26
      0x780E0005,  //  0067  JMPF	R3	#006E
      0x8C0C052B,  //  0068  GETMET	R3	R2	K43
      0x5C140200,  //  0069  MOVE	R5	R1
      0x5818001A,  //  006A  LDCONST	R6	K26
      0x7C0C0600,  //  006B  CALL	R3	3
      0x90024003,  //  006C  SETMBR	R0	K32	R3
      0x0004031A,  //  006D  ADD	R1	R1	K26
      0x880C012D,  //  006E  GETMBR	R3	R0	K45
      0x780E0005,  //  006F  JMPF	R3	#0076
      0x8C0C052B,  //  0070  GETMET	R3	R2	K43
      0x5C140200,  //  0071  MOVE	R5	R1
      0x5818001A,  //  0072  LDCONST	R6	K26
      0x7C0C0600,  //  0073  CALL	R3	3
      0x0010071A,  //  0074  ADD	R4	R3	K26
      0x00040204,  //  0075  ADD	R1	R1	R4
      0x90020801,  //  0076  SETMBR	R0	K4	R1
      0x500C0200,  //  0077  LDBOOL	R3	1	0
      0x80040600,  //  0078  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: debug
********************************************************************/
be_local_closure(class_Matter_Frame_debug,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Frame,     /* shared constants */
    be_str_weak(debug),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x80000400,  //  0000  RET	0
      0xB80A5C00,  //  0001  GETNGBL	R2	K46
      0x8C08052F,  //  0002  GETMET	R2	R2	K47
      0x88100106,  //  0003  GETMBR	R4	R0	K6
      0x5C140200,  //  0004  MOVE	R5	R1
      0x7C080600,  //  0005  CALL	R2	3
      0x8C0C0530,  //  0006  GETMET	R3	R2	K48
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C0C0531,  //  0008  GETMET	R3	R2	K49
      0x7C0C0200,  //  0009  CALL	R3	1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: build_response
********************************************************************/
be_local_closure(class_Matter_Frame_build_response,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Frame,     /* shared constants */
    be_str_weak(build_response),
    &be_const_str_solidified,
    ( &(const binstruction[90]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x1C100604,  //  0001  EQ	R4	R3	R4
      0x78120005,  //  0002  JMPF	R4	#0009
      0x60100006,  //  0003  GETGBL	R4	G6
      0x5C140000,  //  0004  MOVE	R5	R0
      0x7C100200,  //  0005  CALL	R4	1
      0x88140106,  //  0006  GETMBR	R5	R0	K6
      0x7C100200,  //  0007  CALL	R4	1
      0x5C0C0800,  //  0008  MOVE	R3	R4
      0x88100113,  //  0009  GETMBR	R4	R0	K19
      0x900E2604,  //  000A  SETMBR	R3	K19	R4
      0x88100114,  //  000B  GETMBR	R4	R0	K20
      0x900E2804,  //  000C  SETMBR	R3	K20	R4
      0x88100116,  //  000D  GETMBR	R4	R0	K22
      0x78120003,  //  000E  JMPF	R4	#0013
      0x900E2F0B,  //  000F  SETMBR	R3	K23	K11
      0x8810011E,  //  0010  GETMBR	R4	R0	K30
      0x900E3E04,  //  0011  SETMBR	R3	K31	R4
      0x70020000,  //  0012  JMP		#0014
      0x900E2F12,  //  0013  SETMBR	R3	K23	K18
      0x88100100,  //  0014  GETMBR	R4	R0	K0
      0x900E0004,  //  0015  SETMBR	R3	K0	R4
      0x88100119,  //  0016  GETMBR	R4	R0	K25
      0x20100912,  //  0017  NE	R4	R4	K18
      0x7812000D,  //  0018  JMPF	R4	#0027
      0x88100100,  //  0019  GETMBR	R4	R0	K0
      0x7812000B,  //  001A  JMPF	R4	#0027
      0x88100100,  //  001B  GETMBR	R4	R0	K0
      0x88100932,  //  001C  GETMBR	R4	R4	K50
      0x20100912,  //  001D  NE	R4	R4	K18
      0x78120007,  //  001E  JMPF	R4	#0027
      0x88100100,  //  001F  GETMBR	R4	R0	K0
      0x8C100933,  //  0020  GETMET	R4	R4	K51
      0x7C100200,  //  0021  CALL	R4	1
      0x900E1804,  //  0022  SETMBR	R3	K12	R4
      0x88100100,  //  0023  GETMBR	R4	R0	K0
      0x88100932,  //  0024  GETMBR	R4	R4	K50
      0x900E3204,  //  0025  SETMBR	R3	K25	R4
      0x70020005,  //  0026  JMP		#002D
      0x88100100,  //  0027  GETMBR	R4	R0	K0
      0x88100934,  //  0028  GETMBR	R4	R4	K52
      0x8C100935,  //  0029  GETMET	R4	R4	K53
      0x7C100200,  //  002A  CALL	R4	1
      0x900E1804,  //  002B  SETMBR	R3	K12	R4
      0x900E3312,  //  002C  SETMBR	R3	K25	K18
      0x88100125,  //  002D  GETMBR	R4	R0	K37
      0x78120001,  //  002E  JMPF	R4	#0031
      0x58100012,  //  002F  LDCONST	R4	K18
      0x70020000,  //  0030  JMP		#0032
      0x5810000B,  //  0031  LDCONST	R4	K11
      0x900E4A04,  //  0032  SETMBR	R3	K37	R4
      0x900E4C01,  //  0033  SETMBR	R3	K38	R1
      0x88100127,  //  0034  GETMBR	R4	R0	K39
      0x900E4E04,  //  0035  SETMBR	R3	K39	R4
      0x88100128,  //  0036  GETMBR	R4	R0	K40
      0x900E5004,  //  0037  SETMBR	R3	K40	R4
      0x88100123,  //  0038  GETMBR	R4	R0	K35
      0x78120002,  //  0039  JMPF	R4	#003D
      0x900E490B,  //  003A  SETMBR	R3	K36	K11
      0x8810010C,  //  003B  GETMBR	R4	R0	K12
      0x900E5204,  //  003C  SETMBR	R3	K41	R4
      0x780A0001,  //  003D  JMPF	R2	#0040
      0x5810000B,  //  003E  LDCONST	R4	K11
      0x70020000,  //  003F  JMP		#0041
      0x58100012,  //  0040  LDCONST	R4	K18
      0x900E4604,  //  0041  SETMBR	R3	K35	R4
      0x88100719,  //  0042  GETMBR	R4	R3	K25
      0x1C100912,  //  0043  EQ	R4	R4	K18
      0x78120013,  //  0044  JMPF	R4	#0059
      0xB8125C00,  //  0045  GETNGBL	R4	K46
      0x8C100936,  //  0046  GETMET	R4	R4	K54
      0x88180726,  //  0047  GETMBR	R6	R3	K38
      0x7C100400,  //  0048  CALL	R4	2
      0x5C140800,  //  0049  MOVE	R5	R4
      0x74160004,  //  004A  JMPT	R5	#0050
      0x60140018,  //  004B  GETGBL	R5	G24
      0x58180037,  //  004C  LDCONST	R6	K55
      0x881C0726,  //  004D  GETMBR	R7	R3	K38
      0x7C140400,  //  004E  CALL	R5	2
      0x5C100A00,  //  004F  MOVE	R4	R5
      0xB8167000,  //  0050  GETNGBL	R5	K56
      0x60180018,  //  0051  GETGBL	R6	G24
      0x581C0039,  //  0052  LDCONST	R7	K57
      0x88200700,  //  0053  GETMBR	R8	R3	K0
      0x88201119,  //  0054  GETMBR	R8	R8	K25
      0x5C240800,  //  0055  MOVE	R9	R4
      0x7C180600,  //  0056  CALL	R6	3
      0x581C0018,  //  0057  LDCONST	R7	K24
      0x7C140400,  //  0058  CALL	R5	2
      0x80040600,  //  0059  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: decrypt
********************************************************************/
be_local_closure(class_Matter_Frame_decrypt,   /* name */
  be_nested_proto(
    23,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Frame,     /* shared constants */
    be_str_weak(decrypt),
    &be_const_str_solidified,
    ( &(const binstruction[115]) {  /* code */
      0xA4060400,  //  0000  IMPORT	R1	K2
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x880C0103,  //  0002  GETMBR	R3	R0	K3
      0x88100104,  //  0003  GETMBR	R4	R0	K4
      0x5416000F,  //  0004  LDINT	R5	16
      0x8C18053A,  //  0005  GETMET	R6	R2	K58
      0x7C180200,  //  0006  CALL	R6	1
      0x881C011B,  //  0007  GETMBR	R7	R0	K27
      0x781E002B,  //  0008  JMPF	R7	#0035
      0xB81E7000,  //  0009  GETNGBL	R7	K56
      0x5820003B,  //  000A  LDCONST	R8	K59
      0x5824001A,  //  000B  LDCONST	R9	K26
      0x7C1C0400,  //  000C  CALL	R7	2
      0x8C1C053C,  //  000D  GETMET	R7	R2	K60
      0x7C1C0200,  //  000E  CALL	R7	1
      0x5421FFEF,  //  000F  LDINT	R8	-16
      0x4020113D,  //  0010  CONNECT	R8	R8	K61
      0x94200608,  //  0011  GETIDX	R8	R3	R8
      0x60240015,  //  0012  GETGBL	R9	G21
      0x7C240000,  //  0013  CALL	R9	0
      0x8C241309,  //  0014  GETMET	R9	R9	K9
      0x882C0119,  //  0015  GETMBR	R11	R0	K25
      0x5431FFFD,  //  0016  LDINT	R12	-2
      0x7C240600,  //  0017  CALL	R9	3
      0x542A0004,  //  0018  LDINT	R10	5
      0x542E000E,  //  0019  LDINT	R11	15
      0x4028140B,  //  001A  CONNECT	R10	R10	R11
      0x9428100A,  //  001B  GETIDX	R10	R8	R10
      0x0024120A,  //  001C  ADD	R9	R9	R10
      0x542A0003,  //  001D  LDINT	R10	4
      0x882C0104,  //  001E  GETMBR	R11	R0	K4
      0x042C170B,  //  001F  SUB	R11	R11	K11
      0x4028140B,  //  0020  CONNECT	R10	R10	R11
      0x882C0103,  //  0021  GETMBR	R11	R0	K3
      0x9428160A,  //  0022  GETIDX	R10	R11	R10
      0x8C30033E,  //  0023  GETMET	R12	R1	K62
      0x5C380E00,  //  0024  MOVE	R14	R7
      0x7C300400,  //  0025  CALL	R12	2
      0x8C30193F,  //  0026  GETMET	R12	R12	K63
      0x5C381400,  //  0027  MOVE	R14	R10
      0x5C3C1200,  //  0028  MOVE	R15	R9
      0x5840001A,  //  0029  LDCONST	R16	K26
      0x7C300800,  //  002A  CALL	R12	4
      0x5C2C1800,  //  002B  MOVE	R11	R12
      0x40322518,  //  002C  CONNECT	R12	K18	K24
      0x88340103,  //  002D  GETMBR	R13	R0	K3
      0x94301A0C,  //  002E  GETIDX	R12	R13	R12
      0x0030180B,  //  002F  ADD	R12	R12	R11
      0x88340104,  //  0030  GETMBR	R13	R0	K4
      0x40341B3D,  //  0031  CONNECT	R13	R13	K61
      0x9434140D,  //  0032  GETIDX	R13	R10	R13
      0x0030180D,  //  0033  ADD	R12	R12	R13
      0x9002060C,  //  0034  SETMBR	R0	K3	R12
      0x881C0106,  //  0035  GETMBR	R7	R0	K6
      0x881C0F07,  //  0036  GETMBR	R7	R7	K7
      0x8C200F08,  //  0037  GETMET	R8	R7	K8
      0x7C200200,  //  0038  CALL	R8	1
      0x8C200F09,  //  0039  GETMET	R8	R7	K9
      0x8828010A,  //  003A  GETMBR	R10	R0	K10
      0x582C000B,  //  003B  LDCONST	R11	K11
      0x7C200600,  //  003C  CALL	R8	3
      0x8C200F09,  //  003D  GETMET	R8	R7	K9
      0x8828010C,  //  003E  GETMBR	R10	R0	K12
      0x542E0003,  //  003F  LDINT	R11	4
      0x7C200600,  //  0040  CALL	R8	3
      0x8820011E,  //  0041  GETMBR	R8	R0	K30
      0x78220002,  //  0042  JMPF	R8	#0046
      0x8820011E,  //  0043  GETMBR	R8	R0	K30
      0x40200E08,  //  0044  CONNECT	R8	R7	R8
      0x70020006,  //  0045  JMP		#004D
      0x88200540,  //  0046  GETMBR	R8	R2	K64
      0x78220001,  //  0047  JMPF	R8	#004A
      0x88200540,  //  0048  GETMBR	R8	R2	K64
      0x40200E08,  //  0049  CONNECT	R8	R7	R8
      0x8C200F0F,  //  004A  GETMET	R8	R7	K15
      0x542A000C,  //  004B  LDINT	R10	13
      0x7C200400,  //  004C  CALL	R8	2
      0x88200310,  //  004D  GETMBR	R8	R1	K16
      0x8C201141,  //  004E  GETMET	R8	R8	K65
      0x5C280C00,  //  004F  MOVE	R10	R6
      0x5C2C0E00,  //  0050  MOVE	R11	R7
      0x58300012,  //  0051  LDCONST	R12	K18
      0x6034000C,  //  0052  GETGBL	R13	G12
      0x5C380E00,  //  0053  MOVE	R14	R7
      0x7C340200,  //  0054  CALL	R13	1
      0x5C380600,  //  0055  MOVE	R14	R3
      0x583C0012,  //  0056  LDCONST	R15	K18
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
      0x8C24070F,  //  0067  GETMET	R9	R3	K15
      0x602C000C,  //  0068  GETGBL	R11	G12
      0x5C300600,  //  0069  MOVE	R12	R3
      0x7C2C0200,  //  006A  CALL	R11	1
      0x042C1605,  //  006B  SUB	R11	R11	R5
      0x7C240400,  //  006C  CALL	R9	2
      0x70020003,  //  006D  JMP		#0072
      0xB8267000,  //  006E  GETNGBL	R9	K56
      0x58280042,  //  006F  LDCONST	R10	K66
      0x582C0018,  //  0070  LDCONST	R11	K24
      0x7C240400,  //  0071  CALL	R9	2
      0x80041000,  //  0072  RET	1	R8
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: build_standalone_ack
********************************************************************/
be_local_closure(class_Matter_Frame_build_standalone_ack,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Frame,     /* shared constants */
    be_str_weak(build_standalone_ack),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x60080006,  //  0000  GETGBL	R2	G6
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x880C0106,  //  0003  GETMBR	R3	R0	K6
      0x7C080200,  //  0004  CALL	R2	1
      0x880C0113,  //  0005  GETMBR	R3	R0	K19
      0x900A2603,  //  0006  SETMBR	R2	K19	R3
      0x880C0114,  //  0007  GETMBR	R3	R0	K20
      0x900A2803,  //  0008  SETMBR	R2	K20	R3
      0x880C0116,  //  0009  GETMBR	R3	R0	K22
      0x780E0003,  //  000A  JMPF	R3	#000F
      0x900A2F0B,  //  000B  SETMBR	R2	K23	K11
      0x880C011E,  //  000C  GETMBR	R3	R0	K30
      0x900A3E03,  //  000D  SETMBR	R2	K31	R3
      0x70020000,  //  000E  JMP		#0010
      0x900A2F12,  //  000F  SETMBR	R2	K23	K18
      0x880C0100,  //  0010  GETMBR	R3	R0	K0
      0x900A0003,  //  0011  SETMBR	R2	K0	R3
      0x880C0100,  //  0012  GETMBR	R3	R0	K0
      0x8C0C0733,  //  0013  GETMET	R3	R3	K51
      0x7C0C0200,  //  0014  CALL	R3	1
      0x900A1803,  //  0015  SETMBR	R2	K12	R3
      0x880C0100,  //  0016  GETMBR	R3	R0	K0
      0x880C0732,  //  0017  GETMBR	R3	R3	K50
      0x900A3203,  //  0018  SETMBR	R2	K25	R3
      0x880C0125,  //  0019  GETMBR	R3	R0	K37
      0x780E0001,  //  001A  JMPF	R3	#001D
      0x580C0012,  //  001B  LDCONST	R3	K18
      0x70020000,  //  001C  JMP		#001E
      0x580C000B,  //  001D  LDCONST	R3	K11
      0x900A4A03,  //  001E  SETMBR	R2	K37	R3
      0x540E000F,  //  001F  LDINT	R3	16
      0x900A4C03,  //  0020  SETMBR	R2	K38	R3
      0x880C0127,  //  0021  GETMBR	R3	R0	K39
      0x900A4E03,  //  0022  SETMBR	R2	K39	R3
      0x900A5112,  //  0023  SETMBR	R2	K40	K18
      0x900A490B,  //  0024  SETMBR	R2	K36	K11
      0x880C010C,  //  0025  GETMBR	R3	R0	K12
      0x900A5203,  //  0026  SETMBR	R2	K41	R3
      0x78060001,  //  0027  JMPF	R1	#002A
      0x580C000B,  //  0028  LDCONST	R3	K11
      0x70020000,  //  0029  JMP		#002B
      0x580C0012,  //  002A  LDCONST	R3	K18
      0x900A4603,  //  002B  SETMBR	R2	K35	R3
      0x80040400,  //  002C  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: initiate_response
********************************************************************/
be_local_closure(class_Matter_Frame_initiate_response,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    5,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Frame,     /* shared constants */
    be_str_weak(initiate_response),
    &be_const_str_solidified,
    ( &(const binstruction[46]) {  /* code */
      0x58140043,  //  0000  LDCONST	R5	K67
      0x4C180000,  //  0001  LDNIL	R6
      0x1C180806,  //  0002  EQ	R6	R4	R6
      0x781A0004,  //  0003  JMPF	R6	#0009
      0xB81A5C00,  //  0004  GETNGBL	R6	K46
      0x8C180D2F,  //  0005  GETMET	R6	R6	K47
      0x5C200000,  //  0006  MOVE	R8	R0
      0x7C180400,  //  0007  CALL	R6	2
      0x5C100C00,  //  0008  MOVE	R4	R6
      0x88180344,  //  0009  GETMBR	R6	R1	K68
      0x90122606,  //  000A  SETMBR	R4	K19	R6
      0x88180345,  //  000B  GETMBR	R6	R1	K69
      0x90122806,  //  000C  SETMBR	R4	K20	R6
      0x90122F12,  //  000D  SETMBR	R4	K23	K18
      0x90120001,  //  000E  SETMBR	R4	K0	R1
      0x78060008,  //  000F  JMPF	R1	#0019
      0x88180332,  //  0010  GETMBR	R6	R1	K50
      0x20180D12,  //  0011  NE	R6	R6	K18
      0x781A0005,  //  0012  JMPF	R6	#0019
      0x8C180333,  //  0013  GETMET	R6	R1	K51
      0x7C180200,  //  0014  CALL	R6	1
      0x90121806,  //  0015  SETMBR	R4	K12	R6
      0x88180332,  //  0016  GETMBR	R6	R1	K50
      0x90123206,  //  0017  SETMBR	R4	K25	R6
      0x70020004,  //  0018  JMP		#001E
      0x88180334,  //  0019  GETMBR	R6	R1	K52
      0x8C180D35,  //  001A  GETMET	R6	R6	K53
      0x7C180200,  //  001B  CALL	R6	1
      0x90121806,  //  001C  SETMBR	R4	K12	R6
      0x90123312,  //  001D  SETMBR	R4	K25	K18
      0x90124B0B,  //  001E  SETMBR	R4	K37	K11
      0x90124C02,  //  001F  SETMBR	R4	K38	R2
      0x88180346,  //  0020  GETMBR	R6	R1	K70
      0x00180D0B,  //  0021  ADD	R6	R6	K11
      0x90068C06,  //  0022  SETMBR	R1	K70	R6
      0x88180346,  //  0023  GETMBR	R6	R1	K70
      0x541EFFFF,  //  0024  LDINT	R7	65536
      0x30180C07,  //  0025  OR	R6	R6	R7
      0x90124E06,  //  0026  SETMBR	R4	K39	R6
      0x9012510B,  //  0027  SETMBR	R4	K40	K11
      0x780E0001,  //  0028  JMPF	R3	#002B
      0x5818000B,  //  0029  LDCONST	R6	K11
      0x70020000,  //  002A  JMP		#002C
      0x58180012,  //  002B  LDCONST	R6	K18
      0x90124606,  //  002C  SETMBR	R4	K35	R6
      0x80040800,  //  002D  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: decode_payload
********************************************************************/
be_local_closure(class_Matter_Frame_decode_payload,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Frame,     /* shared constants */
    be_str_weak(decode_payload),
    &be_const_str_solidified,
    ( &(const binstruction[93]) {  /* code */
      0x88040104,  //  0000  GETMBR	R1	R0	K4
      0x88080103,  //  0001  GETMBR	R2	R0	K3
      0x8C0C052B,  //  0002  GETMET	R3	R2	K43
      0x5C140200,  //  0003  MOVE	R5	R1
      0x5818000B,  //  0004  LDCONST	R6	K11
      0x7C0C0600,  //  0005  CALL	R3	3
      0x90024203,  //  0006  SETMBR	R0	K33	R3
      0x8C0C052C,  //  0007  GETMET	R3	R2	K44
      0x54160007,  //  0008  LDINT	R5	8
      0x08140205,  //  0009  MUL	R5	R1	R5
      0x541A0003,  //  000A  LDINT	R6	4
      0x00140A06,  //  000B  ADD	R5	R5	R6
      0x5818000B,  //  000C  LDCONST	R6	K11
      0x7C0C0600,  //  000D  CALL	R3	3
      0x90024403,  //  000E  SETMBR	R0	K34	R3
      0x8C0C052C,  //  000F  GETMET	R3	R2	K44
      0x54160007,  //  0010  LDINT	R5	8
      0x08140205,  //  0011  MUL	R5	R1	R5
      0x00140B18,  //  0012  ADD	R5	R5	K24
      0x5818000B,  //  0013  LDCONST	R6	K11
      0x7C0C0600,  //  0014  CALL	R3	3
      0x90028E03,  //  0015  SETMBR	R0	K71	R3
      0x8C0C052C,  //  0016  GETMET	R3	R2	K44
      0x54160007,  //  0017  LDINT	R5	8
      0x08140205,  //  0018  MUL	R5	R1	R5
      0x00140B1A,  //  0019  ADD	R5	R5	K26
      0x5818000B,  //  001A  LDCONST	R6	K11
      0x7C0C0600,  //  001B  CALL	R3	3
      0x90024603,  //  001C  SETMBR	R0	K35	R3
      0x8C0C052C,  //  001D  GETMET	R3	R2	K44
      0x54160007,  //  001E  LDINT	R5	8
      0x08140205,  //  001F  MUL	R5	R1	R5
      0x00140B0B,  //  0020  ADD	R5	R5	K11
      0x5818000B,  //  0021  LDCONST	R6	K11
      0x7C0C0600,  //  0022  CALL	R3	3
      0x90024803,  //  0023  SETMBR	R0	K36	R3
      0x8C0C052C,  //  0024  GETMET	R3	R2	K44
      0x54160007,  //  0025  LDINT	R5	8
      0x08140205,  //  0026  MUL	R5	R1	R5
      0x5818000B,  //  0027  LDCONST	R6	K11
      0x7C0C0600,  //  0028  CALL	R3	3
      0x90024A03,  //  0029  SETMBR	R0	K37	R3
      0x8C0C052B,  //  002A  GETMET	R3	R2	K43
      0x0014030B,  //  002B  ADD	R5	R1	K11
      0x5818000B,  //  002C  LDCONST	R6	K11
      0x7C0C0600,  //  002D  CALL	R3	3
      0x90024C03,  //  002E  SETMBR	R0	K38	R3
      0x8C0C052B,  //  002F  GETMET	R3	R2	K43
      0x0014031A,  //  0030  ADD	R5	R1	K26
      0x5818001A,  //  0031  LDCONST	R6	K26
      0x7C0C0600,  //  0032  CALL	R3	3
      0x90024E03,  //  0033  SETMBR	R0	K39	R3
      0x880C0125,  //  0034  GETMBR	R3	R0	K37
      0x740E0003,  //  0035  JMPT	R3	#003A
      0x880C0127,  //  0036  GETMBR	R3	R0	K39
      0x5412FFFF,  //  0037  LDINT	R4	65536
      0x300C0604,  //  0038  OR	R3	R3	R4
      0x90024E03,  //  0039  SETMBR	R0	K39	R3
      0x8C0C052B,  //  003A  GETMET	R3	R2	K43
      0x54160003,  //  003B  LDINT	R5	4
      0x00140205,  //  003C  ADD	R5	R1	R5
      0x5818001A,  //  003D  LDCONST	R6	K26
      0x7C0C0600,  //  003E  CALL	R3	3
      0x90025003,  //  003F  SETMBR	R0	K40	R3
      0x540E0005,  //  0040  LDINT	R3	6
      0x00040203,  //  0041  ADD	R1	R1	R3
      0x880C0122,  //  0042  GETMBR	R3	R0	K34
      0x780E0005,  //  0043  JMPF	R3	#004A
      0x8C0C052B,  //  0044  GETMET	R3	R2	K43
      0x5C140200,  //  0045  MOVE	R5	R1
      0x5818001A,  //  0046  LDCONST	R6	K26
      0x7C0C0600,  //  0047  CALL	R3	3
      0x90029003,  //  0048  SETMBR	R0	K72	R3
      0x0004031A,  //  0049  ADD	R1	R1	K26
      0x880C0124,  //  004A  GETMBR	R3	R0	K36
      0x780E0006,  //  004B  JMPF	R3	#0053
      0x8C0C052B,  //  004C  GETMET	R3	R2	K43
      0x5C140200,  //  004D  MOVE	R5	R1
      0x541A0003,  //  004E  LDINT	R6	4
      0x7C0C0600,  //  004F  CALL	R3	3
      0x90025203,  //  0050  SETMBR	R0	K41	R3
      0x540E0003,  //  0051  LDINT	R3	4
      0x00040203,  //  0052  ADD	R1	R1	R3
      0x880C0147,  //  0053  GETMBR	R3	R0	K71
      0x780E0005,  //  0054  JMPF	R3	#005B
      0x8C0C052B,  //  0055  GETMET	R3	R2	K43
      0x5C140200,  //  0056  MOVE	R5	R1
      0x5818001A,  //  0057  LDCONST	R6	K26
      0x7C0C0600,  //  0058  CALL	R3	3
      0x0010071A,  //  0059  ADD	R4	R3	K26
      0x00040204,  //  005A  ADD	R1	R1	R4
      0x90025401,  //  005B  SETMBR	R0	K42	R1
      0x80040000,  //  005C  RET	1	R0
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
    be_nested_map(43,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(decode_payload, 35), be_const_closure(class_Matter_Frame_decode_payload_closure) },
        { be_const_key_weak(raw, -1), be_const_var(2) },
        { be_const_key_weak(sec_c, 30), be_const_var(10) },
        { be_const_key_weak(message_counter, 11), be_const_var(13) },
        { be_const_key_weak(message_handler, -1), be_const_var(0) },
        { be_const_key_weak(remote_ip, 4), be_const_var(30) },
        { be_const_key_weak(x_flag_sx, -1), be_const_var(19) },
        { be_const_key_weak(get_node_id, -1), be_const_closure(class_Matter_Frame_get_node_id_closure) },
        { be_const_key_weak(sec_mx, -1), be_const_var(11) },
        { be_const_key_weak(flag_dsiz, -1), be_const_var(6) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Frame_init_closure) },
        { be_const_key_weak(x_flag_r, 9), be_const_var(20) },
        { be_const_key_weak(encode_frame, 36), be_const_closure(class_Matter_Frame_encode_frame_closure) },
        { be_const_key_weak(exchange_id, -1), be_const_var(24) },
        { be_const_key_weak(x_flag_v, -1), be_const_var(18) },
        { be_const_key_weak(sec_extensions, -1), be_const_var(28) },
        { be_const_key_weak(opcode, -1), be_const_var(23) },
        { be_const_key_weak(local_session_id, -1), be_const_var(7) },
        { be_const_key_weak(decode_header, -1), be_const_closure(class_Matter_Frame_decode_header_closure) },
        { be_const_key_weak(debug, 22), be_const_closure(class_Matter_Frame_debug_closure) },
        { be_const_key_weak(dest_node_id_8, -1), be_const_var(16) },
        { be_const_key_weak(vendor_id, 32), be_const_var(26) },
        { be_const_key_weak(initiate_response, -1), be_const_static_closure(class_Matter_Frame_initiate_response_closure) },
        { be_const_key_weak(build_response, -1), be_const_closure(class_Matter_Frame_build_response_closure) },
        { be_const_key_weak(payload_idx, -1), be_const_var(3) },
        { be_const_key_weak(encrypt, 23), be_const_closure(class_Matter_Frame_encrypt_closure) },
        { be_const_key_weak(dest_node_id_2, 21), be_const_var(15) },
        { be_const_key_weak(sec_p, -1), be_const_var(9) },
        { be_const_key_weak(sec_sesstype, 16), be_const_var(12) },
        { be_const_key_weak(protocol_id, -1), be_const_var(25) },
        { be_const_key_weak(build_standalone_ack, 38), be_const_closure(class_Matter_Frame_build_standalone_ack_closure) },
        { be_const_key_weak(remote_port, -1), be_const_var(31) },
        { be_const_key_weak(decrypt, -1), be_const_closure(class_Matter_Frame_decrypt_closure) },
        { be_const_key_weak(session, 24), be_const_var(1) },
        { be_const_key_weak(x_flag_i, -1), be_const_var(22) },
        { be_const_key_weak(flag_s, -1), be_const_var(5) },
        { be_const_key_weak(sec_flags, 37), be_const_var(8) },
        { be_const_key_weak(x_flag_a, -1), be_const_var(21) },
        { be_const_key_weak(app_payload_idx, -1), be_const_var(29) },
        { be_const_key_weak(flags, -1), be_const_var(4) },
        { be_const_key_weak(ack_message_counter, 8), be_const_var(27) },
        { be_const_key_weak(source_node_id, -1), be_const_var(14) },
        { be_const_key_weak(x_flags, 0), be_const_var(17) },
    })),
    be_str_weak(Matter_Frame)
);
/********************************************************************/
/* End of solidification */
