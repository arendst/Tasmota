/* Solidification of Matter_Plugin_2_Light0.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Plugin_Light0' ktab size: 60, total: 104 (saved 352 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Light0[60] = {
  /* K0   */  be_nested_str_weak(matter),
  /* K1   */  be_nested_str_weak(TLV),
  /* K2   */  be_nested_str_weak(cluster),
  /* K3   */  be_nested_str_weak(attribute),
  /* K4   */  be_nested_str_weak(update_shadow_lazy),
  /* K5   */  be_const_int(0),
  /* K6   */  be_nested_str_weak(set),
  /* K7   */  be_nested_str_weak(BOOL),
  /* K8   */  be_nested_str_weak(shadow_onoff),
  /* K9   */  be_nested_str_weak(read_attribute),
  /* K10  */  be_nested_str_weak(find),
  /* K11  */  be_nested_str_weak(Power),
  /* K12  */  be_nested_str_weak(set_onoff),
  /* K13  */  be_nested_str_weak(update_virtual),
  /* K14  */  be_nested_str_weak(tasmota_relay_index),
  /* K15  */  be_const_int(1),
  /* K16  */  be_nested_str_weak(contains),
  /* K17  */  be_nested_str_weak(POWER),
  /* K18  */  be_nested_str_weak(ON),
  /* K19  */  be_nested_str_weak(attribute_updated),
  /* K20  */  be_nested_str_weak(math),
  /* K21  */  be_nested_str_weak(log),
  /* K22  */  be_nested_str_weak(MTR_X3A_X20zigbee_received_X20Ox_X2504X_X20attr_list_X3D_X25s_X20type_X28attr_list_X29_X3D_X25s),
  /* K23  */  be_nested_str_weak(zigbee_mapper),
  /* K24  */  be_nested_str_weak(shortaddr),
  /* K25  */  be_const_int(3),
  /* K26  */  be_nested_str_weak(key),
  /* K27  */  be_nested_str_weak(val),
  /* K28  */  be_nested_str_weak(Dimmer),
  /* K29  */  be_nested_str_weak(CT),
  /* K30  */  be_nested_str_weak(MTR_X3A_X20_X5B_X2502X_X5D_X20Light2_X20updated_X20_X25s),
  /* K31  */  be_nested_str_weak(endpoint),
  /* K32  */  be_nested_str_weak(BRIDGE),
  /* K33  */  be_nested_str_weak(call_remote_sync),
  /* K34  */  be_nested_str_weak(1),
  /* K35  */  be_nested_str_weak(0),
  /* K36  */  be_nested_str_weak(parse_status),
  /* K37  */  be_nested_str_weak(VIRTUAL),
  /* K38  */  be_nested_str_weak(tasmota),
  /* K39  */  be_nested_str_weak(set_power),
  /* K40  */  be_nested_str_weak(update_shadow),
  /* K41  */  be_nested_str_weak(light),
  /* K42  */  be_nested_str_weak(power),
  /* K43  */  be_nested_str_weak(light_index),
  /* K44  */  be_nested_str_weak(webserver),
  /* K45  */  be_nested_str_weak(web_values_prefix),
  /* K46  */  be_nested_str_weak(content_send),
  /* K47  */  be_nested_str_weak(_X25s),
  /* K48  */  be_nested_str_weak(web_value_onoff),
  /* K49  */  be_nested_str_weak(init),
  /* K50  */  be_nested_str_weak(command),
  /* K51  */  be_nested_str_weak(publish_command),
  /* K52  */  be_const_int(2),
  /* K53  */  be_nested_str_weak(parse_configuration),
  /* K54  */  be_nested_str_weak(relay),
  /* K55  */  be_nested_str_weak(get_power),
  /* K56  */  be_nested_str_weak(get_name),
  /* K57  */  be_nested_str_weak(PREFIX),
  /* K58  */  be_nested_str_weak(html_escape),
  /* K59  */  be_nested_str_weak(),
};


