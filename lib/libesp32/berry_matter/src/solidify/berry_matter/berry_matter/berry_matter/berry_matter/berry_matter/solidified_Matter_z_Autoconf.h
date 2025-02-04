/* Solidification of Matter_z_Autoconf.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Autoconf' ktab size: 73, total: 90 (saved 136 bytes)
static const bvalue be_ktab_class_Matter_Autoconf[73] = {
  /* K0   */  be_nested_str_weak(device),
  /* K1   */  be_nested_str_weak(k2l_num),
  /* K2   */  be_nested_str_weak(plugins),
  /* K3   */  be_nested_str_weak(push),
  /* K4   */  be_nested_str_weak(matter),
  /* K5   */  be_nested_str_weak(Plugin_Root),
  /* K6   */  be_const_int(0),
  /* K7   */  be_nested_str_weak(log),
  /* K8   */  be_nested_str_weak(MTR_X3A_X20Configuring_X20endpoints),
  /* K9   */  be_const_int(2),
  /* K10  */  be_nested_str_weak(MTR_X3A_X20_X20_X20endpoint_X20_X3D_X20_X255i_X20type_X3A_X25s_X25s),
  /* K11  */  be_nested_str_weak(root),
  /* K12  */  be_nested_str_weak(),
  /* K13  */  be_nested_str_weak(Plugin_Aggregator),
  /* K14  */  be_nested_str_weak(AGGREGATOR_ENDPOINT),
  /* K15  */  be_nested_str_weak(aggregator),
  /* K16  */  be_nested_str_weak(find),
  /* K17  */  be_nested_str_weak(type),
  /* K18  */  be_nested_str_weak(MTR_X3A_X20no_X20class_X20name_X2C_X20skipping),
  /* K19  */  be_const_int(3),
  /* K20  */  be_nested_str_weak(MTR_X3A_X20only_X20one_X20root_X20node_X20allowed),
  /* K21  */  be_nested_str_weak(plugins_classes),
  /* K22  */  be_nested_str_weak(MTR_X3A_X20unknown_X20class_X20name_X20_X27),
  /* K23  */  be_nested_str_weak(_X27_X20skipping),
  /* K24  */  be_nested_str_weak(conf_to_log),
  /* K25  */  be_nested_str_weak(MTR_X3A_X20Exception),
  /* K26  */  be_nested_str_weak(_X7C),
  /* K27  */  be_nested_str_weak(stop_iteration),
  /* K28  */  be_nested_str_weak(tasmota),
  /* K29  */  be_nested_str_weak(publish_result),
  /* K30  */  be_nested_str_weak(_X7B_X22Matter_X22_X3A_X7B_X22Initialized_X22_X3A1_X7D_X7D),
  /* K31  */  be_nested_str_weak(Matter),
  /* K32  */  be_nested_str_weak(json),
  /* K33  */  be_nested_str_weak(START_ENDPOINT),
  /* K34  */  be_nested_str_weak(light),
  /* K35  */  be_nested_str_weak(get),
  /* K36  */  be_nested_str_weak(channels),
  /* K37  */  be_const_int(1),
  /* K38  */  be_nested_str_weak(light1),
  /* K39  */  be_nested_str_weak(light2),
  /* K40  */  be_nested_str_weak(light3),
  /* K41  */  be_nested_str_weak(cmd),
  /* K42  */  be_nested_str_weak(Status_X2013),
  /* K43  */  be_nested_str_weak(MTR_X3A_X20Status_X2013_X20_X3D_X20),
  /* K44  */  be_nested_str_weak(contains),
  /* K45  */  be_nested_str_weak(StatusSHT),
  /* K46  */  be_nested_str_weak(SHT),
  /* K47  */  be_nested_str_weak(MTR_X3A_X20_X27_X25s_X27_X20_X3D_X20_X25s),
  /* K48  */  be_nested_str_weak(Relay1),
  /* K49  */  be_nested_str_weak(Relay2),
  /* K50  */  be_nested_str_weak(MTR_X3A_X20relay1_X3D_X25s_X20relay2_X3D_X25s),
  /* K51  */  be_nested_str_weak(TiltConfig),
  /* K52  */  be_nested_str_weak(shutter_X2Btilt),
  /* K53  */  be_nested_str_weak(shutter),
  /* K54  */  be_nested_str_weak(get_power),
  /* K55  */  be_nested_str_weak(relay),
  /* K56  */  be_nested_str_weak(load),
  /* K57  */  be_nested_str_weak(read_sensors),
  /* K58  */  be_nested_str_weak(autoconf_sensors_list),
  /* K59  */  be_nested_str_weak(k2l),
  /* K60  */  be_nested_str_weak(Temperature),
  /* K61  */  be_nested_str_weak(_X23Temperature),
  /* K62  */  be_nested_str_weak(temperature),
  /* K63  */  be_nested_str_weak(filter),
  /* K64  */  be_nested_str_weak(Pressure),
  /* K65  */  be_nested_str_weak(_X23Pressure),
  /* K66  */  be_nested_str_weak(pressure),
  /* K67  */  be_nested_str_weak(Illuminance),
  /* K68  */  be_nested_str_weak(_X23Illuminance),
  /* K69  */  be_nested_str_weak(illuminance),
  /* K70  */  be_nested_str_weak(Humidity),
  /* K71  */  be_nested_str_weak(_X23Humidity),
  /* K72  */  be_nested_str_weak(humidity),
};


