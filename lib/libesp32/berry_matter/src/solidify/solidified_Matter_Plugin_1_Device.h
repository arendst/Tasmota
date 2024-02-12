/* Solidification of Matter_Plugin_1_Device.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Plugin_Device;

/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(Matter_Plugin_Device_read_attribute,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[36]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_const_int(3),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(set),
    /* K7   */  be_nested_str_weak(U2),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(U1),
    /* K10  */  be_nested_str_weak(Matter_TLV_array),
    /* K11  */  be_nested_str_weak(TYPES),
    /* K12  */  be_nested_str_weak(keys),
    /* K13  */  be_nested_str_weak(add_struct),
    /* K14  */  be_nested_str_weak(add_TLV),
    /* K15  */  be_nested_str_weak(stop_iteration),
    /* K16  */  be_nested_str_weak(NON_BRIDGE_VENDOR),
    /* K17  */  be_nested_str_weak(find),
    /* K18  */  be_nested_str_weak(get_admin_vendor),
    /* K19  */  be_nested_str_weak(device),
    /* K20  */  be_nested_str_weak(disable_bridge_mode),
    /* K21  */  be_nested_str_weak(string),
    /* K22  */  be_nested_str_weak(UTF1),
    /* K23  */  be_nested_str_weak(tasmota),
    /* K24  */  be_nested_str_weak(cmd),
    /* K25  */  be_nested_str_weak(DeviceName),
    /* K26  */  be_nested_str_weak(get_name),
    /* K27  */  be_nested_str_weak(Status_X202),
    /* K28  */  be_nested_str_weak(StatusFWR),
    /* K29  */  be_nested_str_weak(Version),
    /* K30  */  be_nested_str_weak(_X28),
    /* K31  */  be_nested_str_weak(wifi),
    /* K32  */  be_nested_str_weak(mac),
    /* K33  */  be_nested_str_weak(),
    /* K34  */  be_nested_str_weak(BOOL),
    /* K35  */  be_nested_str_weak(read_attribute),
    }),
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[189]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140502,  //  0002  GETMBR	R5	R2	K2
      0x88180503,  //  0003  GETMBR	R6	R2	K3
      0x1C1C0B04,  //  0004  EQ	R7	R5	K4
      0x781E000F,  //  0005  JMPF	R7	#0016
      0x1C1C0D05,  //  0006  EQ	R7	R6	K5
      0x781E0005,  //  0007  JMPF	R7	#000E
      0x8C1C0706,  //  0008  GETMET	R7	R3	K6
      0x88240907,  //  0009  GETMBR	R9	R4	K7
      0x58280005,  //  000A  LDCONST	R10	K5
      0x7C1C0600,  //  000B  CALL	R7	3
      0x80040E00,  //  000C  RET	1	R7
      0x70020006,  //  000D  JMP		#0015
      0x1C1C0D08,  //  000E  EQ	R7	R6	K8
      0x781E0004,  //  000F  JMPF	R7	#0015
      0x8C1C0706,  //  0010  GETMET	R7	R3	K6
      0x88240909,  //  0011  GETMBR	R9	R4	K9
      0x58280005,  //  0012  LDCONST	R10	K5
      0x7C1C0600,  //  0013  CALL	R7	3
      0x80040E00,  //  0014  RET	1	R7
      0x7002009D,  //  0015  JMP		#00B4
      0x541E0003,  //  0016  LDINT	R7	4
      0x1C1C0A07,  //  0017  EQ	R7	R5	R7
      0x781E0004,  //  0018  JMPF	R7	#001E
      0x1C1C0D05,  //  0019  EQ	R7	R6	K5
      0x781E0001,  //  001A  JMPF	R7	#001D
      0x4C1C0000,  //  001B  LDNIL	R7
      0x80040E00,  //  001C  RET	1	R7
      0x70020095,  //  001D  JMP		#00B4
      0x541E0004,  //  001E  LDINT	R7	5
      0x1C1C0A07,  //  001F  EQ	R7	R5	R7
      0x781E0000,  //  0020  JMPF	R7	#0022
      0x70020091,  //  0021  JMP		#00B4
      0x541E001C,  //  0022  LDINT	R7	29
      0x1C1C0A07,  //  0023  EQ	R7	R5	R7
      0x781E0034,  //  0024  JMPF	R7	#005A
      0x1C1C0D05,  //  0025  EQ	R7	R6	K5
      0x781E0031,  //  0026  JMPF	R7	#0059
      0x8C1C090A,  //  0027  GETMET	R7	R4	K10
      0x7C1C0200,  //  0028  CALL	R7	1
      0x8820010B,  //  0029  GETMBR	R8	R0	K11
      0x60240010,  //  002A  GETGBL	R9	G16
      0x8C28110C,  //  002B  GETMET	R10	R8	K12
      0x7C280200,  //  002C  CALL	R10	1
      0x7C240200,  //  002D  CALL	R9	1
      0xA802000E,  //  002E  EXBLK	0	#003E
      0x5C281200,  //  002F  MOVE	R10	R9
      0x7C280000,  //  0030  CALL	R10	0
      0x8C2C0F0D,  //  0031  GETMET	R11	R7	K13
      0x7C2C0200,  //  0032  CALL	R11	1
      0x8C30170E,  //  0033  GETMET	R12	R11	K14
      0x58380005,  //  0034  LDCONST	R14	K5
      0x883C0907,  //  0035  GETMBR	R15	R4	K7
      0x5C401400,  //  0036  MOVE	R16	R10
      0x7C300800,  //  0037  CALL	R12	4
      0x8C30170E,  //  0038  GETMET	R12	R11	K14
      0x58380008,  //  0039  LDCONST	R14	K8
      0x883C0907,  //  003A  GETMBR	R15	R4	K7
      0x9440100A,  //  003B  GETIDX	R16	R8	R10
      0x7C300800,  //  003C  CALL	R12	4
      0x7001FFF0,  //  003D  JMP		#002F
      0x5824000F,  //  003E  LDCONST	R9	K15
      0xAC240200,  //  003F  CATCH	R9	1	0
      0xB0080000,  //  0040  RAISE	2	R0	R0
      0x88240110,  //  0041  GETMBR	R9	R0	K16
      0x8C241311,  //  0042  GETMET	R9	R9	K17
      0x8C2C0312,  //  0043  GETMET	R11	R1	K18
      0x7C2C0200,  //  0044  CALL	R11	1
      0x7C240400,  //  0045  CALL	R9	2
      0x4C280000,  //  0046  LDNIL	R10
      0x1C24120A,  //  0047  EQ	R9	R9	R10
      0x7826000E,  //  0048  JMPF	R9	#0058
      0x88240113,  //  0049  GETMBR	R9	R0	K19
      0x88241314,  //  004A  GETMBR	R9	R9	K20
      0x7426000B,  //  004B  JMPT	R9	#0058
      0x8C240F0D,  //  004C  GETMET	R9	R7	K13
      0x7C240200,  //  004D  CALL	R9	1
      0x8C28130E,  //  004E  GETMET	R10	R9	K14
      0x58300005,  //  004F  LDCONST	R12	K5
      0x88340907,  //  0050  GETMBR	R13	R4	K7
      0x543A0012,  //  0051  LDINT	R14	19
      0x7C280800,  //  0052  CALL	R10	4
      0x8C28130E,  //  0053  GETMET	R10	R9	K14
      0x58300008,  //  0054  LDCONST	R12	K8
      0x88340907,  //  0055  GETMBR	R13	R4	K7
      0x58380008,  //  0056  LDCONST	R14	K8
      0x7C280800,  //  0057  CALL	R10	4
      0x80040E00,  //  0058  RET	1	R7
      0x70020059,  //  0059  JMP		#00B4
      0x541E0038,  //  005A  LDINT	R7	57
      0x1C1C0A07,  //  005B  EQ	R7	R5	R7
      0x781E0056,  //  005C  JMPF	R7	#00B4
      0xA41E2A00,  //  005D  IMPORT	R7	K21
      0x1C200D04,  //  005E  EQ	R8	R6	K4
      0x7822000A,  //  005F  JMPF	R8	#006B
      0x8C200706,  //  0060  GETMET	R8	R3	K6
      0x88280916,  //  0061  GETMBR	R10	R4	K22
      0xB82E2E00,  //  0062  GETNGBL	R11	K23
      0x8C2C1718,  //  0063  GETMET	R11	R11	K24
      0x58340019,  //  0064  LDCONST	R13	K25
      0x50380200,  //  0065  LDBOOL	R14	1	0
      0x7C2C0600,  //  0066  CALL	R11	3
      0x942C1719,  //  0067  GETIDX	R11	R11	K25
      0x7C200600,  //  0068  CALL	R8	3
      0x80041000,  //  0069  RET	1	R8
      0x70020048,  //  006A  JMP		#00B4
      0x54220004,  //  006B  LDINT	R8	5
      0x1C200C08,  //  006C  EQ	R8	R6	R8
      0x78220006,  //  006D  JMPF	R8	#0075
      0x8C200706,  //  006E  GETMET	R8	R3	K6
      0x88280916,  //  006F  GETMBR	R10	R4	K22
      0x8C2C011A,  //  0070  GETMET	R11	R0	K26
      0x7C2C0200,  //  0071  CALL	R11	1
      0x7C200600,  //  0072  CALL	R8	3
      0x80041000,  //  0073  RET	1	R8
      0x7002003E,  //  0074  JMP		#00B4
      0x54220009,  //  0075  LDINT	R8	10
      0x1C200C08,  //  0076  EQ	R8	R6	R8
      0x78220015,  //  0077  JMPF	R8	#008E
      0xB8222E00,  //  0078  GETNGBL	R8	K23
      0x8C201118,  //  0079  GETMET	R8	R8	K24
      0x5828001B,  //  007A  LDCONST	R10	K27
      0x502C0200,  //  007B  LDBOOL	R11	1	0
      0x7C200600,  //  007C  CALL	R8	3
      0x9420111C,  //  007D  GETIDX	R8	R8	K28
      0x9420111D,  //  007E  GETIDX	R8	R8	K29
      0x8C240F11,  //  007F  GETMET	R9	R7	K17
      0x5C2C1000,  //  0080  MOVE	R11	R8
      0x5830001E,  //  0081  LDCONST	R12	K30
      0x7C240600,  //  0082  CALL	R9	3
      0x24281305,  //  0083  GT	R10	R9	K5
      0x782A0002,  //  0084  JMPF	R10	#0088
      0x04281308,  //  0085  SUB	R10	R9	K8
      0x402A0A0A,  //  0086  CONNECT	R10	K5	R10
      0x9420100A,  //  0087  GETIDX	R8	R8	R10
      0x8C280706,  //  0088  GETMET	R10	R3	K6
      0x88300916,  //  0089  GETMBR	R12	R4	K22
      0x5C341000,  //  008A  MOVE	R13	R8
      0x7C280600,  //  008B  CALL	R10	3
      0x80041400,  //  008C  RET	1	R10
      0x70020025,  //  008D  JMP		#00B4
      0x5422000E,  //  008E  LDINT	R8	15
      0x1C200C08,  //  008F  EQ	R8	R6	R8
      0x7822000B,  //  0090  JMPF	R8	#009D
      0x8C200706,  //  0091  GETMET	R8	R3	K6
      0x88280916,  //  0092  GETMBR	R10	R4	K22
      0xB82E2E00,  //  0093  GETNGBL	R11	K23
      0x8C2C171F,  //  0094  GETMET	R11	R11	K31
      0x7C2C0200,  //  0095  CALL	R11	1
      0x8C2C1711,  //  0096  GETMET	R11	R11	K17
      0x58340020,  //  0097  LDCONST	R13	K32
      0x58380021,  //  0098  LDCONST	R14	K33
      0x7C2C0600,  //  0099  CALL	R11	3
      0x7C200600,  //  009A  CALL	R8	3
      0x80041000,  //  009B  RET	1	R8
      0x70020016,  //  009C  JMP		#00B4
      0x54220010,  //  009D  LDINT	R8	17
      0x1C200C08,  //  009E  EQ	R8	R6	R8
      0x78220005,  //  009F  JMPF	R8	#00A6
      0x8C200706,  //  00A0  GETMET	R8	R3	K6
      0x88280922,  //  00A1  GETMBR	R10	R4	K34
      0x582C0008,  //  00A2  LDCONST	R11	K8
      0x7C200600,  //  00A3  CALL	R8	3
      0x80041000,  //  00A4  RET	1	R8
      0x7002000D,  //  00A5  JMP		#00B4
      0x54220011,  //  00A6  LDINT	R8	18
      0x1C200C08,  //  00A7  EQ	R8	R6	R8
      0x7822000A,  //  00A8  JMPF	R8	#00B4
      0x8C200706,  //  00A9  GETMET	R8	R3	K6
      0x88280916,  //  00AA  GETMBR	R10	R4	K22
      0xB82E2E00,  //  00AB  GETNGBL	R11	K23
      0x8C2C171F,  //  00AC  GETMET	R11	R11	K31
      0x7C2C0200,  //  00AD  CALL	R11	1
      0x8C2C1711,  //  00AE  GETMET	R11	R11	K17
      0x58340020,  //  00AF  LDCONST	R13	K32
      0x58380021,  //  00B0  LDCONST	R14	K33
      0x7C2C0600,  //  00B1  CALL	R11	3
      0x7C200600,  //  00B2  CALL	R8	3
      0x80041000,  //  00B3  RET	1	R8
      0x601C0003,  //  00B4  GETGBL	R7	G3
      0x5C200000,  //  00B5  MOVE	R8	R0
      0x7C1C0200,  //  00B6  CALL	R7	1
      0x8C1C0F23,  //  00B7  GETMET	R7	R7	K35
      0x5C240200,  //  00B8  MOVE	R9	R1
      0x5C280400,  //  00B9  MOVE	R10	R2
      0x5C2C0600,  //  00BA  MOVE	R11	R3
      0x7C1C0800,  //  00BB  CALL	R7	4
      0x80040E00,  //  00BC  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: append_state_json