extern const bclass be_class_Matter_Plugin_Light0;

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
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x541E0005,  //  0004  LDINT	R7	6
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E0008,  //  0006  JMPF	R7	#0010
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D05,  //  0009  EQ	R7	R6	K5
      0x781E0004,  //  000A  JMPF	R7	#0010
      0x8C1C0706,  //  000B  GETMET	R7	R3	K6
      0x88240907,  //  000C  GETMBR	R9	R4	K7
      0x88280108,  //  000D  GETMBR	R10	R0	K8
      0x7C1C0600,  //  000E  CALL	R7	3
      0x80040E00,  //  000F  RET	1	R7
      0x601C0003,  //  0010  GETGBL	R7	G3
      0x5C200000,  //  0011  MOVE	R8	R0
      0x7C1C0200,  //  0012  CALL	R7	1
      0x8C1C0F09,  //  0013  GETMET	R7	R7	K9
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
      0x8C08030A,  //  0000  GETMET	R2	R1	K10
      0x5810000B,  //  0001  LDCONST	R4	K11
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x200C0403,  //  0004  NE	R3	R2	R3
      0x780E0004,  //  0005  JMPF	R3	#000B
      0x8C0C010C,  //  0006  GETMET	R3	R0	K12
      0x60140017,  //  0007  GETGBL	R5	G23
      0x5C180400,  //  0008  MOVE	R6	R2
      0x7C140200,  //  0009  CALL	R5	1
      0x7C0C0400,  //  000A  CALL	R3	2
      0x600C0003,  //  000B  GETGBL	R3	G3
      0x5C100000,  //  000C  MOVE	R4	R0
      0x7C0C0200,  //  000D  CALL	R3	1
      0x8C0C070D,  //  000E  GETMET	R3	R3	K13
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
      0x8810010E,  //  0004  GETMBR	R4	R0	K14
      0x1C10090F,  //  0005  EQ	R4	R4	K15
      0x78120009,  //  0006  JMPF	R4	#0011
      0x8C100310,  //  0007  GETMET	R4	R1	K16
      0x58180011,  //  0008  LDCONST	R6	K17
      0x7C100400,  //  0009  CALL	R4	2
      0x78120005,  //  000A  JMPF	R4	#0011
      0x8C10030A,  //  000B  GETMET	R4	R1	K10
      0x58180011,  //  000C  LDCONST	R6	K17
      0x7C100400,  //  000D  CALL	R4	2
      0x1C100912,  //  000E  EQ	R4	R4	K18
      0x5C0C0800,  //  000F  MOVE	R3	R4
      0x70020007,  //  0010  JMP		#0019
      0x8C10030A,  //  0011  GETMET	R4	R1	K10
      0x60180008,  //  0012  GETGBL	R6	G8
      0x881C010E,  //  0013  GETMBR	R7	R0	K14
      0x7C180200,  //  0014  CALL	R6	1
      0x001A2206,  //  0015  ADD	R6	K17	R6
      0x7C100400,  //  0016  CALL	R4	2
      0x1C100912,  //  0017  EQ	R4	R4	K18
      0x5C0C0800,  //  0018  MOVE	R3	R4
      0x88100108,  //  0019  GETMBR	R4	R0	K8
      0x60140017,  //  001A  GETGBL	R5	G23
      0x5C180600,  //  001B  MOVE	R6	R3
      0x7C140200,  //  001C  CALL	R5	1
      0x20100805,  //  001D  NE	R4	R4	R5
      0x78120004,  //  001E  JMPF	R4	#0024
      0x8C100113,  //  001F  GETMET	R4	R0	K19
      0x541A0005,  //  0020  LDINT	R6	6
      0x581C0005,  //  0021  LDCONST	R7	K5
      0x7C100600,  //  0022  CALL	R4	3
      0x90021003,  //  0023  SETMBR	R0	K8	R3
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
      0xA40E2800,  //  0000  IMPORT	R3	K20
      0xB8122A00,  //  0001  GETNGBL	R4	K21
      0x60140018,  //  0002  GETGBL	R5	G24
      0x58180016,  //  0003  LDCONST	R6	K22
      0x881C0117,  //  0004  GETMBR	R7	R0	K23
      0x881C0F18,  //  0005  GETMBR	R7	R7	K24
      0x5C200400,  //  0006  MOVE	R8	R2
      0x60240004,  //  0007  GETGBL	R9	G4
      0x5C280400,  //  0008  MOVE	R10	R2
      0x7C240200,  //  0009  CALL	R9	1
      0x7C140800,  //  000A  CALL	R5	4
      0x58180019,  //  000B  LDCONST	R6	K25
      0x7C100400,  //  000C  CALL	R4	2
      0x58100005,  //  000D  LDCONST	R4	K5
      0x60140013,  //  000E  GETGBL	R5	G19
      0x7C140000,  //  000F  CALL	R5	0
      0x6018000C,  //  0010  GETGBL	R6	G12
      0x5C1C0400,  //  0011  MOVE	R7	R2
      0x7C180200,  //  0012  CALL	R6	1
      0x14180806,  //  0013  LT	R6	R4	R6
      0x781A0017,  //  0014  JMPF	R6	#002D
      0x94180404,  //  0015  GETIDX	R6	R2	R4
      0x881C0D1A,  //  0016  GETMBR	R7	R6	K26
      0x1C1C0F0B,  //  0017  EQ	R7	R7	K11
      0x781E0003,  //  0018  JMPF	R7	#001D
      0x601C0009,  //  0019  GETGBL	R7	G9
      0x88200D1B,  //  001A  GETMBR	R8	R6	K27
      0x7C1C0200,  //  001B  CALL	R7	1
      0x98161607,  //  001C  SETIDX	R5	K11	R7
      0x881C0D1A,  //  001D  GETMBR	R7	R6	K26
      0x1C1C0F1C,  //  001E  EQ	R7	R7	K28
      0x781E0003,  //  001F  JMPF	R7	#0024
      0x601C0009,  //  0020  GETGBL	R7	G9
      0x88200D1B,  //  0021  GETMBR	R8	R6	K27
      0x7C1C0200,  //  0022  CALL	R7	1
      0x98163807,  //  0023  SETIDX	R5	K28	R7
      0x881C0D1A,  //  0024  GETMBR	R7	R6	K26
      0x1C1C0F1D,  //  0025  EQ	R7	R7	K29
      0x781E0003,  //  0026  JMPF	R7	#002B
      0x601C0009,  //  0027  GETGBL	R7	G9
      0x88200D1B,  //  0028  GETMBR	R8	R6	K27
      0x7C1C0200,  //  0029  CALL	R7	1
      0x98163A07,  //  002A  SETIDX	R5	K29	R7
      0x0010090F,  //  002B  ADD	R4	R4	K15
      0x7001FFE2,  //  002C  JMP		#0010
      0x6018000C,  //  002D  GETGBL	R6	G12
      0x5C1C0A00,  //  002E  MOVE	R7	R5
      0x7C180200,  //  002F  CALL	R6	1
      0x24180D05,  //  0030  GT	R6	R6	K5
      0x781A000A,  //  0031  JMPF	R6	#003D
      0x8C18010D,  //  0032  GETMET	R6	R0	K13
      0x5C200A00,  //  0033  MOVE	R8	R5
      0x7C180400,  //  0034  CALL	R6	2
      0xB81A2A00,  //  0035  GETNGBL	R6	K21
      0x601C0018,  //  0036  GETGBL	R7	G24
      0x5820001E,  //  0037  LDCONST	R8	K30
      0x8824011F,  //  0038  GETMBR	R9	R0	K31
      0x5C280A00,  //  0039  MOVE	R10	R5
      0x7C1C0600,  //  003A  CALL	R7	3
      0x58200019,  //  003B  LDCONST	R8	K25
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
      0x88080120,  //  0000  GETMBR	R2	R0	K32
      0x780A0011,  //  0001  JMPF	R2	#0014
      0x8C080121,  //  0002  GETMET	R2	R0	K33
      0x60100008,  //  0003  GETGBL	R4	G8
      0x8814010E,  //  0004  GETMBR	R5	R0	K14
      0x7C100200,  //  0005  CALL	R4	1
      0x00121604,  //  0006  ADD	R4	K11	R4
      0x78060001,  //  0007  JMPF	R1	#000A
      0x58140022,  //  0008  LDCONST	R5	K34
      0x70020000,  //  0009  JMP		#000B
      0x58140023,  //  000A  LDCONST	R5	K35
      0x7C080600,  //  000B  CALL	R2	3
      0x4C0C0000,  //  000C  LDNIL	R3
      0x200C0403,  //  000D  NE	R3	R2	R3
      0x780E0003,  //  000E  JMPF	R3	#0013
      0x8C0C0124,  //  000F  GETMET	R3	R0	K36
      0x5C140400,  //  0010  MOVE	R5	R2
      0x541A000A,  //  0011  LDINT	R6	11
      0x7C0C0600,  //  0012  CALL	R3	3
      0x70020022,  //  0013  JMP		#0037
      0x88080125,  //  0014  GETMBR	R2	R0	K37
      0x780A0008,  //  0015  JMPF	R2	#001F
      0x88080108,  //  0016  GETMBR	R2	R0	K8
      0x20080202,  //  0017  NE	R2	R1	R2
      0x780A0004,  //  0018  JMPF	R2	#001E
      0x8C080113,  //  0019  GETMET	R2	R0	K19
      0x54120005,  //  001A  LDINT	R4	6
      0x58140005,  //  001B  LDCONST	R5	K5
      0x7C080600,  //  001C  CALL	R2	3
      0x90021001,  //  001D  SETMBR	R0	K8	R1
      0x70020017,  //  001E  JMP		#0037
      0x8808010E,  //  001F  GETMBR	R2	R0	K14
      0x4C0C0000,  //  0020  LDNIL	R3
      0x20080403,  //  0021  NE	R2	R2	R3
      0x780A000A,  //  0022  JMPF	R2	#002E
      0xB80A4C00,  //  0023  GETNGBL	R2	K38
      0x8C080527,  //  0024  GETMET	R2	R2	K39
      0x8810010E,  //  0025  GETMBR	R4	R0	K14
      0x0410090F,  //  0026  SUB	R4	R4	K15
      0x60140017,  //  0027  GETGBL	R5	G23
      0x5C180200,  //  0028  MOVE	R6	R1
      0x7C140200,  //  0029  CALL	R5	1
      0x7C080600,  //  002A  CALL	R2	3
      0x8C080128,  //  002B  GETMET	R2	R0	K40
      0x7C080200,  //  002C  CALL	R2	1
      0x70020008,  //  002D  JMP		#0037
      0xA40A5200,  //  002E  IMPORT	R2	K41
      0x8C0C0506,  //  002F  GETMET	R3	R2	K6
      0x60140013,  //  0030  GETGBL	R5	G19
      0x7C140000,  //  0031  CALL	R5	0
      0x98165401,  //  0032  SETIDX	R5	K42	R1
      0x8818012B,  //  0033  GETMBR	R6	R0	K43
      0x7C0C0600,  //  0034  CALL	R3	3
      0x8C0C0128,  //  0035  GETMET	R3	R0	K40
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
      0xA4065800,  //  0000  IMPORT	R1	K44
      0x8C08012D,  //  0001  GETMET	R2	R0	K45
      0x7C080200,  //  0002  CALL	R2	1
      0x8C08032E,  //  0003  GETMET	R2	R1	K46
      0x60100018,  //  0004  GETGBL	R4	G24
      0x5814002F,  //  0005  LDCONST	R5	K47
      0x8C180130,  //  0006  GETMET	R6	R0	K48
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
      0x90025705,  //  0002  SETMBR	R0	K43	K5
      0x60100003,  //  0003  GETGBL	R4	G3
      0x5C140000,  //  0004  MOVE	R5	R0
      0x7C100200,  //  0005  CALL	R4	1
      0x8C100931,  //  0006  GETMET	R4	R4	K49
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
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140702,  //  0002  GETMBR	R5	R3	K2
      0x88180732,  //  0003  GETMBR	R6	R3	K50
      0x541E0005,  //  0004  LDINT	R7	6
      0x1C1C0A07,  //  0005  EQ	R7	R5	R7
      0x781E002B,  //  0006  JMPF	R7	#0033
      0x8C1C0104,  //  0007  GETMET	R7	R0	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x1C1C0D05,  //  0009  EQ	R7	R6	K5
      0x781E0009,  //  000A  JMPF	R7	#0015
      0x8C1C010C,  //  000B  GETMET	R7	R0	K12
      0x50240000,  //  000C  LDBOOL	R9	0	0
      0x7C1C0400,  //  000D  CALL	R7	2
      0x8C1C0133,  //  000E  GETMET	R7	R0	K51
      0x5824000B,  //  000F  LDCONST	R9	K11
      0x58280005,  //  0010  LDCONST	R10	K5
      0x7C1C0600,  //  0011  CALL	R7	3
      0x501C0200,  //  0012  LDBOOL	R7	1	0
      0x80040E00,  //  0013  RET	1	R7
      0x7002001D,  //  0014  JMP		#0033
      0x1C1C0D0F,  //  0015  EQ	R7	R6	K15
      0x781E0009,  //  0016  JMPF	R7	#0021
      0x8C1C010C,  //  0017  GETMET	R7	R0	K12
      0x50240200,  //  0018  LDBOOL	R9	1	0
      0x7C1C0400,  //  0019  CALL	R7	2
      0x8C1C0133,  //  001A  GETMET	R7	R0	K51
      0x5824000B,  //  001B  LDCONST	R9	K11
      0x5828000F,  //  001C  LDCONST	R10	K15
      0x7C1C0600,  //  001D  CALL	R7	3
      0x501C0200,  //  001E  LDBOOL	R7	1	0
      0x80040E00,  //  001F  RET	1	R7
      0x70020011,  //  0020  JMP		#0033
      0x1C1C0D34,  //  0021  EQ	R7	R6	K52
      0x781E000F,  //  0022  JMPF	R7	#0033
      0x8C1C010C,  //  0023  GETMET	R7	R0	K12
      0x88240108,  //  0024  GETMBR	R9	R0	K8
      0x78260000,  //  0025  JMPF	R9	#0027
      0x50240001,  //  0026  LDBOOL	R9	0	1
      0x50240200,  //  0027  LDBOOL	R9	1	0
      0x7C1C0400,  //  0028  CALL	R7	2
      0x8C1C0133,  //  0029  GETMET	R7	R0	K51
      0x5824000B,  //  002A  LDCONST	R9	K11
      0x88280108,  //  002B  GETMBR	R10	R0	K8
      0x782A0001,  //  002C  JMPF	R10	#002F
      0x5828000F,  //  002D  LDCONST	R10	K15
      0x70020000,  //  002E  JMP		#0030
      0x58280005,  //  002F  LDCONST	R10	K5
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
      0x8C080535,  //  0003  GETMET	R2	R2	K53
      0x5C100200,  //  0004  MOVE	R4	R1
      0x7C080400,  //  0005  CALL	R2	2
      0x60080009,  //  0006  GETGBL	R2	G9
      0x8C0C030A,  //  0007  GETMET	R3	R1	K10
      0x58140036,  //  0008  LDCONST	R5	K54
      0x4C180000,  //  0009  LDNIL	R6
      0x7C0C0600,  //  000A  CALL	R3	3
      0x7C080200,  //  000B  CALL	R2	1
      0x90021C02,  //  000C  SETMBR	R0	K14	R2
      0x8808010E,  //  000D  GETMBR	R2	R0	K14
      0x4C0C0000,  //  000E  LDNIL	R3
      0x20080403,  //  000F  NE	R2	R2	R3
      0x780A0003,  //  0010  JMPF	R2	#0015
      0x8808010E,  //  0011  GETMBR	R2	R0	K14
      0x18080505,  //  0012  LE	R2	R2	K5
      0x780A0000,  //  0013  JMPF	R2	#0015
      0x90021D0F,  //  0014  SETMBR	R0	K14	K15
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
      0x88040125,  //  0000  GETMBR	R1	R0	K37
      0x74060018,  //  0001  JMPT	R1	#001B
      0x88040120,  //  0002  GETMBR	R1	R0	K32
      0x74060016,  //  0003  JMPT	R1	#001B
      0x8804010E,  //  0004  GETMBR	R1	R0	K14
      0x4C080000,  //  0005  LDNIL	R2
      0x20040202,  //  0006  NE	R1	R1	R2
      0x78060012,  //  0007  JMPF	R1	#001B
      0xB8064C00,  //  0008  GETNGBL	R1	K38
      0x8C040337,  //  0009  GETMET	R1	R1	K55
      0x880C010E,  //  000A  GETMBR	R3	R0	K14
      0x040C070F,  //  000B  SUB	R3	R3	K15
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
      0x8C080113,  //  0016  GETMET	R2	R0	K19
      0x54120005,  //  0017  LDINT	R4	6
      0x58140005,  //  0018  LDCONST	R5	K5
      0x7C080600,  //  0019  CALL	R2	3
      0x90021001,  //  001A  SETMBR	R0	K8	R1
      0x60040003,  //  001B  GETGBL	R1	G3
      0x5C080000,  //  001C  MOVE	R2	R0
      0x7C040200,  //  001D  CALL	R1	1
      0x8C040328,  //  001E  GETMET	R1	R1	K40
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
      0xA4065800,  //  0000  IMPORT	R1	K44
      0x8C080138,  //  0001  GETMET	R2	R0	K56
      0x7C080200,  //  0002  CALL	R2	1
      0x5C0C0400,  //  0003  MOVE	R3	R2
      0x740E0004,  //  0004  JMPT	R3	#000A
      0x600C0008,  //  0005  GETGBL	R3	G8
      0x8810010E,  //  0006  GETMBR	R4	R0	K14
      0x7C0C0200,  //  0007  CALL	R3	1
      0x000E1603,  //  0008  ADD	R3	K11	R3
      0x5C080600,  //  0009  MOVE	R2	R3
      0x8C0C032E,  //  000A  GETMET	R3	R1	K46
      0x60140018,  //  000B  GETGBL	R5	G24
      0x88180139,  //  000C  GETMBR	R6	R0	K57
      0x780A0003,  //  000D  JMPF	R2	#0012
      0x8C1C033A,  //  000E  GETMET	R7	R1	K58
      0x5C240400,  //  000F  MOVE	R9	R2
      0x7C1C0400,  //  0010  CALL	R7	2
      0x70020000,  //  0011  JMP		#0013
      0x581C003B,  //  0012  LDCONST	R7	K59
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