extern const bclass be_class_Matter_Autoconf;

/********************************************************************
** Solidified function: instantiate_plugins_from_config
********************************************************************/
be_local_closure(class_Matter_Autoconf_instantiate_plugins_from_config,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Autoconf,     /* shared constants */
    be_str_weak(instantiate_plugins_from_config),
    &be_const_str_solidified,
    ( &(const binstruction[148]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x880C0100,  //  0004  GETMBR	R3	R0	K0
      0x880C0702,  //  0005  GETMBR	R3	R3	K2
      0x8C100703,  //  0006  GETMET	R4	R3	K3
      0xB81A0800,  //  0007  GETNGBL	R6	K4
      0x8C180D05,  //  0008  GETMET	R6	R6	K5
      0x88200100,  //  0009  GETMBR	R8	R0	K0
      0x58240006,  //  000A  LDCONST	R9	K6
      0x60280013,  //  000B  GETGBL	R10	G19
      0x7C280000,  //  000C  CALL	R10	0
      0x7C180800,  //  000D  CALL	R6	4
      0x7C100400,  //  000E  CALL	R4	2
      0xB8120E00,  //  000F  GETNGBL	R4	K7
      0x58140008,  //  0010  LDCONST	R5	K8
      0x58180009,  //  0011  LDCONST	R6	K9
      0x7C100400,  //  0012  CALL	R4	2
      0xB8120E00,  //  0013  GETNGBL	R4	K7
      0x60140018,  //  0014  GETGBL	R5	G24
      0x5818000A,  //  0015  LDCONST	R6	K10
      0x581C0006,  //  0016  LDCONST	R7	K6
      0x5820000B,  //  0017  LDCONST	R8	K11
      0x5824000C,  //  0018  LDCONST	R9	K12
      0x7C140800,  //  0019  CALL	R5	4
      0x58180009,  //  001A  LDCONST	R6	K9
      0x7C100400,  //  001B  CALL	R4	2
      0x8C100703,  //  001C  GETMET	R4	R3	K3
      0xB81A0800,  //  001D  GETNGBL	R6	K4
      0x8C180D0D,  //  001E  GETMET	R6	R6	K13
      0x88200100,  //  001F  GETMBR	R8	R0	K0
      0xB8260800,  //  0020  GETNGBL	R9	K4
      0x8824130E,  //  0021  GETMBR	R9	R9	K14
      0x60280013,  //  0022  GETGBL	R10	G19
      0x7C280000,  //  0023  CALL	R10	0
      0x7C180800,  //  0024  CALL	R6	4
      0x7C100400,  //  0025  CALL	R4	2
      0xB8120E00,  //  0026  GETNGBL	R4	K7
      0x60140018,  //  0027  GETGBL	R5	G24
      0x5818000A,  //  0028  LDCONST	R6	K10
      0xB81E0800,  //  0029  GETNGBL	R7	K4
      0x881C0F0E,  //  002A  GETMBR	R7	R7	K14
      0x5820000F,  //  002B  LDCONST	R8	K15
      0x5824000C,  //  002C  LDCONST	R9	K12
      0x7C140800,  //  002D  CALL	R5	4
      0x58180009,  //  002E  LDCONST	R6	K9
      0x7C100400,  //  002F  CALL	R4	2
      0x60100010,  //  0030  GETGBL	R4	G16
      0x5C140400,  //  0031  MOVE	R5	R2
      0x7C100200,  //  0032  CALL	R4	1
      0xA8020056,  //  0033  EXBLK	0	#008B
      0x5C140800,  //  0034  MOVE	R5	R4
      0x7C140000,  //  0035  CALL	R5	0
      0x1C180B06,  //  0036  EQ	R6	R5	K6
      0x781A0000,  //  0037  JMPF	R6	#0039
      0x7001FFFA,  //  0038  JMP		#0034
      0xA802003F,  //  0039  EXBLK	0	#007A
      0x60180008,  //  003A  GETGBL	R6	G8
      0x5C1C0A00,  //  003B  MOVE	R7	R5
      0x7C180200,  //  003C  CALL	R6	1
      0x94180206,  //  003D  GETIDX	R6	R1	R6
      0x8C1C0D10,  //  003E  GETMET	R7	R6	K16
      0x58240011,  //  003F  LDCONST	R9	K17
      0x7C1C0400,  //  0040  CALL	R7	2
      0x4C200000,  //  0041  LDNIL	R8
      0x1C200E08,  //  0042  EQ	R8	R7	R8
      0x78220005,  //  0043  JMPF	R8	#004A
      0xB8220E00,  //  0044  GETNGBL	R8	K7
      0x58240012,  //  0045  LDCONST	R9	K18
      0x58280013,  //  0046  LDCONST	R10	K19
      0x7C200400,  //  0047  CALL	R8	2
      0xA8040001,  //  0048  EXBLK	1	1
      0x7001FFE9,  //  0049  JMP		#0034
      0x1C200F0B,  //  004A  EQ	R8	R7	K11
      0x78220005,  //  004B  JMPF	R8	#0052
      0xB8220E00,  //  004C  GETNGBL	R8	K7
      0x58240014,  //  004D  LDCONST	R9	K20
      0x58280013,  //  004E  LDCONST	R10	K19
      0x7C200400,  //  004F  CALL	R8	2
      0xA8040001,  //  0050  EXBLK	1	1
      0x7001FFE1,  //  0051  JMP		#0034
      0x88200100,  //  0052  GETMBR	R8	R0	K0
      0x88201115,  //  0053  GETMBR	R8	R8	K21
      0x8C201110,  //  0054  GETMET	R8	R8	K16
      0x5C280E00,  //  0055  MOVE	R10	R7
      0x7C200400,  //  0056  CALL	R8	2
      0x4C240000,  //  0057  LDNIL	R9
      0x1C241009,  //  0058  EQ	R9	R8	R9
      0x78260009,  //  0059  JMPF	R9	#0064
      0xB8260E00,  //  005A  GETNGBL	R9	K7
      0x60280008,  //  005B  GETGBL	R10	G8
      0x5C2C0E00,  //  005C  MOVE	R11	R7
      0x7C280200,  //  005D  CALL	R10	1
      0x002A2C0A,  //  005E  ADD	R10	K22	R10
      0x00281517,  //  005F  ADD	R10	R10	K23
      0x582C0009,  //  0060  LDCONST	R11	K9
      0x7C240400,  //  0061  CALL	R9	2
      0xA8040001,  //  0062  EXBLK	1	1
      0x7001FFCF,  //  0063  JMP		#0034
      0x5C241000,  //  0064  MOVE	R9	R8
      0x88280100,  //  0065  GETMBR	R10	R0	K0
      0x5C2C0A00,  //  0066  MOVE	R11	R5
      0x5C300C00,  //  0067  MOVE	R12	R6
      0x7C240600,  //  0068  CALL	R9	3
      0x8C280703,  //  0069  GETMET	R10	R3	K3
      0x5C301200,  //  006A  MOVE	R12	R9
      0x7C280400,  //  006B  CALL	R10	2
      0xB82A0E00,  //  006C  GETNGBL	R10	K7
      0x602C0018,  //  006D  GETGBL	R11	G24
      0x5830000A,  //  006E  LDCONST	R12	K10
      0x5C340A00,  //  006F  MOVE	R13	R5
      0x5C380E00,  //  0070  MOVE	R14	R7
      0x883C0100,  //  0071  GETMBR	R15	R0	K0
      0x8C3C1F18,  //  0072  GETMET	R15	R15	K24
      0x5C440C00,  //  0073  MOVE	R17	R6
      0x7C3C0400,  //  0074  CALL	R15	2
      0x7C2C0800,  //  0075  CALL	R11	4
      0x58300009,  //  0076  LDCONST	R12	K9
      0x7C280400,  //  0077  CALL	R10	2
      0xA8040001,  //  0078  EXBLK	1	1
      0x7002000F,  //  0079  JMP		#008A
      0xAC180002,  //  007A  CATCH	R6	0	2
      0x7002000C,  //  007B  JMP		#0089
      0xB8220E00,  //  007C  GETNGBL	R8	K7
      0x60240008,  //  007D  GETGBL	R9	G8
      0x5C280C00,  //  007E  MOVE	R10	R6
      0x7C240200,  //  007F  CALL	R9	1
      0x00263209,  //  0080  ADD	R9	K25	R9
      0x0024131A,  //  0081  ADD	R9	R9	K26
      0x60280008,  //  0082  GETGBL	R10	G8
      0x5C2C0E00,  //  0083  MOVE	R11	R7
      0x7C280200,  //  0084  CALL	R10	1
      0x0024120A,  //  0085  ADD	R9	R9	R10
      0x58280009,  //  0086  LDCONST	R10	K9
      0x7C200400,  //  0087  CALL	R8	2
      0x70020000,  //  0088  JMP		#008A
      0xB0080000,  //  0089  RAISE	2	R0	R0
      0x7001FFA8,  //  008A  JMP		#0034
      0x5810001B,  //  008B  LDCONST	R4	K27
      0xAC100200,  //  008C  CATCH	R4	1	0
      0xB0080000,  //  008D  RAISE	2	R0	R0
      0xB8123800,  //  008E  GETNGBL	R4	K28
      0x8C10091D,  //  008F  GETMET	R4	R4	K29
      0x5818001E,  //  0090  LDCONST	R6	K30
      0x581C001F,  //  0091  LDCONST	R7	K31
      0x7C100600,  //  0092  CALL	R4	3
      0x80000000,  //  0093  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: autoconf_device_map
********************************************************************/
be_local_closure(class_Matter_Autoconf_autoconf_device_map,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Autoconf,     /* shared constants */
    be_str_weak(autoconf_device_map),
    &be_const_str_solidified,
    ( &(const binstruction[262]) {  /* code */
      0xA4064000,  //  0000  IMPORT	R1	K32
      0x60080013,  //  0001  GETGBL	R2	G19
      0x7C080000,  //  0002  CALL	R2	0
      0xB80E0800,  //  0003  GETNGBL	R3	K4
      0x880C0721,  //  0004  GETMBR	R3	R3	K33
      0x58100006,  //  0005  LDCONST	R4	K6
      0xA4164400,  //  0006  IMPORT	R5	K34
      0x8C180B23,  //  0007  GETMET	R6	R5	K35
      0x58200006,  //  0008  LDCONST	R8	K6
      0x7C180400,  //  0009  CALL	R6	2
      0x4C1C0000,  //  000A  LDNIL	R7
      0x201C0C07,  //  000B  NE	R7	R6	R7
      0x781E0066,  //  000C  JMPF	R7	#0074
      0x601C000C,  //  000D  GETGBL	R7	G12
      0x8C200D10,  //  000E  GETMET	R8	R6	K16
      0x58280024,  //  000F  LDCONST	R10	K36
      0x582C000C,  //  0010  LDCONST	R11	K12
      0x7C200600,  //  0011  CALL	R8	3
      0x7C1C0200,  //  0012  CALL	R7	1
      0x58100025,  //  0013  LDCONST	R4	K37
      0x24200F06,  //  0014  GT	R8	R7	K6
      0x7822005D,  //  0015  JMPF	R8	#0074
      0x1C200F25,  //  0016  EQ	R8	R7	K37
      0x7822001E,  //  0017  JMPF	R8	#0037
      0x60200008,  //  0018  GETGBL	R8	G8
      0x5C240600,  //  0019  MOVE	R9	R3
      0x7C200200,  //  001A  CALL	R8	1
      0x60240013,  //  001B  GETGBL	R9	G19
      0x7C240000,  //  001C  CALL	R9	0
      0x98262326,  //  001D  SETIDX	R9	K17	K38
      0x98081009,  //  001E  SETIDX	R2	R8	R9
      0x000C0725,  //  001F  ADD	R3	R3	K37
      0x58200025,  //  0020  LDCONST	R8	K37
      0x4C240000,  //  0021  LDNIL	R9
      0x8C280B23,  //  0022  GETMET	R10	R5	K35
      0x5C301000,  //  0023  MOVE	R12	R8
      0x7C280400,  //  0024  CALL	R10	2
      0x5C241400,  //  0025  MOVE	R9	R10
      0x4C2C0000,  //  0026  LDNIL	R11
      0x2028140B,  //  0027  NE	R10	R10	R11
      0x782A000C,  //  0028  JMPF	R10	#0036
      0x60280008,  //  0029  GETGBL	R10	G8
      0x5C2C0600,  //  002A  MOVE	R11	R3
      0x7C280200,  //  002B  CALL	R10	1
      0x602C0013,  //  002C  GETGBL	R11	G19
      0x7C2C0000,  //  002D  CALL	R11	0
      0x982E2326,  //  002E  SETIDX	R11	K17	K38
      0x00301125,  //  002F  ADD	R12	R8	K37
      0x982E440C,  //  0030  SETIDX	R11	K34	R12
      0x9808140B,  //  0031  SETIDX	R2	R10	R11
      0x000C0725,  //  0032  ADD	R3	R3	K37
      0x00100925,  //  0033  ADD	R4	R4	K37
      0x00201125,  //  0034  ADD	R8	R8	K37
      0x7001FFEB,  //  0035  JMP		#0022
      0x7002003C,  //  0036  JMP		#0074
      0x1C200F09,  //  0037  EQ	R8	R7	K9
      0x78220008,  //  0038  JMPF	R8	#0042
      0x60200008,  //  0039  GETGBL	R8	G8
      0x5C240600,  //  003A  MOVE	R9	R3
      0x7C200200,  //  003B  CALL	R8	1
      0x60240013,  //  003C  GETGBL	R9	G19
      0x7C240000,  //  003D  CALL	R9	0
      0x98262327,  //  003E  SETIDX	R9	K17	K39
      0x98081009,  //  003F  SETIDX	R2	R8	R9
      0x000C0725,  //  0040  ADD	R3	R3	K37
      0x70020031,  //  0041  JMP		#0074
      0x1C200F13,  //  0042  EQ	R8	R7	K19
      0x7822002B,  //  0043  JMPF	R8	#0070
      0x60200008,  //  0044  GETGBL	R8	G8
      0x5C240600,  //  0045  MOVE	R9	R3
      0x7C200200,  //  0046  CALL	R8	1
      0x60240013,  //  0047  GETGBL	R9	G19
      0x7C240000,  //  0048  CALL	R9	0
      0x98262328,  //  0049  SETIDX	R9	K17	K40
      0x98081009,  //  004A  SETIDX	R2	R8	R9
      0x000C0725,  //  004B  ADD	R3	R3	K37
      0x8C200B23,  //  004C  GETMET	R8	R5	K35
      0x58280025,  //  004D  LDCONST	R10	K37
      0x7C200400,  //  004E  CALL	R8	2
      0x4C240000,  //  004F  LDNIL	R9
      0x20241009,  //  0050  NE	R9	R8	R9
      0x7826001C,  //  0051  JMPF	R9	#006F
      0x6024000C,  //  0052  GETGBL	R9	G12
      0x8C281110,  //  0053  GETMET	R10	R8	K16
      0x58300024,  //  0054  LDCONST	R12	K36
      0x5834000C,  //  0055  LDCONST	R13	K12
      0x7C280600,  //  0056  CALL	R10	3
      0x7C240200,  //  0057  CALL	R9	1
      0x1C281325,  //  0058  EQ	R10	R9	K37
      0x782A0009,  //  0059  JMPF	R10	#0064
      0x60280008,  //  005A  GETGBL	R10	G8
      0x5C2C0600,  //  005B  MOVE	R11	R3
      0x7C280200,  //  005C  CALL	R10	1
      0x602C0013,  //  005D  GETGBL	R11	G19
      0x7C2C0000,  //  005E  CALL	R11	0
      0x982E2326,  //  005F  SETIDX	R11	K17	K38
      0x9808140B,  //  0060  SETIDX	R2	R10	R11
      0x000C0725,  //  0061  ADD	R3	R3	K37
      0x00100925,  //  0062  ADD	R4	R4	K37
      0x7002000A,  //  0063  JMP		#006F
      0x1C281309,  //  0064  EQ	R10	R9	K9
      0x782A0008,  //  0065  JMPF	R10	#006F
      0x60280008,  //  0066  GETGBL	R10	G8
      0x5C2C0600,  //  0067  MOVE	R11	R3
      0x7C280200,  //  0068  CALL	R10	1
      0x602C0013,  //  0069  GETGBL	R11	G19
      0x7C2C0000,  //  006A  CALL	R11	0
      0x982E2327,  //  006B  SETIDX	R11	K17	K39
      0x9808140B,  //  006C  SETIDX	R2	R10	R11
      0x000C0725,  //  006D  ADD	R3	R3	K37
      0x00100925,  //  006E  ADD	R4	R4	K37
      0x70020003,  //  006F  JMP		#0074
      0x54220003,  //  0070  LDINT	R8	4
      0x1C200E08,  //  0071  EQ	R8	R7	R8
      0x78220000,  //  0072  JMPF	R8	#0074
      0x7001FFFF,  //  0073  JMP		#0074
      0xB81E3800,  //  0074  GETNGBL	R7	K28
      0x8C1C0F29,  //  0075  GETMET	R7	R7	K41
      0x5824002A,  //  0076  LDCONST	R9	K42
      0x50280200,  //  0077  LDBOOL	R10	1	0
      0x7C1C0600,  //  0078  CALL	R7	3
      0x60200012,  //  0079  GETGBL	R8	G18
      0x7C200000,  //  007A  CALL	R8	0
      0xB8260E00,  //  007B  GETNGBL	R9	K7
      0x60280008,  //  007C  GETGBL	R10	G8
      0x5C2C0E00,  //  007D  MOVE	R11	R7
      0x7C280200,  //  007E  CALL	R10	1
      0x002A560A,  //  007F  ADD	R10	K43	R10
      0x582C0013,  //  0080  LDCONST	R11	K19
      0x7C240400,  //  0081  CALL	R9	2
      0x4C240000,  //  0082  LDNIL	R9
      0x20240E09,  //  0083  NE	R9	R7	R9
      0x7826004D,  //  0084  JMPF	R9	#00D3
      0x8C240F2C,  //  0085  GETMET	R9	R7	K44
      0x582C002D,  //  0086  LDCONST	R11	K45
      0x7C240400,  //  0087  CALL	R9	2
      0x78260049,  //  0088  JMPF	R9	#00D3
      0x941C0F2D,  //  0089  GETIDX	R7	R7	K45
      0x58240006,  //  008A  LDCONST	R9	K6
      0x50280200,  //  008B  LDBOOL	R10	1	0
      0x782A0045,  //  008C  JMPF	R10	#00D3
      0x60280008,  //  008D  GETGBL	R10	G8
      0x5C2C1200,  //  008E  MOVE	R11	R9
      0x7C280200,  //  008F  CALL	R10	1
      0x002A5C0A,  //  0090  ADD	R10	K46	R10
      0x8C2C0F2C,  //  0091  GETMET	R11	R7	K44
      0x5C341400,  //  0092  MOVE	R13	R10
      0x7C2C0400,  //  0093  CALL	R11	2
      0x742E0000,  //  0094  JMPT	R11	#0096
      0x7002003C,  //  0095  JMP		#00D3
      0x942C0E0A,  //  0096  GETIDX	R11	R7	R10
      0xB8320E00,  //  0097  GETNGBL	R12	K7
      0x60340018,  //  0098  GETGBL	R13	G24
      0x5838002F,  //  0099  LDCONST	R14	K47
      0x5C3C1400,  //  009A  MOVE	R15	R10
      0x60400008,  //  009B  GETGBL	R16	G8
      0x5C441600,  //  009C  MOVE	R17	R11
      0x7C400200,  //  009D  CALL	R16	1
      0x7C340600,  //  009E  CALL	R13	3
      0x58380013,  //  009F  LDCONST	R14	K19
      0x7C300400,  //  00A0  CALL	R12	2
      0x8C301710,  //  00A1  GETMET	R12	R11	K16
      0x58380030,  //  00A2  LDCONST	R14	K48
      0x543DFFFE,  //  00A3  LDINT	R15	-1
      0x7C300600,  //  00A4  CALL	R12	3
      0x8C341710,  //  00A5  GETMET	R13	R11	K16
      0x583C0031,  //  00A6  LDCONST	R15	K49
      0x5441FFFE,  //  00A7  LDINT	R16	-1
      0x7C340600,  //  00A8  CALL	R13	3
      0x24381906,  //  00A9  GT	R14	R12	K6
      0x783A0002,  //  00AA  JMPF	R14	#00AE
      0x8C381103,  //  00AB  GETMET	R14	R8	K3
      0x04401925,  //  00AC  SUB	R16	R12	K37
      0x7C380400,  //  00AD  CALL	R14	2
      0x24381B06,  //  00AE  GT	R14	R13	K6
      0x783A0002,  //  00AF  JMPF	R14	#00B3
      0x8C381103,  //  00B0  GETMET	R14	R8	K3
      0x04401B25,  //  00B1  SUB	R16	R13	K37
      0x7C380400,  //  00B2  CALL	R14	2
      0xB83A0E00,  //  00B3  GETNGBL	R14	K7
      0x603C0018,  //  00B4  GETGBL	R15	G24
      0x58400032,  //  00B5  LDCONST	R16	K50
      0x5C441800,  //  00B6  MOVE	R17	R12
      0x5C481A00,  //  00B7  MOVE	R18	R13
      0x7C3C0600,  //  00B8  CALL	R15	3
      0x58400013,  //  00B9  LDCONST	R16	K19
      0x7C380400,  //  00BA  CALL	R14	2
      0x8C381710,  //  00BB  GETMET	R14	R11	K16
      0x58400033,  //  00BC  LDCONST	R16	K51
      0x7C380400,  //  00BD  CALL	R14	2
      0x783A0002,  //  00BE  JMPF	R14	#00C2
      0x943C1D09,  //  00BF  GETIDX	R15	R14	K9
      0x243C1F06,  //  00C0  GT	R15	R15	K6
      0x743E0000,  //  00C1  JMPT	R15	#00C3
      0x503C0001,  //  00C2  LDBOOL	R15	0	1
      0x503C0200,  //  00C3  LDBOOL	R15	1	0
      0x60400008,  //  00C4  GETGBL	R16	G8
      0x5C440600,  //  00C5  MOVE	R17	R3
      0x7C400200,  //  00C6  CALL	R16	1
      0x60440013,  //  00C7  GETGBL	R17	G19
      0x7C440000,  //  00C8  CALL	R17	0
      0x783E0001,  //  00C9  JMPF	R15	#00CC
      0x58480034,  //  00CA  LDCONST	R18	K52
      0x70020000,  //  00CB  JMP		#00CD
      0x58480035,  //  00CC  LDCONST	R18	K53
      0x98462212,  //  00CD  SETIDX	R17	K17	R18
      0x98466A09,  //  00CE  SETIDX	R17	K53	R9
      0x98082011,  //  00CF  SETIDX	R2	R16	R17
      0x000C0725,  //  00D0  ADD	R3	R3	K37
      0x00241325,  //  00D1  ADD	R9	R9	K37
      0x7001FFB7,  //  00D2  JMP		#008B
      0x6024000C,  //  00D3  GETGBL	R9	G12
      0xB82A3800,  //  00D4  GETNGBL	R10	K28
      0x8C281536,  //  00D5  GETMET	R10	R10	K54
      0x7C280200,  //  00D6  CALL	R10	1
      0x7C240200,  //  00D7  CALL	R9	1
      0x58280006,  //  00D8  LDCONST	R10	K6
      0x04241204,  //  00D9  SUB	R9	R9	R4
      0x142C1409,  //  00DA  LT	R11	R10	R9
      0x782E0011,  //  00DB  JMPF	R11	#00EE
      0x8C2C1110,  //  00DC  GETMET	R11	R8	K16
      0x5C341400,  //  00DD  MOVE	R13	R10
      0x7C2C0400,  //  00DE  CALL	R11	2
      0x4C300000,  //  00DF  LDNIL	R12
      0x1C2C160C,  //  00E0  EQ	R11	R11	R12
      0x782E0009,  //  00E1  JMPF	R11	#00EC
      0x602C0008,  //  00E2  GETGBL	R11	G8
      0x5C300600,  //  00E3  MOVE	R12	R3
      0x7C2C0200,  //  00E4  CALL	R11	1
      0x60300013,  //  00E5  GETGBL	R12	G19
      0x7C300000,  //  00E6  CALL	R12	0
      0x98322337,  //  00E7  SETIDX	R12	K17	K55
      0x00341525,  //  00E8  ADD	R13	R10	K37
      0x98326E0D,  //  00E9  SETIDX	R12	K55	R13
      0x9808160C,  //  00EA  SETIDX	R2	R11	R12
      0x000C0725,  //  00EB  ADD	R3	R3	K37
      0x00281525,  //  00EC  ADD	R10	R10	K37
      0x7001FFEB,  //  00ED  JMP		#00DA
      0x8C2C0338,  //  00EE  GETMET	R11	R1	K56
      0xB8363800,  //  00EF  GETNGBL	R13	K28
      0x8C341B39,  //  00F0  GETMET	R13	R13	K57
      0x7C340200,  //  00F1  CALL	R13	1
      0x7C2C0400,  //  00F2  CALL	R11	2
      0x8C30013A,  //  00F3  GETMET	R12	R0	K58
      0x5C381600,  //  00F4  MOVE	R14	R11
      0x7C300400,  //  00F5  CALL	R12	2
      0x60340010,  //  00F6  GETGBL	R13	G16
      0x5C381800,  //  00F7  MOVE	R14	R12
      0x7C340200,  //  00F8  CALL	R13	1
      0xA8020007,  //  00F9  EXBLK	0	#0102
      0x5C381A00,  //  00FA  MOVE	R14	R13
      0x7C380000,  //  00FB  CALL	R14	0
      0x603C0008,  //  00FC  GETGBL	R15	G8
      0x5C400600,  //  00FD  MOVE	R16	R3
      0x7C3C0200,  //  00FE  CALL	R15	1
      0x98081E0E,  //  00FF  SETIDX	R2	R15	R14
      0x000C0725,  //  0100  ADD	R3	R3	K37
      0x7001FFF7,  //  0101  JMP		#00FA
      0x5834001B,  //  0102  LDCONST	R13	K27
      0xAC340200,  //  0103  CATCH	R13	1	0
      0xB0080000,  //  0104  RAISE	2	R0	R0
      0x80040400,  //  0105  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Autoconf_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Autoconf,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: autoconf_sensors_list
********************************************************************/
be_local_closure(class_Matter_Autoconf_autoconf_sensors_list,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Autoconf,     /* shared constants */
    be_str_weak(autoconf_sensors_list),
    &be_const_str_solidified,
    ( &(const binstruction[121]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8808053B,  //  0001  GETMBR	R2	R2	K59
      0x600C0012,  //  0002  GETGBL	R3	G18
      0x7C0C0000,  //  0003  CALL	R3	0
      0x60100010,  //  0004  GETGBL	R4	G16
      0x5C140400,  //  0005  MOVE	R5	R2
      0x5C180200,  //  0006  MOVE	R6	R1
      0x7C140200,  //  0007  CALL	R5	1
      0x7C100200,  //  0008  CALL	R4	1
      0xA8020013,  //  0009  EXBLK	0	#001E
      0x5C140800,  //  000A  MOVE	R5	R4
      0x7C140000,  //  000B  CALL	R5	0
      0x94180205,  //  000C  GETIDX	R6	R1	R5
      0x601C000F,  //  000D  GETGBL	R7	G15
      0x5C200C00,  //  000E  MOVE	R8	R6
      0x60240013,  //  000F  GETGBL	R9	G19
      0x7C1C0400,  //  0010  CALL	R7	2
      0x781E000A,  //  0011  JMPF	R7	#001D
      0x8C1C0D2C,  //  0012  GETMET	R7	R6	K44
      0x5824003C,  //  0013  LDCONST	R9	K60
      0x7C1C0400,  //  0014  CALL	R7	2
      0x781E0006,  //  0015  JMPF	R7	#001D
      0x001C0B3D,  //  0016  ADD	R7	R5	K61
      0x8C200703,  //  0017  GETMET	R8	R3	K3
      0x60280013,  //  0018  GETGBL	R10	G19
      0x7C280000,  //  0019  CALL	R10	0
      0x982A233E,  //  001A  SETIDX	R10	K17	K62
      0x982A7E07,  //  001B  SETIDX	R10	K63	R7
      0x7C200400,  //  001C  CALL	R8	2
      0x7001FFEB,  //  001D  JMP		#000A
      0x5810001B,  //  001E  LDCONST	R4	K27
      0xAC100200,  //  001F  CATCH	R4	1	0
      0xB0080000,  //  0020  RAISE	2	R0	R0
      0x60100010,  //  0021  GETGBL	R4	G16
      0x5C140400,  //  0022  MOVE	R5	R2
      0x5C180200,  //  0023  MOVE	R6	R1
      0x7C140200,  //  0024  CALL	R5	1
      0x7C100200,  //  0025  CALL	R4	1
      0xA8020013,  //  0026  EXBLK	0	#003B
      0x5C140800,  //  0027  MOVE	R5	R4
      0x7C140000,  //  0028  CALL	R5	0
      0x94180205,  //  0029  GETIDX	R6	R1	R5
      0x601C000F,  //  002A  GETGBL	R7	G15
      0x5C200C00,  //  002B  MOVE	R8	R6
      0x60240013,  //  002C  GETGBL	R9	G19
      0x7C1C0400,  //  002D  CALL	R7	2
      0x781E000A,  //  002E  JMPF	R7	#003A
      0x8C1C0D2C,  //  002F  GETMET	R7	R6	K44
      0x58240040,  //  0030  LDCONST	R9	K64
      0x7C1C0400,  //  0031  CALL	R7	2
      0x781E0006,  //  0032  JMPF	R7	#003A
      0x001C0B41,  //  0033  ADD	R7	R5	K65
      0x8C200703,  //  0034  GETMET	R8	R3	K3
      0x60280013,  //  0035  GETGBL	R10	G19
      0x7C280000,  //  0036  CALL	R10	0
      0x982A2342,  //  0037  SETIDX	R10	K17	K66
      0x982A7E07,  //  0038  SETIDX	R10	K63	R7
      0x7C200400,  //  0039  CALL	R8	2
      0x7001FFEB,  //  003A  JMP		#0027
      0x5810001B,  //  003B  LDCONST	R4	K27
      0xAC100200,  //  003C  CATCH	R4	1	0
      0xB0080000,  //  003D  RAISE	2	R0	R0
      0x60100010,  //  003E  GETGBL	R4	G16
      0x5C140400,  //  003F  MOVE	R5	R2
      0x5C180200,  //  0040  MOVE	R6	R1
      0x7C140200,  //  0041  CALL	R5	1
      0x7C100200,  //  0042  CALL	R4	1
      0xA8020013,  //  0043  EXBLK	0	#0058
      0x5C140800,  //  0044  MOVE	R5	R4
      0x7C140000,  //  0045  CALL	R5	0
      0x94180205,  //  0046  GETIDX	R6	R1	R5
      0x601C000F,  //  0047  GETGBL	R7	G15
      0x5C200C00,  //  0048  MOVE	R8	R6
      0x60240013,  //  0049  GETGBL	R9	G19
      0x7C1C0400,  //  004A  CALL	R7	2
      0x781E000A,  //  004B  JMPF	R7	#0057
      0x8C1C0D2C,  //  004C  GETMET	R7	R6	K44
      0x58240043,  //  004D  LDCONST	R9	K67
      0x7C1C0400,  //  004E  CALL	R7	2
      0x781E0006,  //  004F  JMPF	R7	#0057
      0x001C0B44,  //  0050  ADD	R7	R5	K68
      0x8C200703,  //  0051  GETMET	R8	R3	K3
      0x60280013,  //  0052  GETGBL	R10	G19
      0x7C280000,  //  0053  CALL	R10	0
      0x982A2345,  //  0054  SETIDX	R10	K17	K69
      0x982A7E07,  //  0055  SETIDX	R10	K63	R7
      0x7C200400,  //  0056  CALL	R8	2
      0x7001FFEB,  //  0057  JMP		#0044
      0x5810001B,  //  0058  LDCONST	R4	K27
      0xAC100200,  //  0059  CATCH	R4	1	0
      0xB0080000,  //  005A  RAISE	2	R0	R0
      0x60100010,  //  005B  GETGBL	R4	G16
      0x5C140400,  //  005C  MOVE	R5	R2
      0x5C180200,  //  005D  MOVE	R6	R1
      0x7C140200,  //  005E  CALL	R5	1
      0x7C100200,  //  005F  CALL	R4	1
      0xA8020013,  //  0060  EXBLK	0	#0075
      0x5C140800,  //  0061  MOVE	R5	R4
      0x7C140000,  //  0062  CALL	R5	0
      0x94180205,  //  0063  GETIDX	R6	R1	R5
      0x601C000F,  //  0064  GETGBL	R7	G15
      0x5C200C00,  //  0065  MOVE	R8	R6
      0x60240013,  //  0066  GETGBL	R9	G19
      0x7C1C0400,  //  0067  CALL	R7	2
      0x781E000A,  //  0068  JMPF	R7	#0074
      0x8C1C0D2C,  //  0069  GETMET	R7	R6	K44
      0x58240046,  //  006A  LDCONST	R9	K70
      0x7C1C0400,  //  006B  CALL	R7	2
      0x781E0006,  //  006C  JMPF	R7	#0074
      0x001C0B47,  //  006D  ADD	R7	R5	K71
      0x8C200703,  //  006E  GETMET	R8	R3	K3
      0x60280013,  //  006F  GETGBL	R10	G19
      0x7C280000,  //  0070  CALL	R10	0
      0x982A2348,  //  0071  SETIDX	R10	K17	K72
      0x982A7E07,  //  0072  SETIDX	R10	K63	R7
      0x7C200400,  //  0073  CALL	R8	2
      0x7001FFEB,  //  0074  JMP		#0061
      0x5810001B,  //  0075  LDCONST	R4	K27
      0xAC100200,  //  0076  CATCH	R4	1	0
      0xB0080000,  //  0077  RAISE	2	R0	R0
      0x80040600,  //  0078  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Autoconf
********************************************************************/
be_local_class(Matter_Autoconf,
    1,
    NULL,
    be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(instantiate_plugins_from_config, 3), be_const_closure(class_Matter_Autoconf_instantiate_plugins_from_config_closure) },
        { be_const_key_weak(device, -1), be_const_var(0) },
        { be_const_key_weak(autoconf_device_map, -1), be_const_closure(class_Matter_Autoconf_autoconf_device_map_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Autoconf_init_closure) },
        { be_const_key_weak(autoconf_sensors_list, -1), be_const_closure(class_Matter_Autoconf_autoconf_sensors_list_closure) },
    })),
    be_str_weak(Matter_Autoconf)
);
/********************************************************************/
/* End of solidification */
