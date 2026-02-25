/* Solidification of Matter_Plugin_4_Light3.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Light3' ktab size: 64, total: 109 (saved 360 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Light3[64] = {
  /* K0   */  be_nested_str_weak(cluster),
  /* K1   */  be_nested_str_weak(attribute),
  /* K2   */  be_nested_str_weak(update_shadow_lazy),
  /* K3   */  be_const_int(0),
  /* K4   */  be_nested_str_weak(set_or_nil),
  /* K5   */  be_nested_str_weak(shadow_hue),
  /* K6   */  be_const_int(1),
  /* K7   */  be_nested_str_weak(shadow_sat),
  /* K8   */  be_nested_str_weak(set),
  /* K9   */  be_nested_str_weak(read_attribute),
  /* K10  */  be_nested_str_weak(BRIDGE),
  /* K11  */  be_nested_str_weak(tasmota),
  /* K12  */  be_nested_str_weak(scale_uint),
  /* K13  */  be_nested_str_weak(call_remote_sync),
  /* K14  */  be_nested_str_weak(HSBColor1),
  /* K15  */  be_nested_str_weak(parse_status),
  /* K16  */  be_nested_str_weak(HSBColor2),
  /* K17  */  be_nested_str_weak(VIRTUAL),
  /* K18  */  be_nested_str_weak(attribute_updated),
  /* K19  */  be_nested_str_weak(light),
  /* K20  */  be_nested_str_weak(hue),
  /* K21  */  be_nested_str_weak(sat),
  /* K22  */  be_nested_str_weak(light_index),
  /* K23  */  be_nested_str_weak(update_shadow),
  /* K24  */  be_nested_str_weak(find),
  /* K25  */  be_nested_str_weak(HSBColor),
  /* K26  */  be_nested_str_weak(string),
  /* K27  */  be_nested_str_weak(split),
  /* K28  */  be_nested_str_weak(_X2C),
  /* K29  */  be_nested_str_weak(matter),
  /* K30  */  be_nested_str_weak(TLV),
  /* K31  */  be_nested_str_weak(command),
  /* K32  */  be_nested_str_weak(findsubval),
  /* K33  */  be_nested_str_weak(set_hue_sat),
  /* K34  */  be_nested_str_weak(log),
  /* K35  */  be_nested_str_weak(hue_X3A),
  /* K36  */  be_nested_str_weak(publish_command),
  /* K37  */  be_nested_str_weak(Hue),
  /* K38  */  be_const_int(2),
  /* K39  */  be_const_int(3),
  /* K40  */  be_nested_str_weak(sat_X3A),
  /* K41  */  be_nested_str_weak(Sat),
  /* K42  */  be_nested_str_weak(_X20sat_X3A),
  /* K43  */  be_nested_str_weak(invoke_request),
  /* K44  */  be_nested_str_weak(update_virtual),
  /* K45  */  be_nested_str_weak(light_state),
  /* K46  */  be_nested_str_weak(set_bri),
  /* K47  */  be_nested_str_weak(set_huesat),
  /* K48  */  be_nested_str_weak(_X23_X2502X_X2502X_X2502X),
  /* K49  */  be_nested_str_weak(r),
  /* K50  */  be_nested_str_weak(g),
  /* K51  */  be_nested_str_weak(b),
  /* K52  */  be_nested_str_weak(_X3Ci_X20class_X3D_X22bxm_X22_X20style_X3D_X22_X2D_X2Dcl_X3A_X25s_X22_X3E_X3C_X2Fi_X3E_X25s),
  /* K53  */  be_nested_str_weak(),
  /* K54  */  be_nested_str_weak(webserver),
  /* K55  */  be_nested_str_weak(web_values_prefix),
  /* K56  */  be_nested_str_weak(content_send),
  /* K57  */  be_nested_str_weak(_X25s_X20_X25s_X20_X25s),
  /* K58  */  be_nested_str_weak(web_value_onoff),
  /* K59  */  be_nested_str_weak(shadow_onoff),
  /* K60  */  be_nested_str_weak(web_value_dimmer),
  /* K61  */  be_nested_str_weak(web_value_RGB),
  /* K62  */  be_nested_str_weak(init),
  /* K63  */  be_nested_str_weak(get),
};


