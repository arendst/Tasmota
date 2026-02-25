/* Solidification of Matter_Plugin_2_Light0.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Light0' ktab size: 59, total: 101 (saved 336 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Light0[59] = {
  /* K0   */  be_nested_str_weak(cluster),
  /* K1   */  be_nested_str_weak(attribute),
  /* K2   */  be_nested_str_weak(update_shadow_lazy),
  /* K3   */  be_const_int(0),
  /* K4   */  be_nested_str_weak(set),
  /* K5   */  be_nested_str_weak(shadow_onoff),
  /* K6   */  be_nested_str_weak(read_attribute),
  /* K7   */  be_nested_str_weak(find),
  /* K8   */  be_nested_str_weak(Power),
  /* K9   */  be_nested_str_weak(set_onoff),
  /* K10  */  be_nested_str_weak(update_virtual),
  /* K11  */  be_nested_str_weak(tasmota_relay_index),
  /* K12  */  be_const_int(1),
  /* K13  */  be_nested_str_weak(contains),
  /* K14  */  be_nested_str_weak(POWER),
  /* K15  */  be_nested_str_weak(ON),
  /* K16  */  be_nested_str_weak(attribute_updated),
  /* K17  */  be_nested_str_weak(math),
  /* K18  */  be_nested_str_weak(log),
  /* K19  */  be_nested_str_weak(MTR_X3A_X20zigbee_received_X20Ox_X2504X_X20attr_list_X3D_X25s_X20type_X28attr_list_X29_X3D_X25s),
  /* K20  */  be_nested_str_weak(zigbee_mapper),
  /* K21  */  be_nested_str_weak(shortaddr),
  /* K22  */  be_const_int(3),
  /* K23  */  be_nested_str_weak(key),
  /* K24  */  be_nested_str_weak(val),
  /* K25  */  be_nested_str_weak(Dimmer),
  /* K26  */  be_nested_str_weak(CT),
  /* K27  */  be_nested_str_weak(MTR_X3A_X20_X5B_X2502X_X5D_X20Light2_X20updated_X20_X25s),
  /* K28  */  be_nested_str_weak(endpoint),
  /* K29  */  be_nested_str_weak(BRIDGE),
  /* K30  */  be_nested_str_weak(call_remote_sync),
  /* K31  */  be_nested_str_weak(1),
  /* K32  */  be_nested_str_weak(0),
  /* K33  */  be_nested_str_weak(parse_status),
  /* K34  */  be_nested_str_weak(VIRTUAL),
  /* K35  */  be_nested_str_weak(tasmota),
  /* K36  */  be_nested_str_weak(set_power),
  /* K37  */  be_nested_str_weak(update_shadow),
  /* K38  */  be_nested_str_weak(light),
  /* K39  */  be_nested_str_weak(power),
  /* K40  */  be_nested_str_weak(light_index),
  /* K41  */  be_nested_str_weak(webserver),
  /* K42  */  be_nested_str_weak(web_values_prefix),
  /* K43  */  be_nested_str_weak(content_send),
  /* K44  */  be_nested_str_weak(_X25s),
  /* K45  */  be_nested_str_weak(web_value_onoff),
  /* K46  */  be_nested_str_weak(init),
  /* K47  */  be_nested_str_weak(matter),
  /* K48  */  be_nested_str_weak(TLV),
  /* K49  */  be_nested_str_weak(command),
  /* K50  */  be_nested_str_weak(publish_command),
  /* K51  */  be_const_int(2),
  /* K52  */  be_nested_str_weak(parse_configuration),
  /* K53  */  be_nested_str_weak(relay),
  /* K54  */  be_nested_str_weak(get_power),
  /* K55  */  be_nested_str_weak(get_name),
  /* K56  */  be_nested_str_weak(PREFIX),
  /* K57  */  be_nested_str_weak(html_escape),
  /* K58  */  be_nested_str_weak(),
};


