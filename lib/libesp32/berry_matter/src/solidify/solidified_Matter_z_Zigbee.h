/* Solidification of Matter_z_Zigbee.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Zigbee_Mapper' ktab size: 46, total: 85 (saved 312 bytes)
static const bvalue be_ktab_class_Matter_Zigbee_Mapper[46] = {
  /* K0   */  be_nested_str_weak(resolve_zb_device),
  /* K1   */  be_nested_str_weak(endpoint),
  /* K2   */  be_nested_str_weak(device_arg_str),
  /* K3   */  be_nested_str_weak(string),
  /* K4   */  be_nested_str_weak(0x_X2504X),
  /* K5   */  be_nested_str_weak(shortaddr),
  /* K6   */  be_nested_str_weak(pi),
  /* K7   */  be_nested_str_weak(consolidate_update_commands),
  /* K8   */  be_nested_str_weak(Power),
  /* K9   */  be_nested_str_weak(Bri),
  /* K10  */  be_nested_str_weak(Dimmer),
  /* K11  */  be_nested_str_weak(CT),
  /* K12  */  be_nested_str_weak(tasmota),
  /* K13  */  be_nested_str_weak(cmd),
  /* K14  */  be_nested_str_weak(ZbRead_X20_X7B_X22Device_X22_X3A_X22_X25s_X22_X2C_X22Endpoint_X22_X3A_X25s_X2C_X22Read_X22_X3A_X22_X25s_X22_X7D),
  /* K15  */  be_nested_str_weak(stop_iteration),
  /* K16  */  be_nested_str_weak(read_zb_info),
  /* K17  */  be_nested_str_weak(log),
  /* K18  */  be_nested_str_weak(MTR_X3A_X20Read_X20information_X20for_X20zigbee_X20device_X200x_X2504X),
  /* K19  */  be_const_int(3),
  /* K20  */  be_nested_str_weak(zigbee_received),
  /* K21  */  be_nested_str_weak(_X2C_X22Endpoint_X22_X3A_X25s),
  /* K22  */  be_nested_str_weak(),
  /* K23  */  be_nested_str_weak(ZbSend_X20_X7B_X22Device_X22_X3A_X22_X25s_X22_X25s_X2C_X22Send_X22_X3A_X7B_X22Power_X22_X3A_X25i_X7D_X7D),
  /* K24  */  be_nested_str_weak(ZbSend_X20_X7B_X22Device_X22_X3A_X22_X25s_X22_X25s_X2C_X22Send_X22_X3A_X7B_X22Dimmer_X22_X3A_X25i_X7D_X7D),
  /* K25  */  be_nested_str_weak(ZbSend_X20_X7B_X22Device_X22_X3A_X22_X25s_X22_X25s_X2C_X22Send_X22_X3A_X7B_X22CT_X22_X3A_X25i_X7D_X7D),
  /* K26  */  be_nested_str_weak(loglevel),
  /* K27  */  be_nested_str_weak(MTR_X3A_X20_X27_X25s_X27),
  /* K28  */  be_nested_str_weak(zigbee),
  /* K29  */  be_nested_str_weak(device_arg),
  /* K30  */  be_nested_str_weak(zigbee_device),
  /* K31  */  be_nested_str_weak(find),
  /* K32  */  be_nested_str_weak(startswith),
  /* K33  */  be_nested_str_weak(0x),
  /* K34  */  be_nested_str_weak(0X),
  /* K35  */  be_nested_str_weak(_lookup_endpoint_by_name),
  /* K36  */  be_nested_str_weak(MTR_X3A_X20cannot_X20find_X20zigbee_X20device_X20_X27_X25s_X27),
  /* K37  */  be_nested_str_weak(keys),
  /* K38  */  be_const_int(0),
  /* K39  */  be_nested_str_weak(_search_for_ep),
  /* K40  */  be_nested_str_weak(_X3A),
  /* K41  */  be_const_int(1),
  /* K42  */  be_const_int(2147483647),
  /* K43  */  be_nested_str_weak(set_timer),
  /* K44  */  be_nested_str_weak(ZbName_X20_X25s),
  /* K45  */  be_nested_str_weak(info),
};


extern const bclass be_class_Matter_Zigbee_Mapper;

