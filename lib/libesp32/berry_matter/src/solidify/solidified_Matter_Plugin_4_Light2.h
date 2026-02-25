/* Solidification of Matter_Plugin_4_Light2.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Light2' ktab size: 50, total: 87 (saved 296 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Light2[50] = {
  /* K0   */  be_nested_str_weak(cluster),
  /* K1   */  be_nested_str_weak(attribute),
  /* K2   */  be_nested_str_weak(update_shadow_lazy),
  /* K3   */  be_nested_str_weak(set_or_nil),
  /* K4   */  be_nested_str_weak(shadow_ct),
  /* K5   */  be_nested_str_weak(set),
  /* K6   */  be_const_int(2),
  /* K7   */  be_const_int(0),
  /* K8   */  be_nested_str_weak(ct_min),
  /* K9   */  be_nested_str_weak(ct_max),
  /* K10  */  be_nested_str_weak(read_attribute),
  /* K11  */  be_nested_str_weak(tasmota),
  /* K12  */  be_nested_str_weak(get_option),
  /* K13  */  be_nested_str_weak(matter),
  /* K14  */  be_nested_str_weak(TLV),
  /* K15  */  be_nested_str_weak(command),
  /* K16  */  be_nested_str_weak(findsubval),
  /* K17  */  be_nested_str_weak(set_ct),
  /* K18  */  be_nested_str_weak(log),
  /* K19  */  be_nested_str_weak(ct_X3A),
  /* K20  */  be_nested_str_weak(publish_command),
  /* K21  */  be_nested_str_weak(CT),
  /* K22  */  be_nested_str_weak(invoke_request),
  /* K23  */  be_nested_str_weak(init),
  /* K24  */  be_nested_str_weak(BRIDGE),
  /* K25  */  be_nested_str_weak(light),
  /* K26  */  be_nested_str_weak(get),
  /* K27  */  be_const_int(1),
  /* K28  */  be_nested_str_weak(light_index),
  /* K29  */  be_nested_str_weak(update_ct_minmax),
  /* K30  */  be_nested_str_weak(),
  /* K31  */  be_const_int(1000000),
  /* K32  */  be_nested_str_weak(_X25iK),
  /* K33  */  be_nested_str_weak(_X26_X239898_X3B_X20),
  /* K34  */  be_nested_str_weak(VIRTUAL),
  /* K35  */  be_nested_str_weak(update_shadow),
  /* K36  */  be_nested_str_weak(find),
  /* K37  */  be_nested_str_weak(ct),
  /* K38  */  be_nested_str_weak(attribute_updated),
  /* K39  */  be_nested_str_weak(update_virtual),
  /* K40  */  be_nested_str_weak(webserver),
  /* K41  */  be_nested_str_weak(web_values_prefix),
  /* K42  */  be_nested_str_weak(content_send),
  /* K43  */  be_nested_str_weak(_X25s_X20_X25s_X20_X25s),
  /* K44  */  be_nested_str_weak(web_value_onoff),
  /* K45  */  be_nested_str_weak(shadow_onoff),
  /* K46  */  be_nested_str_weak(web_value_dimmer),
  /* K47  */  be_nested_str_weak(web_value_ct),
  /* K48  */  be_nested_str_weak(call_remote_sync),
  /* K49  */  be_nested_str_weak(parse_status),
};