extern const bclass be_class_Matter_Plugin_Light0;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(class_Matter_Plugin_Light0_read_attribute,   /* name */
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
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x88100500,  //  0000  GETMBR	R4	R2	K0
      0x88140501,  //  0001  GETMBR	R5	R2	K1
      0x541A0005,  //  0002  LDINT	R6	6
      0x1C180806,  //  0003  EQ	R6	R4	R6
      0x781A0008,  //  0004  JMPF	R6	#000E
      0x8C180102,  //  0005  GETMET	R6	R0	K2
      0x7C180200,  //  0006  CALL	R6	1
      0x1C180B03,  //  0007  EQ	R6	R5	K3
      0x781A0004,  //  0008  JMPF	R6	#000E
      0x8C180704,  //  0009  GETMET	R6	R3	K4
      0x54220007,  //  000A  LDINT	R8	8
      0x88240105,  //  000B  GETMBR	R9	R0	K5
      0x7C180600,  //  000C  CALL	R6	3
      0x80040C00,  //  000D  RET	1	R6
      0x60180003,  //  000E  GETGBL	R6	G3
      0x5C1C0000,  //  000F  MOVE	R7	R0
      0x7C180200,  //  0010  CALL	R6	1
      0x8C180D06,  //  0011  GETMET	R6	R6	K6
      0x5C200200,  //  0012  MOVE	R8	R1
      0x5C240400,  //  0013  MOVE	R9	R2
      0x5C280600,  //  0014  MOVE	R10	R3
      0x7C180800,  //  0015  CALL	R6	4
      0x80040C00,  //  0016  RET	1	R6
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
      0x8C080307,  //  0000  GETMET	R2	R1	K7
      0x58100008,  //  0001  LDCONST	R4	K8
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0004,  //  0005  JMPF	R3	#000B
      0x8C0C0109,  //  0006  GETMET	R3	R0	K9
      0x60140017,  //  0007  GETGBL	R5	G23
      0x5C180400,  //  0008  MOVE	R6	R2
      0x7C140200,  //  0009  CALL	R5	1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x600C0003,  //  000B  GETGBL	R3	G3
      0x5C100000,  //  000C  MOVE	R4	R0
      0x7C0C0200,  //  000D  CALL	R3	1
      0x8C0C070A,  //  000E  GETMET	R3	R3	K10
      0x5C140200,  //  000F  MOVE	R5	R1
      0x7C0C0400,  //  0010  CALL	R3	2
      0x80000000,  //  0011  RET	0
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
      0x8810010B,  //  0004  GETMBR	R4	R0	K11
      0x1C10090C,  //  0005  EQ	R4	R4	K12
      0x78120009,  //  0006  JMPF	R4	#0011
      0x8C10030D,  //  0007  GETMET	R4	R1	K13
      0x5818000E,  //  0008  LDCONST	R6	K14
      0x7C100400,  //  0009  CALL	R4	2
      0x78120005,  //  000A  JMPF	R4	#0011
      0x8C100307,  //  000B  GETMET	R4	R1	K7
      0x5818000E,  //  000C  LDCONST	R6	K14
      0x7C100400,  //  000D  CALL	R4	2
      0x1C10090F,  //  000E  EQ	R4	R4	K15
      0x5C0C0800,  //  000F  MOVE	R3	R4
      0x70020007,  //  0010  JMP		#0019
      0x8C100307,  //  0011  GETMET	R4	R1	K7
      0x60180008,  //  0012  GETGBL	R6	G8
      0x881C010B,  //  0013  GETMBR	R7	R0	K11
      0x7C180200,  //  0014  CALL	R6	1
      0x001A1C06,  //  0015  ADD	R6	K14	R6
      0x7C100400,  //  0016  CALL	R4	2
      0x1C10090F,  //  0017  EQ	R4	R4	K15
      0x5C0C0800,  //  0018  MOVE	R3	R4
      0x88100105,  //  0019  GETMBR	R4	R0	K5
      0x60140017,  //  001A  GETGBL	R5	G23
      0x5C180600,  //  001B  MOVE	R6	R3
      0x7C140200,  //  001C  CALL	R5	1
      0x20100805,  //  001D  NE	R4	R4	R5
      0x78120004,  //  001E  JMPF	R4	#0024
      0x8C100110,  //  001F  GETMET	R4	R0	K16
      0x541A0005,  //  0020  LDINT	R6	6
      0x581C0003,  //  0021  LDCONST	R7	K3
      0x7C100600,  //  0022  CALL	R4	3
      0x90020A03,  //  0023  SETMBR	R0	K5	R3
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: zigbee_received
********************************************************************/
be_local_closure(class_Matter_Plugin_Light0_zigbee_received,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Light0,     /* shared constants */
    be_str_weak(zigbee_received),
    &be_const_str_solidified,
    ( &(const binstruction[62]) {  /* code */
      0xA40E2200,  //  0000  IMPORT	R3	K17
      0xB8122400,  //  0001  GETNGBL	R4	K18
      0x60140018,  //  0002  GETGBL	R5	G24
      0x58180013,  //  0003  LDCONST	R6	K19
      0x881C0114,  //  0004  GETMBR	R7	R0	K20
      0x881C0F15,  //  0005  GETMBR	R7	R7	K21
      0x5C200400,  //  0006  MOVE	R8	R2
      0x60240004,  //  0007  GETGBL	R9	G4
      0x5C280400,  //  0008  MOVE	R10	R2
      0x7C240200,  //  0009  CALL	R9	1
      0x7C140800,  //  000A  CALL	R5	4
      0x58180016,  //  000B  LDCONST	R6	K22
      0x7C100400,  //  000C  CALL	R4	2
      0x58100003,  //  000D  LDCONST	R4	K3
      0x60140013,  //  000E  GETGBL	R5	G19
      0x7C140000,  //  000F  CALL	R5	0
      0x6018000C,  //  0010  GETGBL	R6	G12
      0x5C1C0400,  //  0011  MOVE	R7	R2
      0x7C180200,  //  0012  CALL	R6	1
      0x14180806,  //  0013  LT	R6	R4	R6
      0x781A0017,  //  0014  JMPF	R6	#002D
      0x94180404,  //  0015  GETIDX	R6	R2	R4
      0x881C0D17,  //  0016  GETMBR	R7	R6	K23
      0x1C1C0F08,  //  0017  EQ	R7	R7	K8
      0x781E0003,  //  0018  JMPF	R7	#001D
      0x601C0009,  //  0019  GETGBL	R7	G9
      0x88200D18,  //  001A  GETMBR	R8	R6	K24
      0x7C1C0200,  //  001B  CALL	R7	1
      0x98161007,  //  001C  SETIDX	R5	K8	R7
      0x881C0D17,  //  001D  GETMBR	R7	R6	K23
      0x1C1C0F19,  //  001E  EQ	R7	R7	K25
      0x781E0003,  //  001F  JMPF	R7	#0024
      0x601C0009,  //  0020  GETGBL	R7	G9
      0x88200D18,  //  0021  GETMBR	R8	R6	K24
      0x7C1C0200,  //  0022  CALL	R7	1
      0x98163207,  //  0023  SETIDX	R5	K25	R7
      0x881C0D17,  //  0024  GETMBR	R7	R6	K23
      0x1C1C0F1A,  //  0025  EQ	R7	R7	K26
      0x781E0003,  //  0026  JMPF	R7	#002B
      0x601C0009,  //  0027  GETGBL	R7	G9
      0x88200D18,  //  0028  GETMBR	R8	R6	K24
      0x7C1C0200,  //  0029  CALL	R7	1
      0x98163407,  //  002A  SETIDX	R5	K26	R7
      0x0010090C,  //  002B  ADD	R4	R4	K12
      0x7001FFE2,  //  002C  JMP		#0010
      0x6018000C,  //  002D  GETGBL	R6	G12
      0x5C1C0A00,  //  002E  MOVE	R7	R5
      0x7C180200,  //  002F  CALL	R6	1
      0x24180D03,  //  0030  GT	R6	R6	K3
      0x781A000A,  //  0031  JMPF	R6	#003D
      0x8C18010A,  //  0032  GETMET	R6	R0	K10
      0x5C200A00,  //  0033  MOVE	R8	R5
      0x7C180400,  //  0034  CALL	R6	2
      0xB81A2400,  //  0035  GETNGBL	R6	K18
      0x601C0018,  //  0036  GETGBL	R7	G24
      0x5820001B,  //  0037  LDCONST	R8	K27
      0x8824011C,  //  0038  GETMBR	R9	R0	K28
      0x5C280A00,  //  0039  MOVE	R10	R5
      0x7C1C0600,  //  003A  CALL	R7	3
      0x58200016,  //  003B  LDCONST	R8	K22
      0x7C180400,  //  003C  CALL	R6	2
      0x80000000,  //  003D  RET	0
    })
  )
);
/*******************************************************************/


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
      0x8808011D,  //  0000  GETMBR	R2	R0	K29
      0x780A0011,  //  0001  JMPF	R2	#0014
      0x8C08011E,  //  0002  GETMET	R2	R0	K30
      0x60100008,  //  0003  GETGBL	R4	G8
      0x8814010B,  //  0004  GETMBR	R5	R0	K11
      0x7C100200,  //  0005  CALL	R4	1
      0x00121004,  //  0006  ADD	R4	K8	R4
      0x78060001,  //  0007  JMPF	R1	#000A
      0x5814001F,  //  0008  LDCONST	R5	K31
      0x70020000,  //  0009  JMP		#000B
      0x58140020,  //  000A  LDCONST	R5	K32
      0x7C080600,  //  000B  CALL	R2	3
      0x4C0C0000,  //  000C  LDNIL	R3
      0x200C0403,  //  000D  NE	R3	R2	R3
      0x780E0003,  //  000E  JMPF	R3	#0013
      0x8C0C0121,  //  000F  GETMET	R3	R0	K33
      0x5C140400,  //  0010  MOVE	R5	R2
      0x541A000A,  //  0011  LDINT	R6	11
      0x7C0C0600,  //  0012  CALL	R3	3
      0x70020022,  //  0013  JMP		#0037
      0x88080122,  //  0014  GETMBR	R2	R0	K34
      0x780A0008,  //  0015  JMPF	R2	#001F
      0x88080105,  //  0016  GETMBR	R2	R0	K5
      0x20080202,  //  0017  NE	R2	R1	R2
      0x780A0004,  //  0018  JMPF	R2	#001E
      0x8C080110,  //  0019  GETMET	R2	R0	K16
      0x54120005,  //  001A  LDINT	R4	6
      0x58140003,  //  001B  LDCONST	R5	K3
      0x7C080600,  //  001C  CALL	R2	3
      0x90020A01,  //  001D  SETMBR	R0	K5	R1
      0x70020017,  //  001E  JMP		#0037
      0x8808010B,  //  001F  GETMBR	R2	R0	K11
      0x4C0C0000,  //  0020  LDNIL	R3
      0x20080403,  //  0021  NE	R2	R2	R3
      0x780A000A,  //  0022  JMPF	R2	#002E
      0xB80A4600,  //  0023  GETNGBL	R2	K35
      0x8C080524,  //  0024  GETMET	R2	R2	K36
      0x8810010B,  //  0025  GETMBR	R4	R0	K11
      0x0410090C,  //  0026  SUB	R4	R4	K12
      0x60140017,  //  0027  GETGBL	R5	G23
      0x5C180200,  //  0028  MOVE	R6	R1
      0x7C140200,  //  0029  CALL	R5	1
      0x7C080600,  //  002A  CALL	R2	3
      0x8C080125,  //  002B  GETMET	R2	R0	K37
      0x7C080200,  //  002C  CALL	R2	1
      0x70020008,  //  002D  JMP		#0037
      0xA40A4C00,  //  002E  IMPORT	R2	K38
      0x8C0C0504,  //  002F  GETMET	R3	R2	K4
      0x60140013,  //  0030  GETGBL	R5	G19
      0x7C140000,  //  0031  CALL	R5	0
      0x98164E01,  //  0032  SETIDX	R5	K39	R1
      0x88180128,  //  0033  GETMBR	R6	R0	K40
      0x7C0C0600,  //  0034  CALL	R3	3
      0x8C0C0125,  //  0035  GETMET	R3	R0	K37
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
      0xA4065200,  //  0000  IMPORT	R1	K41
      0x8C08012A,  //  0001  GETMET	R2	R0	K42
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08032B,  //  0003  GETMET	R2	R1	K43
      0x60100018,  //  0004  GETGBL	R4	G24
      0x5814002C,  //  0005  LDCONST	R5	K44
      0x8C18012D,  //  0006  GETMET	R6	R0	K45
      0x88200105,  //  0007  GETMBR	R8	R0	K5
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
      0x90020A04,  //  0001  SETMBR	R0	K5	R4
      0x90025103,  //  0002  SETMBR	R0	K40	K3
      0x60100003,  //  0003  GETGBL	R4	G3
      0x5C140000,  //  0004  MOVE	R5	R0
      0x7C100200,  //  0005  CALL	R4	1
      0x8C10092E,  //  0006  GETMET	R4	R4	K46
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
      0xB8125E00,  //  0000  GETNGBL	R4	K47
      0x88100930,  //  0001  GETMBR	R4	R4	K48
      0x88140700,  //  0002  GETMBR	R5	R3	K0
      0x88180731,  //  0003  GETMBR	R6	R3	K49
      0x541E0005,  //  0004  LDINT	R7	6
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E002B,  //  0006  JMPF	R7	#0033
      0x8C1C0102,  //  0007  GETMET	R7	R0	K2
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D03,  //  0009  EQ	R7	R6	K3
      0x781E0009,  //  000A  JMPF	R7	#0015
      0x8C1C0109,  //  000B  GETMET	R7	R0	K9
      0x50240000,  //  000C  LDBOOL	R9	0	0
      0x7C1C0400,  //  000D  CALL	R7	2
      0x8C1C0132,  //  000E  GETMET	R7	R0	K50
      0x58240008,  //  000F  LDCONST	R9	K8
      0x58280003,  //  0010  LDCONST	R10	K3
      0x7C1C0600,  //  0011  CALL	R7	3
      0x501C0200,  //  0012  LDBOOL	R7	1	0
      0x80040E00,  //  0013  RET	1	R7
      0x7002001D,  //  0014  JMP		#0033
      0x1C1C0D0C,  //  0015  EQ	R7	R6	K12
      0x781E0009,  //  0016  JMPF	R7	#0021
      0x8C1C0109,  //  0017  GETMET	R7	R0	K9
      0x50240200,  //  0018  LDBOOL	R9	1	0
      0x7C1C0400,  //  0019  CALL	R7	2
      0x8C1C0132,  //  001A  GETMET	R7	R0	K50
      0x58240008,  //  001B  LDCONST	R9	K8
      0x5828000C,  //  001C  LDCONST	R10	K12
      0x7C1C0600,  //  001D  CALL	R7	3
      0x501C0200,  //  001E  LDBOOL	R7	1	0
      0x80040E00,  //  001F  RET	1	R7
      0x70020011,  //  0020  JMP		#0033
      0x1C1C0D33,  //  0021  EQ	R7	R6	K51
      0x781E000F,  //  0022  JMPF	R7	#0033
      0x8C1C0109,  //  0023  GETMET	R7	R0	K9
      0x88240105,  //  0024  GETMBR	R9	R0	K5
      0x78260000,  //  0025  JMPF	R9	#0027
      0x50240001,  //  0026  LDBOOL	R9	0	1
      0x50240200,  //  0027  LDBOOL	R9	1	0
      0x7C1C0400,  //  0028  CALL	R7	2
      0x8C1C0132,  //  0029  GETMET	R7	R0	K50
      0x58240008,  //  002A  LDCONST	R9	K8
      0x88280105,  //  002B  GETMBR	R10	R0	K5
      0x782A0001,  //  002C  JMPF	R10	#002F
      0x5828000C,  //  002D  LDCONST	R10	K12
      0x70020000,  //  002E  JMP		#0030
      0x58280003,  //  002F  LDCONST	R10	K3
      0x7C1C0600,  //  0030  CALL	R7	3
      0x501C0200,  //  0031  LDBOOL	R7	1	0
      0x80040E00,  //  0032  RET	1	R7
      0x80000000,  //  0033  RET	0
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
    ( &(const binstruction[22]) {  /* code */
      0x60080003,  //  0000  GETGBL	R2	G3
      0x5C0C0000,  //  0001  MOVE	R3	R0
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080534,  //  0003  GETMET	R2	R2	K52
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080009,  //  0006  GETGBL	R2	G9
      0x8C0C0307,  //  0007  GETMET	R3	R1	K7
      0x58140035,  //  0008  LDCONST	R5	K53
      0x4C180000,  //  0009  LDNIL	R6
      0x7C0C0600,  //  000A  CALL	R3	3
      0x7C080200,  //  000B  CALL	R2	1
      0x90021602,  //  000C  SETMBR	R0	K11	R2
      0x8808010B,  //  000D  GETMBR	R2	R0	K11
      0x4C0C0000,  //  000E  LDNIL	R3
      0x20080403,  //  000F  NE	R2	R2	R3
      0x780A0003,  //  0010  JMPF	R2	#0015
      0x8808010B,  //  0011  GETMBR	R2	R0	K11
      0x18080503,  //  0012  LE	R2	R2	K3
      0x780A0000,  //  0013  JMPF	R2	#0015
      0x9002170C,  //  0014  SETMBR	R0	K11	K12
      0x80000000,  //  0015  RET	0
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
      0x88040122,  //  0000  GETMBR	R1	R0	K34
      0x74060018,  //  0001  JMPT	R1	#001B
      0x8804011D,  //  0002  GETMBR	R1	R0	K29
      0x74060016,  //  0003  JMPT	R1	#001B
      0x8804010B,  //  0004  GETMBR	R1	R0	K11
      0x4C080000,  //  0005  LDNIL	R2
      0x20040202,  //  0006  NE	R1	R1	R2
      0x78060012,  //  0007  JMPF	R1	#001B
      0xB8064600,  //  0008  GETNGBL	R1	K35
      0x8C040336,  //  0009  GETMET	R1	R1	K54
      0x880C010B,  //  000A  GETMBR	R3	R0	K11
      0x040C070C,  //  000B  SUB	R3	R3	K12
      0x7C040400,  //  000C  CALL	R1	2
      0x4C080000,  //  000D  LDNIL	R2
      0x20080202,  //  000E  NE	R2	R1	R2
      0x780A000A,  //  000F  JMPF	R2	#001B
      0x88080105,  //  0010  GETMBR	R2	R0	K5
      0x600C0017,  //  0011  GETGBL	R3	G23
      0x5C100200,  //  0012  MOVE	R4	R1
      0x7C0C0200,  //  0013  CALL	R3	1
      0x20080403,  //  0014  NE	R2	R2	R3
      0x780A0003,  //  0015  JMPF	R2	#001A
      0x8C080110,  //  0016  GETMET	R2	R0	K16
      0x54120005,  //  0017  LDINT	R4	6
      0x58140003,  //  0018  LDCONST	R5	K3
      0x7C080600,  //  0019  CALL	R2	3
      0x90020A01,  //  001A  SETMBR	R0	K5	R1
      0x60040003,  //  001B  GETGBL	R1	G3
      0x5C080000,  //  001C  MOVE	R2	R0
      0x7C040200,  //  001D  CALL	R1	1
      0x8C040325,  //  001E  GETMET	R1	R1	K37
      0x7C040200,  //  001F  CALL	R1	1
      0x80000000,  //  0020  RET	0
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
      0xA4065200,  //  0000  IMPORT	R1	K41
      0x8C080137,  //  0001  GETMET	R2	R0	K55
      0x7C080200,  //  0002  CALL	R2	1
      0x5C0C0400,  //  0003  MOVE	R3	R2
      0x740E0004,  //  0004  JMPT	R3	#000A
      0x600C0008,  //  0005  GETGBL	R3	G8
      0x8810010B,  //  0006  GETMBR	R4	R0	K11
      0x7C0C0200,  //  0007  CALL	R3	1
      0x000E1003,  //  0008  ADD	R3	K8	R3
      0x5C080600,  //  0009  MOVE	R2	R3
      0x8C0C032B,  //  000A  GETMET	R3	R1	K43
      0x60140018,  //  000B  GETGBL	R5	G24
      0x88180138,  //  000C  GETMBR	R6	R0	K56
      0x780A0003,  //  000D  JMPF	R2	#0012
      0x8C1C0339,  //  000E  GETMET	R7	R1	K57
      0x5C240400,  //  000F  MOVE	R9	R2
      0x7C1C0400,  //  0010  CALL	R7	2
      0x70020000,  //  0011  JMP		#0013
      0x581C003A,  //  0012  LDCONST	R7	K58
      0x7C140400,  //  0013  CALL	R5	2
      0x7C0C0400,  //  0014  CALL	R3	2
      0x80000000,  //  0015  RET	0
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
    be_nested_map(21,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(read_attribute, -1), be_const_closure(class_Matter_Plugin_Light0_read_attribute_closure) },
        { be_const_key_weak(tasmota_relay_index, -1), be_const_var(0) },
        { be_const_key_weak(update_virtual, 1), be_const_closure(class_Matter_Plugin_Light0_update_virtual_closure) },
        { be_const_key_weak(parse_status, 5), be_const_closure(class_Matter_Plugin_Light0_parse_status_closure) },
        { be_const_key_weak(zigbee_received, 16), be_const_closure(class_Matter_Plugin_Light0_zigbee_received_closure) },
        { be_const_key_weak(SCHEMA, 18), be_nested_str_weak(relay_X7Cl_X3ARelay_X20number_X7Ct_X3Ai_X7Ch_X3ARelay_X3Cx_X3E_X20number) },
        { be_const_key_weak(set_onoff, -1), be_const_closure(class_Matter_Plugin_Light0_set_onoff_closure) },
        { be_const_key_weak(CLUSTERS, 8), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(6, -1), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(5, -1), be_const_bytes_instance(000000010002000300040005FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(57, -1), be_const_bytes_instance(00030005000A000F00110012FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(3, 2), be_const_bytes_instance(00000001FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(4, -1), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(29, 1), be_const_bytes_instance(0000000100020003FFF8FFF9FFFAFFFBFFFCFFFD) },
    }))    ) } )) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(class_Matter_Plugin_Light0_update_shadow_closure) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(250) },
        { be_const_key_weak(TYPE, -1), be_nested_str_weak(light0) },
        { be_const_key_weak(web_values, -1), be_const_closure(class_Matter_Plugin_Light0_web_values_closure) },
        { be_const_key_weak(DISPLAY_NAME, 19), be_nested_str_weak(Light_X200_X20OnOff) },
        { be_const_key_weak(shadow_onoff, -1), be_const_var(2) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(class_Matter_Plugin_Light0_invoke_request_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(256, -1), be_const_int(3) },
    }))    ) } )) },
        { be_const_key_weak(parse_configuration, -1), be_const_closure(class_Matter_Plugin_Light0_parse_configuration_closure) },
        { be_const_key_weak(light_index, 15), be_const_var(1) },
        { be_const_key_weak(UPDATE_COMMANDS, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(1,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Power),
    }))    ) } )) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Plugin_Light0_init_closure) },
        { be_const_key_weak(web_values_prefix, -1), be_const_closure(class_Matter_Plugin_Light0_web_values_prefix_closure) },
    })),
    be_str_weak(Matter_Plugin_Light0)
);
/********************************************************************/
/* End of solidification */
