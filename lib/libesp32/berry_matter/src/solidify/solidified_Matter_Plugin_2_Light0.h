/* Solidification of Matter_Plugin_2_Light0.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Light0' ktab size: 47, total: 87 (saved 320 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Light0[47] = {
  /* K0   */  be_nested_str_weak(BRIDGE),
  /* K1   */  be_nested_str_weak(call_remote_sync),
  /* K2   */  be_nested_str_weak(Power),
  /* K3   */  be_nested_str_weak(tasmota_relay_index),
  /* K4   */  be_nested_str_weak(1),
  /* K5   */  be_nested_str_weak(0),
  /* K6   */  be_nested_str_weak(parse_status),
  /* K7   */  be_nested_str_weak(VIRTUAL),
  /* K8   */  be_nested_str_weak(shadow_onoff),
  /* K9   */  be_nested_str_weak(attribute_updated),
  /* K10  */  be_const_int(0),
  /* K11  */  be_nested_str_weak(tasmota),
  /* K12  */  be_nested_str_weak(set_power),
  /* K13  */  be_const_int(1),
  /* K14  */  be_nested_str_weak(update_shadow),
  /* K15  */  be_nested_str_weak(light),
  /* K16  */  be_nested_str_weak(set),
  /* K17  */  be_nested_str_weak(power),
  /* K18  */  be_nested_str_weak(light_index),
  /* K19  */  be_nested_str_weak(webserver),
  /* K20  */  be_nested_str_weak(web_values_prefix),
  /* K21  */  be_nested_str_weak(content_send),
  /* K22  */  be_nested_str_weak(_X25s),
  /* K23  */  be_nested_str_weak(web_value_onoff),
  /* K24  */  be_nested_str_weak(init),
  /* K25  */  be_nested_str_weak(find),
  /* K26  */  be_nested_str_weak(ARG),
  /* K27  */  be_nested_str_weak(set_onoff),
  /* K28  */  be_nested_str_weak(update_virtual),
  /* K29  */  be_nested_str_weak(matter),
  /* K30  */  be_nested_str_weak(TLV),
  /* K31  */  be_nested_str_weak(cluster),
  /* K32  */  be_nested_str_weak(attribute),
  /* K33  */  be_nested_str_weak(update_shadow_lazy),
  /* K34  */  be_nested_str_weak(BOOL),
  /* K35  */  be_nested_str_weak(read_attribute),
  /* K36  */  be_nested_str_weak(get_name),
  /* K37  */  be_nested_str_weak(PREFIX),
  /* K38  */  be_nested_str_weak(html_escape),
  /* K39  */  be_nested_str_weak(),
  /* K40  */  be_nested_str_weak(get_power),
  /* K41  */  be_nested_str_weak(command),
  /* K42  */  be_nested_str_weak(publish_command),
  /* K43  */  be_const_int(2),
  /* K44  */  be_nested_str_weak(contains),
  /* K45  */  be_nested_str_weak(POWER),
  /* K46  */  be_nested_str_weak(ON),
};


extern const bclass be_class_Matter_Plugin_Light0;