/********************************************************************
** Solidified function: probe_zb_values
********************************************************************/
be_local_closure(class_Matter_Zigbee_Mapper_probe_zb_values,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Zigbee_Mapper,     /* shared constants */
    be_str_weak(probe_zb_values),
    &be_const_str_solidified,
    ( &(const binstruction[75]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x74060000,  //  0002  JMPT	R1	#0004
      0x80000200,  //  0003  RET	0
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x4C080000,  //  0005  LDNIL	R2
      0x20040202,  //  0006  NE	R1	R1	R2
      0x78060030,  //  0007  JMPF	R1	#0039
      0x60040004,  //  0008  GETGBL	R1	G4
      0x88080102,  //  0009  GETMBR	R2	R0	K2
      0x7C040200,  //  000A  CALL	R1	1
      0x1C040303,  //  000B  EQ	R1	R1	K3
      0x78060001,  //  000C  JMPF	R1	#000F
      0x88040102,  //  000D  GETMBR	R1	R0	K2
      0x70020003,  //  000E  JMP		#0013
      0x60040018,  //  000F  GETGBL	R1	G24
      0x58080004,  //  0010  LDCONST	R2	K4
      0x880C0105,  //  0011  GETMBR	R3	R0	K5
      0x7C040400,  //  0012  CALL	R1	2
      0x60080010,  //  0013  GETGBL	R2	G16
      0x880C0106,  //  0014  GETMBR	R3	R0	K6
      0x8C0C0707,  //  0015  GETMET	R3	R3	K7
      0x7C0C0200,  //  0016  CALL	R3	1
      0x7C080200,  //  0017  CALL	R2	1
      0xA802001B,  //  0018  EXBLK	0	#0035
      0x5C0C0400,  //  0019  MOVE	R3	R2
      0x7C0C0000,  //  001A  CALL	R3	0
      0x4C100000,  //  001B  LDNIL	R4
      0x1C140708,  //  001C  EQ	R5	R3	K8
      0x78160001,  //  001D  JMPF	R5	#0020
      0x58100008,  //  001E  LDCONST	R4	K8
      0x70020006,  //  001F  JMP		#0027
      0x1C140709,  //  0020  EQ	R5	R3	K9
      0x78160001,  //  0021  JMPF	R5	#0024
      0x5810000A,  //  0022  LDCONST	R4	K10
      0x70020002,  //  0023  JMP		#0027
      0x1C14070B,  //  0024  EQ	R5	R3	K11
      0x78160000,  //  0025  JMPF	R5	#0027
      0x5810000B,  //  0026  LDCONST	R4	K11
      0x4C140000,  //  0027  LDNIL	R5
      0x20140805,  //  0028  NE	R5	R4	R5
      0x78160009,  //  0029  JMPF	R5	#0034
      0xB8161800,  //  002A  GETNGBL	R5	K12
      0x8C140B0D,  //  002B  GETMET	R5	R5	K13
      0x601C0018,  //  002C  GETGBL	R7	G24
      0x5820000E,  //  002D  LDCONST	R8	K14
      0x5C240200,  //  002E  MOVE	R9	R1
      0x88280101,  //  002F  GETMBR	R10	R0	K1
      0x5C2C0800,  //  0030  MOVE	R11	R4
      0x7C1C0800,  //  0031  CALL	R7	4
      0x50200200,  //  0032  LDBOOL	R8	1	0
      0x7C140600,  //  0033  CALL	R5	3
      0x7001FFE3,  //  0034  JMP		#0019
      0x5808000F,  //  0035  LDCONST	R2	K15
      0xAC080200,  //  0036  CATCH	R2	1	0
      0xB0080000,  //  0037  RAISE	2	R0	R0
      0x80000400,  //  0038  RET	0
      0x8C040110,  //  0039  GETMET	R1	R0	K16
      0x7C040200,  //  003A  CALL	R1	1
      0x4C080000,  //  003B  LDNIL	R2
      0x20080202,  //  003C  NE	R2	R1	R2
      0x780A000B,  //  003D  JMPF	R2	#004A
      0xB80A2200,  //  003E  GETNGBL	R2	K17
      0x600C0018,  //  003F  GETGBL	R3	G24
      0x58100012,  //  0040  LDCONST	R4	K18
      0x88140105,  //  0041  GETMBR	R5	R0	K5
      0x7C0C0400,  //  0042  CALL	R3	2
      0x58100013,  //  0043  LDCONST	R4	K19
      0x7C080400,  //  0044  CALL	R2	2
      0x88080106,  //  0045  GETMBR	R2	R0	K6
      0x8C080514,  //  0046  GETMET	R2	R2	K20
      0x4C100000,  //  0047  LDNIL	R4
      0x5C140200,  //  0048  MOVE	R5	R1
      0x7C080600,  //  0049  CALL	R2	3
      0x80000000,  //  004A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Zigbee_Mapper_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Zigbee_Mapper,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020C01,  //  0000  SETMBR	R0	K6	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: zb_single_command
********************************************************************/
be_local_closure(class_Matter_Zigbee_Mapper_zb_single_command,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Zigbee_Mapper,     /* shared constants */
    be_str_weak(zb_single_command),
    &be_const_str_solidified,
    ( &(const binstruction[72]) {  /* code */
      0x4C0C0000,  //  0000  LDNIL	R3
      0x60100004,  //  0001  GETGBL	R4	G4
      0x88140102,  //  0002  GETMBR	R5	R0	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x1C100903,  //  0004  EQ	R4	R4	K3
      0x78120001,  //  0005  JMPF	R4	#0008
      0x88100102,  //  0006  GETMBR	R4	R0	K2
      0x70020003,  //  0007  JMP		#000C
      0x60100018,  //  0008  GETGBL	R4	G24
      0x58140004,  //  0009  LDCONST	R5	K4
      0x88180105,  //  000A  GETMBR	R6	R0	K5
      0x7C100400,  //  000B  CALL	R4	2
      0x88140101,  //  000C  GETMBR	R5	R0	K1
      0x4C180000,  //  000D  LDNIL	R6
      0x20140A06,  //  000E  NE	R5	R5	R6
      0x78160004,  //  000F  JMPF	R5	#0015
      0x60140018,  //  0010  GETGBL	R5	G24
      0x58180015,  //  0011  LDCONST	R6	K21
      0x881C0101,  //  0012  GETMBR	R7	R0	K1
      0x7C140400,  //  0013  CALL	R5	2
      0x70020000,  //  0014  JMP		#0016
      0x58140016,  //  0015  LDCONST	R5	K22
      0x1C180308,  //  0016  EQ	R6	R1	K8
      0x781A0007,  //  0017  JMPF	R6	#0020
      0x60180018,  //  0018  GETGBL	R6	G24
      0x581C0017,  //  0019  LDCONST	R7	K23
      0x5C200800,  //  001A  MOVE	R8	R4
      0x5C240A00,  //  001B  MOVE	R9	R5
      0x5C280400,  //  001C  MOVE	R10	R2
      0x7C180800,  //  001D  CALL	R6	4
      0x5C0C0C00,  //  001E  MOVE	R3	R6
      0x70020012,  //  001F  JMP		#0033
      0x1C180309,  //  0020  EQ	R6	R1	K9
      0x781A0007,  //  0021  JMPF	R6	#002A
      0x60180018,  //  0022  GETGBL	R6	G24
      0x581C0018,  //  0023  LDCONST	R7	K24
      0x5C200800,  //  0024  MOVE	R8	R4
      0x5C240A00,  //  0025  MOVE	R9	R5
      0x5C280400,  //  0026  MOVE	R10	R2
      0x7C180800,  //  0027  CALL	R6	4
      0x5C0C0C00,  //  0028  MOVE	R3	R6
      0x70020008,  //  0029  JMP		#0033
      0x1C18030B,  //  002A  EQ	R6	R1	K11
      0x781A0006,  //  002B  JMPF	R6	#0033
      0x60180018,  //  002C  GETGBL	R6	G24
      0x581C0019,  //  002D  LDCONST	R7	K25
      0x5C200800,  //  002E  MOVE	R8	R4
      0x5C240A00,  //  002F  MOVE	R9	R5
      0x5C280400,  //  0030  MOVE	R10	R2
      0x7C180800,  //  0031  CALL	R6	4
      0x5C0C0C00,  //  0032  MOVE	R3	R6
      0x4C180000,  //  0033  LDNIL	R6
      0x20180606,  //  0034  NE	R6	R3	R6
      0x781A0010,  //  0035  JMPF	R6	#0047
      0xB81A1800,  //  0036  GETNGBL	R6	K12
      0x8C180D1A,  //  0037  GETMET	R6	R6	K26
      0x58200013,  //  0038  LDCONST	R8	K19
      0x7C180400,  //  0039  CALL	R6	2
      0x781A0006,  //  003A  JMPF	R6	#0042
      0xB81A2200,  //  003B  GETNGBL	R6	K17
      0x601C0018,  //  003C  GETGBL	R7	G24
      0x5820001B,  //  003D  LDCONST	R8	K27
      0x5C240600,  //  003E  MOVE	R9	R3
      0x7C1C0400,  //  003F  CALL	R7	2
      0x58200013,  //  0040  LDCONST	R8	K19
      0x7C180400,  //  0041  CALL	R6	2
      0xB81A1800,  //  0042  GETNGBL	R6	K12
      0x8C180D0D,  //  0043  GETMET	R6	R6	K13
      0x5C200600,  //  0044  MOVE	R8	R3
      0x50240200,  //  0045  LDBOOL	R9	1	0
      0x7C180600,  //  0046  CALL	R6	3
      0x80000000,  //  0047  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: resolve_zb_device
********************************************************************/
be_local_closure(class_Matter_Zigbee_Mapper_resolve_zb_device,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Zigbee_Mapper,     /* shared constants */
    be_str_weak(resolve_zb_device),
    &be_const_str_solidified,
    ( &(const binstruction[59]) {  /* code */
      0xA4063800,  //  0000  IMPORT	R1	K28
      0xA40A0600,  //  0001  IMPORT	R2	K3
      0x880C011D,  //  0002  GETMBR	R3	R0	K29
      0x4C100000,  //  0003  LDNIL	R4
      0x1C0C0604,  //  0004  EQ	R3	R3	R4
      0x780E0001,  //  0005  JMPF	R3	#0008
      0x500C0000,  //  0006  LDBOOL	R3	0	0
      0x80040600,  //  0007  RET	1	R3
      0x880C0105,  //  0008  GETMBR	R3	R0	K5
      0x4C100000,  //  0009  LDNIL	R4
      0x200C0604,  //  000A  NE	R3	R3	R4
      0x780E0001,  //  000B  JMPF	R3	#000E
      0x500C0200,  //  000C  LDBOOL	R3	1	0
      0x80040600,  //  000D  RET	1	R3
      0x8C0C031F,  //  000E  GETMET	R3	R1	K31
      0x8814011D,  //  000F  GETMBR	R5	R0	K29
      0x7C0C0400,  //  0010  CALL	R3	2
      0x90023C03,  //  0011  SETMBR	R0	K30	R3
      0x880C011E,  //  0012  GETMBR	R3	R0	K30
      0x780E001C,  //  0013  JMPF	R3	#0031
      0x880C011E,  //  0014  GETMBR	R3	R0	K30
      0x880C0705,  //  0015  GETMBR	R3	R3	K5
      0x90020A03,  //  0016  SETMBR	R0	K5	R3
      0x880C0101,  //  0017  GETMBR	R3	R0	K1
      0x4C100000,  //  0018  LDNIL	R4
      0x1C0C0604,  //  0019  EQ	R3	R3	R4
      0x780E0012,  //  001A  JMPF	R3	#002E
      0x600C0004,  //  001B  GETGBL	R3	G4
      0x88100102,  //  001C  GETMBR	R4	R0	K2
      0x7C0C0200,  //  001D  CALL	R3	1
      0x1C0C0703,  //  001E  EQ	R3	R3	K3
      0x780E000D,  //  001F  JMPF	R3	#002E
      0x8C0C0520,  //  0020  GETMET	R3	R2	K32
      0x88140102,  //  0021  GETMBR	R5	R0	K2
      0x58180021,  //  0022  LDCONST	R6	K33
      0x7C0C0600,  //  0023  CALL	R3	3
      0x740E0008,  //  0024  JMPT	R3	#002E
      0x8C0C0520,  //  0025  GETMET	R3	R2	K32
      0x88140102,  //  0026  GETMBR	R5	R0	K2
      0x58180022,  //  0027  LDCONST	R6	K34
      0x7C0C0600,  //  0028  CALL	R3	3
      0x740E0003,  //  0029  JMPT	R3	#002E
      0x8C0C0123,  //  002A  GETMET	R3	R0	K35
      0x88140102,  //  002B  GETMBR	R5	R0	K2
      0x7C0C0400,  //  002C  CALL	R3	2
      0x90020203,  //  002D  SETMBR	R0	K1	R3
      0x500C0200,  //  002E  LDBOOL	R3	1	0
      0x80040600,  //  002F  RET	1	R3
      0x70020008,  //  0030  JMP		#003A
      0xB80E2200,  //  0031  GETNGBL	R3	K17
      0x60100018,  //  0032  GETGBL	R4	G24
      0x58140024,  //  0033  LDCONST	R5	K36
      0x8818011D,  //  0034  GETMBR	R6	R0	K29
      0x7C100400,  //  0035  CALL	R4	2
      0x58140013,  //  0036  LDCONST	R5	K19
      0x7C0C0400,  //  0037  CALL	R3	2
      0x500C0000,  //  0038  LDBOOL	R3	0	0
      0x80040600,  //  0039  RET	1	R3
      0x80000000,  //  003A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _search_for_ep
********************************************************************/
be_local_closure(class_Matter_Zigbee_Mapper__search_for_ep,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Zigbee_Mapper,     /* shared constants */
    be_str_weak(_search_for_ep),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0x600C000F,  //  0000  GETGBL	R3	G15
      0x5C100200,  //  0001  MOVE	R4	R1
      0x60140013,  //  0002  GETGBL	R5	G19
      0x7C0C0400,  //  0003  CALL	R3	2
      0x740E0001,  //  0004  JMPT	R3	#0007
      0x4C0C0000,  //  0005  LDNIL	R3
      0x80040600,  //  0006  RET	1	R3
      0x600C0010,  //  0007  GETGBL	R3	G16
      0x8C100325,  //  0008  GETMET	R4	R1	K37
      0x7C100200,  //  0009  CALL	R4	1
      0x7C0C0200,  //  000A  CALL	R3	1
      0xA8020015,  //  000B  EXBLK	0	#0022
      0x5C100600,  //  000C  MOVE	R4	R3
      0x7C100000,  //  000D  CALL	R4	0
      0x94140204,  //  000E  GETIDX	R5	R1	R4
      0x1C180A02,  //  000F  EQ	R6	R5	R2
      0x781A0006,  //  0010  JMPF	R6	#0018
      0x60180009,  //  0011  GETGBL	R6	G9
      0x5C1C0800,  //  0012  MOVE	R7	R4
      0x7C180200,  //  0013  CALL	R6	1
      0x241C0D26,  //  0014  GT	R7	R6	K38
      0x781E0001,  //  0015  JMPF	R7	#0018
      0xA8040001,  //  0016  EXBLK	1	1
      0x80040C00,  //  0017  RET	1	R6
      0x8C180127,  //  0018  GETMET	R6	R0	K39
      0x5C200A00,  //  0019  MOVE	R8	R5
      0x5C240400,  //  001A  MOVE	R9	R2
      0x7C180600,  //  001B  CALL	R6	3
      0x4C1C0000,  //  001C  LDNIL	R7
      0x201C0C07,  //  001D  NE	R7	R6	R7
      0x781E0001,  //  001E  JMPF	R7	#0021
      0xA8040001,  //  001F  EXBLK	1	1
      0x80040C00,  //  0020  RET	1	R6
      0x7001FFE9,  //  0021  JMP		#000C
      0x580C000F,  //  0022  LDCONST	R3	K15
      0xAC0C0200,  //  0023  CATCH	R3	1	0
      0xB0080000,  //  0024  RAISE	2	R0	R0
      0x4C0C0000,  //  0025  LDNIL	R3
      0x80040600,  //  0026  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_configuration
********************************************************************/
be_local_closure(class_Matter_Zigbee_Mapper_parse_configuration,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        2,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(probe_zb_values),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80040000,  //  0003  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_Matter_Zigbee_Mapper,     /* shared constants */
    be_str_weak(parse_configuration),
    &be_const_str_solidified,
    ( &(const binstruction[62]) {  /* code */
      0xA40A3800,  //  0000  IMPORT	R2	K28
      0xA40E0600,  //  0001  IMPORT	R3	K3
      0x8C10031F,  //  0002  GETMET	R4	R1	K31
      0x5818001E,  //  0003  LDCONST	R6	K30
      0x4C1C0000,  //  0004  LDNIL	R7
      0x7C100600,  //  0005  CALL	R4	3
      0x90023A04,  //  0006  SETMBR	R0	K29	R4
      0x60100004,  //  0007  GETGBL	R4	G4
      0x8814011D,  //  0008  GETMBR	R5	R0	K29
      0x7C100200,  //  0009  CALL	R4	1
      0x1C100903,  //  000A  EQ	R4	R4	K3
      0x78120013,  //  000B  JMPF	R4	#0020
      0x8C10071F,  //  000C  GETMET	R4	R3	K31
      0x8818011D,  //  000D  GETMBR	R6	R0	K29
      0x581C0028,  //  000E  LDCONST	R7	K40
      0x7C100600,  //  000F  CALL	R4	3
      0x28140926,  //  0010  GE	R5	R4	K38
      0x7816000B,  //  0011  JMPF	R5	#001E
      0x60140009,  //  0012  GETGBL	R5	G9
      0x00180929,  //  0013  ADD	R6	R4	K41
      0x40180D2A,  //  0014  CONNECT	R6	R6	K42
      0x881C011D,  //  0015  GETMBR	R7	R0	K29
      0x94180E06,  //  0016  GETIDX	R6	R7	R6
      0x7C140200,  //  0017  CALL	R5	1
      0x90020205,  //  0018  SETMBR	R0	K1	R5
      0x04140929,  //  0019  SUB	R5	R4	K41
      0x40164C05,  //  001A  CONNECT	R5	K38	R5
      0x8818011D,  //  001B  GETMBR	R6	R0	K29
      0x94140C05,  //  001C  GETIDX	R5	R6	R5
      0x90023A05,  //  001D  SETMBR	R0	K29	R5
      0x8814011D,  //  001E  GETMBR	R5	R0	K29
      0x90020405,  //  001F  SETMBR	R0	K2	R5
      0x60100004,  //  0020  GETGBL	R4	G4
      0x8814011D,  //  0021  GETMBR	R5	R0	K29
      0x7C100200,  //  0022  CALL	R4	1
      0x1C100903,  //  0023  EQ	R4	R4	K3
      0x7812000D,  //  0024  JMPF	R4	#0033
      0x8C100720,  //  0025  GETMET	R4	R3	K32
      0x8818011D,  //  0026  GETMBR	R6	R0	K29
      0x581C0021,  //  0027  LDCONST	R7	K33
      0x7C100600,  //  0028  CALL	R4	3
      0x74120004,  //  0029  JMPT	R4	#002F
      0x8C100720,  //  002A  GETMET	R4	R3	K32
      0x8818011D,  //  002B  GETMBR	R6	R0	K29
      0x581C0022,  //  002C  LDCONST	R7	K34
      0x7C100600,  //  002D  CALL	R4	3
      0x78120003,  //  002E  JMPF	R4	#0033
      0x60100009,  //  002F  GETGBL	R4	G9
      0x8814011D,  //  0030  GETMBR	R5	R0	K29
      0x7C100200,  //  0031  CALL	R4	1
      0x90023A04,  //  0032  SETMBR	R0	K29	R4
      0x8810011D,  //  0033  GETMBR	R4	R0	K29
      0x4C140000,  //  0034  LDNIL	R5
      0x20100805,  //  0035  NE	R4	R4	R5
      0x78120004,  //  0036  JMPF	R4	#003C
      0xB8121800,  //  0037  GETNGBL	R4	K12
      0x8C10092B,  //  0038  GETMET	R4	R4	K43
      0x541A0063,  //  0039  LDINT	R6	100
      0x841C0000,  //  003A  CLOSURE	R7	P0
      0x7C100600,  //  003B  CALL	R4	3
      0xA0000000,  //  003C  CLOSE	R0
      0x80000000,  //  003D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _lookup_endpoint_by_name
********************************************************************/
be_local_closure(class_Matter_Zigbee_Mapper__lookup_endpoint_by_name,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Zigbee_Mapper,     /* shared constants */
    be_str_weak(_lookup_endpoint_by_name),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xB80A1800,  //  0000  GETNGBL	R2	K12
      0x8C08050D,  //  0001  GETMET	R2	R2	K13
      0x60100018,  //  0002  GETGBL	R4	G24
      0x5814002C,  //  0003  LDCONST	R5	K44
      0x5C180200,  //  0004  MOVE	R6	R1
      0x7C100400,  //  0005  CALL	R4	2
      0x50140200,  //  0006  LDBOOL	R5	1	0
      0x7C080600,  //  0007  CALL	R2	3
      0x8C0C0127,  //  0008  GETMET	R3	R0	K39
      0x5C140400,  //  0009  MOVE	R5	R2
      0x5C180200,  //  000A  MOVE	R6	R1
      0x7C0C0600,  //  000B  CALL	R3	3
      0x80040600,  //  000C  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_zb_info
********************************************************************/
be_local_closure(class_Matter_Zigbee_Mapper_read_zb_info,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Zigbee_Mapper,     /* shared constants */
    be_str_weak(read_zb_info),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x78060009,  //  0002  JMPF	R1	#000D
      0xA4063800,  //  0003  IMPORT	R1	K28
      0x8C08031F,  //  0004  GETMET	R2	R1	K31
      0x88100105,  //  0005  GETMBR	R4	R0	K5
      0x7C080400,  //  0006  CALL	R2	2
      0x4C0C0000,  //  0007  LDNIL	R3
      0x200C0403,  //  0008  NE	R3	R2	R3
      0x780E0002,  //  0009  JMPF	R3	#000D
      0x8C0C052D,  //  000A  GETMET	R3	R2	K45
      0x7C0C0200,  //  000B  CALL	R3	1
      0x80040600,  //  000C  RET	1	R3
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Zigbee_Mapper
********************************************************************/
be_local_class(Matter_Zigbee_Mapper,
    6,
    NULL,
    be_nested_map(14,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(pi, 3), be_const_var(0) },
        { be_const_key_weak(read_zb_info, -1), be_const_closure(class_Matter_Zigbee_Mapper_read_zb_info_closure) },
        { be_const_key_weak(shortaddr, 10), be_const_var(4) },
        { be_const_key_weak(endpoint, -1), be_const_var(5) },
        { be_const_key_weak(zigbee_device, -1), be_const_var(3) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Zigbee_Mapper_init_closure) },
        { be_const_key_weak(device_arg_str, -1), be_const_var(2) },
        { be_const_key_weak(zb_single_command, -1), be_const_closure(class_Matter_Zigbee_Mapper_zb_single_command_closure) },
        { be_const_key_weak(device_arg, 7), be_const_var(1) },
        { be_const_key_weak(resolve_zb_device, -1), be_const_closure(class_Matter_Zigbee_Mapper_resolve_zb_device_closure) },
        { be_const_key_weak(_search_for_ep, -1), be_const_closure(class_Matter_Zigbee_Mapper__search_for_ep_closure) },
        { be_const_key_weak(parse_configuration, -1), be_const_closure(class_Matter_Zigbee_Mapper_parse_configuration_closure) },
        { be_const_key_weak(_lookup_endpoint_by_name, -1), be_const_closure(class_Matter_Zigbee_Mapper__lookup_endpoint_by_name_closure) },
        { be_const_key_weak(probe_zb_values, 1), be_const_closure(class_Matter_Zigbee_Mapper_probe_zb_values_closure) },
    })),
    be_str_weak(Matter_Zigbee_Mapper)
);

/********************************************************************
** Solidified function: matter_zigbee_init
********************************************************************/
be_local_closure(matter_zigbee_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    0,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(Matter_Zigbee),
    }),
    be_str_weak(matter_zigbee_init),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/

// compact class 'Matter_Zigbee' ktab size: 13, total: 14 (saved 8 bytes)
static const bvalue be_ktab_class_Matter_Zigbee[13] = {
  /* K0   */  be_nested_str_weak(zigbee),
  /* K1   */  be_nested_str_weak(device),
  /* K2   */  be_nested_str_weak(add_handler),
  /* K3   */  be_nested_str_weak(plugins),
  /* K4   */  be_const_int(0),
  /* K5   */  be_nested_str_weak(ZIGBEE),
  /* K6   */  be_nested_str_weak(zigbee_mapper),
  /* K7   */  be_nested_str_weak(resolve_zb_device),
  /* K8   */  be_nested_str_weak(shortaddr),
  /* K9   */  be_nested_str_weak(endpoint),
  /* K10  */  be_nested_str_weak(_src_ep),
  /* K11  */  be_nested_str_weak(zigbee_received),
  /* K12  */  be_const_int(1),
};


extern const bclass be_class_Matter_Zigbee;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Zigbee_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Zigbee,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x90020201,  //  0001  SETMBR	R0	K1	R1
      0x8C0C0502,  //  0002  GETMET	R3	R2	K2
      0x5C140000,  //  0003  MOVE	R5	R0
      0x7C0C0400,  //  0004  CALL	R3	2
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: attributes_final
********************************************************************/
be_local_closure(class_Matter_Zigbee_attributes_final,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Zigbee,     /* shared constants */
    be_str_weak(attributes_final),
    &be_const_str_solidified,
    ( &(const binstruction[46]) {  /* code */
      0x88140101,  //  0000  GETMBR	R5	R0	K1
      0x88140B03,  //  0001  GETMBR	R5	R5	K3
      0x58180004,  //  0002  LDCONST	R6	K4
      0x601C000C,  //  0003  GETGBL	R7	G12
      0x5C200A00,  //  0004  MOVE	R8	R5
      0x7C1C0200,  //  0005  CALL	R7	1
      0x141C0C07,  //  0006  LT	R7	R6	R7
      0x781E0024,  //  0007  JMPF	R7	#002D
      0x941C0A06,  //  0008  GETIDX	R7	R5	R6
      0x88200F05,  //  0009  GETMBR	R8	R7	K5
      0x7822001F,  //  000A  JMPF	R8	#002B
      0x88200F06,  //  000B  GETMBR	R8	R7	K6
      0x7822001D,  //  000C  JMPF	R8	#002B
      0x88200F06,  //  000D  GETMBR	R8	R7	K6
      0x8C201107,  //  000E  GETMET	R8	R8	K7
      0x7C200200,  //  000F  CALL	R8	1
      0x78220019,  //  0010  JMPF	R8	#002B
      0x88200F06,  //  0011  GETMBR	R8	R7	K6
      0x88201108,  //  0012  GETMBR	R8	R8	K8
      0x1C201004,  //  0013  EQ	R8	R8	R4
      0x78220015,  //  0014  JMPF	R8	#002B
      0x88200F06,  //  0015  GETMBR	R8	R7	K6
      0x88201109,  //  0016  GETMBR	R8	R8	K9
      0x4C240000,  //  0017  LDNIL	R9
      0x20240609,  //  0018  NE	R9	R3	R9
      0x78260001,  //  0019  JMPF	R9	#001C
      0x8824070A,  //  001A  GETMBR	R9	R3	K10
      0x70020000,  //  001B  JMP		#001D
      0x4C240000,  //  001C  LDNIL	R9
      0x4C280000,  //  001D  LDNIL	R10
      0x1C28100A,  //  001E  EQ	R10	R8	R10
      0x742A0006,  //  001F  JMPT	R10	#0027
      0x4C280000,  //  0020  LDNIL	R10
      0x1C28120A,  //  0021  EQ	R10	R9	R10
      0x742A0003,  //  0022  JMPT	R10	#0027
      0x1C281304,  //  0023  EQ	R10	R9	K4
      0x742A0001,  //  0024  JMPT	R10	#0027
      0x1C281009,  //  0025  EQ	R10	R8	R9
      0x782A0003,  //  0026  JMPF	R10	#002B
      0x8C280F0B,  //  0027  GETMET	R10	R7	K11
      0x5C300400,  //  0028  MOVE	R12	R2
      0x5C340600,  //  0029  MOVE	R13	R3
      0x7C280600,  //  002A  CALL	R10	3
      0x00180D0C,  //  002B  ADD	R6	R6	K12
      0x7001FFD5,  //  002C  JMP		#0003
      0x80000000,  //  002D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Zigbee
********************************************************************/
be_local_class(Matter_Zigbee,
    1,
    NULL,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(init, 3), be_const_closure(class_Matter_Zigbee_init_closure) },
        { be_const_key_weak(Matter_Zigbee_Mapper, 4), be_const_class(be_class_Matter_Zigbee_Mapper) },
        { be_const_key_weak(attributes_final, -1), be_const_closure(class_Matter_Zigbee_attributes_final_closure) },
        { be_const_key_weak(_CLASSES_TYPES, -1), be_nested_str_weak(_X2Dzigbee_X7Cz_light0_X7Cz_light1_X7Cz_light2_X7Cz_temp_X7Cz_pressure_X7Cz_humidity_X7Cz_occupancy) },
        { be_const_key_weak(device, -1), be_const_var(0) },
    })),
    be_str_weak(Matter_Zigbee)
);

/********************************************************************
** Solidified module: matter_zigbee
********************************************************************/
be_local_module(matter_zigbee,
    "matter_zigbee",
    be_nested_map(2,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(Matter_Zigbee, -1), be_const_class(be_class_Matter_Zigbee) },
        { be_const_key_weak(init, 0), be_const_closure(matter_zigbee_init_closure) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(matter_zigbee);
/********************************************************************/
/********************************************************************/
/* End of solidification */