extern const bclass be_class_Matter_Plugin_Light3;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(class_Matter_Plugin_Light3_read_attribute,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light3,     /* shared constants */
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[85]) {  /* code */
      0x88100500,  //  0000  GETMBR	R4	R2	K0
      0x88140501,  //  0001  GETMBR	R5	R2	K1
      0x541A02FF,  //  0002  LDINT	R6	768
      0x1C180806,  //  0003  EQ	R6	R4	R6
      0x781A0046,  //  0004  JMPF	R6	#004C
      0x8C180102,  //  0005  GETMET	R6	R0	K2
      0x7C180200,  //  0006  CALL	R6	1
      0x1C180B03,  //  0007  EQ	R6	R5	K3
      0x781A0005,  //  0008  JMPF	R6	#000F
      0x8C180704,  //  0009  GETMET	R6	R3	K4
      0x54220003,  //  000A  LDINT	R8	4
      0x88240105,  //  000B  GETMBR	R9	R0	K5
      0x7C180600,  //  000C  CALL	R6	3
      0x80040C00,  //  000D  RET	1	R6
      0x7002003C,  //  000E  JMP		#004C
      0x1C180B06,  //  000F  EQ	R6	R5	K6
      0x781A0005,  //  0010  JMPF	R6	#0017
      0x8C180704,  //  0011  GETMET	R6	R3	K4
      0x54220003,  //  0012  LDINT	R8	4
      0x88240107,  //  0013  GETMBR	R9	R0	K7
      0x7C180600,  //  0014  CALL	R6	3
      0x80040C00,  //  0015  RET	1	R6
      0x70020034,  //  0016  JMP		#004C
      0x541A0007,  //  0017  LDINT	R6	8
      0x1C180A06,  //  0018  EQ	R6	R5	R6
      0x781A0005,  //  0019  JMPF	R6	#0020
      0x8C180708,  //  001A  GETMET	R6	R3	K8
      0x54220003,  //  001B  LDINT	R8	4
      0x58240003,  //  001C  LDCONST	R9	K3
      0x7C180600,  //  001D  CALL	R6	3
      0x80040C00,  //  001E  RET	1	R6
      0x7002002B,  //  001F  JMP		#004C
      0x541A000E,  //  0020  LDINT	R6	15
      0x1C180A06,  //  0021  EQ	R6	R5	R6
      0x781A0005,  //  0022  JMPF	R6	#0029
      0x8C180708,  //  0023  GETMET	R6	R3	K8
      0x54220003,  //  0024  LDINT	R8	4
      0x58240003,  //  0025  LDCONST	R9	K3
      0x7C180600,  //  0026  CALL	R6	3
      0x80040C00,  //  0027  RET	1	R6
      0x70020022,  //  0028  JMP		#004C
      0x541A4000,  //  0029  LDINT	R6	16385
      0x1C180A06,  //  002A  EQ	R6	R5	R6
      0x781A0005,  //  002B  JMPF	R6	#0032
      0x8C180708,  //  002C  GETMET	R6	R3	K8
      0x54220003,  //  002D  LDINT	R8	4
      0x58240003,  //  002E  LDCONST	R9	K3
      0x7C180600,  //  002F  CALL	R6	3
      0x80040C00,  //  0030  RET	1	R6
      0x70020019,  //  0031  JMP		#004C
      0x541A4009,  //  0032  LDINT	R6	16394
      0x1C180A06,  //  0033  EQ	R6	R5	R6
      0x781A0005,  //  0034  JMPF	R6	#003B
      0x8C180708,  //  0035  GETMET	R6	R3	K8
      0x54220004,  //  0036  LDINT	R8	5
      0x58240006,  //  0037  LDCONST	R9	K6
      0x7C180600,  //  0038  CALL	R6	3
      0x80040C00,  //  0039  RET	1	R6
      0x70020010,  //  003A  JMP		#004C
      0x541A000F,  //  003B  LDINT	R6	16
      0x1C180A06,  //  003C  EQ	R6	R5	R6
      0x781A0005,  //  003D  JMPF	R6	#0044
      0x8C180708,  //  003E  GETMET	R6	R3	K8
      0x54220003,  //  003F  LDINT	R8	4
      0x58240003,  //  0040  LDCONST	R9	K3
      0x7C180600,  //  0041  CALL	R6	3
      0x80040C00,  //  0042  RET	1	R6
      0x70020007,  //  0043  JMP		#004C
      0x541AFFFB,  //  0044  LDINT	R6	65532
      0x1C180A06,  //  0045  EQ	R6	R5	R6
      0x781A0004,  //  0046  JMPF	R6	#004C
      0x8C180708,  //  0047  GETMET	R6	R3	K8
      0x54220005,  //  0048  LDINT	R8	6
      0x58240006,  //  0049  LDCONST	R9	K6
      0x7C180600,  //  004A  CALL	R6	3
      0x80040C00,  //  004B  RET	1	R6
      0x60180003,  //  004C  GETGBL	R6	G3
      0x5C1C0000,  //  004D  MOVE	R7	R0
      0x7C180200,  //  004E  CALL	R6	1
      0x8C180D09,  //  004F  GETMET	R6	R6	K9
      0x5C200200,  //  0050  MOVE	R8	R1
      0x5C240400,  //  0051  MOVE	R9	R2
      0x5C280600,  //  0052  MOVE	R10	R3
      0x7C180800,  //  0053  CALL	R6	4
      0x80040C00,  //  0054  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_hue_sat
********************************************************************/
be_local_closure(class_Matter_Plugin_Light3_set_hue_sat,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light3,     /* shared constants */
    be_str_weak(set_hue_sat),
    &be_const_str_solidified,
    ( &(const binstruction[154]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x200C0203,  //  0001  NE	R3	R1	R3
      0x780E0006,  //  0002  JMPF	R3	#000A
      0x140C0303,  //  0003  LT	R3	R1	K3
      0x780E0000,  //  0004  JMPF	R3	#0006
      0x58040003,  //  0005  LDCONST	R1	K3
      0x540E00FD,  //  0006  LDINT	R3	254
      0x240C0203,  //  0007  GT	R3	R1	R3
      0x780E0000,  //  0008  JMPF	R3	#000A
      0x540600FD,  //  0009  LDINT	R1	254
      0x4C0C0000,  //  000A  LDNIL	R3
      0x200C0403,  //  000B  NE	R3	R2	R3
      0x780E0006,  //  000C  JMPF	R3	#0014
      0x140C0503,  //  000D  LT	R3	R2	K3
      0x780E0000,  //  000E  JMPF	R3	#0010
      0x58080003,  //  000F  LDCONST	R2	K3
      0x540E00FD,  //  0010  LDINT	R3	254
      0x240C0403,  //  0011  GT	R3	R2	R3
      0x780E0000,  //  0012  JMPF	R3	#0014
      0x540A00FD,  //  0013  LDINT	R2	254
      0x880C010A,  //  0014  GETMBR	R3	R0	K10
      0x780E002C,  //  0015  JMPF	R3	#0043
      0x4C0C0000,  //  0016  LDNIL	R3
      0x200C0203,  //  0017  NE	R3	R1	R3
      0x780E0012,  //  0018  JMPF	R3	#002C
      0xB80E1600,  //  0019  GETNGBL	R3	K11
      0x8C0C070C,  //  001A  GETMET	R3	R3	K12
      0x5C140200,  //  001B  MOVE	R5	R1
      0x58180003,  //  001C  LDCONST	R6	K3
      0x541E00FD,  //  001D  LDINT	R7	254
      0x58200003,  //  001E  LDCONST	R8	K3
      0x54260167,  //  001F  LDINT	R9	360
      0x7C0C0C00,  //  0020  CALL	R3	6
      0x8C10010D,  //  0021  GETMET	R4	R0	K13
      0x5818000E,  //  0022  LDCONST	R6	K14
      0x5C1C0600,  //  0023  MOVE	R7	R3
      0x7C100600,  //  0024  CALL	R4	3
      0x4C140000,  //  0025  LDNIL	R5
      0x20140805,  //  0026  NE	R5	R4	R5
      0x78160003,  //  0027  JMPF	R5	#002C
      0x8C14010F,  //  0028  GETMET	R5	R0	K15
      0x5C1C0800,  //  0029  MOVE	R7	R4
      0x5422000A,  //  002A  LDINT	R8	11
      0x7C140600,  //  002B  CALL	R5	3
      0x4C0C0000,  //  002C  LDNIL	R3
      0x200C0403,  //  002D  NE	R3	R2	R3
      0x780E0012,  //  002E  JMPF	R3	#0042
      0xB80E1600,  //  002F  GETNGBL	R3	K11
      0x8C0C070C,  //  0030  GETMET	R3	R3	K12
      0x5C140400,  //  0031  MOVE	R5	R2
      0x58180003,  //  0032  LDCONST	R6	K3
      0x541E00FD,  //  0033  LDINT	R7	254
      0x58200003,  //  0034  LDCONST	R8	K3
      0x54260063,  //  0035  LDINT	R9	100
      0x7C0C0C00,  //  0036  CALL	R3	6
      0x8C10010D,  //  0037  GETMET	R4	R0	K13
      0x58180010,  //  0038  LDCONST	R6	K16
      0x5C1C0600,  //  0039  MOVE	R7	R3
      0x7C100600,  //  003A  CALL	R4	3
      0x4C140000,  //  003B  LDNIL	R5
      0x20140805,  //  003C  NE	R5	R4	R5
      0x78160003,  //  003D  JMPF	R5	#0042
      0x8C14010F,  //  003E  GETMET	R5	R0	K15
      0x5C1C0800,  //  003F  MOVE	R7	R4
      0x5422000A,  //  0040  LDINT	R8	11
      0x7C140600,  //  0041  CALL	R5	3
      0x70020055,  //  0042  JMP		#0099
      0x880C0111,  //  0043  GETMBR	R3	R0	K17
      0x780E0016,  //  0044  JMPF	R3	#005C
      0x4C0C0000,  //  0045  LDNIL	R3
      0x200C0203,  //  0046  NE	R3	R1	R3
      0x780E0007,  //  0047  JMPF	R3	#0050
      0x880C0105,  //  0048  GETMBR	R3	R0	K5
      0x200C0203,  //  0049  NE	R3	R1	R3
      0x780E0004,  //  004A  JMPF	R3	#0050
      0x8C0C0112,  //  004B  GETMET	R3	R0	K18
      0x541602FF,  //  004C  LDINT	R5	768
      0x58180003,  //  004D  LDCONST	R6	K3
      0x7C0C0600,  //  004E  CALL	R3	3
      0x90020A01,  //  004F  SETMBR	R0	K5	R1
      0x4C0C0000,  //  0050  LDNIL	R3
      0x200C0403,  //  0051  NE	R3	R2	R3
      0x780E0007,  //  0052  JMPF	R3	#005B
      0x880C0107,  //  0053  GETMBR	R3	R0	K7
      0x200C0403,  //  0054  NE	R3	R2	R3
      0x780E0004,  //  0055  JMPF	R3	#005B
      0x8C0C0112,  //  0056  GETMET	R3	R0	K18
      0x541602FF,  //  0057  LDINT	R5	768
      0x58180006,  //  0058  LDCONST	R6	K6
      0x7C0C0600,  //  0059  CALL	R3	3
      0x90020E02,  //  005A  SETMBR	R0	K7	R2
      0x7002003C,  //  005B  JMP		#0099
      0x4C0C0000,  //  005C  LDNIL	R3
      0x200C0203,  //  005D  NE	R3	R1	R3
      0x780E0008,  //  005E  JMPF	R3	#0068
      0xB80E1600,  //  005F  GETNGBL	R3	K11
      0x8C0C070C,  //  0060  GETMET	R3	R3	K12
      0x5C140200,  //  0061  MOVE	R5	R1
      0x58180003,  //  0062  LDCONST	R6	K3
      0x541E00FD,  //  0063  LDINT	R7	254
      0x58200003,  //  0064  LDCONST	R8	K3
      0x54260167,  //  0065  LDINT	R9	360
      0x7C0C0C00,  //  0066  CALL	R3	6
      0x70020000,  //  0067  JMP		#0069
      0x4C0C0000,  //  0068  LDNIL	R3
      0x4C100000,  //  0069  LDNIL	R4
      0x20100404,  //  006A  NE	R4	R2	R4
      0x78120008,  //  006B  JMPF	R4	#0075
      0xB8121600,  //  006C  GETNGBL	R4	K11
      0x8C10090C,  //  006D  GETMET	R4	R4	K12
      0x5C180400,  //  006E  MOVE	R6	R2
      0x581C0003,  //  006F  LDCONST	R7	K3
      0x542200FD,  //  0070  LDINT	R8	254
      0x58240003,  //  0071  LDCONST	R9	K3
      0x542A00FE,  //  0072  LDINT	R10	255
      0x7C100C00,  //  0073  CALL	R4	6
      0x70020000,  //  0074  JMP		#0076
      0x4C100000,  //  0075  LDNIL	R4
      0x4C140000,  //  0076  LDNIL	R5
      0x20140605,  //  0077  NE	R5	R3	R5
      0x7816000B,  //  0078  JMPF	R5	#0085
      0x4C140000,  //  0079  LDNIL	R5
      0x20140805,  //  007A  NE	R5	R4	R5
      0x78160008,  //  007B  JMPF	R5	#0085
      0xB8162600,  //  007C  GETNGBL	R5	K19
      0x8C140B08,  //  007D  GETMET	R5	R5	K8
      0x601C0013,  //  007E  GETGBL	R7	G19
      0x7C1C0000,  //  007F  CALL	R7	0
      0x981E2803,  //  0080  SETIDX	R7	K20	R3
      0x981E2A04,  //  0081  SETIDX	R7	K21	R4
      0x88200116,  //  0082  GETMBR	R8	R0	K22
      0x7C140600,  //  0083  CALL	R5	3
      0x70020011,  //  0084  JMP		#0097
      0x4C140000,  //  0085  LDNIL	R5
      0x20140605,  //  0086  NE	R5	R3	R5
      0x78160007,  //  0087  JMPF	R5	#0090
      0xB8162600,  //  0088  GETNGBL	R5	K19
      0x8C140B08,  //  0089  GETMET	R5	R5	K8
      0x601C0013,  //  008A  GETGBL	R7	G19
      0x7C1C0000,  //  008B  CALL	R7	0
      0x981E2803,  //  008C  SETIDX	R7	K20	R3
      0x88200116,  //  008D  GETMBR	R8	R0	K22
      0x7C140600,  //  008E  CALL	R5	3
      0x70020006,  //  008F  JMP		#0097
      0xB8162600,  //  0090  GETNGBL	R5	K19
      0x8C140B08,  //  0091  GETMET	R5	R5	K8
      0x601C0013,  //  0092  GETGBL	R7	G19
      0x7C1C0000,  //  0093  CALL	R7	0
      0x981E2A04,  //  0094  SETIDX	R7	K21	R4
      0x88200116,  //  0095  GETMBR	R8	R0	K22
      0x7C140600,  //  0096  CALL	R5	3
      0x8C140117,  //  0097  GETMET	R5	R0	K23
      0x7C140200,  //  0098  CALL	R5	1
      0x80000000,  //  0099  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_status
********************************************************************/
be_local_closure(class_Matter_Plugin_Light3_parse_status,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light3,     /* shared constants */
    be_str_weak(parse_status),
    &be_const_str_solidified,
    ( &(const binstruction[70]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C070F,  //  0003  GETMET	R3	R3	K15
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x540E000A,  //  0007  LDINT	R3	11
      0x1C0C0403,  //  0008  EQ	R3	R2	R3
      0x780E003A,  //  0009  JMPF	R3	#0045
      0x8C0C0318,  //  000A  GETMET	R3	R1	K24
      0x58140019,  //  000B  LDCONST	R5	K25
      0x7C0C0400,  //  000C  CALL	R3	2
      0x780E0036,  //  000D  JMPF	R3	#0045
      0xA4123400,  //  000E  IMPORT	R4	K26
      0x8C14091B,  //  000F  GETMET	R5	R4	K27
      0x5C1C0600,  //  0010  MOVE	R7	R3
      0x5820001C,  //  0011  LDCONST	R8	K28
      0x7C140600,  //  0012  CALL	R5	3
      0x60180009,  //  0013  GETGBL	R6	G9
      0x941C0B03,  //  0014  GETIDX	R7	R5	K3
      0x7C180200,  //  0015  CALL	R6	1
      0x601C0009,  //  0016  GETGBL	R7	G9
      0x94200B06,  //  0017  GETIDX	R8	R5	K6
      0x7C1C0200,  //  0018  CALL	R7	1
      0x4C200000,  //  0019  LDNIL	R8
      0x20200C08,  //  001A  NE	R8	R6	R8
      0x78220009,  //  001B  JMPF	R8	#0026
      0xB8221600,  //  001C  GETNGBL	R8	K11
      0x8C20110C,  //  001D  GETMET	R8	R8	K12
      0x5C280C00,  //  001E  MOVE	R10	R6
      0x582C0003,  //  001F  LDCONST	R11	K3
      0x54320167,  //  0020  LDINT	R12	360
      0x58340003,  //  0021  LDCONST	R13	K3
      0x543A00FD,  //  0022  LDINT	R14	254
      0x7C200C00,  //  0023  CALL	R8	6
      0x5C181000,  //  0024  MOVE	R6	R8
      0x70020000,  //  0025  JMP		#0027
      0x88180105,  //  0026  GETMBR	R6	R0	K5
      0x4C200000,  //  0027  LDNIL	R8
      0x20200E08,  //  0028  NE	R8	R7	R8
      0x78220009,  //  0029  JMPF	R8	#0034
      0xB8221600,  //  002A  GETNGBL	R8	K11
      0x8C20110C,  //  002B  GETMET	R8	R8	K12
      0x5C280E00,  //  002C  MOVE	R10	R7
      0x582C0003,  //  002D  LDCONST	R11	K3
      0x54320063,  //  002E  LDINT	R12	100
      0x58340003,  //  002F  LDCONST	R13	K3
      0x543A00FD,  //  0030  LDINT	R14	254
      0x7C200C00,  //  0031  CALL	R8	6
      0x5C1C1000,  //  0032  MOVE	R7	R8
      0x70020000,  //  0033  JMP		#0035
      0x881C0107,  //  0034  GETMBR	R7	R0	K7
      0x88200105,  //  0035  GETMBR	R8	R0	K5
      0x20200C08,  //  0036  NE	R8	R6	R8
      0x78220004,  //  0037  JMPF	R8	#003D
      0x8C200112,  //  0038  GETMET	R8	R0	K18
      0x542A02FF,  //  0039  LDINT	R10	768
      0x582C0003,  //  003A  LDCONST	R11	K3
      0x7C200600,  //  003B  CALL	R8	3
      0x90020A06,  //  003C  SETMBR	R0	K5	R6
      0x88200107,  //  003D  GETMBR	R8	R0	K7
      0x20200E08,  //  003E  NE	R8	R7	R8
      0x78220004,  //  003F  JMPF	R8	#0045
      0x8C200112,  //  0040  GETMET	R8	R0	K18
      0x542A02FF,  //  0041  LDINT	R10	768
      0x582C0006,  //  0042  LDCONST	R11	K6
      0x7C200600,  //  0043  CALL	R8	3
      0x90020E07,  //  0044  SETMBR	R0	K7	R7
      0x80000000,  //  0045  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(class_Matter_Plugin_Light3_invoke_request,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light3,     /* shared constants */
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[121]) {  /* code */
      0xB8123A00,  //  0000  GETNGBL	R4	K29
      0x8810091E,  //  0001  GETMBR	R4	R4	K30
      0x88140700,  //  0002  GETMBR	R5	R3	K0
      0x8818071F,  //  0003  GETMBR	R6	R3	K31
      0x541E02FF,  //  0004  LDINT	R7	768
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0067,  //  0006  JMPF	R7	#006F
      0x8C1C0102,  //  0007  GETMET	R7	R0	K2
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D03,  //  0009  EQ	R7	R6	K3
      0x781E0012,  //  000A  JMPF	R7	#001E
      0x8C1C0520,  //  000B  GETMET	R7	R2	K32
      0x58240003,  //  000C  LDCONST	R9	K3
      0x7C1C0400,  //  000D  CALL	R7	2
      0x8C200121,  //  000E  GETMET	R8	R0	K33
      0x5C280E00,  //  000F  MOVE	R10	R7
      0x4C2C0000,  //  0010  LDNIL	R11
      0x7C200600,  //  0011  CALL	R8	3
      0x60200008,  //  0012  GETGBL	R8	G8
      0x5C240E00,  //  0013  MOVE	R9	R7
      0x7C200200,  //  0014  CALL	R8	1
      0x00224608,  //  0015  ADD	R8	K35	R8
      0x900E4408,  //  0016  SETMBR	R3	K34	R8
      0x8C200124,  //  0017  GETMET	R8	R0	K36
      0x58280025,  //  0018  LDCONST	R10	K37
      0x5C2C0E00,  //  0019  MOVE	R11	R7
      0x7C200600,  //  001A  CALL	R8	3
      0x50200200,  //  001B  LDBOOL	R8	1	0
      0x80041000,  //  001C  RET	1	R8
      0x7002004F,  //  001D  JMP		#006E
      0x1C1C0D06,  //  001E  EQ	R7	R6	K6
      0x781E0002,  //  001F  JMPF	R7	#0023
      0x501C0200,  //  0020  LDBOOL	R7	1	0
      0x80040E00,  //  0021  RET	1	R7
      0x7002004A,  //  0022  JMP		#006E
      0x1C1C0D26,  //  0023  EQ	R7	R6	K38
      0x781E0002,  //  0024  JMPF	R7	#0028
      0x501C0200,  //  0025  LDBOOL	R7	1	0
      0x80040E00,  //  0026  RET	1	R7
      0x70020045,  //  0027  JMP		#006E
      0x1C1C0D27,  //  0028  EQ	R7	R6	K39
      0x781E0012,  //  0029  JMPF	R7	#003D
      0x8C1C0520,  //  002A  GETMET	R7	R2	K32
      0x58240003,  //  002B  LDCONST	R9	K3
      0x7C1C0400,  //  002C  CALL	R7	2
      0x8C200121,  //  002D  GETMET	R8	R0	K33
      0x4C280000,  //  002E  LDNIL	R10
      0x5C2C0E00,  //  002F  MOVE	R11	R7
      0x7C200600,  //  0030  CALL	R8	3
      0x60200008,  //  0031  GETGBL	R8	G8
      0x5C240E00,  //  0032  MOVE	R9	R7
      0x7C200200,  //  0033  CALL	R8	1
      0x00225008,  //  0034  ADD	R8	K40	R8
      0x900E4408,  //  0035  SETMBR	R3	K34	R8
      0x8C200124,  //  0036  GETMET	R8	R0	K36
      0x58280029,  //  0037  LDCONST	R10	K41
      0x5C2C0E00,  //  0038  MOVE	R11	R7
      0x7C200600,  //  0039  CALL	R8	3
      0x50200200,  //  003A  LDBOOL	R8	1	0
      0x80041000,  //  003B  RET	1	R8
      0x70020030,  //  003C  JMP		#006E
      0x541E0003,  //  003D  LDINT	R7	4
      0x1C1C0C07,  //  003E  EQ	R7	R6	R7
      0x781E0002,  //  003F  JMPF	R7	#0043
      0x501C0200,  //  0040  LDBOOL	R7	1	0
      0x80040E00,  //  0041  RET	1	R7
      0x7002002A,  //  0042  JMP		#006E
      0x541E0004,  //  0043  LDINT	R7	5
      0x1C1C0C07,  //  0044  EQ	R7	R6	R7
      0x781E0002,  //  0045  JMPF	R7	#0049
      0x501C0200,  //  0046  LDBOOL	R7	1	0
      0x80040E00,  //  0047  RET	1	R7
      0x70020024,  //  0048  JMP		#006E
      0x541E0005,  //  0049  LDINT	R7	6
      0x1C1C0C07,  //  004A  EQ	R7	R6	R7
      0x781E001C,  //  004B  JMPF	R7	#0069
      0x8C1C0520,  //  004C  GETMET	R7	R2	K32
      0x58240003,  //  004D  LDCONST	R9	K3
      0x7C1C0400,  //  004E  CALL	R7	2
      0x8C200520,  //  004F  GETMET	R8	R2	K32
      0x58280006,  //  0050  LDCONST	R10	K6
      0x7C200400,  //  0051  CALL	R8	2
      0x8C240121,  //  0052  GETMET	R9	R0	K33
      0x5C2C0E00,  //  0053  MOVE	R11	R7
      0x5C301000,  //  0054  MOVE	R12	R8
      0x7C240600,  //  0055  CALL	R9	3
      0x60240008,  //  0056  GETGBL	R9	G8
      0x5C280E00,  //  0057  MOVE	R10	R7
      0x7C240200,  //  0058  CALL	R9	1
      0x00264609,  //  0059  ADD	R9	K35	R9
      0x0024132A,  //  005A  ADD	R9	R9	K42
      0x60280008,  //  005B  GETGBL	R10	G8
      0x5C2C1000,  //  005C  MOVE	R11	R8
      0x7C280200,  //  005D  CALL	R10	1
      0x0024120A,  //  005E  ADD	R9	R9	R10
      0x900E4409,  //  005F  SETMBR	R3	K34	R9
      0x8C240124,  //  0060  GETMET	R9	R0	K36
      0x582C0025,  //  0061  LDCONST	R11	K37
      0x5C300E00,  //  0062  MOVE	R12	R7
      0x58340029,  //  0063  LDCONST	R13	K41
      0x5C381000,  //  0064  MOVE	R14	R8
      0x7C240A00,  //  0065  CALL	R9	5
      0x50240200,  //  0066  LDBOOL	R9	1	0
      0x80041200,  //  0067  RET	1	R9
      0x70020004,  //  0068  JMP		#006E
      0x541E0046,  //  0069  LDINT	R7	71
      0x1C1C0C07,  //  006A  EQ	R7	R6	R7
      0x781E0001,  //  006B  JMPF	R7	#006E
      0x501C0200,  //  006C  LDBOOL	R7	1	0
      0x80040E00,  //  006D  RET	1	R7
      0x70020008,  //  006E  JMP		#0078
      0x601C0003,  //  006F  GETGBL	R7	G3
      0x5C200000,  //  0070  MOVE	R8	R0
      0x7C1C0200,  //  0071  CALL	R7	1
      0x8C1C0F2B,  //  0072  GETMET	R7	R7	K43
      0x5C240200,  //  0073  MOVE	R9	R1
      0x5C280400,  //  0074  MOVE	R10	R2
      0x5C2C0600,  //  0075  MOVE	R11	R3
      0x7C1C0800,  //  0076  CALL	R7	4
      0x80040E00,  //  0077  RET	1	R7
      0x80000000,  //  0078  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_virtual
********************************************************************/
be_local_closure(class_Matter_Plugin_Light3_update_virtual,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light3,     /* shared constants */
    be_str_weak(update_virtual),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x8C0C0318,  //  0001  GETMET	R3	R1	K24
      0x58140025,  //  0002  LDCONST	R5	K37
      0x7C0C0400,  //  0003  CALL	R3	2
      0x7C080200,  //  0004  CALL	R2	1
      0x600C0009,  //  0005  GETGBL	R3	G9
      0x8C100318,  //  0006  GETMET	R4	R1	K24
      0x58180029,  //  0007  LDCONST	R6	K41
      0x7C100400,  //  0008  CALL	R4	2
      0x7C0C0200,  //  0009  CALL	R3	1
      0x4C100000,  //  000A  LDNIL	R4
      0x20100404,  //  000B  NE	R4	R2	R4
      0x74120002,  //  000C  JMPT	R4	#0010
      0x4C100000,  //  000D  LDNIL	R4
      0x20100604,  //  000E  NE	R4	R3	R4
      0x78120003,  //  000F  JMPF	R4	#0014
      0x8C100121,  //  0010  GETMET	R4	R0	K33
      0x5C180400,  //  0011  MOVE	R6	R2
      0x5C1C0600,  //  0012  MOVE	R7	R3
      0x7C100600,  //  0013  CALL	R4	3
      0x60100003,  //  0014  GETGBL	R4	G3
      0x5C140000,  //  0015  MOVE	R5	R0
      0x7C100200,  //  0016  CALL	R4	1
      0x8C10092C,  //  0017  GETMET	R4	R4	K44
      0x5C180200,  //  0018  MOVE	R6	R1
      0x7C100400,  //  0019  CALL	R4	2
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_value_RGB
********************************************************************/
be_local_closure(class_Matter_Plugin_Light3_web_value_RGB,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light3,     /* shared constants */
    be_str_weak(web_value_RGB),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x88040105,  //  0000  GETMBR	R1	R0	K5
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060027,  //  0003  JMPF	R1	#002C
      0x88040107,  //  0004  GETMBR	R1	R0	K7
      0x4C080000,  //  0005  LDNIL	R2
      0x20040202,  //  0006  NE	R1	R1	R2
      0x78060023,  //  0007  JMPF	R1	#002C
      0xB8065A00,  //  0008  GETNGBL	R1	K45
      0x58080027,  //  0009  LDCONST	R2	K39
      0x7C040200,  //  000A  CALL	R1	1
      0x8C08032E,  //  000B  GETMET	R2	R1	K46
      0x541200FE,  //  000C  LDINT	R4	255
      0x7C080400,  //  000D  CALL	R2	2
      0x8C08032F,  //  000E  GETMET	R2	R1	K47
      0xB8121600,  //  000F  GETNGBL	R4	K11
      0x8C10090C,  //  0010  GETMET	R4	R4	K12
      0x88180105,  //  0011  GETMBR	R6	R0	K5
      0x581C0003,  //  0012  LDCONST	R7	K3
      0x542200FD,  //  0013  LDINT	R8	254
      0x58240003,  //  0014  LDCONST	R9	K3
      0x542A0167,  //  0015  LDINT	R10	360
      0x7C100C00,  //  0016  CALL	R4	6
      0xB8161600,  //  0017  GETNGBL	R5	K11
      0x8C140B0C,  //  0018  GETMET	R5	R5	K12
      0x881C0107,  //  0019  GETMBR	R7	R0	K7
      0x58200003,  //  001A  LDCONST	R8	K3
      0x542600FD,  //  001B  LDINT	R9	254
      0x58280003,  //  001C  LDCONST	R10	K3
      0x542E00FE,  //  001D  LDINT	R11	255
      0x7C140C00,  //  001E  CALL	R5	6
      0x7C080600,  //  001F  CALL	R2	3
      0x60080018,  //  0020  GETGBL	R2	G24
      0x580C0030,  //  0021  LDCONST	R3	K48
      0x88100331,  //  0022  GETMBR	R4	R1	K49
      0x88140332,  //  0023  GETMBR	R5	R1	K50
      0x88180333,  //  0024  GETMBR	R6	R1	K51
      0x7C080800,  //  0025  CALL	R2	4
      0x600C0018,  //  0026  GETGBL	R3	G24
      0x58100034,  //  0027  LDCONST	R4	K52
      0x5C140400,  //  0028  MOVE	R5	R2
      0x5C180400,  //  0029  MOVE	R6	R2
      0x7C0C0600,  //  002A  CALL	R3	3
      0x80040600,  //  002B  RET	1	R3
      0x80066A00,  //  002C  RET	1	K53
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values
********************************************************************/
be_local_closure(class_Matter_Plugin_Light3_web_values,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light3,     /* shared constants */
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0xA4066C00,  //  0000  IMPORT	R1	K54
      0x8C080137,  //  0001  GETMET	R2	R0	K55
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080338,  //  0003  GETMET	R2	R1	K56
      0x60100018,  //  0004  GETGBL	R4	G24
      0x58140039,  //  0005  LDCONST	R5	K57
      0x8C18013A,  //  0006  GETMET	R6	R0	K58
      0x8820013B,  //  0007  GETMBR	R8	R0	K59
      0x7C180400,  //  0008  CALL	R6	2
      0x8C1C013C,  //  0009  GETMET	R7	R0	K60
      0x7C1C0200,  //  000A  CALL	R7	1
      0x8C20013D,  //  000B  GETMET	R8	R0	K61
      0x7C200200,  //  000C  CALL	R8	1
      0x7C100800,  //  000D  CALL	R4	4
      0x7C080400,  //  000E  CALL	R2	2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Plugin_Light3_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light3,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C10093E,  //  0003  GETMET	R4	R4	K62
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x90020B03,  //  0008  SETMBR	R0	K5	K3
      0x90020F03,  //  0009  SETMBR	R0	K7	K3
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(class_Matter_Plugin_Light3_update_shadow,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light3,     /* shared constants */
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[75]) {  /* code */
      0x88040111,  //  0000  GETMBR	R1	R0	K17
      0x74060042,  //  0001  JMPT	R1	#0045
      0x8804010A,  //  0002  GETMBR	R1	R0	K10
      0x74060040,  //  0003  JMPT	R1	#0045
      0xA4062600,  //  0004  IMPORT	R1	K19
      0x60080003,  //  0005  GETGBL	R2	G3
      0x5C0C0000,  //  0006  MOVE	R3	R0
      0x7C080200,  //  0007  CALL	R2	1
      0x8C080517,  //  0008  GETMET	R2	R2	K23
      0x7C080200,  //  0009  CALL	R2	1
      0x8C08033F,  //  000A  GETMET	R2	R1	K63
      0x88100116,  //  000B  GETMBR	R4	R0	K22
      0x7C080400,  //  000C  CALL	R2	2
      0x4C0C0000,  //  000D  LDNIL	R3
      0x200C0403,  //  000E  NE	R3	R2	R3
      0x780E0033,  //  000F  JMPF	R3	#0044
      0x8C0C0518,  //  0010  GETMET	R3	R2	K24
      0x58140014,  //  0011  LDCONST	R5	K20
      0x4C180000,  //  0012  LDNIL	R6
      0x7C0C0600,  //  0013  CALL	R3	3
      0x8C100518,  //  0014  GETMET	R4	R2	K24
      0x58180015,  //  0015  LDCONST	R6	K21
      0x4C1C0000,  //  0016  LDNIL	R7
      0x7C100600,  //  0017  CALL	R4	3
      0x4C140000,  //  0018  LDNIL	R5
      0x20140605,  //  0019  NE	R5	R3	R5
      0x78160009,  //  001A  JMPF	R5	#0025
      0xB8161600,  //  001B  GETNGBL	R5	K11
      0x8C140B0C,  //  001C  GETMET	R5	R5	K12
      0x5C1C0600,  //  001D  MOVE	R7	R3
      0x58200003,  //  001E  LDCONST	R8	K3
      0x54260167,  //  001F  LDINT	R9	360
      0x58280003,  //  0020  LDCONST	R10	K3
      0x542E00FD,  //  0021  LDINT	R11	254
      0x7C140C00,  //  0022  CALL	R5	6
      0x5C0C0A00,  //  0023  MOVE	R3	R5
      0x70020000,  //  0024  JMP		#0026
      0x880C0105,  //  0025  GETMBR	R3	R0	K5
      0x4C140000,  //  0026  LDNIL	R5
      0x20140805,  //  0027  NE	R5	R4	R5
      0x78160009,  //  0028  JMPF	R5	#0033
      0xB8161600,  //  0029  GETNGBL	R5	K11
      0x8C140B0C,  //  002A  GETMET	R5	R5	K12
      0x5C1C0800,  //  002B  MOVE	R7	R4
      0x58200003,  //  002C  LDCONST	R8	K3
      0x542600FE,  //  002D  LDINT	R9	255
      0x58280003,  //  002E  LDCONST	R10	K3
      0x542E00FD,  //  002F  LDINT	R11	254
      0x7C140C00,  //  0030  CALL	R5	6
      0x5C100A00,  //  0031  MOVE	R4	R5
      0x70020000,  //  0032  JMP		#0034
      0x88100107,  //  0033  GETMBR	R4	R0	K7
      0x88140105,  //  0034  GETMBR	R5	R0	K5
      0x20140605,  //  0035  NE	R5	R3	R5
      0x78160004,  //  0036  JMPF	R5	#003C
      0x8C140112,  //  0037  GETMET	R5	R0	K18
      0x541E02FF,  //  0038  LDINT	R7	768
      0x58200003,  //  0039  LDCONST	R8	K3
      0x7C140600,  //  003A  CALL	R5	3
      0x90020A03,  //  003B  SETMBR	R0	K5	R3
      0x88140107,  //  003C  GETMBR	R5	R0	K7
      0x20140805,  //  003D  NE	R5	R4	R5
      0x78160004,  //  003E  JMPF	R5	#0044
      0x8C140112,  //  003F  GETMET	R5	R0	K18
      0x541E02FF,  //  0040  LDINT	R7	768
      0x58200006,  //  0041  LDCONST	R8	K6
      0x7C140600,  //  0042  CALL	R5	3
      0x90020E04,  //  0043  SETMBR	R0	K7	R4
      0x70020004,  //  0044  JMP		#004A
      0x60040003,  //  0045  GETGBL	R1	G3
      0x5C080000,  //  0046  MOVE	R2	R0
      0x7C040200,  //  0047  CALL	R1	1
      0x8C040317,  //  0048  GETMET	R1	R1	K23
      0x7C040200,  //  0049  CALL	R1	1
      0x80000000,  //  004A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Light3
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light1;
be_local_class(Matter_Plugin_Light3,
    2,
    &be_class_Matter_Plugin_Light1,
    be_nested_map(17,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(SCHEMA, -1), be_const_nil() },
        { be_const_key_weak(update_shadow, 12), be_const_closure(class_Matter_Plugin_Light3_update_shadow_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Plugin_Light3_init_closure) },
        { be_const_key_weak(shadow_sat, 16), be_const_var(1) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(269, -1), be_const_int(4) },
    }))    ) } )) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Light_X203_X20RGB) },
        { be_const_key_weak(parse_status, 1), be_const_closure(class_Matter_Plugin_Light3_parse_status_closure) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(class_Matter_Plugin_Light3_invoke_request_closure) },
        { be_const_key_weak(update_virtual, -1), be_const_closure(class_Matter_Plugin_Light3_update_virtual_closure) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(8, 7), be_const_bytes_instance(000000020003000F0011FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(57, -1), be_const_bytes_instance(00030005000A000F00110012FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(29, -1), be_const_bytes_instance(0000000100020003FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(3, -1), be_const_bytes_instance(00000001FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(4, -1), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(5, 2), be_const_bytes_instance(000000010002000300040005FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(6, -1), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(768, -1), be_const_bytes_instance(000000010008000F4001400AFFF8FFF9FFFAFFFBFFFCFFFD) },
    }))    ) } )) },
        { be_const_key_weak(shadow_hue, -1), be_const_var(0) },
        { be_const_key_weak(read_attribute, 5), be_const_closure(class_Matter_Plugin_Light3_read_attribute_closure) },
        { be_const_key_weak(web_value_RGB, -1), be_const_closure(class_Matter_Plugin_Light3_web_value_RGB_closure) },
        { be_const_key_weak(web_values, -1), be_const_closure(class_Matter_Plugin_Light3_web_values_closure) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(light3) },
        { be_const_key_weak(UPDATE_COMMANDS, 2), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(4,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Power),
        be_nested_str_weak(Bri),
        be_nested_str_weak(Hue),
        be_nested_str_weak(Sat),
    }))    ) } )) },
        { be_const_key_weak(set_hue_sat, -1), be_const_closure(class_Matter_Plugin_Light3_set_hue_sat_closure) },
    })),
    be_str_weak(Matter_Plugin_Light3)
);
/********************************************************************/
/* End of solidification */