/********************************************************************
** Solidified function: set_onoff
********************************************************************/
be_local_closure(class_Matter_Plugin_Light0_set_onoff,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light0,     /* shared constants */
    be_str_weak(set_onoff),
    &be_const_str_solidified,
    ( &(const binstruction[56]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A0011,  //  0001  JMPF	R2	#0014
      0x8C080101,  //  0002  GETMET	R2	R0	K1
      0x60100008,  //  0003  GETGBL	R4	G8
      0x88140103,  //  0004  GETMBR	R5	R0	K3
      0x7C100200,  //  0005  CALL	R4	1
      0x00120404,  //  0006  ADD	R4	K2	R4
      0x78060001,  //  0007  JMPF	R1	#000A
      0x58140004,  //  0008  LDCONST	R5	K4
      0x70020000,  //  0009  JMP		#000B
      0x58140005,  //  000A  LDCONST	R5	K5
      0x7C080600,  //  000B  CALL	R2	3
      0x4C0C0000,  //  000C  LDNIL	R3
      0x200C0403,  //  000D  NE	R3	R2	R3
      0x780E0003,  //  000E  JMPF	R3	#0013
      0x8C0C0106,  //  000F  GETMET	R3	R0	K6
      0x5C140400,  //  0010  MOVE	R5	R2
      0x541A000A,  //  0011  LDINT	R6	11
      0x7C0C0600,  //  0012  CALL	R3	3
      0x70020022,  //  0013  JMP		#0037
      0x88080107,  //  0014  GETMBR	R2	R0	K7
      0x780A0008,  //  0015  JMPF	R2	#001F
      0x88080108,  //  0016  GETMBR	R2	R0	K8
      0x20080202,  //  0017  NE	R2	R1	R2
      0x780A0004,  //  0018  JMPF	R2	#001E
      0x8C080109,  //  0019  GETMET	R2	R0	K9
      0x54120005,  //  001A  LDINT	R4	6
      0x5814000A,  //  001B  LDCONST	R5	K10
      0x7C080600,  //  001C  CALL	R2	3
      0x90021001,  //  001D  SETMBR	R0	K8	R1
      0x70020017,  //  001E  JMP		#0037
      0x88080103,  //  001F  GETMBR	R2	R0	K3
      0x4C0C0000,  //  0020  LDNIL	R3
      0x20080403,  //  0021  NE	R2	R2	R3
      0x780A000A,  //  0022  JMPF	R2	#002E
      0xB80A1600,  //  0023  GETNGBL	R2	K11
      0x8C08050C,  //  0024  GETMET	R2	R2	K12
      0x88100103,  //  0025  GETMBR	R4	R0	K3
      0x0410090D,  //  0026  SUB	R4	R4	K13
      0x60140017,  //  0027  GETGBL	R5	G23
      0x5C180200,  //  0028  MOVE	R6	R1
      0x7C140200,  //  0029  CALL	R5	1
      0x7C080600,  //  002A  CALL	R2	3
      0x8C08010E,  //  002B  GETMET	R2	R0	K14
      0x7C080200,  //  002C  CALL	R2	1
      0x70020008,  //  002D  JMP		#0037
      0xA40A1E00,  //  002E  IMPORT	R2	K15
      0x8C0C0510,  //  002F  GETMET	R3	R2	K16
      0x60140013,  //  0030  GETGBL	R5	G19
      0x7C140000,  //  0031  CALL	R5	0
      0x98162201,  //  0032  SETIDX	R5	K17	R1
      0x88180112,  //  0033  GETMBR	R6	R0	K18
      0x7C0C0600,  //  0034  CALL	R3	3
      0x8C0C010E,  //  0035  GETMET	R3	R0	K14
      0x7C0C0200,  //  0036  CALL	R3	1
      0x80000000,  //  0037  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values
********************************************************************/
be_local_closure(class_Matter_Plugin_Light0_web_values,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light0,     /* shared constants */
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xA4062600,  //  0000  IMPORT	R1	K19
      0x8C080114,  //  0001  GETMET	R2	R0	K20
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080315,  //  0003  GETMET	R2	R1	K21
      0x60100018,  //  0004  GETGBL	R4	G24
      0x58140016,  //  0005  LDCONST	R5	K22
      0x8C180117,  //  0006  GETMET	R6	R0	K23
      0x88200108,  //  0007  GETMBR	R8	R0	K8
      0x7C180400,  //  0008  CALL	R6	2
      0x7C100400,  //  0009  CALL	R4	2
      0x7C080400,  //  000A  CALL	R2	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Plugin_Light0_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light0,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x50100000,  //  0000  LDBOOL	R4	0	0
      0x90021004,  //  0001  SETMBR	R0	K8	R4
      0x9002250A,  //  0002  SETMBR	R0	K18	K10
      0x60100003,  //  0003  GETGBL	R4	G3
      0x5C140000,  //  0004  MOVE	R5	R0
      0x7C100200,  //  0005  CALL	R4	1
      0x8C100918,  //  0006  GETMET	R4	R4	K24
      0x5C180200,  //  0007  MOVE	R6	R1
      0x5C1C0400,  //  0008  MOVE	R7	R2
      0x5C200600,  //  0009  MOVE	R8	R3
      0x7C100800,  //  000A  CALL	R4	4
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_configuration
********************************************************************/
be_local_closure(class_Matter_Plugin_Light0_parse_configuration,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light0,     /* shared constants */
    be_str_weak(parse_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x60080009,  //  0000  GETGBL	R2	G9
      0x8C0C0319,  //  0001  GETMET	R3	R1	K25
      0x8814011A,  //  0002  GETMBR	R5	R0	K26
      0x4C180000,  //  0003  LDNIL	R6
      0x7C0C0600,  //  0004  CALL	R3	3
      0x7C080200,  //  0005  CALL	R2	1
      0x90020602,  //  0006  SETMBR	R0	K3	R2
      0x88080103,  //  0007  GETMBR	R2	R0	K3
      0x4C0C0000,  //  0008  LDNIL	R3
      0x20080403,  //  0009  NE	R2	R2	R3
      0x780A0003,  //  000A  JMPF	R2	#000F
      0x88080103,  //  000B  GETMBR	R2	R0	K3
      0x1808050A,  //  000C  LE	R2	R2	K10
      0x780A0000,  //  000D  JMPF	R2	#000F
      0x9002070D,  //  000E  SETMBR	R0	K3	K13
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_virtual
********************************************************************/
be_local_closure(class_Matter_Plugin_Light0_update_virtual,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light0,     /* shared constants */
    be_str_weak(update_virtual),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C080319,  //  0000  GETMET	R2	R1	K25
      0x58100002,  //  0001  LDCONST	R4	K2
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0004,  //  0005  JMPF	R3	#000B
      0x8C0C011B,  //  0006  GETMET	R3	R0	K27
      0x60140017,  //  0007  GETGBL	R5	G23
      0x5C180400,  //  0008  MOVE	R6	R2
      0x7C140200,  //  0009  CALL	R5	1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x600C0003,  //  000B  GETGBL	R3	G3
      0x5C100000,  //  000C  MOVE	R4	R0
      0x7C0C0200,  //  000D  CALL	R3	1
      0x8C0C071C,  //  000E  GETMET	R3	R3	K28
      0x5C140200,  //  000F  MOVE	R5	R1
      0x7C0C0400,  //  0010  CALL	R3	2
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(class_Matter_Plugin_Light0_read_attribute,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light0,     /* shared constants */
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0xB8123A00,  //  0000  GETNGBL	R4	K29
      0x8810091E,  //  0001  GETMBR	R4	R4	K30
      0x8814051F,  //  0002  GETMBR	R5	R2	K31
      0x88180520,  //  0003  GETMBR	R6	R2	K32
      0x541E0005,  //  0004  LDINT	R7	6
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0008,  //  0006  JMPF	R7	#0010
      0x8C1C0121,  //  0007  GETMET	R7	R0	K33
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D0A,  //  0009  EQ	R7	R6	K10
      0x781E0004,  //  000A  JMPF	R7	#0010
      0x8C1C0710,  //  000B  GETMET	R7	R3	K16
      0x88240922,  //  000C  GETMBR	R9	R4	K34
      0x88280108,  //  000D  GETMBR	R10	R0	K8
      0x7C1C0600,  //  000E  CALL	R7	3
      0x80040E00,  //  000F  RET	1	R7
      0x601C0003,  //  0010  GETGBL	R7	G3
      0x5C200000,  //  0011  MOVE	R8	R0
      0x7C1C0200,  //  0012  CALL	R7	1
      0x8C1C0F23,  //  0013  GETMET	R7	R7	K35
      0x5C240200,  //  0014  MOVE	R9	R1
      0x5C280400,  //  0015  MOVE	R10	R2
      0x5C2C0600,  //  0016  MOVE	R11	R3
      0x7C1C0800,  //  0017  CALL	R7	4
      0x80040E00,  //  0018  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values_prefix
********************************************************************/
be_local_closure(class_Matter_Plugin_Light0_web_values_prefix,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light0,     /* shared constants */
    be_str_weak(web_values_prefix),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0xA4062600,  //  0000  IMPORT	R1	K19
      0x8C080124,  //  0001  GETMET	R2	R0	K36
      0x7C080200,  //  0002  CALL	R2	1
      0x5C0C0400,  //  0003  MOVE	R3	R2
      0x740E0004,  //  0004  JMPT	R3	#000A
      0x600C0008,  //  0005  GETGBL	R3	G8
      0x88100103,  //  0006  GETMBR	R4	R0	K3
      0x7C0C0200,  //  0007  CALL	R3	1
      0x000E0403,  //  0008  ADD	R3	K2	R3
      0x5C080600,  //  0009  MOVE	R2	R3
      0x8C0C0315,  //  000A  GETMET	R3	R1	K21
      0x60140018,  //  000B  GETGBL	R5	G24
      0x88180125,  //  000C  GETMBR	R6	R0	K37
      0x780A0003,  //  000D  JMPF	R2	#0012
      0x8C1C0326,  //  000E  GETMET	R7	R1	K38
      0x5C240400,  //  000F  MOVE	R9	R2
      0x7C1C0400,  //  0010  CALL	R7	2
      0x70020000,  //  0011  JMP		#0013
      0x581C0027,  //  0012  LDCONST	R7	K39
      0x7C140400,  //  0013  CALL	R5	2
      0x7C0C0400,  //  0014  CALL	R3	2
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: <lambda>
********************************************************************/
be_local_closure(class_Matter_Plugin_Light0__X3Clambda_X3E,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    8,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light0,     /* shared constants */
    be_str_weak(_X3Clambda_X3E),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x60040009,  //  0000  GETGBL	R1	G9
      0x5C080000,  //  0001  MOVE	R2	R0
      0x7C040200,  //  0002  CALL	R1	1
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_shadow
********************************************************************/
be_local_closure(class_Matter_Plugin_Light0_update_shadow,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light0,     /* shared constants */
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x88040107,  //  0000  GETMBR	R1	R0	K7
      0x74060018,  //  0001  JMPT	R1	#001B
      0x88040100,  //  0002  GETMBR	R1	R0	K0
      0x74060016,  //  0003  JMPT	R1	#001B
      0x88040103,  //  0004  GETMBR	R1	R0	K3
      0x4C080000,  //  0005  LDNIL	R2
      0x20040202,  //  0006  NE	R1	R1	R2
      0x78060012,  //  0007  JMPF	R1	#001B
      0xB8061600,  //  0008  GETNGBL	R1	K11
      0x8C040328,  //  0009  GETMET	R1	R1	K40
      0x880C0103,  //  000A  GETMBR	R3	R0	K3
      0x040C070D,  //  000B  SUB	R3	R3	K13
      0x7C040400,  //  000C  CALL	R1	2
      0x4C080000,  //  000D  LDNIL	R2
      0x20080202,  //  000E  NE	R2	R1	R2
      0x780A000A,  //  000F  JMPF	R2	#001B
      0x88080108,  //  0010  GETMBR	R2	R0	K8
      0x600C0017,  //  0011  GETGBL	R3	G23
      0x5C100200,  //  0012  MOVE	R4	R1
      0x7C0C0200,  //  0013  CALL	R3	1
      0x20080403,  //  0014  NE	R2	R2	R3
      0x780A0003,  //  0015  JMPF	R2	#001A
      0x8C080109,  //  0016  GETMET	R2	R0	K9
      0x54120005,  //  0017  LDINT	R4	6
      0x5814000A,  //  0018  LDCONST	R5	K10
      0x7C080600,  //  0019  CALL	R2	3
      0x90021001,  //  001A  SETMBR	R0	K8	R1
      0x60040003,  //  001B  GETGBL	R1	G3
      0x5C080000,  //  001C  MOVE	R2	R0
      0x7C040200,  //  001D  CALL	R1	1
      0x8C04030E,  //  001E  GETMET	R1	R1	K14
      0x7C040200,  //  001F  CALL	R1	1
      0x80000000,  //  0020  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(class_Matter_Plugin_Light0_invoke_request,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light0,     /* shared constants */
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0xB8123A00,  //  0000  GETNGBL	R4	K29
      0x8810091E,  //  0001  GETMBR	R4	R4	K30
      0x8814071F,  //  0002  GETMBR	R5	R3	K31
      0x88180729,  //  0003  GETMBR	R6	R3	K41
      0x541E0005,  //  0004  LDINT	R7	6
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E002B,  //  0006  JMPF	R7	#0033
      0x8C1C0121,  //  0007  GETMET	R7	R0	K33
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D0A,  //  0009  EQ	R7	R6	K10
      0x781E0009,  //  000A  JMPF	R7	#0015
      0x8C1C011B,  //  000B  GETMET	R7	R0	K27
      0x50240000,  //  000C  LDBOOL	R9	0	0
      0x7C1C0400,  //  000D  CALL	R7	2
      0x8C1C012A,  //  000E  GETMET	R7	R0	K42
      0x58240002,  //  000F  LDCONST	R9	K2
      0x5828000A,  //  0010  LDCONST	R10	K10
      0x7C1C0600,  //  0011  CALL	R7	3
      0x501C0200,  //  0012  LDBOOL	R7	1	0
      0x80040E00,  //  0013  RET	1	R7
      0x7002001D,  //  0014  JMP		#0033
      0x1C1C0D0D,  //  0015  EQ	R7	R6	K13
      0x781E0009,  //  0016  JMPF	R7	#0021
      0x8C1C011B,  //  0017  GETMET	R7	R0	K27
      0x50240200,  //  0018  LDBOOL	R9	1	0
      0x7C1C0400,  //  0019  CALL	R7	2
      0x8C1C012A,  //  001A  GETMET	R7	R0	K42
      0x58240002,  //  001B  LDCONST	R9	K2
      0x5828000D,  //  001C  LDCONST	R10	K13
      0x7C1C0600,  //  001D  CALL	R7	3
      0x501C0200,  //  001E  LDBOOL	R7	1	0
      0x80040E00,  //  001F  RET	1	R7
      0x70020011,  //  0020  JMP		#0033
      0x1C1C0D2B,  //  0021  EQ	R7	R6	K43
      0x781E000F,  //  0022  JMPF	R7	#0033
      0x8C1C011B,  //  0023  GETMET	R7	R0	K27
      0x88240108,  //  0024  GETMBR	R9	R0	K8
      0x78260000,  //  0025  JMPF	R9	#0027
      0x50240001,  //  0026  LDBOOL	R9	0	1
      0x50240200,  //  0027  LDBOOL	R9	1	0
      0x7C1C0400,  //  0028  CALL	R7	2
      0x8C1C012A,  //  0029  GETMET	R7	R0	K42
      0x58240002,  //  002A  LDCONST	R9	K2
      0x88280108,  //  002B  GETMBR	R10	R0	K8
      0x782A0001,  //  002C  JMPF	R10	#002F
      0x5828000D,  //  002D  LDCONST	R10	K13
      0x70020000,  //  002E  JMP		#0030
      0x5828000A,  //  002F  LDCONST	R10	K10
      0x7C1C0600,  //  0030  CALL	R7	3
      0x501C0200,  //  0031  LDBOOL	R7	1	0
      0x80040E00,  //  0032  RET	1	R7
      0x80000000,  //  0033  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_status
********************************************************************/
be_local_closure(class_Matter_Plugin_Light0_parse_status,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light0,     /* shared constants */
    be_str_weak(parse_status),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0x540E000A,  //  0000  LDINT	R3	11
      0x1C0C0403,  //  0001  EQ	R3	R2	R3
      0x780E0020,  //  0002  JMPF	R3	#0024
      0x500C0000,  //  0003  LDBOOL	R3	0	0
      0x88100103,  //  0004  GETMBR	R4	R0	K3
      0x1C10090D,  //  0005  EQ	R4	R4	K13
      0x78120009,  //  0006  JMPF	R4	#0011
      0x8C10032C,  //  0007  GETMET	R4	R1	K44
      0x5818002D,  //  0008  LDCONST	R6	K45
      0x7C100400,  //  0009  CALL	R4	2
      0x78120005,  //  000A  JMPF	R4	#0011
      0x8C100319,  //  000B  GETMET	R4	R1	K25
      0x5818002D,  //  000C  LDCONST	R6	K45
      0x7C100400,  //  000D  CALL	R4	2
      0x1C10092E,  //  000E  EQ	R4	R4	K46
      0x5C0C0800,  //  000F  MOVE	R3	R4
      0x70020007,  //  0010  JMP		#0019
      0x8C100319,  //  0011  GETMET	R4	R1	K25
      0x60180008,  //  0012  GETGBL	R6	G8
      0x881C0103,  //  0013  GETMBR	R7	R0	K3
      0x7C180200,  //  0014  CALL	R6	1
      0x001A5A06,  //  0015  ADD	R6	K45	R6
      0x7C100400,  //  0016  CALL	R4	2
      0x1C10092E,  //  0017  EQ	R4	R4	K46
      0x5C0C0800,  //  0018  MOVE	R3	R4
      0x88100108,  //  0019  GETMBR	R4	R0	K8
      0x60140017,  //  001A  GETGBL	R5	G23
      0x5C180600,  //  001B  MOVE	R6	R3
      0x7C140200,  //  001C  CALL	R5	1
      0x20100805,  //  001D  NE	R4	R4	R5
      0x78120004,  //  001E  JMPF	R4	#0024
      0x8C100109,  //  001F  GETMET	R4	R0	K9
      0x541A0005,  //  0020  LDINT	R6	6
      0x581C000A,  //  0021  LDCONST	R7	K10
      0x7C100600,  //  0022  CALL	R4	3
      0x90021003,  //  0023  SETMBR	R0	K8	R3
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Light0
********************************************************************/
extern const bclass be_class_Matter_Plugin_Device;
be_local_class(Matter_Plugin_Light0,
    3,
    &be_class_Matter_Plugin_Device,
    be_nested_map(22,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(parse_status, 8), be_const_closure(class_Matter_Plugin_Light0_parse_status_closure) },
        { be_const_key_weak(ARG_HINT, -1), be_nested_str_weak(Relay_X3Cx_X3E_X20number) },
        { be_const_key_weak(light_index, -1), be_const_var(1) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(250) },
        { be_const_key_weak(set_onoff, 18), be_const_closure(class_Matter_Plugin_Light0_set_onoff_closure) },
        { be_const_key_weak(tasmota_relay_index, -1), be_const_var(0) },
        { be_const_key_weak(CLUSTERS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(6, -1), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(5, -1), be_const_bytes_instance(000000010002000300040005FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(57, -1), be_const_bytes_instance(00030005000A000F00110012FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(3, 2), be_const_bytes_instance(00000001FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(4, -1), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(29, 1), be_const_bytes_instance(0000000100020003FFF8FFF9FFFAFFFBFFFCFFFD) },
    }))    ) } )) },
        { be_const_key_weak(ARG, -1), be_nested_str_weak(relay) },
        { be_const_key_weak(invoke_request, 10), be_const_closure(class_Matter_Plugin_Light0_invoke_request_closure) },
        { be_const_key_weak(init, 5), be_const_closure(class_Matter_Plugin_Light0_init_closure) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Power),
    }))    ) } )) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(class_Matter_Plugin_Light0_read_attribute_closure) },
        { be_const_key_weak(update_virtual, -1), be_const_closure(class_Matter_Plugin_Light0_update_virtual_closure) },
        { be_const_key_weak(web_values_prefix, -1), be_const_closure(class_Matter_Plugin_Light0_web_values_prefix_closure) },
        { be_const_key_weak(shadow_onoff, 11), be_const_var(2) },
        { be_const_key_weak(TYPE, 13), be_nested_str_weak(light0) },
        { be_const_key_weak(ARG_TYPE, -1), be_const_static_closure(class_Matter_Plugin_Light0__X3Clambda_X3E_closure) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(class_Matter_Plugin_Light0_update_shadow_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(256, -1), be_const_int(2) },
    }))    ) } )) },
        { be_const_key_weak(DISPLAY_NAME, -1), be_nested_str_weak(Light_X200_X20On) },
        { be_const_key_weak(web_values, 3), be_const_closure(class_Matter_Plugin_Light0_web_values_closure) },
        { be_const_key_weak(parse_configuration, 0), be_const_closure(class_Matter_Plugin_Light0_parse_configuration_closure) },
    })),
    be_str_weak(Matter_Plugin_Light0)
);
/********************************************************************/
/* End of solidification */
