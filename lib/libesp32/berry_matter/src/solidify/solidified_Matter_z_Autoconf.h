/* Solidification of Matter_z_Autoconf.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Autoconf' ktab size: 74, total: 91 (saved 136 bytes)
static const bvalue be_ktab_class_Matter_Autoconf[74] = {
  /* K0   */  be_nested_str_weak(device),
  /* K1   */  be_nested_str_weak(k2l_num),
  /* K2   */  be_nested_str_weak(plugins),
  /* K3   */  be_nested_str_weak(push),
  /* K4   */  be_nested_str_weak(matter),
  /* K5   */  be_nested_str_weak(Plugin_Root),
  /* K6   */  be_const_int(0),
  /* K7   */  be_nested_str_weak(find),
  /* K8   */  be_nested_str_weak(0),
  /* K9   */  be_nested_str_weak(log),
  /* K10  */  be_nested_str_weak(MTR_X3A_X20Configuring_X20endpoints),
  /* K11  */  be_const_int(2),
  /* K12  */  be_nested_str_weak(MTR_X3A_X20_X20_X20endpoint_X20_X3D_X20_X255i_X20type_X3A_X25s_X25s),
  /* K13  */  be_nested_str_weak(root),
  /* K14  */  be_nested_str_weak(),
  /* K15  */  be_nested_str_weak(Plugin_Aggregator),
  /* K16  */  be_nested_str_weak(AGGREGATOR_ENDPOINT),
  /* K17  */  be_nested_str_weak(aggregator),
  /* K18  */  be_nested_str_weak(type),
  /* K19  */  be_nested_str_weak(MTR_X3A_X20no_X20class_X20name_X2C_X20skipping),
  /* K20  */  be_const_int(3),
  /* K21  */  be_nested_str_weak(MTR_X3A_X20only_X20one_X20root_X20node_X20allowed),
  /* K22  */  be_nested_str_weak(plugins_classes),
  /* K23  */  be_nested_str_weak(MTR_X3A_X20unknown_X20class_X20name_X20_X27),
  /* K24  */  be_nested_str_weak(_X27_X20skipping),
  /* K25  */  be_nested_str_weak(conf_to_log),
  /* K26  */  be_nested_str_weak(MTR_X3A_X20Exception),
  /* K27  */  be_nested_str_weak(_X7C),
  /* K28  */  be_nested_str_weak(stop_iteration),
  /* K29  */  be_nested_str_weak(tasmota),
  /* K30  */  be_nested_str_weak(publish_result),
  /* K31  */  be_nested_str_weak(_X7B_X22Matter_X22_X3A_X7B_X22Initialized_X22_X3A1_X7D_X7D),
  /* K32  */  be_nested_str_weak(Matter),
  /* K33  */  be_nested_str_weak(json),
  /* K34  */  be_nested_str_weak(START_ENDPOINT),
  /* K35  */  be_nested_str_weak(light),
  /* K36  */  be_nested_str_weak(get),
  /* K37  */  be_nested_str_weak(channels),
  /* K38  */  be_const_int(1),
  /* K39  */  be_nested_str_weak(light1),
  /* K40  */  be_nested_str_weak(light2),
  /* K41  */  be_nested_str_weak(light3),
  /* K42  */  be_nested_str_weak(cmd),
  /* K43  */  be_nested_str_weak(Status_X2013),
  /* K44  */  be_nested_str_weak(MTR_X3A_X20Status_X2013_X20_X3D_X20),
  /* K45  */  be_nested_str_weak(contains),
  /* K46  */  be_nested_str_weak(StatusSHT),
  /* K47  */  be_nested_str_weak(SHT),
  /* K48  */  be_nested_str_weak(MTR_X3A_X20_X27_X25s_X27_X20_X3D_X20_X25s),
  /* K49  */  be_nested_str_weak(Relay1),
  /* K50  */  be_nested_str_weak(Relay2),
  /* K51  */  be_nested_str_weak(MTR_X3A_X20relay1_X3D_X25s_X20relay2_X3D_X25s),
  /* K52  */  be_nested_str_weak(TiltConfig),
  /* K53  */  be_nested_str_weak(shutter_X2Btilt),
  /* K54  */  be_nested_str_weak(shutter),
  /* K55  */  be_nested_str_weak(get_power),
  /* K56  */  be_nested_str_weak(relay),
  /* K57  */  be_nested_str_weak(load),
  /* K58  */  be_nested_str_weak(read_sensors),
  /* K59  */  be_nested_str_weak(autoconf_sensors_list),
  /* K60  */  be_nested_str_weak(k2l),
  /* K61  */  be_nested_str_weak(Temperature),
  /* K62  */  be_nested_str_weak(_X23Temperature),
  /* K63  */  be_nested_str_weak(temperature),
  /* K64  */  be_nested_str_weak(filter),
  /* K65  */  be_nested_str_weak(Pressure),
  /* K66  */  be_nested_str_weak(_X23Pressure),
  /* K67  */  be_nested_str_weak(pressure),
  /* K68  */  be_nested_str_weak(Illuminance),
  /* K69  */  be_nested_str_weak(_X23Illuminance),
  /* K70  */  be_nested_str_weak(illuminance),
  /* K71  */  be_nested_str_weak(Humidity),
  /* K72  */  be_nested_str_weak(_X23Humidity),
  /* K73  */  be_nested_str_weak(humidity),
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
    ( &(const binstruction[151]) {  /* code */
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
      0x8C280307,  //  000B  GETMET	R10	R1	K7
      0x58300008,  //  000C  LDCONST	R12	K8
      0x60340013,  //  000D  GETGBL	R13	G19
      0x7C340000,  //  000E  CALL	R13	0
      0x7C280600,  //  000F  CALL	R10	3
      0x7C180800,  //  0010  CALL	R6	4
      0x7C100400,  //  0011  CALL	R4	2
      0xB8121200,  //  0012  GETNGBL	R4	K9
      0x5814000A,  //  0013  LDCONST	R5	K10
      0x5818000B,  //  0014  LDCONST	R6	K11
      0x7C100400,  //  0015  CALL	R4	2
      0xB8121200,  //  0016  GETNGBL	R4	K9
      0x60140018,  //  0017  GETGBL	R5	G24
      0x5818000C,  //  0018  LDCONST	R6	K12
      0x581C0006,  //  0019  LDCONST	R7	K6
      0x5820000D,  //  001A  LDCONST	R8	K13
      0x5824000E,  //  001B  LDCONST	R9	K14
      0x7C140800,  //  001C  CALL	R5	4
      0x5818000B,  //  001D  LDCONST	R6	K11
      0x7C100400,  //  001E  CALL	R4	2
      0x8C100703,  //  001F  GETMET	R4	R3	K3
      0xB81A0800,  //  0020  GETNGBL	R6	K4
      0x8C180D0F,  //  0021  GETMET	R6	R6	K15
      0x88200100,  //  0022  GETMBR	R8	R0	K0
      0xB8260800,  //  0023  GETNGBL	R9	K4
      0x88241310,  //  0024  GETMBR	R9	R9	K16
      0x60280013,  //  0025  GETGBL	R10	G19
      0x7C280000,  //  0026  CALL	R10	0
      0x7C180800,  //  0027  CALL	R6	4
      0x7C100400,  //  0028  CALL	R4	2
      0xB8121200,  //  0029  GETNGBL	R4	K9
      0x60140018,  //  002A  GETGBL	R5	G24
      0x5818000C,  //  002B  LDCONST	R6	K12
      0xB81E0800,  //  002C  GETNGBL	R7	K4
      0x881C0F10,  //  002D  GETMBR	R7	R7	K16
      0x58200011,  //  002E  LDCONST	R8	K17
      0x5824000E,  //  002F  LDCONST	R9	K14
      0x7C140800,  //  0030  CALL	R5	4
      0x5818000B,  //  0031  LDCONST	R6	K11
      0x7C100400,  //  0032  CALL	R4	2
      0x60100010,  //  0033  GETGBL	R4	G16
      0x5C140400,  //  0034  MOVE	R5	R2
      0x7C100200,  //  0035  CALL	R4	1
      0xA8020056,  //  0036  EXBLK	0	#008E
      0x5C140800,  //  0037  MOVE	R5	R4
      0x7C140000,  //  0038  CALL	R5	0
      0x1C180B06,  //  0039  EQ	R6	R5	K6
      0x781A0000,  //  003A  JMPF	R6	#003C
      0x7001FFFA,  //  003B  JMP		#0037
      0xA802003F,  //  003C  EXBLK	0	#007D
      0x60180008,  //  003D  GETGBL	R6	G8
      0x5C1C0A00,  //  003E  MOVE	R7	R5
      0x7C180200,  //  003F  CALL	R6	1
      0x94180206,  //  0040  GETIDX	R6	R1	R6
      0x8C1C0D07,  //  0041  GETMET	R7	R6	K7
      0x58240012,  //  0042  LDCONST	R9	K18
      0x7C1C0400,  //  0043  CALL	R7	2
      0x4C200000,  //  0044  LDNIL	R8
      0x1C200E08,  //  0045  EQ	R8	R7	R8
      0x78220005,  //  0046  JMPF	R8	#004D
      0xB8221200,  //  0047  GETNGBL	R8	K9
      0x58240013,  //  0048  LDCONST	R9	K19
      0x58280014,  //  0049  LDCONST	R10	K20
      0x7C200400,  //  004A  CALL	R8	2
      0xA8040001,  //  004B  EXBLK	1	1
      0x7001FFE9,  //  004C  JMP		#0037
      0x1C200F0D,  //  004D  EQ	R8	R7	K13
      0x78220005,  //  004E  JMPF	R8	#0055
      0xB8221200,  //  004F  GETNGBL	R8	K9
      0x58240015,  //  0050  LDCONST	R9	K21
      0x58280014,  //  0051  LDCONST	R10	K20
      0x7C200400,  //  0052  CALL	R8	2
      0xA8040001,  //  0053  EXBLK	1	1
      0x7001FFE1,  //  0054  JMP		#0037
      0x88200100,  //  0055  GETMBR	R8	R0	K0
      0x88201116,  //  0056  GETMBR	R8	R8	K22
      0x8C201107,  //  0057  GETMET	R8	R8	K7
      0x5C280E00,  //  0058  MOVE	R10	R7
      0x7C200400,  //  0059  CALL	R8	2
      0x4C240000,  //  005A  LDNIL	R9
      0x1C241009,  //  005B  EQ	R9	R8	R9
      0x78260009,  //  005C  JMPF	R9	#0067
      0xB8261200,  //  005D  GETNGBL	R9	K9
      0x60280008,  //  005E  GETGBL	R10	G8
      0x5C2C0E00,  //  005F  MOVE	R11	R7
      0x7C280200,  //  0060  CALL	R10	1
      0x002A2E0A,  //  0061  ADD	R10	K23	R10
      0x00281518,  //  0062  ADD	R10	R10	K24
      0x582C000B,  //  0063  LDCONST	R11	K11
      0x7C240400,  //  0064  CALL	R9	2
      0xA8040001,  //  0065  EXBLK	1	1
      0x7001FFCF,  //  0066  JMP		#0037
      0x5C241000,  //  0067  MOVE	R9	R8
      0x88280100,  //  0068  GETMBR	R10	R0	K0
      0x5C2C0A00,  //  0069  MOVE	R11	R5
      0x5C300C00,  //  006A  MOVE	R12	R6
      0x7C240600,  //  006B  CALL	R9	3
      0x8C280703,  //  006C  GETMET	R10	R3	K3
      0x5C301200,  //  006D  MOVE	R12	R9
      0x7C280400,  //  006E  CALL	R10	2
      0xB82A1200,  //  006F  GETNGBL	R10	K9
      0x602C0018,  //  0070  GETGBL	R11	G24
      0x5830000C,  //  0071  LDCONST	R12	K12
      0x5C340A00,  //  0072  MOVE	R13	R5
      0x5C380E00,  //  0073  MOVE	R14	R7
      0x883C0100,  //  0074  GETMBR	R15	R0	K0
      0x8C3C1F19,  //  0075  GETMET	R15	R15	K25
      0x5C440C00,  //  0076  MOVE	R17	R6
      0x7C3C0400,  //  0077  CALL	R15	2
      0x7C2C0800,  //  0078  CALL	R11	4
      0x5830000B,  //  0079  LDCONST	R12	K11
      0x7C280400,  //  007A  CALL	R10	2
      0xA8040001,  //  007B  EXBLK	1	1
      0x7002000F,  //  007C  JMP		#008D
      0xAC180002,  //  007D  CATCH	R6	0	2
      0x7002000C,  //  007E  JMP		#008C
      0xB8221200,  //  007F  GETNGBL	R8	K9
      0x60240008,  //  0080  GETGBL	R9	G8
      0x5C280C00,  //  0081  MOVE	R10	R6
      0x7C240200,  //  0082  CALL	R9	1
      0x00263409,  //  0083  ADD	R9	K26	R9
      0x0024131B,  //  0084  ADD	R9	R9	K27
      0x60280008,  //  0085  GETGBL	R10	G8
      0x5C2C0E00,  //  0086  MOVE	R11	R7
      0x7C280200,  //  0087  CALL	R10	1
      0x0024120A,  //  0088  ADD	R9	R9	R10
      0x5828000B,  //  0089  LDCONST	R10	K11
      0x7C200400,  //  008A  CALL	R8	2
      0x70020000,  //  008B  JMP		#008D
      0xB0080000,  //  008C  RAISE	2	R0	R0
      0x7001FFA8,  //  008D  JMP		#0037
      0x5810001C,  //  008E  LDCONST	R4	K28
      0xAC100200,  //  008F  CATCH	R4	1	0
      0xB0080000,  //  0090  RAISE	2	R0	R0
      0xB8123A00,  //  0091  GETNGBL	R4	K29
      0x8C10091E,  //  0092  GETMET	R4	R4	K30
      0x5818001F,  //  0093  LDCONST	R6	K31
      0x581C0020,  //  0094  LDCONST	R7	K32
      0x7C100600,  //  0095  CALL	R4	3
      0x80000000,  //  0096  RET	0
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
      0xA4064200,  //  0000  IMPORT	R1	K33
      0x60080013,  //  0001  GETGBL	R2	G19
      0x7C080000,  //  0002  CALL	R2	0
      0xB80E0800,  //  0003  GETNGBL	R3	K4
      0x880C0722,  //  0004  GETMBR	R3	R3	K34
      0x58100006,  //  0005  LDCONST	R4	K6
      0xA4164600,  //  0006  IMPORT	R5	K35
      0x8C180B24,  //  0007  GETMET	R6	R5	K36
      0x58200006,  //  0008  LDCONST	R8	K6
      0x7C180400,  //  0009  CALL	R6	2
      0x4C1C0000,  //  000A  LDNIL	R7
      0x201C0C07,  //  000B  NE	R7	R6	R7
      0x781E0066,  //  000C  JMPF	R7	#0074
      0x601C000C,  //  000D  GETGBL	R7	G12
      0x8C200D07,  //  000E  GETMET	R8	R6	K7
      0x58280025,  //  000F  LDCONST	R10	K37
      0x582C000E,  //  0010  LDCONST	R11	K14
      0x7C200600,  //  0011  CALL	R8	3
      0x7C1C0200,  //  0012  CALL	R7	1
      0x58100026,  //  0013  LDCONST	R4	K38
      0x24200F06,  //  0014  GT	R8	R7	K6
      0x7822005D,  //  0015  JMPF	R8	#0074
      0x1C200F26,  //  0016  EQ	R8	R7	K38
      0x7822001E,  //  0017  JMPF	R8	#0037
      0x60200008,  //  0018  GETGBL	R8	G8
      0x5C240600,  //  0019  MOVE	R9	R3
      0x7C200200,  //  001A  CALL	R8	1
      0x60240013,  //  001B  GETGBL	R9	G19
      0x7C240000,  //  001C  CALL	R9	0
      0x98262527,  //  001D  SETIDX	R9	K18	K39
      0x98081009,  //  001E  SETIDX	R2	R8	R9
      0x000C0726,  //  001F  ADD	R3	R3	K38
      0x58200026,  //  0020  LDCONST	R8	K38
      0x4C240000,  //  0021  LDNIL	R9
      0x8C280B24,  //  0022  GETMET	R10	R5	K36
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
      0x982E2527,  //  002E  SETIDX	R11	K18	K39
      0x00301126,  //  002F  ADD	R12	R8	K38
      0x982E460C,  //  0030  SETIDX	R11	K35	R12
      0x9808140B,  //  0031  SETIDX	R2	R10	R11
      0x000C0726,  //  0032  ADD	R3	R3	K38
      0x00100926,  //  0033  ADD	R4	R4	K38
      0x00201126,  //  0034  ADD	R8	R8	K38
      0x7001FFEB,  //  0035  JMP		#0022
      0x7002003C,  //  0036  JMP		#0074
      0x1C200F0B,  //  0037  EQ	R8	R7	K11
      0x78220008,  //  0038  JMPF	R8	#0042
      0x60200008,  //  0039  GETGBL	R8	G8
      0x5C240600,  //  003A  MOVE	R9	R3
      0x7C200200,  //  003B  CALL	R8	1
      0x60240013,  //  003C  GETGBL	R9	G19
      0x7C240000,  //  003D  CALL	R9	0
      0x98262528,  //  003E  SETIDX	R9	K18	K40
      0x98081009,  //  003F  SETIDX	R2	R8	R9
      0x000C0726,  //  0040  ADD	R3	R3	K38
      0x70020031,  //  0041  JMP		#0074
      0x1C200F14,  //  0042  EQ	R8	R7	K20
      0x7822002B,  //  0043  JMPF	R8	#0070
      0x60200008,  //  0044  GETGBL	R8	G8
      0x5C240600,  //  0045  MOVE	R9	R3
      0x7C200200,  //  0046  CALL	R8	1
      0x60240013,  //  0047  GETGBL	R9	G19
      0x7C240000,  //  0048  CALL	R9	0
      0x98262529,  //  0049  SETIDX	R9	K18	K41
      0x98081009,  //  004A  SETIDX	R2	R8	R9
      0x000C0726,  //  004B  ADD	R3	R3	K38
      0x8C200B24,  //  004C  GETMET	R8	R5	K36
      0x58280026,  //  004D  LDCONST	R10	K38
      0x7C200400,  //  004E  CALL	R8	2
      0x4C240000,  //  004F  LDNIL	R9
      0x20241009,  //  0050  NE	R9	R8	R9
      0x7826001C,  //  0051  JMPF	R9	#006F
      0x6024000C,  //  0052  GETGBL	R9	G12
      0x8C281107,  //  0053  GETMET	R10	R8	K7
      0x58300025,  //  0054  LDCONST	R12	K37
      0x5834000E,  //  0055  LDCONST	R13	K14
      0x7C280600,  //  0056  CALL	R10	3
      0x7C240200,  //  0057  CALL	R9	1
      0x1C281326,  //  0058  EQ	R10	R9	K38
      0x782A0009,  //  0059  JMPF	R10	#0064
      0x60280008,  //  005A  GETGBL	R10	G8
      0x5C2C0600,  //  005B  MOVE	R11	R3
      0x7C280200,  //  005C  CALL	R10	1
      0x602C0013,  //  005D  GETGBL	R11	G19
      0x7C2C0000,  //  005E  CALL	R11	0
      0x982E2527,  //  005F  SETIDX	R11	K18	K39
      0x9808140B,  //  0060  SETIDX	R2	R10	R11
      0x000C0726,  //  0061  ADD	R3	R3	K38
      0x00100926,  //  0062  ADD	R4	R4	K38
      0x7002000A,  //  0063  JMP		#006F
      0x1C28130B,  //  0064  EQ	R10	R9	K11
      0x782A0008,  //  0065  JMPF	R10	#006F
      0x60280008,  //  0066  GETGBL	R10	G8
      0x5C2C0600,  //  0067  MOVE	R11	R3
      0x7C280200,  //  0068  CALL	R10	1
      0x602C0013,  //  0069  GETGBL	R11	G19
      0x7C2C0000,  //  006A  CALL	R11	0
      0x982E2528,  //  006B  SETIDX	R11	K18	K40
      0x9808140B,  //  006C  SETIDX	R2	R10	R11
      0x000C0726,  //  006D  ADD	R3	R3	K38
      0x00100926,  //  006E  ADD	R4	R4	K38
      0x70020003,  //  006F  JMP		#0074
      0x54220003,  //  0070  LDINT	R8	4
      0x1C200E08,  //  0071  EQ	R8	R7	R8
      0x78220000,  //  0072  JMPF	R8	#0074
      0x7001FFFF,  //  0073  JMP		#0074
      0xB81E3A00,  //  0074  GETNGBL	R7	K29
      0x8C1C0F2A,  //  0075  GETMET	R7	R7	K42
      0x5824002B,  //  0076  LDCONST	R9	K43
      0x50280200,  //  0077  LDBOOL	R10	1	0
      0x7C1C0600,  //  0078  CALL	R7	3
      0x60200012,  //  0079  GETGBL	R8	G18
      0x7C200000,  //  007A  CALL	R8	0
      0xB8261200,  //  007B  GETNGBL	R9	K9
      0x60280008,  //  007C  GETGBL	R10	G8
      0x5C2C0E00,  //  007D  MOVE	R11	R7
      0x7C280200,  //  007E  CALL	R10	1
      0x002A580A,  //  007F  ADD	R10	K44	R10
      0x582C0014,  //  0080  LDCONST	R11	K20
      0x7C240400,  //  0081  CALL	R9	2
      0x4C240000,  //  0082  LDNIL	R9
      0x20240E09,  //  0083  NE	R9	R7	R9
      0x7826004D,  //  0084  JMPF	R9	#00D3
      0x8C240F2D,  //  0085  GETMET	R9	R7	K45
      0x582C002E,  //  0086  LDCONST	R11	K46
      0x7C240400,  //  0087  CALL	R9	2
      0x78260049,  //  0088  JMPF	R9	#00D3
      0x941C0F2E,  //  0089  GETIDX	R7	R7	K46
      0x58240006,  //  008A  LDCONST	R9	K6
      0x50280200,  //  008B  LDBOOL	R10	1	0
      0x782A0045,  //  008C  JMPF	R10	#00D3
      0x60280008,  //  008D  GETGBL	R10	G8
      0x5C2C1200,  //  008E  MOVE	R11	R9
      0x7C280200,  //  008F  CALL	R10	1
      0x002A5E0A,  //  0090  ADD	R10	K47	R10
      0x8C2C0F2D,  //  0091  GETMET	R11	R7	K45
      0x5C341400,  //  0092  MOVE	R13	R10
      0x7C2C0400,  //  0093  CALL	R11	2
      0x742E0000,  //  0094  JMPT	R11	#0096
      0x7002003C,  //  0095  JMP		#00D3
      0x942C0E0A,  //  0096  GETIDX	R11	R7	R10
      0xB8321200,  //  0097  GETNGBL	R12	K9
      0x60340018,  //  0098  GETGBL	R13	G24
      0x58380030,  //  0099  LDCONST	R14	K48
      0x5C3C1400,  //  009A  MOVE	R15	R10
      0x60400008,  //  009B  GETGBL	R16	G8
      0x5C441600,  //  009C  MOVE	R17	R11
      0x7C400200,  //  009D  CALL	R16	1
      0x7C340600,  //  009E  CALL	R13	3
      0x58380014,  //  009F  LDCONST	R14	K20
      0x7C300400,  //  00A0  CALL	R12	2
      0x8C301707,  //  00A1  GETMET	R12	R11	K7
      0x58380031,  //  00A2  LDCONST	R14	K49
      0x543DFFFE,  //  00A3  LDINT	R15	-1
      0x7C300600,  //  00A4  CALL	R12	3
      0x8C341707,  //  00A5  GETMET	R13	R11	K7
      0x583C0032,  //  00A6  LDCONST	R15	K50
      0x5441FFFE,  //  00A7  LDINT	R16	-1
      0x7C340600,  //  00A8  CALL	R13	3
      0x24381906,  //  00A9  GT	R14	R12	K6
      0x783A0002,  //  00AA  JMPF	R14	#00AE
      0x8C381103,  //  00AB  GETMET	R14	R8	K3
      0x04401926,  //  00AC  SUB	R16	R12	K38
      0x7C380400,  //  00AD  CALL	R14	2
      0x24381B06,  //  00AE  GT	R14	R13	K6
      0x783A0002,  //  00AF  JMPF	R14	#00B3
      0x8C381103,  //  00B0  GETMET	R14	R8	K3
      0x04401B26,  //  00B1  SUB	R16	R13	K38
      0x7C380400,  //  00B2  CALL	R14	2
      0xB83A1200,  //  00B3  GETNGBL	R14	K9
      0x603C0018,  //  00B4  GETGBL	R15	G24
      0x58400033,  //  00B5  LDCONST	R16	K51
      0x5C441800,  //  00B6  MOVE	R17	R12
      0x5C481A00,  //  00B7  MOVE	R18	R13
      0x7C3C0600,  //  00B8  CALL	R15	3
      0x58400014,  //  00B9  LDCONST	R16	K20
      0x7C380400,  //  00BA  CALL	R14	2
      0x8C381707,  //  00BB  GETMET	R14	R11	K7
      0x58400034,  //  00BC  LDCONST	R16	K52
      0x7C380400,  //  00BD  CALL	R14	2
      0x783A0002,  //  00BE  JMPF	R14	#00C2
      0x943C1D0B,  //  00BF  GETIDX	R15	R14	K11
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
      0x58480035,  //  00CA  LDCONST	R18	K53
      0x70020000,  //  00CB  JMP		#00CD
      0x58480036,  //  00CC  LDCONST	R18	K54
      0x98462412,  //  00CD  SETIDX	R17	K18	R18
      0x98466C09,  //  00CE  SETIDX	R17	K54	R9
      0x98082011,  //  00CF  SETIDX	R2	R16	R17
      0x000C0726,  //  00D0  ADD	R3	R3	K38
      0x00241326,  //  00D1  ADD	R9	R9	K38
      0x7001FFB7,  //  00D2  JMP		#008B
      0x6024000C,  //  00D3  GETGBL	R9	G12
      0xB82A3A00,  //  00D4  GETNGBL	R10	K29
      0x8C281537,  //  00D5  GETMET	R10	R10	K55
      0x7C280200,  //  00D6  CALL	R10	1
      0x7C240200,  //  00D7  CALL	R9	1
      0x58280006,  //  00D8  LDCONST	R10	K6
      0x04241204,  //  00D9  SUB	R9	R9	R4
      0x142C1409,  //  00DA  LT	R11	R10	R9
      0x782E0011,  //  00DB  JMPF	R11	#00EE
      0x8C2C1107,  //  00DC  GETMET	R11	R8	K7
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
      0x98322538,  //  00E7  SETIDX	R12	K18	K56
      0x00341526,  //  00E8  ADD	R13	R10	K38
      0x9832700D,  //  00E9  SETIDX	R12	K56	R13
      0x9808160C,  //  00EA  SETIDX	R2	R11	R12
      0x000C0726,  //  00EB  ADD	R3	R3	K38
      0x00281526,  //  00EC  ADD	R10	R10	K38
      0x7001FFEB,  //  00ED  JMP		#00DA
      0x8C2C0339,  //  00EE  GETMET	R11	R1	K57
      0xB8363A00,  //  00EF  GETNGBL	R13	K29
      0x8C341B3A,  //  00F0  GETMET	R13	R13	K58
      0x7C340200,  //  00F1  CALL	R13	1
      0x7C2C0400,  //  00F2  CALL	R11	2
      0x8C30013B,  //  00F3  GETMET	R12	R0	K59
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
      0x000C0726,  //  0100  ADD	R3	R3	K38
      0x7001FFF7,  //  0101  JMP		#00FA
      0x5834001C,  //  0102  LDCONST	R13	K28
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
      0x8808053C,  //  0001  GETMBR	R2	R2	K60
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
      0x8C1C0D2D,  //  0012  GETMET	R7	R6	K45
      0x5824003D,  //  0013  LDCONST	R9	K61
      0x7C1C0400,  //  0014  CALL	R7	2
      0x781E0006,  //  0015  JMPF	R7	#001D
      0x001C0B3E,  //  0016  ADD	R7	R5	K62
      0x8C200703,  //  0017  GETMET	R8	R3	K3
      0x60280013,  //  0018  GETGBL	R10	G19
      0x7C280000,  //  0019  CALL	R10	0
      0x982A253F,  //  001A  SETIDX	R10	K18	K63
      0x982A8007,  //  001B  SETIDX	R10	K64	R7
      0x7C200400,  //  001C  CALL	R8	2
      0x7001FFEB,  //  001D  JMP		#000A
      0x5810001C,  //  001E  LDCONST	R4	K28
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
      0x8C1C0D2D,  //  002F  GETMET	R7	R6	K45
      0x58240041,  //  0030  LDCONST	R9	K65
      0x7C1C0400,  //  0031  CALL	R7	2
      0x781E0006,  //  0032  JMPF	R7	#003A
      0x001C0B42,  //  0033  ADD	R7	R5	K66
      0x8C200703,  //  0034  GETMET	R8	R3	K3
      0x60280013,  //  0035  GETGBL	R10	G19
      0x7C280000,  //  0036  CALL	R10	0
      0x982A2543,  //  0037  SETIDX	R10	K18	K67
      0x982A8007,  //  0038  SETIDX	R10	K64	R7
      0x7C200400,  //  0039  CALL	R8	2
      0x7001FFEB,  //  003A  JMP		#0027
      0x5810001C,  //  003B  LDCONST	R4	K28
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
      0x8C1C0D2D,  //  004C  GETMET	R7	R6	K45
      0x58240044,  //  004D  LDCONST	R9	K68
      0x7C1C0400,  //  004E  CALL	R7	2
      0x781E0006,  //  004F  JMPF	R7	#0057
      0x001C0B45,  //  0050  ADD	R7	R5	K69
      0x8C200703,  //  0051  GETMET	R8	R3	K3
      0x60280013,  //  0052  GETGBL	R10	G19
      0x7C280000,  //  0053  CALL	R10	0
      0x982A2546,  //  0054  SETIDX	R10	K18	K70
      0x982A8007,  //  0055  SETIDX	R10	K64	R7
      0x7C200400,  //  0056  CALL	R8	2
      0x7001FFEB,  //  0057  JMP		#0044
      0x5810001C,  //  0058  LDCONST	R4	K28
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
      0x8C1C0D2D,  //  0069  GETMET	R7	R6	K45
      0x58240047,  //  006A  LDCONST	R9	K71
      0x7C1C0400,  //  006B  CALL	R7	2
      0x781E0006,  //  006C  JMPF	R7	#0074
      0x001C0B48,  //  006D  ADD	R7	R5	K72
      0x8C200703,  //  006E  GETMET	R8	R3	K3
      0x60280013,  //  006F  GETGBL	R10	G19
      0x7C280000,  //  0070  CALL	R10	0
      0x982A2549,  //  0071  SETIDX	R10	K18	K73
      0x982A8007,  //  0072  SETIDX	R10	K64	R7
      0x7C200400,  //  0073  CALL	R8	2
      0x7001FFEB,  //  0074  JMP		#0061
      0x5810001C,  //  0075  LDCONST	R4	K28
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
