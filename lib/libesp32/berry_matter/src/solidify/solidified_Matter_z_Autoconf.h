/* Solidification of Matter_z_Autoconf.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Autoconf' ktab size: 72, total: 89 (saved 136 bytes)
static const bvalue be_ktab_class_Matter_Autoconf[72] = {
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
  /* K16  */  be_const_int(1),
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
  /* K34  */  be_nested_str_weak(light),
  /* K35  */  be_nested_str_weak(get),
  /* K36  */  be_nested_str_weak(channels),
  /* K37  */  be_nested_str_weak(light1),
  /* K38  */  be_nested_str_weak(light2),
  /* K39  */  be_nested_str_weak(light3),
  /* K40  */  be_nested_str_weak(cmd),
  /* K41  */  be_nested_str_weak(Status_X2013),
  /* K42  */  be_nested_str_weak(MTR_X3A_X20Status_X2013_X20_X3D_X20),
  /* K43  */  be_nested_str_weak(contains),
  /* K44  */  be_nested_str_weak(StatusSHT),
  /* K45  */  be_nested_str_weak(SHT),
  /* K46  */  be_nested_str_weak(MTR_X3A_X20_X27_X25s_X27_X20_X3D_X20_X25s),
  /* K47  */  be_nested_str_weak(Relay1),
  /* K48  */  be_nested_str_weak(Relay2),
  /* K49  */  be_nested_str_weak(MTR_X3A_X20relay1_X3D_X25s_X20relay2_X3D_X25s),
  /* K50  */  be_nested_str_weak(TiltConfig),
  /* K51  */  be_nested_str_weak(shutter_X2Btilt),
  /* K52  */  be_nested_str_weak(shutter),
  /* K53  */  be_nested_str_weak(get_power),
  /* K54  */  be_nested_str_weak(relay),
  /* K55  */  be_nested_str_weak(load),
  /* K56  */  be_nested_str_weak(read_sensors),
  /* K57  */  be_nested_str_weak(autoconf_sensors_list),
  /* K58  */  be_nested_str_weak(k2l),
  /* K59  */  be_nested_str_weak(Temperature),
  /* K60  */  be_nested_str_weak(_X23Temperature),
  /* K61  */  be_nested_str_weak(temperature),
  /* K62  */  be_nested_str_weak(filter),
  /* K63  */  be_nested_str_weak(Pressure),
  /* K64  */  be_nested_str_weak(_X23Pressure),
  /* K65  */  be_nested_str_weak(pressure),
  /* K66  */  be_nested_str_weak(Illuminance),
  /* K67  */  be_nested_str_weak(_X23Illuminance),
  /* K68  */  be_nested_str_weak(illuminance),
  /* K69  */  be_nested_str_weak(Humidity),
  /* K70  */  be_nested_str_weak(_X23Humidity),
  /* K71  */  be_nested_str_weak(humidity),
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
    ( &(const binstruction[149]) {  /* code */
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
      0x58240010,  //  0023  LDCONST	R9	K16
      0x60280013,  //  0024  GETGBL	R10	G19
      0x7C280000,  //  0025  CALL	R10	0
      0x7C180800,  //  0026  CALL	R6	4
      0x7C100400,  //  0027  CALL	R4	2
      0xB8121200,  //  0028  GETNGBL	R4	K9
      0x60140018,  //  0029  GETGBL	R5	G24
      0x5818000C,  //  002A  LDCONST	R6	K12
      0x581C0010,  //  002B  LDCONST	R7	K16
      0x58200011,  //  002C  LDCONST	R8	K17
      0x5824000E,  //  002D  LDCONST	R9	K14
      0x7C140800,  //  002E  CALL	R5	4
      0x5818000B,  //  002F  LDCONST	R6	K11
      0x7C100400,  //  0030  CALL	R4	2
      0x60100010,  //  0031  GETGBL	R4	G16
      0x5C140400,  //  0032  MOVE	R5	R2
      0x7C100200,  //  0033  CALL	R4	1
      0xA8020056,  //  0034  EXBLK	0	#008C
      0x5C140800,  //  0035  MOVE	R5	R4
      0x7C140000,  //  0036  CALL	R5	0
      0x1C180B06,  //  0037  EQ	R6	R5	K6
      0x781A0000,  //  0038  JMPF	R6	#003A
      0x7001FFFA,  //  0039  JMP		#0035
      0xA802003F,  //  003A  EXBLK	0	#007B
      0x60180008,  //  003B  GETGBL	R6	G8
      0x5C1C0A00,  //  003C  MOVE	R7	R5
      0x7C180200,  //  003D  CALL	R6	1
      0x94180206,  //  003E  GETIDX	R6	R1	R6
      0x8C1C0D07,  //  003F  GETMET	R7	R6	K7
      0x58240012,  //  0040  LDCONST	R9	K18
      0x7C1C0400,  //  0041  CALL	R7	2
      0x4C200000,  //  0042  LDNIL	R8
      0x1C200E08,  //  0043  EQ	R8	R7	R8
      0x78220005,  //  0044  JMPF	R8	#004B
      0xB8221200,  //  0045  GETNGBL	R8	K9
      0x58240013,  //  0046  LDCONST	R9	K19
      0x58280014,  //  0047  LDCONST	R10	K20
      0x7C200400,  //  0048  CALL	R8	2
      0xA8040001,  //  0049  EXBLK	1	1
      0x7001FFE9,  //  004A  JMP		#0035
      0x1C200F0D,  //  004B  EQ	R8	R7	K13
      0x78220005,  //  004C  JMPF	R8	#0053
      0xB8221200,  //  004D  GETNGBL	R8	K9
      0x58240015,  //  004E  LDCONST	R9	K21
      0x58280014,  //  004F  LDCONST	R10	K20
      0x7C200400,  //  0050  CALL	R8	2
      0xA8040001,  //  0051  EXBLK	1	1
      0x7001FFE1,  //  0052  JMP		#0035
      0x88200100,  //  0053  GETMBR	R8	R0	K0
      0x88201116,  //  0054  GETMBR	R8	R8	K22
      0x8C201107,  //  0055  GETMET	R8	R8	K7
      0x5C280E00,  //  0056  MOVE	R10	R7
      0x7C200400,  //  0057  CALL	R8	2
      0x4C240000,  //  0058  LDNIL	R9
      0x1C241009,  //  0059  EQ	R9	R8	R9
      0x78260009,  //  005A  JMPF	R9	#0065
      0xB8261200,  //  005B  GETNGBL	R9	K9
      0x60280008,  //  005C  GETGBL	R10	G8
      0x5C2C0E00,  //  005D  MOVE	R11	R7
      0x7C280200,  //  005E  CALL	R10	1
      0x002A2E0A,  //  005F  ADD	R10	K23	R10
      0x00281518,  //  0060  ADD	R10	R10	K24
      0x582C000B,  //  0061  LDCONST	R11	K11
      0x7C240400,  //  0062  CALL	R9	2
      0xA8040001,  //  0063  EXBLK	1	1
      0x7001FFCF,  //  0064  JMP		#0035
      0x5C241000,  //  0065  MOVE	R9	R8
      0x88280100,  //  0066  GETMBR	R10	R0	K0
      0x5C2C0A00,  //  0067  MOVE	R11	R5
      0x5C300C00,  //  0068  MOVE	R12	R6
      0x7C240600,  //  0069  CALL	R9	3
      0x8C280703,  //  006A  GETMET	R10	R3	K3
      0x5C301200,  //  006B  MOVE	R12	R9
      0x7C280400,  //  006C  CALL	R10	2
      0xB82A1200,  //  006D  GETNGBL	R10	K9
      0x602C0018,  //  006E  GETGBL	R11	G24
      0x5830000C,  //  006F  LDCONST	R12	K12
      0x5C340A00,  //  0070  MOVE	R13	R5
      0x5C380E00,  //  0071  MOVE	R14	R7
      0x883C0100,  //  0072  GETMBR	R15	R0	K0
      0x8C3C1F19,  //  0073  GETMET	R15	R15	K25
      0x5C440C00,  //  0074  MOVE	R17	R6
      0x7C3C0400,  //  0075  CALL	R15	2
      0x7C2C0800,  //  0076  CALL	R11	4
      0x5830000B,  //  0077  LDCONST	R12	K11
      0x7C280400,  //  0078  CALL	R10	2
      0xA8040001,  //  0079  EXBLK	1	1
      0x7002000F,  //  007A  JMP		#008B
      0xAC180002,  //  007B  CATCH	R6	0	2
      0x7002000C,  //  007C  JMP		#008A
      0xB8221200,  //  007D  GETNGBL	R8	K9
      0x60240008,  //  007E  GETGBL	R9	G8
      0x5C280C00,  //  007F  MOVE	R10	R6
      0x7C240200,  //  0080  CALL	R9	1
      0x00263409,  //  0081  ADD	R9	K26	R9
      0x0024131B,  //  0082  ADD	R9	R9	K27
      0x60280008,  //  0083  GETGBL	R10	G8
      0x5C2C0E00,  //  0084  MOVE	R11	R7
      0x7C280200,  //  0085  CALL	R10	1
      0x0024120A,  //  0086  ADD	R9	R9	R10
      0x5828000B,  //  0087  LDCONST	R10	K11
      0x7C200400,  //  0088  CALL	R8	2
      0x70020000,  //  0089  JMP		#008B
      0xB0080000,  //  008A  RAISE	2	R0	R0
      0x7001FFA8,  //  008B  JMP		#0035
      0x5810001C,  //  008C  LDCONST	R4	K28
      0xAC100200,  //  008D  CATCH	R4	1	0
      0xB0080000,  //  008E  RAISE	2	R0	R0
      0xB8123A00,  //  008F  GETNGBL	R4	K29
      0x8C10091E,  //  0090  GETMET	R4	R4	K30
      0x5818001F,  //  0091  LDCONST	R6	K31
      0x581C0020,  //  0092  LDCONST	R7	K32
      0x7C100600,  //  0093  CALL	R4	3
      0x80000000,  //  0094  RET	0
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
    ( &(const binstruction[261]) {  /* code */
      0xA4064200,  //  0000  IMPORT	R1	K33
      0x60080013,  //  0001  GETGBL	R2	G19
      0x7C080000,  //  0002  CALL	R2	0
      0x580C000B,  //  0003  LDCONST	R3	K11
      0x58100006,  //  0004  LDCONST	R4	K6
      0xA4164400,  //  0005  IMPORT	R5	K34
      0x8C180B23,  //  0006  GETMET	R6	R5	K35
      0x58200006,  //  0007  LDCONST	R8	K6
      0x7C180400,  //  0008  CALL	R6	2
      0x4C1C0000,  //  0009  LDNIL	R7
      0x201C0C07,  //  000A  NE	R7	R6	R7
      0x781E0066,  //  000B  JMPF	R7	#0073
      0x601C000C,  //  000C  GETGBL	R7	G12
      0x8C200D07,  //  000D  GETMET	R8	R6	K7
      0x58280024,  //  000E  LDCONST	R10	K36
      0x582C000E,  //  000F  LDCONST	R11	K14
      0x7C200600,  //  0010  CALL	R8	3
      0x7C1C0200,  //  0011  CALL	R7	1
      0x58100010,  //  0012  LDCONST	R4	K16
      0x24200F06,  //  0013  GT	R8	R7	K6
      0x7822005D,  //  0014  JMPF	R8	#0073
      0x1C200F10,  //  0015  EQ	R8	R7	K16
      0x7822001E,  //  0016  JMPF	R8	#0036
      0x60200008,  //  0017  GETGBL	R8	G8
      0x5C240600,  //  0018  MOVE	R9	R3
      0x7C200200,  //  0019  CALL	R8	1
      0x60240013,  //  001A  GETGBL	R9	G19
      0x7C240000,  //  001B  CALL	R9	0
      0x98262525,  //  001C  SETIDX	R9	K18	K37
      0x98081009,  //  001D  SETIDX	R2	R8	R9
      0x000C0710,  //  001E  ADD	R3	R3	K16
      0x58200010,  //  001F  LDCONST	R8	K16
      0x4C240000,  //  0020  LDNIL	R9
      0x8C280B23,  //  0021  GETMET	R10	R5	K35
      0x5C301000,  //  0022  MOVE	R12	R8
      0x7C280400,  //  0023  CALL	R10	2
      0x5C241400,  //  0024  MOVE	R9	R10
      0x4C2C0000,  //  0025  LDNIL	R11
      0x2028140B,  //  0026  NE	R10	R10	R11
      0x782A000C,  //  0027  JMPF	R10	#0035
      0x60280008,  //  0028  GETGBL	R10	G8
      0x5C2C0600,  //  0029  MOVE	R11	R3
      0x7C280200,  //  002A  CALL	R10	1
      0x602C0013,  //  002B  GETGBL	R11	G19
      0x7C2C0000,  //  002C  CALL	R11	0
      0x982E2525,  //  002D  SETIDX	R11	K18	K37
      0x00301110,  //  002E  ADD	R12	R8	K16
      0x982E440C,  //  002F  SETIDX	R11	K34	R12
      0x9808140B,  //  0030  SETIDX	R2	R10	R11
      0x000C0710,  //  0031  ADD	R3	R3	K16
      0x00100910,  //  0032  ADD	R4	R4	K16
      0x00201110,  //  0033  ADD	R8	R8	K16
      0x7001FFEB,  //  0034  JMP		#0021
      0x7002003C,  //  0035  JMP		#0073
      0x1C200F0B,  //  0036  EQ	R8	R7	K11
      0x78220008,  //  0037  JMPF	R8	#0041
      0x60200008,  //  0038  GETGBL	R8	G8
      0x5C240600,  //  0039  MOVE	R9	R3
      0x7C200200,  //  003A  CALL	R8	1
      0x60240013,  //  003B  GETGBL	R9	G19
      0x7C240000,  //  003C  CALL	R9	0
      0x98262526,  //  003D  SETIDX	R9	K18	K38
      0x98081009,  //  003E  SETIDX	R2	R8	R9
      0x000C0710,  //  003F  ADD	R3	R3	K16
      0x70020031,  //  0040  JMP		#0073
      0x1C200F14,  //  0041  EQ	R8	R7	K20
      0x7822002B,  //  0042  JMPF	R8	#006F
      0x60200008,  //  0043  GETGBL	R8	G8
      0x5C240600,  //  0044  MOVE	R9	R3
      0x7C200200,  //  0045  CALL	R8	1
      0x60240013,  //  0046  GETGBL	R9	G19
      0x7C240000,  //  0047  CALL	R9	0
      0x98262527,  //  0048  SETIDX	R9	K18	K39
      0x98081009,  //  0049  SETIDX	R2	R8	R9
      0x000C0710,  //  004A  ADD	R3	R3	K16
      0x8C200B23,  //  004B  GETMET	R8	R5	K35
      0x58280010,  //  004C  LDCONST	R10	K16
      0x7C200400,  //  004D  CALL	R8	2
      0x4C240000,  //  004E  LDNIL	R9
      0x20241009,  //  004F  NE	R9	R8	R9
      0x7826001C,  //  0050  JMPF	R9	#006E
      0x6024000C,  //  0051  GETGBL	R9	G12
      0x8C281107,  //  0052  GETMET	R10	R8	K7
      0x58300024,  //  0053  LDCONST	R12	K36
      0x5834000E,  //  0054  LDCONST	R13	K14
      0x7C280600,  //  0055  CALL	R10	3
      0x7C240200,  //  0056  CALL	R9	1
      0x1C281310,  //  0057  EQ	R10	R9	K16
      0x782A0009,  //  0058  JMPF	R10	#0063
      0x60280008,  //  0059  GETGBL	R10	G8
      0x5C2C0600,  //  005A  MOVE	R11	R3
      0x7C280200,  //  005B  CALL	R10	1
      0x602C0013,  //  005C  GETGBL	R11	G19
      0x7C2C0000,  //  005D  CALL	R11	0
      0x982E2525,  //  005E  SETIDX	R11	K18	K37
      0x9808140B,  //  005F  SETIDX	R2	R10	R11
      0x000C0710,  //  0060  ADD	R3	R3	K16
      0x00100910,  //  0061  ADD	R4	R4	K16
      0x7002000A,  //  0062  JMP		#006E
      0x1C28130B,  //  0063  EQ	R10	R9	K11
      0x782A0008,  //  0064  JMPF	R10	#006E
      0x60280008,  //  0065  GETGBL	R10	G8
      0x5C2C0600,  //  0066  MOVE	R11	R3
      0x7C280200,  //  0067  CALL	R10	1
      0x602C0013,  //  0068  GETGBL	R11	G19
      0x7C2C0000,  //  0069  CALL	R11	0
      0x982E2526,  //  006A  SETIDX	R11	K18	K38
      0x9808140B,  //  006B  SETIDX	R2	R10	R11
      0x000C0710,  //  006C  ADD	R3	R3	K16
      0x00100910,  //  006D  ADD	R4	R4	K16
      0x70020003,  //  006E  JMP		#0073
      0x54220003,  //  006F  LDINT	R8	4
      0x1C200E08,  //  0070  EQ	R8	R7	R8
      0x78220000,  //  0071  JMPF	R8	#0073
      0x7001FFFF,  //  0072  JMP		#0073
      0xB81E3A00,  //  0073  GETNGBL	R7	K29
      0x8C1C0F28,  //  0074  GETMET	R7	R7	K40
      0x58240029,  //  0075  LDCONST	R9	K41
      0x50280200,  //  0076  LDBOOL	R10	1	0
      0x7C1C0600,  //  0077  CALL	R7	3
      0x60200012,  //  0078  GETGBL	R8	G18
      0x7C200000,  //  0079  CALL	R8	0
      0xB8261200,  //  007A  GETNGBL	R9	K9
      0x60280008,  //  007B  GETGBL	R10	G8
      0x5C2C0E00,  //  007C  MOVE	R11	R7
      0x7C280200,  //  007D  CALL	R10	1
      0x002A540A,  //  007E  ADD	R10	K42	R10
      0x582C0014,  //  007F  LDCONST	R11	K20
      0x7C240400,  //  0080  CALL	R9	2
      0x4C240000,  //  0081  LDNIL	R9
      0x20240E09,  //  0082  NE	R9	R7	R9
      0x7826004D,  //  0083  JMPF	R9	#00D2
      0x8C240F2B,  //  0084  GETMET	R9	R7	K43
      0x582C002C,  //  0085  LDCONST	R11	K44
      0x7C240400,  //  0086  CALL	R9	2
      0x78260049,  //  0087  JMPF	R9	#00D2
      0x941C0F2C,  //  0088  GETIDX	R7	R7	K44
      0x58240006,  //  0089  LDCONST	R9	K6
      0x50280200,  //  008A  LDBOOL	R10	1	0
      0x782A0045,  //  008B  JMPF	R10	#00D2
      0x60280008,  //  008C  GETGBL	R10	G8
      0x5C2C1200,  //  008D  MOVE	R11	R9
      0x7C280200,  //  008E  CALL	R10	1
      0x002A5A0A,  //  008F  ADD	R10	K45	R10
      0x8C2C0F2B,  //  0090  GETMET	R11	R7	K43
      0x5C341400,  //  0091  MOVE	R13	R10
      0x7C2C0400,  //  0092  CALL	R11	2
      0x742E0000,  //  0093  JMPT	R11	#0095
      0x7002003C,  //  0094  JMP		#00D2
      0x942C0E0A,  //  0095  GETIDX	R11	R7	R10
      0xB8321200,  //  0096  GETNGBL	R12	K9
      0x60340018,  //  0097  GETGBL	R13	G24
      0x5838002E,  //  0098  LDCONST	R14	K46
      0x5C3C1400,  //  0099  MOVE	R15	R10
      0x60400008,  //  009A  GETGBL	R16	G8
      0x5C441600,  //  009B  MOVE	R17	R11
      0x7C400200,  //  009C  CALL	R16	1
      0x7C340600,  //  009D  CALL	R13	3
      0x58380014,  //  009E  LDCONST	R14	K20
      0x7C300400,  //  009F  CALL	R12	2
      0x8C301707,  //  00A0  GETMET	R12	R11	K7
      0x5838002F,  //  00A1  LDCONST	R14	K47
      0x543DFFFE,  //  00A2  LDINT	R15	-1
      0x7C300600,  //  00A3  CALL	R12	3
      0x8C341707,  //  00A4  GETMET	R13	R11	K7
      0x583C0030,  //  00A5  LDCONST	R15	K48
      0x5441FFFE,  //  00A6  LDINT	R16	-1
      0x7C340600,  //  00A7  CALL	R13	3
      0x24381906,  //  00A8  GT	R14	R12	K6
      0x783A0002,  //  00A9  JMPF	R14	#00AD
      0x8C381103,  //  00AA  GETMET	R14	R8	K3
      0x04401910,  //  00AB  SUB	R16	R12	K16
      0x7C380400,  //  00AC  CALL	R14	2
      0x24381B06,  //  00AD  GT	R14	R13	K6
      0x783A0002,  //  00AE  JMPF	R14	#00B2
      0x8C381103,  //  00AF  GETMET	R14	R8	K3
      0x04401B10,  //  00B0  SUB	R16	R13	K16
      0x7C380400,  //  00B1  CALL	R14	2
      0xB83A1200,  //  00B2  GETNGBL	R14	K9
      0x603C0018,  //  00B3  GETGBL	R15	G24
      0x58400031,  //  00B4  LDCONST	R16	K49
      0x5C441800,  //  00B5  MOVE	R17	R12
      0x5C481A00,  //  00B6  MOVE	R18	R13
      0x7C3C0600,  //  00B7  CALL	R15	3
      0x58400014,  //  00B8  LDCONST	R16	K20
      0x7C380400,  //  00B9  CALL	R14	2
      0x8C381707,  //  00BA  GETMET	R14	R11	K7
      0x58400032,  //  00BB  LDCONST	R16	K50
      0x7C380400,  //  00BC  CALL	R14	2
      0x783A0002,  //  00BD  JMPF	R14	#00C1
      0x943C1D0B,  //  00BE  GETIDX	R15	R14	K11
      0x243C1F06,  //  00BF  GT	R15	R15	K6
      0x743E0000,  //  00C0  JMPT	R15	#00C2
      0x503C0001,  //  00C1  LDBOOL	R15	0	1
      0x503C0200,  //  00C2  LDBOOL	R15	1	0
      0x60400008,  //  00C3  GETGBL	R16	G8
      0x5C440600,  //  00C4  MOVE	R17	R3
      0x7C400200,  //  00C5  CALL	R16	1
      0x60440013,  //  00C6  GETGBL	R17	G19
      0x7C440000,  //  00C7  CALL	R17	0
      0x783E0001,  //  00C8  JMPF	R15	#00CB
      0x58480033,  //  00C9  LDCONST	R18	K51
      0x70020000,  //  00CA  JMP		#00CC
      0x58480034,  //  00CB  LDCONST	R18	K52
      0x98462412,  //  00CC  SETIDX	R17	K18	R18
      0x98466809,  //  00CD  SETIDX	R17	K52	R9
      0x98082011,  //  00CE  SETIDX	R2	R16	R17
      0x000C0710,  //  00CF  ADD	R3	R3	K16
      0x00241310,  //  00D0  ADD	R9	R9	K16
      0x7001FFB7,  //  00D1  JMP		#008A
      0x6024000C,  //  00D2  GETGBL	R9	G12
      0xB82A3A00,  //  00D3  GETNGBL	R10	K29
      0x8C281535,  //  00D4  GETMET	R10	R10	K53
      0x7C280200,  //  00D5  CALL	R10	1
      0x7C240200,  //  00D6  CALL	R9	1
      0x58280006,  //  00D7  LDCONST	R10	K6
      0x04241204,  //  00D8  SUB	R9	R9	R4
      0x142C1409,  //  00D9  LT	R11	R10	R9
      0x782E0011,  //  00DA  JMPF	R11	#00ED
      0x8C2C1107,  //  00DB  GETMET	R11	R8	K7
      0x5C341400,  //  00DC  MOVE	R13	R10
      0x7C2C0400,  //  00DD  CALL	R11	2
      0x4C300000,  //  00DE  LDNIL	R12
      0x1C2C160C,  //  00DF  EQ	R11	R11	R12
      0x782E0009,  //  00E0  JMPF	R11	#00EB
      0x602C0008,  //  00E1  GETGBL	R11	G8
      0x5C300600,  //  00E2  MOVE	R12	R3
      0x7C2C0200,  //  00E3  CALL	R11	1
      0x60300013,  //  00E4  GETGBL	R12	G19
      0x7C300000,  //  00E5  CALL	R12	0
      0x98322536,  //  00E6  SETIDX	R12	K18	K54
      0x00341510,  //  00E7  ADD	R13	R10	K16
      0x98326C0D,  //  00E8  SETIDX	R12	K54	R13
      0x9808160C,  //  00E9  SETIDX	R2	R11	R12
      0x000C0710,  //  00EA  ADD	R3	R3	K16
      0x00281510,  //  00EB  ADD	R10	R10	K16
      0x7001FFEB,  //  00EC  JMP		#00D9
      0x8C2C0337,  //  00ED  GETMET	R11	R1	K55
      0xB8363A00,  //  00EE  GETNGBL	R13	K29
      0x8C341B38,  //  00EF  GETMET	R13	R13	K56
      0x7C340200,  //  00F0  CALL	R13	1
      0x7C2C0400,  //  00F1  CALL	R11	2
      0x8C300139,  //  00F2  GETMET	R12	R0	K57
      0x5C381600,  //  00F3  MOVE	R14	R11
      0x7C300400,  //  00F4  CALL	R12	2
      0x60340010,  //  00F5  GETGBL	R13	G16
      0x5C381800,  //  00F6  MOVE	R14	R12
      0x7C340200,  //  00F7  CALL	R13	1
      0xA8020007,  //  00F8  EXBLK	0	#0101
      0x5C381A00,  //  00F9  MOVE	R14	R13
      0x7C380000,  //  00FA  CALL	R14	0
      0x603C0008,  //  00FB  GETGBL	R15	G8
      0x5C400600,  //  00FC  MOVE	R16	R3
      0x7C3C0200,  //  00FD  CALL	R15	1
      0x98081E0E,  //  00FE  SETIDX	R2	R15	R14
      0x000C0710,  //  00FF  ADD	R3	R3	K16
      0x7001FFF7,  //  0100  JMP		#00F9
      0x5834001C,  //  0101  LDCONST	R13	K28
      0xAC340200,  //  0102  CATCH	R13	1	0
      0xB0080000,  //  0103  RAISE	2	R0	R0
      0x80040400,  //  0104  RET	1	R2
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
      0x8808053A,  //  0001  GETMBR	R2	R2	K58
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
      0x8C1C0D2B,  //  0012  GETMET	R7	R6	K43
      0x5824003B,  //  0013  LDCONST	R9	K59
      0x7C1C0400,  //  0014  CALL	R7	2
      0x781E0006,  //  0015  JMPF	R7	#001D
      0x001C0B3C,  //  0016  ADD	R7	R5	K60
      0x8C200703,  //  0017  GETMET	R8	R3	K3
      0x60280013,  //  0018  GETGBL	R10	G19
      0x7C280000,  //  0019  CALL	R10	0
      0x982A253D,  //  001A  SETIDX	R10	K18	K61
      0x982A7C07,  //  001B  SETIDX	R10	K62	R7
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
      0x8C1C0D2B,  //  002F  GETMET	R7	R6	K43
      0x5824003F,  //  0030  LDCONST	R9	K63
      0x7C1C0400,  //  0031  CALL	R7	2
      0x781E0006,  //  0032  JMPF	R7	#003A
      0x001C0B40,  //  0033  ADD	R7	R5	K64
      0x8C200703,  //  0034  GETMET	R8	R3	K3
      0x60280013,  //  0035  GETGBL	R10	G19
      0x7C280000,  //  0036  CALL	R10	0
      0x982A2541,  //  0037  SETIDX	R10	K18	K65
      0x982A7C07,  //  0038  SETIDX	R10	K62	R7
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
      0x8C1C0D2B,  //  004C  GETMET	R7	R6	K43
      0x58240042,  //  004D  LDCONST	R9	K66
      0x7C1C0400,  //  004E  CALL	R7	2
      0x781E0006,  //  004F  JMPF	R7	#0057
      0x001C0B43,  //  0050  ADD	R7	R5	K67
      0x8C200703,  //  0051  GETMET	R8	R3	K3
      0x60280013,  //  0052  GETGBL	R10	G19
      0x7C280000,  //  0053  CALL	R10	0
      0x982A2544,  //  0054  SETIDX	R10	K18	K68
      0x982A7C07,  //  0055  SETIDX	R10	K62	R7
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
      0x8C1C0D2B,  //  0069  GETMET	R7	R6	K43
      0x58240045,  //  006A  LDCONST	R9	K69
      0x7C1C0400,  //  006B  CALL	R7	2
      0x781E0006,  //  006C  JMPF	R7	#0074
      0x001C0B46,  //  006D  ADD	R7	R5	K70
      0x8C200703,  //  006E  GETMET	R8	R3	K3
      0x60280013,  //  006F  GETGBL	R10	G19
      0x7C280000,  //  0070  CALL	R10	0
      0x982A2547,  //  0071  SETIDX	R10	K18	K71
      0x982A7C07,  //  0072  SETIDX	R10	K62	R7
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