extern const bclass be_class_Matter_Plugin_Light2;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(class_Matter_Plugin_Light2_read_attribute,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light2,     /* shared constants */
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[96]) {  /* code */
      0x88100500,  //  0000  GETMBR	R4	R2	K0
      0x88140501,  //  0001  GETMBR	R5	R2	K1
      0x541A02FF,  //  0002  LDINT	R6	768
      0x1C180806,  //  0003  EQ	R6	R4	R6
      0x781A0051,  //  0004  JMPF	R6	#0057
      0x8C180102,  //  0005  GETMET	R6	R0	K2
      0x7C180200,  //  0006  CALL	R6	1
      0x541A0006,  //  0007  LDINT	R6	7
      0x1C180A06,  //  0008  EQ	R6	R5	R6
      0x781A0005,  //  0009  JMPF	R6	#0010
      0x8C180703,  //  000A  GETMET	R6	R3	K3
      0x54220004,  //  000B  LDINT	R8	5
      0x88240104,  //  000C  GETMBR	R9	R0	K4
      0x7C180600,  //  000D  CALL	R6	3
      0x80040C00,  //  000E  RET	1	R6
      0x70020046,  //  000F  JMP		#0057
      0x541A0007,  //  0010  LDINT	R6	8
      0x1C180A06,  //  0011  EQ	R6	R5	R6
      0x781A0005,  //  0012  JMPF	R6	#0019
      0x8C180705,  //  0013  GETMET	R6	R3	K5
      0x54220003,  //  0014  LDINT	R8	4
      0x58240006,  //  0015  LDCONST	R9	K6
      0x7C180600,  //  0016  CALL	R6	3
      0x80040C00,  //  0017  RET	1	R6
      0x7002003D,  //  0018  JMP		#0057
      0x541A000E,  //  0019  LDINT	R6	15
      0x1C180A06,  //  001A  EQ	R6	R5	R6
      0x781A0005,  //  001B  JMPF	R6	#0022
      0x8C180705,  //  001C  GETMET	R6	R3	K5
      0x54220003,  //  001D  LDINT	R8	4
      0x58240007,  //  001E  LDCONST	R9	K7
      0x7C180600,  //  001F  CALL	R6	3
      0x80040C00,  //  0020  RET	1	R6
      0x70020034,  //  0021  JMP		#0057
      0x541A4000,  //  0022  LDINT	R6	16385
      0x1C180A06,  //  0023  EQ	R6	R5	R6
      0x781A0005,  //  0024  JMPF	R6	#002B
      0x8C180705,  //  0025  GETMET	R6	R3	K5
      0x54220003,  //  0026  LDINT	R8	4
      0x58240006,  //  0027  LDCONST	R9	K6
      0x7C180600,  //  0028  CALL	R6	3
      0x80040C00,  //  0029  RET	1	R6
      0x7002002B,  //  002A  JMP		#0057
      0x541A4009,  //  002B  LDINT	R6	16394
      0x1C180A06,  //  002C  EQ	R6	R5	R6
      0x781A0005,  //  002D  JMPF	R6	#0034
      0x8C180705,  //  002E  GETMET	R6	R3	K5
      0x54220004,  //  002F  LDINT	R8	5
      0x5426000F,  //  0030  LDINT	R9	16
      0x7C180600,  //  0031  CALL	R6	3
      0x80040C00,  //  0032  RET	1	R6
      0x70020022,  //  0033  JMP		#0057
      0x541A400A,  //  0034  LDINT	R6	16395
      0x1C180A06,  //  0035  EQ	R6	R5	R6
      0x781A0005,  //  0036  JMPF	R6	#003D
      0x8C180705,  //  0037  GETMET	R6	R3	K5
      0x54220004,  //  0038  LDINT	R8	5
      0x88240108,  //  0039  GETMBR	R9	R0	K8
      0x7C180600,  //  003A  CALL	R6	3
      0x80040C00,  //  003B  RET	1	R6
      0x70020019,  //  003C  JMP		#0057
      0x541A400B,  //  003D  LDINT	R6	16396
      0x1C180A06,  //  003E  EQ	R6	R5	R6
      0x781A0005,  //  003F  JMPF	R6	#0046
      0x8C180705,  //  0040  GETMET	R6	R3	K5
      0x54220004,  //  0041  LDINT	R8	5
      0x88240109,  //  0042  GETMBR	R9	R0	K9
      0x7C180600,  //  0043  CALL	R6	3
      0x80040C00,  //  0044  RET	1	R6
      0x70020010,  //  0045  JMP		#0057
      0x541A000F,  //  0046  LDINT	R6	16
      0x1C180A06,  //  0047  EQ	R6	R5	R6
      0x781A0005,  //  0048  JMPF	R6	#004F
      0x8C180705,  //  0049  GETMET	R6	R3	K5
      0x54220003,  //  004A  LDINT	R8	4
      0x58240007,  //  004B  LDCONST	R9	K7
      0x7C180600,  //  004C  CALL	R6	3
      0x80040C00,  //  004D  RET	1	R6
      0x70020007,  //  004E  JMP		#0057
      0x541AFFFB,  //  004F  LDINT	R6	65532
      0x1C180A06,  //  0050  EQ	R6	R5	R6
      0x781A0004,  //  0051  JMPF	R6	#0057
      0x8C180705,  //  0052  GETMET	R6	R3	K5
      0x54220005,  //  0053  LDINT	R8	6
      0x5426000F,  //  0054  LDINT	R9	16
      0x7C180600,  //  0055  CALL	R6	3
      0x80040C00,  //  0056  RET	1	R6
      0x60180003,  //  0057  GETGBL	R6	G3
      0x5C1C0000,  //  0058  MOVE	R7	R0
      0x7C180200,  //  0059  CALL	R6	1
      0x8C180D0A,  //  005A  GETMET	R6	R6	K10
      0x5C200200,  //  005B  MOVE	R8	R1
      0x5C240400,  //  005C  MOVE	R9	R2
      0x5C280600,  //  005D  MOVE	R10	R3
      0x7C180800,  //  005E  CALL	R6	4
      0x80040C00,  //  005F  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_ct_minmax
********************************************************************/
be_local_closure(class_Matter_Plugin_Light2_update_ct_minmax,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light2,     /* shared constants */
    be_str_weak(update_ct_minmax),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xB8061600,  //  0000  GETNGBL	R1	K11
      0x8C04030C,  //  0001  GETMET	R1	R1	K12
      0x540E0051,  //  0002  LDINT	R3	82
      0x7C040400,  //  0003  CALL	R1	2
      0x78060001,  //  0004  JMPF	R1	#0007
      0x540A00C7,  //  0005  LDINT	R2	200
      0x70020000,  //  0006  JMP		#0008
      0x540A0098,  //  0007  LDINT	R2	153
      0x90021002,  //  0008  SETMBR	R0	K8	R2
      0x78060001,  //  0009  JMPF	R1	#000C
      0x540A017B,  //  000A  LDINT	R2	380
      0x70020000,  //  000B  JMP		#000D
      0x540A01F3,  //  000C  LDINT	R2	500
      0x90021202,  //  000D  SETMBR	R0	K9	R2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(class_Matter_Plugin_Light2_invoke_request,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light2,     /* shared constants */
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[66]) {  /* code */
      0xB8121A00,  //  0000  GETNGBL	R4	K13
      0x8810090E,  //  0001  GETMBR	R4	R4	K14
      0x88140700,  //  0002  GETMBR	R5	R3	K0
      0x8818070F,  //  0003  GETMBR	R6	R3	K15
      0x541E02FF,  //  0004  LDINT	R7	768
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0030,  //  0006  JMPF	R7	#0038
      0x8C1C0102,  //  0007  GETMET	R7	R0	K2
      0x7C1C0200,  //  0008  CALL	R7	1
      0x541E0009,  //  0009  LDINT	R7	10
      0x1C1C0C07,  //  000A  EQ	R7	R6	R7
      0x781E0019,  //  000B  JMPF	R7	#0026
      0x8C1C0510,  //  000C  GETMET	R7	R2	K16
      0x58240007,  //  000D  LDCONST	R9	K7
      0x7C1C0400,  //  000E  CALL	R7	2
      0x88200108,  //  000F  GETMBR	R8	R0	K8
      0x14200E08,  //  0010  LT	R8	R7	R8
      0x78220000,  //  0011  JMPF	R8	#0013
      0x881C0108,  //  0012  GETMBR	R7	R0	K8
      0x88200109,  //  0013  GETMBR	R8	R0	K9
      0x24200E08,  //  0014  GT	R8	R7	R8
      0x78220000,  //  0015  JMPF	R8	#0017
      0x881C0109,  //  0016  GETMBR	R7	R0	K9
      0x8C200111,  //  0017  GETMET	R8	R0	K17
      0x5C280E00,  //  0018  MOVE	R10	R7
      0x7C200400,  //  0019  CALL	R8	2
      0x60200008,  //  001A  GETGBL	R8	G8
      0x5C240E00,  //  001B  MOVE	R9	R7
      0x7C200200,  //  001C  CALL	R8	1
      0x00222608,  //  001D  ADD	R8	K19	R8
      0x900E2408,  //  001E  SETMBR	R3	K18	R8
      0x8C200114,  //  001F  GETMET	R8	R0	K20
      0x58280015,  //  0020  LDCONST	R10	K21
      0x5C2C0E00,  //  0021  MOVE	R11	R7
      0x7C200600,  //  0022  CALL	R8	3
      0x50200200,  //  0023  LDBOOL	R8	1	0
      0x80041000,  //  0024  RET	1	R8
      0x70020010,  //  0025  JMP		#0037
      0x541E0046,  //  0026  LDINT	R7	71
      0x1C1C0C07,  //  0027  EQ	R7	R6	R7
      0x781E0002,  //  0028  JMPF	R7	#002C
      0x501C0200,  //  0029  LDBOOL	R7	1	0
      0x80040E00,  //  002A  RET	1	R7
      0x7002000A,  //  002B  JMP		#0037
      0x541E004A,  //  002C  LDINT	R7	75
      0x1C1C0C07,  //  002D  EQ	R7	R6	R7
      0x781E0002,  //  002E  JMPF	R7	#0032
      0x501C0200,  //  002F  LDBOOL	R7	1	0
      0x80040E00,  //  0030  RET	1	R7
      0x70020004,  //  0031  JMP		#0037
      0x541E004B,  //  0032  LDINT	R7	76
      0x1C1C0C07,  //  0033  EQ	R7	R6	R7
      0x781E0001,  //  0034  JMPF	R7	#0037
      0x501C0200,  //  0035  LDBOOL	R7	1	0
      0x80040E00,  //  0036  RET	1	R7
      0x70020008,  //  0037  JMP		#0041
      0x601C0003,  //  0038  GETGBL	R7	G3
      0x5C200000,  //  0039  MOVE	R8	R0
      0x7C1C0200,  //  003A  CALL	R7	1
      0x8C1C0F16,  //  003B  GETMET	R7	R7	K22
      0x5C240200,  //  003C  MOVE	R9	R1
      0x5C280400,  //  003D  MOVE	R10	R2
      0x5C2C0600,  //  003E  MOVE	R11	R3
      0x7C1C0800,  //  003F  CALL	R7	4
      0x80040E00,  //  0040  RET	1	R7
      0x80000000,  //  0041  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Plugin_Light2_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light2,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C100917,  //  0003  GETMET	R4	R4	K23
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x88100118,  //  0008  GETMBR	R4	R0	K24
      0x74120009,  //  0009  JMPT	R4	#0014
      0x54120144,  //  000A  LDINT	R4	325
      0x90020804,  //  000B  SETMBR	R0	K4	R4
      0xA4123200,  //  000C  IMPORT	R4	K25
      0x8C14091A,  //  000D  GETMET	R5	R4	K26
      0x581C001B,  //  000E  LDCONST	R7	K27
      0x7C140400,  //  000F  CALL	R5	2
      0x4C180000,  //  0010  LDNIL	R6
      0x20140A06,  //  0011  NE	R5	R5	R6
      0x78160000,  //  0012  JMPF	R5	#0014
      0x9002391B,  //  0013  SETMBR	R0	K28	K27
      0x8C10011D,  //  0014  GETMET	R4	R0	K29
      0x7C100200,  //  0015  CALL	R4	1
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_value_ct
********************************************************************/
be_local_closure(class_Matter_Plugin_Light2_web_value_ct,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light2,     /* shared constants */
    be_str_weak(web_value_ct),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x5804001E,  //  0000  LDCONST	R1	K30
      0x88080104,  //  0001  GETMBR	R2	R0	K4
      0x4C0C0000,  //  0002  LDNIL	R3
      0x20080403,  //  0003  NE	R2	R2	R3
      0x780A000C,  //  0004  JMPF	R2	#0012
      0x88080104,  //  0005  GETMBR	R2	R0	K4
      0x0C0A3E02,  //  0006  DIV	R2	K31	R2
      0x540E0018,  //  0007  LDINT	R3	25
      0x00080403,  //  0008  ADD	R2	R2	R3
      0x540E0031,  //  0009  LDINT	R3	50
      0x0C080403,  //  000A  DIV	R2	R2	R3
      0x540E0031,  //  000B  LDINT	R3	50
      0x08080403,  //  000C  MUL	R2	R2	R3
      0x600C0018,  //  000D  GETGBL	R3	G24
      0x58100020,  //  000E  LDCONST	R4	K32
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x5C040600,  //  0011  MOVE	R1	R3
      0x000A4201,  //  0012  ADD	R2	K33	R1
      0x80040400,  //  0013  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(class_Matter_Plugin_Light2_update_shadow,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light2,     /* shared constants */
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0x88040122,  //  0000  GETMBR	R1	R0	K34
      0x74060020,  //  0001  JMPT	R1	#0023
      0x88040118,  //  0002  GETMBR	R1	R0	K24
      0x7406001E,  //  0003  JMPT	R1	#0023
      0xA4063200,  //  0004  IMPORT	R1	K25
      0x8C08011D,  //  0005  GETMET	R2	R0	K29
      0x7C080200,  //  0006  CALL	R2	1
      0x60080003,  //  0007  GETGBL	R2	G3
      0x5C0C0000,  //  0008  MOVE	R3	R0
      0x7C080200,  //  0009  CALL	R2	1
      0x8C080523,  //  000A  GETMET	R2	R2	K35
      0x7C080200,  //  000B  CALL	R2	1
      0x8C08031A,  //  000C  GETMET	R2	R1	K26
      0x8810011C,  //  000D  GETMBR	R4	R0	K28
      0x7C080400,  //  000E  CALL	R2	2
      0x4C0C0000,  //  000F  LDNIL	R3
      0x200C0403,  //  0010  NE	R3	R2	R3
      0x780E000F,  //  0011  JMPF	R3	#0022
      0x8C0C0524,  //  0012  GETMET	R3	R2	K36
      0x58140025,  //  0013  LDCONST	R5	K37
      0x4C180000,  //  0014  LDNIL	R6
      0x7C0C0600,  //  0015  CALL	R3	3
      0x4C100000,  //  0016  LDNIL	R4
      0x1C100604,  //  0017  EQ	R4	R3	R4
      0x78120000,  //  0018  JMPF	R4	#001A
      0x880C0104,  //  0019  GETMBR	R3	R0	K4
      0x88100104,  //  001A  GETMBR	R4	R0	K4
      0x20100604,  //  001B  NE	R4	R3	R4
      0x78120004,  //  001C  JMPF	R4	#0022
      0x8C100126,  //  001D  GETMET	R4	R0	K38
      0x541A02FF,  //  001E  LDINT	R6	768
      0x541E0006,  //  001F  LDINT	R7	7
      0x7C100600,  //  0020  CALL	R4	3
      0x90020803,  //  0021  SETMBR	R0	K4	R3
      0x70020004,  //  0022  JMP		#0028
      0x60040003,  //  0023  GETGBL	R1	G3
      0x5C080000,  //  0024  MOVE	R2	R0
      0x7C040200,  //  0025  CALL	R1	1
      0x8C040323,  //  0026  GETMET	R1	R1	K35
      0x7C040200,  //  0027  CALL	R1	1
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_virtual
********************************************************************/
be_local_closure(class_Matter_Plugin_Light2_update_virtual,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light2,     /* shared constants */
    be_str_weak(update_virtual),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x8C0C0324,  //  0001  GETMET	R3	R1	K36
      0x58140015,  //  0002  LDCONST	R5	K21
      0x7C0C0400,  //  0003  CALL	R3	2
      0x7C080200,  //  0004  CALL	R2	1
      0x4C0C0000,  //  0005  LDNIL	R3
      0x200C0403,  //  0006  NE	R3	R2	R3
      0x780E0002,  //  0007  JMPF	R3	#000B
      0x8C0C0111,  //  0008  GETMET	R3	R0	K17
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C0C0400,  //  000A  CALL	R3	2
      0x600C0003,  //  000B  GETGBL	R3	G3
      0x5C100000,  //  000C  MOVE	R4	R0
      0x7C0C0200,  //  000D  CALL	R3	1
      0x8C0C0727,  //  000E  GETMET	R3	R3	K39
      0x5C140200,  //  000F  MOVE	R5	R1
      0x7C0C0400,  //  0010  CALL	R3	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values
********************************************************************/
be_local_closure(class_Matter_Plugin_Light2_web_values,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light2,     /* shared constants */
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0xA4065000,  //  0000  IMPORT	R1	K40
      0x8C080129,  //  0001  GETMET	R2	R0	K41
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08032A,  //  0003  GETMET	R2	R1	K42
      0x60100018,  //  0004  GETGBL	R4	G24
      0x5814002B,  //  0005  LDCONST	R5	K43
      0x8C18012C,  //  0006  GETMET	R6	R0	K44
      0x8820012D,  //  0007  GETMBR	R8	R0	K45
      0x7C180400,  //  0008  CALL	R6	2
      0x8C1C012E,  //  0009  GETMET	R7	R0	K46
      0x7C1C0200,  //  000A  CALL	R7	1
      0x8C20012F,  //  000B  GETMET	R8	R0	K47
      0x7C200200,  //  000C  CALL	R8	1
      0x7C100800,  //  000D  CALL	R4	4
      0x7C080400,  //  000E  CALL	R2	2
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_ct
********************************************************************/
be_local_closure(class_Matter_Plugin_Light2_set_ct,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light2,     /* shared constants */
    be_str_weak(set_ct),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x88080108,  //  0000  GETMBR	R2	R0	K8
      0x14080202,  //  0001  LT	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x88040108,  //  0003  GETMBR	R1	R0	K8
      0x88080109,  //  0004  GETMBR	R2	R0	K9
      0x24080202,  //  0005  GT	R2	R1	R2
      0x780A0000,  //  0006  JMPF	R2	#0008
      0x88040109,  //  0007  GETMBR	R1	R0	K9
      0x88080118,  //  0008  GETMBR	R2	R0	K24
      0x780A000D,  //  0009  JMPF	R2	#0018
      0x8C080130,  //  000A  GETMET	R2	R0	K48
      0x58100015,  //  000B  LDCONST	R4	K21
      0x60140008,  //  000C  GETGBL	R5	G8
      0x5C180200,  //  000D  MOVE	R6	R1
      0x7C140200,  //  000E  CALL	R5	1
      0x7C080600,  //  000F  CALL	R2	3
      0x4C0C0000,  //  0010  LDNIL	R3
      0x200C0403,  //  0011  NE	R3	R2	R3
      0x780E0003,  //  0012  JMPF	R3	#0017
      0x8C0C0131,  //  0013  GETMET	R3	R0	K49
      0x5C140400,  //  0014  MOVE	R5	R2
      0x541A000A,  //  0015  LDINT	R6	11
      0x7C0C0600,  //  0016  CALL	R3	3
      0x70020013,  //  0017  JMP		#002C
      0x88080122,  //  0018  GETMBR	R2	R0	K34
      0x780A0008,  //  0019  JMPF	R2	#0023
      0x88080104,  //  001A  GETMBR	R2	R0	K4
      0x20080202,  //  001B  NE	R2	R1	R2
      0x780A0004,  //  001C  JMPF	R2	#0022
      0x8C080126,  //  001D  GETMET	R2	R0	K38
      0x541202FF,  //  001E  LDINT	R4	768
      0x54160006,  //  001F  LDINT	R5	7
      0x7C080600,  //  0020  CALL	R2	3
      0x90020801,  //  0021  SETMBR	R0	K4	R1
      0x70020008,  //  0022  JMP		#002C
      0xA40A3200,  //  0023  IMPORT	R2	K25
      0x8C0C0505,  //  0024  GETMET	R3	R2	K5
      0x60140013,  //  0025  GETGBL	R5	G19
      0x7C140000,  //  0026  CALL	R5	0
      0x98164A01,  //  0027  SETIDX	R5	K37	R1
      0x8818011C,  //  0028  GETMBR	R6	R0	K28
      0x7C0C0600,  //  0029  CALL	R3	3
      0x8C0C0123,  //  002A  GETMET	R3	R0	K35
      0x7C0C0200,  //  002B  CALL	R3	1
      0x80000000,  //  002C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_status
********************************************************************/
be_local_closure(class_Matter_Plugin_Light2_parse_status,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light2,     /* shared constants */
    be_str_weak(parse_status),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x600C0003,  //  0000  GETGBL	R3	G3
      0x5C100000,  //  0001  MOVE	R4	R0
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C0C0731,  //  0003  GETMET	R3	R3	K49
      0x5C140200,  //  0004  MOVE	R5	R1
      0x5C180400,  //  0005  MOVE	R6	R2
      0x7C0C0600,  //  0006  CALL	R3	3
      0x540E000A,  //  0007  LDINT	R3	11
      0x1C0C0403,  //  0008  EQ	R3	R2	R3
      0x780E0017,  //  0009  JMPF	R3	#0022
      0x600C0009,  //  000A  GETGBL	R3	G9
      0x8C100324,  //  000B  GETMET	R4	R1	K36
      0x58180015,  //  000C  LDCONST	R6	K21
      0x7C100400,  //  000D  CALL	R4	2
      0x7C0C0200,  //  000E  CALL	R3	1
      0x4C100000,  //  000F  LDNIL	R4
      0x20100604,  //  0010  NE	R4	R3	R4
      0x7812000F,  //  0011  JMPF	R4	#0022
      0x88100104,  //  0012  GETMBR	R4	R0	K4
      0x20100604,  //  0013  NE	R4	R3	R4
      0x7812000C,  //  0014  JMPF	R4	#0022
      0x88100108,  //  0015  GETMBR	R4	R0	K8
      0x14100604,  //  0016  LT	R4	R3	R4
      0x78120000,  //  0017  JMPF	R4	#0019
      0x880C0108,  //  0018  GETMBR	R3	R0	K8
      0x88100109,  //  0019  GETMBR	R4	R0	K9
      0x24100604,  //  001A  GT	R4	R3	R4
      0x78120000,  //  001B  JMPF	R4	#001D
      0x880C0109,  //  001C  GETMBR	R3	R0	K9
      0x8C100126,  //  001D  GETMET	R4	R0	K38
      0x541A02FF,  //  001E  LDINT	R6	768
      0x541E0006,  //  001F  LDINT	R7	7
      0x7C100600,  //  0020  CALL	R4	3
      0x90020803,  //  0021  SETMBR	R0	K4	R3
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Light2
********************************************************************/
extern const bclass be_class_Matter_Plugin_Light1;
be_local_class(Matter_Plugin_Light2,
    3,
    &be_class_Matter_Plugin_Light1,
    be_nested_map(19,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ct_min, 18), be_const_var(1) },
        { be_const_key_weak(ct_max, 4), be_const_var(2) },
        { be_const_key_weak(parse_status, -1), be_const_closure(class_Matter_Plugin_Light2_parse_status_closure) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(3,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Power),
        be_nested_str_weak(Bri),
        be_nested_str_weak(CT),
    }))    ) } )) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(268, -1), be_const_int(4) },
    }))    ) } )) },
        { be_const_key_weak(update_ct_minmax, 8), be_const_closure(class_Matter_Plugin_Light2_update_ct_minmax_closure) },
        { be_const_key_weak(set_ct, -1), be_const_closure(class_Matter_Plugin_Light2_set_ct_closure) },
        { be_const_key_weak(update_virtual, -1), be_const_closure(class_Matter_Plugin_Light2_update_virtual_closure) },
        { be_const_key_weak(web_values, 17), be_const_closure(class_Matter_Plugin_Light2_web_values_closure) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(class_Matter_Plugin_Light2_update_shadow_closure) },
        { be_const_key_weak(invoke_request, 7), be_const_closure(class_Matter_Plugin_Light2_invoke_request_closure) },
        { be_const_key_weak(TYPE, 2), be_nested_str_weak(light2) },
        { be_const_key_weak(SCHEMA, -1), be_const_nil() },
        { be_const_key_weak(init, 12), be_const_closure(class_Matter_Plugin_Light2_init_closure) },
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
        { be_const_key_int(768, -1), be_const_bytes_instance(00070008000F00104001400A400B400CFFF8FFF9FFFAFFFBFFFCFFFD) },
    }))    ) } )) },
        { be_const_key_weak(shadow_ct, -1), be_const_var(0) },
        { be_const_key_weak(web_value_ct, 6), be_const_closure(class_Matter_Plugin_Light2_web_value_ct_closure) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Light_X202_X20CT) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(class_Matter_Plugin_Light2_read_attribute_closure) },
    })),
    be_str_weak(Matter_Plugin_Light2)
);
/********************************************************************/
/* End of solidification */