********************************************************************/
be_local_closure(Matter_Plugin_Device_append_state_json,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        12,                          /* nstack */
        2,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 3),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 6]) {     /* constants */
        /* K0   */  be_nested_str_weak(introspect),
        /* K1   */  be_nested_str_weak(json),
        /* K2   */  be_nested_str_weak(get),
        /* K3   */  be_nested_str_weak(bool),
        /* K4   */  be_nested_str_weak(_X2C_X22_X25s_X22_X3A_X25s),
        /* K5   */  be_nested_str_weak(dump),
        }),
        be_str_weak(_stats_json_inner),
        &be_const_str_solidified,
        ( &(const binstruction[31]) {  /* code */
          0xA40A0000,  //  0000  IMPORT	R2	K0
          0xA40E0200,  //  0001  IMPORT	R3	K1
          0x4C100000,  //  0002  LDNIL	R4
          0x8C140502,  //  0003  GETMET	R5	R2	K2
          0x681C0000,  //  0004  GETUPV	R7	U0
          0x5C200000,  //  0005  MOVE	R8	R0
          0x7C140600,  //  0006  CALL	R5	3
          0x5C100A00,  //  0007  MOVE	R4	R5
          0x4C180000,  //  0008  LDNIL	R6
          0x20140A06,  //  0009  NE	R5	R5	R6
          0x78160012,  //  000A  JMPF	R5	#001E
          0x60140004,  //  000B  GETGBL	R5	G4
          0x5C180800,  //  000C  MOVE	R6	R4
          0x7C140200,  //  000D  CALL	R5	1
          0x1C140B03,  //  000E  EQ	R5	R5	K3
          0x78160003,  //  000F  JMPF	R5	#0014
          0x60140009,  //  0010  GETGBL	R5	G9
          0x5C180800,  //  0011  MOVE	R6	R4
          0x7C140200,  //  0012  CALL	R5	1
          0x5C100A00,  //  0013  MOVE	R4	R5
          0x60180018,  //  0014  GETGBL	R6	G24
          0x581C0004,  //  0015  LDCONST	R7	K4
          0x5C200200,  //  0016  MOVE	R8	R1
          0x8C240705,  //  0017  GETMET	R9	R3	K5
          0x5C2C0800,  //  0018  MOVE	R11	R4
          0x7C240400,  //  0019  CALL	R9	2
          0x7C180600,  //  001A  CALL	R6	3
          0x68140001,  //  001B  GETUPV	R5	U1
          0x00140A06,  //  001C  ADD	R5	R5	R6
          0x6C140001,  //  001D  SETUPV	R5	U1
          0x80000000,  //  001E  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_nested_str_weak(),
    /* K3   */  be_nested_str_weak(get),
    /* K4   */  be_nested_str_weak(JSON_NAME),
    /* K5   */  be_nested_str_weak(contains),
    /* K6   */  be_nested_str_weak(shadow_value),
    /* K7   */  be_nested_str_weak(dump),
    /* K8   */  be_nested_str_weak(null),
    /* K9   */  be_nested_str_weak(_X2C_X22_X25s_X22_X3A_X25s),
    /* K10  */  be_nested_str_weak(shadow_onoff),
    /* K11  */  be_nested_str_weak(Power),
    /* K12  */  be_nested_str_weak(shadow_bri),
    /* K13  */  be_nested_str_weak(Bri),
    /* K14  */  be_nested_str_weak(shadow_ct),
    /* K15  */  be_nested_str_weak(CT),
    /* K16  */  be_nested_str_weak(shadow_hue),
    /* K17  */  be_nested_str_weak(Hue),
    /* K18  */  be_nested_str_weak(shadow_sat),
    /* K19  */  be_nested_str_weak(Sat),
    /* K20  */  be_nested_str_weak(shadow_shutter_pos),
    /* K21  */  be_nested_str_weak(ShutterPos),
    /* K22  */  be_nested_str_weak(shadow_shutter_target),
    /* K23  */  be_nested_str_weak(ShutterTarget),
    /* K24  */  be_nested_str_weak(shadow_shutter_tilt),
    /* K25  */  be_nested_str_weak(ShutterTilt),
    /* K26  */  be_nested_str_weak(shadow_contact),
    /* K27  */  be_nested_str_weak(Contact),
    /* K28  */  be_nested_str_weak(shadow_occupancy),
    /* K29  */  be_nested_str_weak(Occupancy),
    }),
    be_str_weak(append_state_json),
    &be_const_str_solidified,
    ( &(const binstruction[71]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x580C0002,  //  0002  LDCONST	R3	K2
      0x84100000,  //  0003  CLOSURE	R4	P0
      0x8C140303,  //  0004  GETMET	R5	R1	K3
      0x5C1C0000,  //  0005  MOVE	R7	R0
      0x58200004,  //  0006  LDCONST	R8	K4
      0x7C140600,  //  0007  CALL	R5	3
      0x78160013,  //  0008  JMPF	R5	#001D
      0x8C180305,  //  0009  GETMET	R6	R1	K5
      0x5C200000,  //  000A  MOVE	R8	R0
      0x58240006,  //  000B  LDCONST	R9	K6
      0x7C180600,  //  000C  CALL	R6	3
      0x781A000E,  //  000D  JMPF	R6	#001D
      0x88180106,  //  000E  GETMBR	R6	R0	K6
      0x4C1C0000,  //  000F  LDNIL	R7
      0x20180C07,  //  0010  NE	R6	R6	R7
      0x781A0003,  //  0011  JMPF	R6	#0016
      0x8C180507,  //  0012  GETMET	R6	R2	K7
      0x88200106,  //  0013  GETMBR	R8	R0	K6
      0x7C180400,  //  0014  CALL	R6	2
      0x70020000,  //  0015  JMP		#0017
      0x58180008,  //  0016  LDCONST	R6	K8
      0x601C0018,  //  0017  GETGBL	R7	G24
      0x58200009,  //  0018  LDCONST	R8	K9
      0x5C240A00,  //  0019  MOVE	R9	R5
      0x5C280C00,  //  001A  MOVE	R10	R6
      0x7C1C0600,  //  001B  CALL	R7	3
      0x000C0607,  //  001C  ADD	R3	R3	R7
      0x5C180800,  //  001D  MOVE	R6	R4
      0x581C000A,  //  001E  LDCONST	R7	K10
      0x5820000B,  //  001F  LDCONST	R8	K11
      0x7C180400,  //  0020  CALL	R6	2
      0x5C180800,  //  0021  MOVE	R6	R4
      0x581C000C,  //  0022  LDCONST	R7	K12
      0x5820000D,  //  0023  LDCONST	R8	K13
      0x7C180400,  //  0024  CALL	R6	2
      0x5C180800,  //  0025  MOVE	R6	R4
      0x581C000E,  //  0026  LDCONST	R7	K14
      0x5820000F,  //  0027  LDCONST	R8	K15
      0x7C180400,  //  0028  CALL	R6	2
      0x5C180800,  //  0029  MOVE	R6	R4
      0x581C0010,  //  002A  LDCONST	R7	K16
      0x58200011,  //  002B  LDCONST	R8	K17
      0x7C180400,  //  002C  CALL	R6	2
      0x5C180800,  //  002D  MOVE	R6	R4
      0x581C0012,  //  002E  LDCONST	R7	K18
      0x58200013,  //  002F  LDCONST	R8	K19
      0x7C180400,  //  0030  CALL	R6	2
      0x5C180800,  //  0031  MOVE	R6	R4
      0x581C0014,  //  0032  LDCONST	R7	K20
      0x58200015,  //  0033  LDCONST	R8	K21
      0x7C180400,  //  0034  CALL	R6	2
      0x5C180800,  //  0035  MOVE	R6	R4
      0x581C0016,  //  0036  LDCONST	R7	K22
      0x58200017,  //  0037  LDCONST	R8	K23
      0x7C180400,  //  0038  CALL	R6	2
      0x5C180800,  //  0039  MOVE	R6	R4
      0x581C0018,  //  003A  LDCONST	R7	K24
      0x58200019,  //  003B  LDCONST	R8	K25
      0x7C180400,  //  003C  CALL	R6	2
      0x5C180800,  //  003D  MOVE	R6	R4
      0x581C001A,  //  003E  LDCONST	R7	K26
      0x5820001B,  //  003F  LDCONST	R8	K27
      0x7C180400,  //  0040  CALL	R6	2
      0x5C180800,  //  0041  MOVE	R6	R4
      0x581C001C,  //  0042  LDCONST	R7	K28
      0x5820001D,  //  0043  LDCONST	R8	K29
      0x7C180400,  //  0044  CALL	R6	2
      0xA0000000,  //  0045  CLOSE	R0
      0x80040600,  //  0046  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Plugin_Device_invoke_request,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(TLV),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(command),
    /* K4   */  be_const_int(3),
    /* K5   */  be_const_int(0),
    /* K6   */  be_const_int(1),
    /* K7   */  be_nested_str_weak(Matter_TLV_struct),
    /* K8   */  be_nested_str_weak(add_TLV),
    /* K9   */  be_nested_str_weak(U2),
    /* K10  */  be_nested_str_weak(invoke_request),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[51]) {  /* code */
      0xB8120000,  //  0000  GETNGBL	R4	K0
      0x88100901,  //  0001  GETMBR	R4	R4	K1
      0x88140702,  //  0002  GETMBR	R5	R3	K2
      0x88180703,  //  0003  GETMBR	R6	R3	K3
      0x1C1C0B04,  //  0004  EQ	R7	R5	K4
      0x781E0016,  //  0005  JMPF	R7	#001D
      0x1C1C0D05,  //  0006  EQ	R7	R6	K5
      0x781E0002,  //  0007  JMPF	R7	#000B
      0x501C0200,  //  0008  LDBOOL	R7	1	0
      0x80040E00,  //  0009  RET	1	R7
      0x70020010,  //  000A  JMP		#001C
      0x1C1C0D06,  //  000B  EQ	R7	R6	K6
      0x781E0009,  //  000C  JMPF	R7	#0017
      0x8C1C0907,  //  000D  GETMET	R7	R4	K7
      0x7C1C0200,  //  000E  CALL	R7	1
      0x8C200F08,  //  000F  GETMET	R8	R7	K8
      0x58280005,  //  0010  LDCONST	R10	K5
      0x882C0909,  //  0011  GETMBR	R11	R4	K9
      0x58300005,  //  0012  LDCONST	R12	K5
      0x7C200800,  //  0013  CALL	R8	4
      0x900E0705,  //  0014  SETMBR	R3	K3	K5
      0x80040E00,  //  0015  RET	1	R7
      0x70020004,  //  0016  JMP		#001C
      0x541E003F,  //  0017  LDINT	R7	64
      0x1C1C0C07,  //  0018  EQ	R7	R6	R7
      0x781E0001,  //  0019  JMPF	R7	#001C
      0x501C0200,  //  001A  LDBOOL	R7	1	0
      0x80040E00,  //  001B  RET	1	R7
      0x70020014,  //  001C  JMP		#0032
      0x541E0003,  //  001D  LDINT	R7	4
      0x1C1C0A07,  //  001E  EQ	R7	R5	R7
      0x781E0002,  //  001F  JMPF	R7	#0023
      0x501C0200,  //  0020  LDBOOL	R7	1	0
      0x80040E00,  //  0021  RET	1	R7
      0x7002000E,  //  0022  JMP		#0032
      0x541E0004,  //  0023  LDINT	R7	5
      0x1C1C0A07,  //  0024  EQ	R7	R5	R7
      0x781E0002,  //  0025  JMPF	R7	#0029
      0x501C0200,  //  0026  LDBOOL	R7	1	0
      0x80040E00,  //  0027  RET	1	R7
      0x70020008,  //  0028  JMP		#0032
      0x601C0003,  //  0029  GETGBL	R7	G3
      0x5C200000,  //  002A  MOVE	R8	R0
      0x7C1C0200,  //  002B  CALL	R7	1
      0x8C1C0F0A,  //  002C  GETMET	R7	R7	K10
      0x5C240200,  //  002D  MOVE	R9	R1
      0x5C280400,  //  002E  MOVE	R10	R2
      0x5C2C0600,  //  002F  MOVE	R11	R3
      0x7C1C0800,  //  0030  CALL	R7	4
      0x80040E00,  //  0031  RET	1	R7
      0x80000000,  //  0032  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Device
********************************************************************/
extern const bclass be_class_Matter_Plugin;
be_local_class(Matter_Plugin_Device,
    0,
    &be_class_Matter_Plugin,
    be_nested_map(6,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(read_attribute, -1), be_const_closure(Matter_Plugin_Device_read_attribute_closure) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Plugin_Device_invoke_request_closure) },
        { be_const_key_weak(TYPES, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(19, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(append_state_json, -1), be_const_closure(Matter_Plugin_Device_append_state_json_closure) },
        { be_const_key_weak(CLUSTERS, 3), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(5, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(12,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(4),
        be_const_int(5),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(29, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(10,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(2),
        be_const_int(3),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(57, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(12,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(3),
        be_const_int(5),
        be_const_int(10),
        be_const_int(15),
        be_const_int(17),
        be_const_int(18),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(3, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(8,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(1),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
        { be_const_key_int(4, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(7,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(65528),
        be_const_int(65529),
        be_const_int(65530),
        be_const_int(65531),
        be_const_int(65532),
        be_const_int(65533),
    }))    ) } )) },
    }))    ) } )) },
        { be_const_key_weak(NON_BRIDGE_VENDOR, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(2,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(4631),
        be_const_int(4993),
    }))    ) } )) },
    })),
    be_str_weak(Matter_Plugin_Device)
);
/*******************************************************************/

void be_load_Matter_Plugin_Device_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Plugin_Device);
    be_setglobal(vm, "Matter_Plugin_Device");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
