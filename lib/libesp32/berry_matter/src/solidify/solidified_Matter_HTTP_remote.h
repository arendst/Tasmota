/* Solidification of Matter_HTTP_remote.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_HTTP_remote' ktab size: 83, total: 161 (saved 624 bytes)
static const bvalue be_ktab_class_Matter_HTTP_remote[83] = {
  /* K0   */  be_nested_str_weak(probe_update_time_map),
  /* K1   */  be_nested_str_weak(contains),
  /* K2   */  be_nested_str_weak(probe_next_timestamp_map),
  /* K3   */  be_nested_str_weak(matter),
  /* K4   */  be_nested_str_weak(jitter),
  /* K5   */  be_nested_str_weak(add_async_cb),
  /* K6   */  be_const_int(0),
  /* K7   */  be_nested_str_weak(async_cb_map),
  /* K8   */  be_nested_str_weak(keys),
  /* K9   */  be_nested_str_weak(current_cmd),
  /* K10  */  be_nested_str_weak(stop_iteration),
  /* K11  */  be_nested_str_weak(info),
  /* K12  */  be_nested_str_weak(find),
  /* K13  */  be_nested_str_weak(DeviceName),
  /* K14  */  be_nested_str_weak(Tasmota),
  /* K15  */  be_nested_str_weak(name),
  /* K16  */  be_nested_str_weak(remove),
  /* K17  */  be_nested_str_weak(log),
  /* K18  */  be_nested_str_weak(MTR_X3A_X20update_X20_X27_X25s_X27_X20name_X3D_X27_X25s_X27),
  /* K19  */  be_nested_str_weak(addr),
  /* K20  */  be_const_int(3),
  /* K21  */  be_nested_str_weak(change_schedule),
  /* K22  */  be_nested_str_weak(UPDATE_CMD0),
  /* K23  */  be_nested_str_weak(UPDATE_TIME2),
  /* K24  */  be_const_int(2),
  /* K25  */  be_nested_str_weak(Version),
  /* K26  */  be_nested_str_weak(Hardware),
  /* K27  */  be_nested_str_weak(version),
  /* K28  */  be_nested_str_weak(MTR_X3A_X20update_X20_X27_X25s_X27_X20version_X3D_X27_X25s_X27),
  /* K29  */  be_nested_str_weak(hardware),
  /* K30  */  be_nested_str_weak(MTR_X3A_X20update_X20_X27_X25s_X27_X20hardware_X3D_X27_X25s_X27),
  /* K31  */  be_nested_str_weak(UPDATE_CMD2),
  /* K32  */  be_nested_str_weak(Mac),
  /* K33  */  be_nested_str_weak(mac),
  /* K34  */  be_nested_str_weak(MTR_X3A_X20update_X20_X27_X25s_X27_X20mac_X3D_X27_X25s_X27),
  /* K35  */  be_nested_str_weak(UPDATE_CMD5),
  /* K36  */  be_nested_str_weak(info_changed),
  /* K37  */  be_nested_str_weak(reachable),
  /* K38  */  be_nested_str_weak(reachable_utc),
  /* K39  */  be_nested_str_weak(tasmota),
  /* K40  */  be_nested_str_weak(rtc_utc),
  /* K41  */  be_nested_str_weak(payload),
  /* K42  */  be_nested_str_weak(nil),
  /* K43  */  be_nested_str_weak(_X2E_X2E_X2E),
  /* K44  */  be_nested_str_weak(MTR_X3A_X20HTTP_X20async_X2Dresp_X20in_X20_X25i_X20ms_X20from_X20_X25s_X3A_X20_X5B_X25i_X5D_X20_X27_X25s_X27),
  /* K45  */  be_nested_str_weak(millis),
  /* K46  */  be_nested_str_weak(time_start),
  /* K47  */  be_nested_str_weak(dispatch_cb),
  /* K48  */  be_nested_str_weak(http_status),
  /* K49  */  be_nested_str_weak(string),
  /* K50  */  be_nested_str_weak(webserver),
  /* K51  */  be_nested_str_weak(wifi),
  /* K52  */  be_nested_str_weak(up),
  /* K53  */  be_nested_str_weak(eth),
  /* K54  */  be_nested_str_weak(_X2Fcm_X3Fcmnd_X3D),
  /* K55  */  be_nested_str_weak(tr),
  /* K56  */  be_nested_str_weak(_X20),
  /* K57  */  be_nested_str_weak(_X2B),
  /* K58  */  be_nested_str_weak(MTR_X3A_X20HTTP_X20async_X20request_X20_X27http_X3A_X2F_X2F_X25s_X3A_X25i_X25s_X27),
  /* K59  */  be_nested_str_weak(port),
  /* K60  */  be_nested_str_weak(begin),
  /* K61  */  be_nested_str_weak(device),
  /* K62  */  be_nested_str_weak(save_param),
  /* K63  */  be_nested_str_weak(seconds_to_dhm),
  /* K64  */  be_nested_str_weak(init),
  /* K65  */  be_nested_str_weak(add_schedule),
  /* K66  */  be_nested_str_weak(UPDATE_TIME),
  /* K67  */  be_nested_str_weak(device_is_alive),
  /* K68  */  be_nested_str_weak(json),
  /* K69  */  be_nested_str_weak(load),
  /* K70  */  be_nested_str_weak(STATUS_PREFIX),
  /* K71  */  be_const_int(1),
  /* K72  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X20failed_X20to_X20parse_X20JSON_X20response_X20payload_X3D_X25s),
  /* K73  */  be_nested_str_weak(MTR_X3A_X20HTTP_X20sync_X20request_X20_X27http_X3A_X2F_X2F_X25s_X3A_X25i_X25s_X27),
  /* K74  */  be_nested_str_weak(begin_sync),
  /* K75  */  be_nested_str_weak(MTR_X3A_X20HTTP_X20sync_X2Dresp_X20_X20in_X20_X25i_X20ms_X20from_X20_X25s_X3A_X20_X5B_X25i_X5D_X20_X27_X25s_X27),
  /* K76  */  be_nested_str_weak(time_reached),
  /* K77  */  be_nested_str_weak(tcp_connected),
  /* K78  */  be_nested_str_weak(probe_async),
  /* K79  */  be_nested_str_weak(MTR_X3A_X20HTTP_X20failed),
  /* K80  */  be_nested_str_weak(MTR_X3A_X20HTTP_X20timeout_X20http_status_X3D_X25i_X20phase_X3D_X25i_X20tcp_status_X3D_X25i_X20size_payload_X3D_X25i),
  /* K81  */  be_nested_str_weak(phase),
  /* K82  */  be_nested_str_weak(status),
};


extern const bclass be_class_Matter_HTTP_remote;

/********************************************************************
** Solidified function: add_schedule
********************************************************************/
be_local_closure(class_Matter_HTTP_remote_add_schedule,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_remote,     /* shared constants */
    be_str_weak(add_schedule),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x8C100901,  //  0001  GETMET	R4	R4	K1
      0x5C180200,  //  0002  MOVE	R6	R1
      0x7C100400,  //  0003  CALL	R4	2
      0x78120003,  //  0004  JMPF	R4	#0009
      0x88100100,  //  0005  GETMBR	R4	R0	K0
      0x94100801,  //  0006  GETIDX	R4	R4	R1
      0x14100404,  //  0007  LT	R4	R2	R4
      0x78120007,  //  0008  JMPF	R4	#0011
      0x88100100,  //  0009  GETMBR	R4	R0	K0
      0x98100202,  //  000A  SETIDX	R4	R1	R2
      0x88100102,  //  000B  GETMBR	R4	R0	K2
      0xB8160600,  //  000C  GETNGBL	R5	K3
      0x8C140B04,  //  000D  GETMET	R5	R5	K4
      0x5C1C0400,  //  000E  MOVE	R7	R2
      0x7C140400,  //  000F  CALL	R5	2
      0x98100205,  //  0010  SETIDX	R4	R1	R5
      0x4C100000,  //  0011  LDNIL	R4
      0x20100604,  //  0012  NE	R4	R3	R4
      0x78120003,  //  0013  JMPF	R4	#0018
      0x8C100105,  //  0014  GETMET	R4	R0	K5
      0x5C180600,  //  0015  MOVE	R6	R3
      0x5C1C0200,  //  0016  MOVE	R7	R1
      0x7C100600,  //  0017  CALL	R4	3
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: dispatch_cb
********************************************************************/
be_local_closure(class_Matter_HTTP_remote_dispatch_cb,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_remote,     /* shared constants */
    be_str_weak(dispatch_cb),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x580C0006,  //  0000  LDCONST	R3	K6
      0x60100010,  //  0001  GETGBL	R4	G16
      0x88140107,  //  0002  GETMBR	R5	R0	K7
      0x8C140B08,  //  0003  GETMET	R5	R5	K8
      0x7C140200,  //  0004  CALL	R5	1
      0x7C100200,  //  0005  CALL	R4	1
      0xA802000F,  //  0006  EXBLK	0	#0017
      0x5C140800,  //  0007  MOVE	R5	R4
      0x7C140000,  //  0008  CALL	R5	0
      0x88180107,  //  0009  GETMBR	R6	R0	K7
      0x94180C05,  //  000A  GETIDX	R6	R6	R5
      0x881C0109,  //  000B  GETMBR	R7	R0	K9
      0x1C1C0C07,  //  000C  EQ	R7	R6	R7
      0x741E0002,  //  000D  JMPT	R7	#0011
      0x4C1C0000,  //  000E  LDNIL	R7
      0x1C1C0C07,  //  000F  EQ	R7	R6	R7
      0x781E0004,  //  0010  JMPF	R7	#0016
      0x5C1C0A00,  //  0011  MOVE	R7	R5
      0x5C200200,  //  0012  MOVE	R8	R1
      0x5C240400,  //  0013  MOVE	R9	R2
      0x88280109,  //  0014  GETMBR	R10	R0	K9
      0x7C1C0600,  //  0015  CALL	R7	3
      0x7001FFEF,  //  0016  JMP		#0007
      0x5810000A,  //  0017  LDCONST	R4	K10
      0xAC100200,  //  0018  CATCH	R4	1	0
      0xB0080000,  //  0019  RAISE	2	R0	R0
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_info
********************************************************************/
be_local_closure(class_Matter_HTTP_remote_get_info,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_remote,     /* shared constants */
    be_str_weak(get_info),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x8804010B,  //  0000  GETMBR	R1	R0	K11
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_status_http
********************************************************************/
be_local_closure(class_Matter_HTTP_remote_parse_status_http,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_remote,     /* shared constants */
    be_str_weak(parse_status_http),
    &be_const_str_solidified,
    ( &(const binstruction[141]) {  /* code */
      0x500C0000,  //  0000  LDBOOL	R3	0	0
      0x1C100506,  //  0001  EQ	R4	R2	K6
      0x78120023,  //  0002  JMPF	R4	#0027
      0x8C10030C,  //  0003  GETMET	R4	R1	K12
      0x5818000D,  //  0004  LDCONST	R6	K13
      0x7C100400,  //  0005  CALL	R4	2
      0x1C14090E,  //  0006  EQ	R5	R4	K14
      0x78160000,  //  0007  JMPF	R5	#0009
      0x4C100000,  //  0008  LDNIL	R4
      0x8814010B,  //  0009  GETMBR	R5	R0	K11
      0x8C140B0C,  //  000A  GETMET	R5	R5	K12
      0x581C000F,  //  000B  LDCONST	R7	K15
      0x7C140400,  //  000C  CALL	R5	2
      0x20140A04,  //  000D  NE	R5	R5	R4
      0x78160012,  //  000E  JMPF	R5	#0022
      0x4C140000,  //  000F  LDNIL	R5
      0x20140805,  //  0010  NE	R5	R4	R5
      0x78160002,  //  0011  JMPF	R5	#0015
      0x8814010B,  //  0012  GETMBR	R5	R0	K11
      0x98161E04,  //  0013  SETIDX	R5	K15	R4
      0x70020003,  //  0014  JMP		#0019
      0x8814010B,  //  0015  GETMBR	R5	R0	K11
      0x8C140B10,  //  0016  GETMET	R5	R5	K16
      0x581C000F,  //  0017  LDCONST	R7	K15
      0x7C140400,  //  0018  CALL	R5	2
      0xB8162200,  //  0019  GETNGBL	R5	K17
      0x60180018,  //  001A  GETGBL	R6	G24
      0x581C0012,  //  001B  LDCONST	R7	K18
      0x88200113,  //  001C  GETMBR	R8	R0	K19
      0x5C240800,  //  001D  MOVE	R9	R4
      0x7C180600,  //  001E  CALL	R6	3
      0x581C0014,  //  001F  LDCONST	R7	K20
      0x7C140400,  //  0020  CALL	R5	2
      0x500C0200,  //  0021  LDBOOL	R3	1	0
      0x8C140115,  //  0022  GETMET	R5	R0	K21
      0x881C0116,  //  0023  GETMBR	R7	R0	K22
      0x88200117,  //  0024  GETMBR	R8	R0	K23
      0x7C140600,  //  0025  CALL	R5	3
      0x70020061,  //  0026  JMP		#0089
      0x1C100518,  //  0027  EQ	R4	R2	K24
      0x7812003C,  //  0028  JMPF	R4	#0066
      0x8C10030C,  //  0029  GETMET	R4	R1	K12
      0x58180019,  //  002A  LDCONST	R6	K25
      0x7C100400,  //  002B  CALL	R4	2
      0x8C14030C,  //  002C  GETMET	R5	R1	K12
      0x581C001A,  //  002D  LDCONST	R7	K26
      0x7C140400,  //  002E  CALL	R5	2
      0x8818010B,  //  002F  GETMBR	R6	R0	K11
      0x8C180D0C,  //  0030  GETMET	R6	R6	K12
      0x5820001B,  //  0031  LDCONST	R8	K27
      0x7C180400,  //  0032  CALL	R6	2
      0x20180C04,  //  0033  NE	R6	R6	R4
      0x781A0012,  //  0034  JMPF	R6	#0048
      0x4C180000,  //  0035  LDNIL	R6
      0x20180806,  //  0036  NE	R6	R4	R6
      0x781A0002,  //  0037  JMPF	R6	#003B
      0x8818010B,  //  0038  GETMBR	R6	R0	K11
      0x981A3604,  //  0039  SETIDX	R6	K27	R4
      0x70020003,  //  003A  JMP		#003F
      0x8818010B,  //  003B  GETMBR	R6	R0	K11
      0x8C180D10,  //  003C  GETMET	R6	R6	K16
      0x5820001B,  //  003D  LDCONST	R8	K27
      0x7C180400,  //  003E  CALL	R6	2
      0xB81A2200,  //  003F  GETNGBL	R6	K17
      0x601C0018,  //  0040  GETGBL	R7	G24
      0x5820001C,  //  0041  LDCONST	R8	K28
      0x88240113,  //  0042  GETMBR	R9	R0	K19
      0x5C280800,  //  0043  MOVE	R10	R4
      0x7C1C0600,  //  0044  CALL	R7	3
      0x58200014,  //  0045  LDCONST	R8	K20
      0x7C180400,  //  0046  CALL	R6	2
      0x500C0200,  //  0047  LDBOOL	R3	1	0
      0x8818010B,  //  0048  GETMBR	R6	R0	K11
      0x8C180D0C,  //  0049  GETMET	R6	R6	K12
      0x5820001D,  //  004A  LDCONST	R8	K29
      0x7C180400,  //  004B  CALL	R6	2
      0x20180C05,  //  004C  NE	R6	R6	R5
      0x781A0012,  //  004D  JMPF	R6	#0061
      0x4C180000,  //  004E  LDNIL	R6
      0x20180A06,  //  004F  NE	R6	R5	R6
      0x781A0002,  //  0050  JMPF	R6	#0054
      0x8818010B,  //  0051  GETMBR	R6	R0	K11
      0x981A3A05,  //  0052  SETIDX	R6	K29	R5
      0x70020003,  //  0053  JMP		#0058
      0x8818010B,  //  0054  GETMBR	R6	R0	K11
      0x8C180D10,  //  0055  GETMET	R6	R6	K16
      0x5820001D,  //  0056  LDCONST	R8	K29
      0x7C180400,  //  0057  CALL	R6	2
      0xB81A2200,  //  0058  GETNGBL	R6	K17
      0x601C0018,  //  0059  GETGBL	R7	G24
      0x5820001E,  //  005A  LDCONST	R8	K30
      0x88240113,  //  005B  GETMBR	R9	R0	K19
      0x5C280A00,  //  005C  MOVE	R10	R5
      0x7C1C0600,  //  005D  CALL	R7	3
      0x58200014,  //  005E  LDCONST	R8	K20
      0x7C180400,  //  005F  CALL	R6	2
      0x500C0200,  //  0060  LDBOOL	R3	1	0
      0x8C180115,  //  0061  GETMET	R6	R0	K21
      0x8820011F,  //  0062  GETMBR	R8	R0	K31
      0x88240117,  //  0063  GETMBR	R9	R0	K23
      0x7C180600,  //  0064  CALL	R6	3
      0x70020022,  //  0065  JMP		#0089
      0x54120004,  //  0066  LDINT	R4	5
      0x1C100404,  //  0067  EQ	R4	R2	R4
      0x7812001F,  //  0068  JMPF	R4	#0089
      0x8C10030C,  //  0069  GETMET	R4	R1	K12
      0x58180020,  //  006A  LDCONST	R6	K32
      0x7C100400,  //  006B  CALL	R4	2
      0x8814010B,  //  006C  GETMBR	R5	R0	K11
      0x8C140B0C,  //  006D  GETMET	R5	R5	K12
      0x581C0021,  //  006E  LDCONST	R7	K33
      0x7C140400,  //  006F  CALL	R5	2
      0x20140A04,  //  0070  NE	R5	R5	R4
      0x78160012,  //  0071  JMPF	R5	#0085
      0x4C140000,  //  0072  LDNIL	R5
      0x20140805,  //  0073  NE	R5	R4	R5
      0x78160002,  //  0074  JMPF	R5	#0078
      0x8814010B,  //  0075  GETMBR	R5	R0	K11
      0x98164204,  //  0076  SETIDX	R5	K33	R4
      0x70020003,  //  0077  JMP		#007C
      0x8814010B,  //  0078  GETMBR	R5	R0	K11
      0x8C140B10,  //  0079  GETMET	R5	R5	K16
      0x581C0021,  //  007A  LDCONST	R7	K33
      0x7C140400,  //  007B  CALL	R5	2
      0xB8162200,  //  007C  GETNGBL	R5	K17
      0x60180018,  //  007D  GETGBL	R6	G24
      0x581C0022,  //  007E  LDCONST	R7	K34
      0x88200113,  //  007F  GETMBR	R8	R0	K19
      0x5C240800,  //  0080  MOVE	R9	R4
      0x7C180600,  //  0081  CALL	R6	3
      0x581C0014,  //  0082  LDCONST	R7	K20
      0x7C140400,  //  0083  CALL	R5	2
      0x500C0200,  //  0084  LDBOOL	R3	1	0
      0x8C140115,  //  0085  GETMET	R5	R0	K21
      0x881C0123,  //  0086  GETMBR	R7	R0	K35
      0x88200117,  //  0087  GETMBR	R8	R0	K23
      0x7C140600,  //  0088  CALL	R5	3
      0x780E0001,  //  0089  JMPF	R3	#008C
      0x8C100124,  //  008A  GETMET	R4	R0	K36
      0x7C100200,  //  008B  CALL	R4	1
      0x80000000,  //  008C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_info
********************************************************************/
be_local_closure(class_Matter_HTTP_remote_set_info,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_remote,     /* shared constants */
    be_str_weak(set_info),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90021601,  //  0000  SETMBR	R0	K11	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: device_is_alive
********************************************************************/
be_local_closure(class_Matter_HTTP_remote_device_is_alive,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_remote,     /* shared constants */
    be_str_weak(device_is_alive),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x78060006,  //  0000  JMPF	R1	#0008
      0x50080200,  //  0001  LDBOOL	R2	1	0
      0x90024A02,  //  0002  SETMBR	R0	K37	R2
      0xB80A4E00,  //  0003  GETNGBL	R2	K39
      0x8C080528,  //  0004  GETMET	R2	R2	K40
      0x7C080200,  //  0005  CALL	R2	1
      0x90024C02,  //  0006  SETMBR	R0	K38	R2
      0x70020001,  //  0007  JMP		#000A
      0x50080000,  //  0008  LDBOOL	R2	0	0
      0x90024A02,  //  0009  SETMBR	R0	K37	R2
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_finished
********************************************************************/
be_local_closure(class_Matter_HTTP_remote_event_http_finished,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_remote,     /* shared constants */
    be_str_weak(event_http_finished),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060000,  //  0003  JMPF	R1	#0005
      0x80000200,  //  0004  RET	0
      0x88040129,  //  0005  GETMBR	R1	R0	K41
      0x4C080000,  //  0006  LDNIL	R2
      0x20040202,  //  0007  NE	R1	R1	R2
      0x78060001,  //  0008  JMPF	R1	#000B
      0x88040129,  //  0009  GETMBR	R1	R0	K41
      0x70020000,  //  000A  JMP		#000C
      0x5804002A,  //  000B  LDCONST	R1	K42
      0x6008000C,  //  000C  GETGBL	R2	G12
      0x5C0C0200,  //  000D  MOVE	R3	R1
      0x7C080200,  //  000E  CALL	R2	1
      0x540E001D,  //  000F  LDINT	R3	30
      0x24080403,  //  0010  GT	R2	R2	R3
      0x780A0004,  //  0011  JMPF	R2	#0017
      0x540A001C,  //  0012  LDINT	R2	29
      0x400A0C02,  //  0013  CONNECT	R2	K6	R2
      0x94080202,  //  0014  GETIDX	R2	R1	R2
      0x0008052B,  //  0015  ADD	R2	R2	K43
      0x5C040400,  //  0016  MOVE	R1	R2
      0xB80A2200,  //  0017  GETNGBL	R2	K17
      0x600C0018,  //  0018  GETGBL	R3	G24
      0x5810002C,  //  0019  LDCONST	R4	K44
      0xB8164E00,  //  001A  GETNGBL	R5	K39
      0x8C140B2D,  //  001B  GETMET	R5	R5	K45
      0x7C140200,  //  001C  CALL	R5	1
      0x8818012E,  //  001D  GETMBR	R6	R0	K46
      0x04140A06,  //  001E  SUB	R5	R5	R6
      0x88180113,  //  001F  GETMBR	R6	R0	K19
      0x601C000C,  //  0020  GETGBL	R7	G12
      0x88200129,  //  0021  GETMBR	R8	R0	K41
      0x7C1C0200,  //  0022  CALL	R7	1
      0x5C200200,  //  0023  MOVE	R8	R1
      0x7C0C0A00,  //  0024  CALL	R3	5
      0x58100014,  //  0025  LDCONST	R4	K20
      0x7C080400,  //  0026  CALL	R2	2
      0x8C08012F,  //  0027  GETMET	R2	R0	K47
      0x88100130,  //  0028  GETMBR	R4	R0	K48
      0x88140129,  //  0029  GETMBR	R5	R0	K41
      0x7C080600,  //  002A  CALL	R2	3
      0x80000000,  //  002B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: probe_async
********************************************************************/
be_local_closure(class_Matter_HTTP_remote_probe_async,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_remote,     /* shared constants */
    be_str_weak(probe_async),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0xA40A6200,  //  0000  IMPORT	R2	K49
      0xA40E6400,  //  0001  IMPORT	R3	K50
      0xB8124E00,  //  0002  GETNGBL	R4	K39
      0x8C100933,  //  0003  GETMET	R4	R4	K51
      0x7C100200,  //  0004  CALL	R4	1
      0x94100934,  //  0005  GETIDX	R4	R4	K52
      0x74120006,  //  0006  JMPT	R4	#000E
      0xB8124E00,  //  0007  GETNGBL	R4	K39
      0x8C100935,  //  0008  GETMET	R4	R4	K53
      0x7C100200,  //  0009  CALL	R4	1
      0x94100934,  //  000A  GETIDX	R4	R4	K52
      0x74120001,  //  000B  JMPT	R4	#000E
      0x4C100000,  //  000C  LDNIL	R4
      0x80040800,  //  000D  RET	1	R4
      0x90021201,  //  000E  SETMBR	R0	K9	R1
      0x8C100537,  //  000F  GETMET	R4	R2	K55
      0x5C180200,  //  0010  MOVE	R6	R1
      0x581C0038,  //  0011  LDCONST	R7	K56
      0x58200039,  //  0012  LDCONST	R8	K57
      0x7C100800,  //  0013  CALL	R4	4
      0x00126C04,  //  0014  ADD	R4	K54	R4
      0xB8162200,  //  0015  GETNGBL	R5	K17
      0x60180018,  //  0016  GETGBL	R6	G24
      0x581C003A,  //  0017  LDCONST	R7	K58
      0x88200113,  //  0018  GETMBR	R8	R0	K19
      0x8824013B,  //  0019  GETMBR	R9	R0	K59
      0x5C280800,  //  001A  MOVE	R10	R4
      0x7C180800,  //  001B  CALL	R6	4
      0x541E0003,  //  001C  LDINT	R7	4
      0x7C140400,  //  001D  CALL	R5	2
      0x8C14013C,  //  001E  GETMET	R5	R0	K60
      0x5C1C0800,  //  001F  MOVE	R7	R4
      0x7C140400,  //  0020  CALL	R5	2
      0x80000000,  //  0021  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: info_changed
********************************************************************/
be_local_closure(class_Matter_HTTP_remote_info_changed,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_remote,     /* shared constants */
    be_str_weak(info_changed),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8804013D,  //  0000  GETMBR	R1	R0	K61
      0x8C04033E,  //  0001  GETMET	R1	R1	K62
      0x7C040200,  //  0002  CALL	R1	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_last_seen
********************************************************************/
be_local_closure(class_Matter_HTTP_remote_web_last_seen,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_remote,     /* shared constants */
    be_str_weak(web_last_seen),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA4066400,  //  0000  IMPORT	R1	K50
      0x5409FFFE,  //  0001  LDINT	R2	-1
      0x880C0126,  //  0002  GETMBR	R3	R0	K38
      0x4C100000,  //  0003  LDNIL	R4
      0x200C0604,  //  0004  NE	R3	R3	R4
      0x780E0005,  //  0005  JMPF	R3	#000C
      0xB80E4E00,  //  0006  GETNGBL	R3	K39
      0x8C0C0728,  //  0007  GETMET	R3	R3	K40
      0x7C0C0200,  //  0008  CALL	R3	1
      0x88100126,  //  0009  GETMBR	R4	R0	K38
      0x040C0604,  //  000A  SUB	R3	R3	R4
      0x5C080600,  //  000B  MOVE	R2	R3
      0xB80E0600,  //  000C  GETNGBL	R3	K3
      0x8C0C073F,  //  000D  GETMET	R3	R3	K63
      0x5C140400,  //  000E  MOVE	R5	R2
      0x7C0C0400,  //  000F  CALL	R3	2
      0x80040600,  //  0010  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_HTTP_remote_init,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 3]) {
      be_nested_proto(
        10,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str_weak(parse_status_response_and_call_method),
        /* K1   */  be_nested_str_weak(parse_status_http),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[10]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x8C0C0700,  //  0001  GETMET	R3	R3	K0
          0x5C140000,  //  0002  MOVE	R5	R0
          0x5C180200,  //  0003  MOVE	R6	R1
          0x5C1C0400,  //  0004  MOVE	R7	R2
          0x68200000,  //  0005  GETUPV	R8	U0
          0x68240000,  //  0006  GETUPV	R9	U0
          0x88241301,  //  0007  GETMBR	R9	R9	K1
          0x7C0C0C00,  //  0008  CALL	R3	6
          0x80040600,  //  0009  RET	1	R3
        })
      ),
      be_nested_proto(
        10,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str_weak(parse_status_response_and_call_method),
        /* K1   */  be_nested_str_weak(parse_status_http),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[10]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x8C0C0700,  //  0001  GETMET	R3	R3	K0
          0x5C140000,  //  0002  MOVE	R5	R0
          0x5C180200,  //  0003  MOVE	R6	R1
          0x5C1C0400,  //  0004  MOVE	R7	R2
          0x68200000,  //  0005  GETUPV	R8	U0
          0x68240000,  //  0006  GETUPV	R9	U0
          0x88241301,  //  0007  GETMBR	R9	R9	K1
          0x7C0C0C00,  //  0008  CALL	R3	6
          0x80040600,  //  0009  RET	1	R3
        })
      ),
      be_nested_proto(
        10,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 2]) {     /* constants */
        /* K0   */  be_nested_str_weak(parse_status_response_and_call_method),
        /* K1   */  be_nested_str_weak(parse_status_http),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[10]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x8C0C0700,  //  0001  GETMET	R3	R3	K0
          0x5C140000,  //  0002  MOVE	R5	R0
          0x5C180200,  //  0003  MOVE	R6	R1
          0x5C1C0400,  //  0004  MOVE	R7	R2
          0x68200000,  //  0005  GETUPV	R8	U0
          0x68240000,  //  0006  GETUPV	R9	U0
          0x88241301,  //  0007  GETMBR	R9	R9	K1
          0x7C0C0C00,  //  0008  CALL	R3	6
          0x80040600,  //  0009  RET	1	R3
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_remote,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x90027A01,  //  0000  SETMBR	R0	K61	R1
      0x60140013,  //  0001  GETGBL	R5	G19
      0x7C140000,  //  0002  CALL	R5	0
      0x90020005,  //  0003  SETMBR	R0	K0	R5
      0x60140013,  //  0004  GETGBL	R5	G19
      0x7C140000,  //  0005  CALL	R5	0
      0x90020405,  //  0006  SETMBR	R0	K2	R5
      0x60140013,  //  0007  GETGBL	R5	G19
      0x7C140000,  //  0008  CALL	R5	0
      0x90020E05,  //  0009  SETMBR	R0	K7	R5
      0x4C140000,  //  000A  LDNIL	R5
      0x90021205,  //  000B  SETMBR	R0	K9	R5
      0x50140000,  //  000C  LDBOOL	R5	0	0
      0x90024A05,  //  000D  SETMBR	R0	K37	R5
      0x60140003,  //  000E  GETGBL	R5	G3
      0x5C180000,  //  000F  MOVE	R6	R0
      0x7C140200,  //  0010  CALL	R5	1
      0x8C140B40,  //  0011  GETMET	R5	R5	K64
      0x5C1C0400,  //  0012  MOVE	R7	R2
      0x5422004F,  //  0013  LDINT	R8	80
      0x5C240600,  //  0014  MOVE	R9	R3
      0x5C280800,  //  0015  MOVE	R10	R4
      0x7C140A00,  //  0016  CALL	R5	5
      0x60140013,  //  0017  GETGBL	R5	G19
      0x7C140000,  //  0018  CALL	R5	0
      0x90021605,  //  0019  SETMBR	R0	K11	R5
      0x8814013D,  //  001A  GETMBR	R5	R0	K61
      0x7816000E,  //  001B  JMPF	R5	#002B
      0x8C140141,  //  001C  GETMET	R5	R0	K65
      0x881C0116,  //  001D  GETMBR	R7	R0	K22
      0x88200142,  //  001E  GETMBR	R8	R0	K66
      0x84240000,  //  001F  CLOSURE	R9	P0
      0x7C140800,  //  0020  CALL	R5	4
      0x8C140141,  //  0021  GETMET	R5	R0	K65
      0x881C011F,  //  0022  GETMBR	R7	R0	K31
      0x88200142,  //  0023  GETMBR	R8	R0	K66
      0x84240001,  //  0024  CLOSURE	R9	P1
      0x7C140800,  //  0025  CALL	R5	4
      0x8C140141,  //  0026  GETMET	R5	R0	K65
      0x881C0123,  //  0027  GETMBR	R7	R0	K35
      0x88200142,  //  0028  GETMBR	R8	R0	K66
      0x84240002,  //  0029  CLOSURE	R9	P2
      0x7C140800,  //  002A  CALL	R5	4
      0xA0000000,  //  002B  CLOSE	R0
      0x80000000,  //  002C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: change_schedule
********************************************************************/
be_local_closure(class_Matter_HTTP_remote_change_schedule,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_remote,     /* shared constants */
    be_str_weak(change_schedule),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x780E0007,  //  0004  JMPF	R3	#000D
      0x880C0100,  //  0005  GETMBR	R3	R0	K0
      0x980C0202,  //  0006  SETIDX	R3	R1	R2
      0x880C0102,  //  0007  GETMBR	R3	R0	K2
      0xB8120600,  //  0008  GETNGBL	R4	K3
      0x8C100904,  //  0009  GETMET	R4	R4	K4
      0x5C180400,  //  000A  MOVE	R6	R2
      0x7C100400,  //  000B  CALL	R4	2
      0x980C0204,  //  000C  SETIDX	R3	R1	R4
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_status_response_and_call_method
********************************************************************/
be_local_closure(class_Matter_HTTP_remote_parse_status_response_and_call_method,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    6,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_remote,     /* shared constants */
    be_str_weak(parse_status_response_and_call_method),
    &be_const_str_solidified,
    ( &(const binstruction[57]) {  /* code */
      0x4C180000,  //  0000  LDNIL	R6
      0x20180206,  //  0001  NE	R6	R1	R6
      0x781A0034,  //  0002  JMPF	R6	#0038
      0x24180306,  //  0003  GT	R6	R1	K6
      0x781A0032,  //  0004  JMPF	R6	#0038
      0x8C180143,  //  0005  GETMET	R6	R0	K67
      0x50200200,  //  0006  LDBOOL	R8	1	0
      0x7C180400,  //  0007  CALL	R6	2
      0x5C180400,  //  0008  MOVE	R6	R2
      0x601C0004,  //  0009  GETGBL	R7	G4
      0x5C200C00,  //  000A  MOVE	R8	R6
      0x7C1C0200,  //  000B  CALL	R7	1
      0x1C1C0F31,  //  000C  EQ	R7	R7	K49
      0x781E0004,  //  000D  JMPF	R7	#0013
      0xA41E8800,  //  000E  IMPORT	R7	K68
      0x8C200F45,  //  000F  GETMET	R8	R7	K69
      0x5C280C00,  //  0010  MOVE	R10	R6
      0x7C200400,  //  0011  CALL	R8	2
      0x5C181000,  //  0012  MOVE	R6	R8
      0x4C1C0000,  //  0013  LDNIL	R7
      0x4C200000,  //  0014  LDNIL	R8
      0x20200C08,  //  0015  NE	R8	R6	R8
      0x78220019,  //  0016  JMPF	R8	#0031
      0x58200006,  //  0017  LDCONST	R8	K6
      0x88240146,  //  0018  GETMBR	R9	R0	K70
      0x6028000C,  //  0019  GETGBL	R10	G12
      0x5C2C1200,  //  001A  MOVE	R11	R9
      0x7C280200,  //  001B  CALL	R10	1
      0x1428100A,  //  001C  LT	R10	R8	R10
      0x782A000C,  //  001D  JMPF	R10	#002B
      0x94281208,  //  001E  GETIDX	R10	R9	R8
      0x4C2C0000,  //  001F  LDNIL	R11
      0x202C140B,  //  0020  NE	R11	R10	R11
      0x782E0006,  //  0021  JMPF	R11	#0029
      0x8C2C0D01,  //  0022  GETMET	R11	R6	K1
      0x5C341400,  //  0023  MOVE	R13	R10
      0x7C2C0400,  //  0024  CALL	R11	2
      0x782E0002,  //  0025  JMPF	R11	#0029
      0x94180C0A,  //  0026  GETIDX	R6	R6	R10
      0x5C1C1000,  //  0027  MOVE	R7	R8
      0x70020001,  //  0028  JMP		#002B
      0x00201147,  //  0029  ADD	R8	R8	K71
      0x7001FFED,  //  002A  JMP		#0019
      0x5C280A00,  //  002B  MOVE	R10	R5
      0x5C2C0800,  //  002C  MOVE	R11	R4
      0x5C300C00,  //  002D  MOVE	R12	R6
      0x5C340E00,  //  002E  MOVE	R13	R7
      0x7C280600,  //  002F  CALL	R10	3
      0x70020006,  //  0030  JMP		#0038
      0xB8222200,  //  0031  GETNGBL	R8	K17
      0x60240018,  //  0032  GETGBL	R9	G24
      0x58280048,  //  0033  LDCONST	R10	K72
      0x5C2C0400,  //  0034  MOVE	R11	R2
      0x7C240400,  //  0035  CALL	R9	2
      0x58280014,  //  0036  LDCONST	R10	K20
      0x7C200400,  //  0037  CALL	R8	2
      0x80000000,  //  0038  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: call_sync
********************************************************************/
be_local_closure(class_Matter_HTTP_remote_call_sync,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_remote,     /* shared constants */
    be_str_weak(call_sync),
    &be_const_str_solidified,
    ( &(const binstruction[70]) {  /* code */
      0xA40E6200,  //  0000  IMPORT	R3	K49
      0xA4126400,  //  0001  IMPORT	R4	K50
      0xB8164E00,  //  0002  GETNGBL	R5	K39
      0x8C140B33,  //  0003  GETMET	R5	R5	K51
      0x7C140200,  //  0004  CALL	R5	1
      0x94140B34,  //  0005  GETIDX	R5	R5	K52
      0x74160006,  //  0006  JMPT	R5	#000E
      0xB8164E00,  //  0007  GETNGBL	R5	K39
      0x8C140B35,  //  0008  GETMET	R5	R5	K53
      0x7C140200,  //  0009  CALL	R5	1
      0x94140B34,  //  000A  GETIDX	R5	R5	K52
      0x74160001,  //  000B  JMPT	R5	#000E
      0x4C140000,  //  000C  LDNIL	R5
      0x80040A00,  //  000D  RET	1	R5
      0x4C140000,  //  000E  LDNIL	R5
      0x90021205,  //  000F  SETMBR	R0	K9	R5
      0x8C140737,  //  0010  GETMET	R5	R3	K55
      0x5C1C0200,  //  0011  MOVE	R7	R1
      0x58200038,  //  0012  LDCONST	R8	K56
      0x58240039,  //  0013  LDCONST	R9	K57
      0x7C140800,  //  0014  CALL	R5	4
      0x00166C05,  //  0015  ADD	R5	K54	R5
      0xB81A2200,  //  0016  GETNGBL	R6	K17
      0x601C0018,  //  0017  GETGBL	R7	G24
      0x58200049,  //  0018  LDCONST	R8	K73
      0x88240113,  //  0019  GETMBR	R9	R0	K19
      0x8828013B,  //  001A  GETMBR	R10	R0	K59
      0x5C2C0A00,  //  001B  MOVE	R11	R5
      0x7C1C0800,  //  001C  CALL	R7	4
      0x54220003,  //  001D  LDINT	R8	4
      0x7C180400,  //  001E  CALL	R6	2
      0x60180003,  //  001F  GETGBL	R6	G3
      0x5C1C0000,  //  0020  MOVE	R7	R0
      0x7C180200,  //  0021  CALL	R6	1
      0x8C180D4A,  //  0022  GETMET	R6	R6	K74
      0x5C200A00,  //  0023  MOVE	R8	R5
      0x5C240400,  //  0024  MOVE	R9	R2
      0x7C180600,  //  0025  CALL	R6	3
      0x781A0001,  //  0026  JMPF	R6	#0029
      0x5C1C0C00,  //  0027  MOVE	R7	R6
      0x70020000,  //  0028  JMP		#002A
      0x581C002A,  //  0029  LDCONST	R7	K42
      0x6020000C,  //  002A  GETGBL	R8	G12
      0x5C240E00,  //  002B  MOVE	R9	R7
      0x7C200200,  //  002C  CALL	R8	1
      0x5426001D,  //  002D  LDINT	R9	30
      0x24201009,  //  002E  GT	R8	R8	R9
      0x78220004,  //  002F  JMPF	R8	#0035
      0x5422001C,  //  0030  LDINT	R8	29
      0x40220C08,  //  0031  CONNECT	R8	K6	R8
      0x94200E08,  //  0032  GETIDX	R8	R7	R8
      0x0020112B,  //  0033  ADD	R8	R8	K43
      0x5C1C1000,  //  0034  MOVE	R7	R8
      0xB8222200,  //  0035  GETNGBL	R8	K17
      0x60240018,  //  0036  GETGBL	R9	G24
      0x5828004B,  //  0037  LDCONST	R10	K75
      0xB82E4E00,  //  0038  GETNGBL	R11	K39
      0x8C2C172D,  //  0039  GETMET	R11	R11	K45
      0x7C2C0200,  //  003A  CALL	R11	1
      0x8830012E,  //  003B  GETMBR	R12	R0	K46
      0x042C160C,  //  003C  SUB	R11	R11	R12
      0x88300113,  //  003D  GETMBR	R12	R0	K19
      0x6034000C,  //  003E  GETGBL	R13	G12
      0x88380129,  //  003F  GETMBR	R14	R0	K41
      0x7C340200,  //  0040  CALL	R13	1
      0x5C380E00,  //  0041  MOVE	R14	R7
      0x7C240A00,  //  0042  CALL	R9	5
      0x58280014,  //  0043  LDCONST	R10	K20
      0x7C200400,  //  0044  CALL	R8	2
      0x80040C00,  //  0045  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scheduler
********************************************************************/
be_local_closure(class_Matter_HTTP_remote_scheduler,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_remote,     /* shared constants */
    be_str_weak(scheduler),
    &be_const_str_solidified,
    ( &(const binstruction[66]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x60080010,  //  0001  GETGBL	R2	G16
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x8C0C0708,  //  0003  GETMET	R3	R3	K8
      0x7C0C0200,  //  0004  CALL	R3	1
      0x7C080200,  //  0005  CALL	R2	1
      0xA802000A,  //  0006  EXBLK	0	#0012
      0x5C0C0400,  //  0007  MOVE	R3	R2
      0x7C0C0000,  //  0008  CALL	R3	0
      0x88100102,  //  0009  GETMBR	R4	R0	K2
      0x94100803,  //  000A  GETIDX	R4	R4	R3
      0x1C100906,  //  000B  EQ	R4	R4	K6
      0x78120001,  //  000C  JMPF	R4	#000F
      0x5C040600,  //  000D  MOVE	R1	R3
      0x70020000,  //  000E  JMP		#0010
      0x7001FFF6,  //  000F  JMP		#0007
      0xA8040001,  //  0010  EXBLK	1	1
      0x70020002,  //  0011  JMP		#0015
      0x5808000A,  //  0012  LDCONST	R2	K10
      0xAC080200,  //  0013  CATCH	R2	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x4C080000,  //  0015  LDNIL	R2
      0x1C080202,  //  0016  EQ	R2	R1	R2
      0x780A0015,  //  0017  JMPF	R2	#002E
      0x60080010,  //  0018  GETGBL	R2	G16
      0x880C0102,  //  0019  GETMBR	R3	R0	K2
      0x8C0C0708,  //  001A  GETMET	R3	R3	K8
      0x7C0C0200,  //  001B  CALL	R3	1
      0x7C080200,  //  001C  CALL	R2	1
      0xA802000C,  //  001D  EXBLK	0	#002B
      0x5C0C0400,  //  001E  MOVE	R3	R2
      0x7C0C0000,  //  001F  CALL	R3	0
      0xB8124E00,  //  0020  GETNGBL	R4	K39
      0x8C10094C,  //  0021  GETMET	R4	R4	K76
      0x88180102,  //  0022  GETMBR	R6	R0	K2
      0x94180C03,  //  0023  GETIDX	R6	R6	R3
      0x7C100400,  //  0024  CALL	R4	2
      0x78120001,  //  0025  JMPF	R4	#0028
      0x5C040600,  //  0026  MOVE	R1	R3
      0x70020000,  //  0027  JMP		#0029
      0x7001FFF4,  //  0028  JMP		#001E
      0xA8040001,  //  0029  EXBLK	1	1
      0x70020002,  //  002A  JMP		#002E
      0x5808000A,  //  002B  LDCONST	R2	K10
      0xAC080200,  //  002C  CATCH	R2	1	0
      0xB0080000,  //  002D  RAISE	2	R0	R0
      0x4C080000,  //  002E  LDNIL	R2
      0x1C080202,  //  002F  EQ	R2	R1	R2
      0x780A0000,  //  0030  JMPF	R2	#0032
      0x80000400,  //  0031  RET	0
      0x8808014D,  //  0032  GETMBR	R2	R0	K77
      0x780A0002,  //  0033  JMPF	R2	#0037
      0x88080102,  //  0034  GETMBR	R2	R0	K2
      0x98080306,  //  0035  SETIDX	R2	R1	K6
      0x80000400,  //  0036  RET	0
      0x88080102,  //  0037  GETMBR	R2	R0	K2
      0xB80E4E00,  //  0038  GETNGBL	R3	K39
      0x8C0C072D,  //  0039  GETMET	R3	R3	K45
      0x88140100,  //  003A  GETMBR	R5	R0	K0
      0x94140A01,  //  003B  GETIDX	R5	R5	R1
      0x7C0C0400,  //  003C  CALL	R3	2
      0x98080203,  //  003D  SETIDX	R2	R1	R3
      0x8C08014E,  //  003E  GETMET	R2	R0	K78
      0x5C100200,  //  003F  MOVE	R4	R1
      0x7C080400,  //  0040  CALL	R2	2
      0x80000000,  //  0041  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_failed
********************************************************************/
be_local_closure(class_Matter_HTTP_remote_event_http_failed,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_remote,     /* shared constants */
    be_str_weak(event_http_failed),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060000,  //  0003  JMPF	R1	#0005
      0x80000200,  //  0004  RET	0
      0xB8062200,  //  0005  GETNGBL	R1	K17
      0x5808004F,  //  0006  LDCONST	R2	K79
      0x580C0014,  //  0007  LDCONST	R3	K20
      0x7C040400,  //  0008  CALL	R1	2
      0x8C04012F,  //  0009  GETMET	R1	R0	K47
      0x880C0130,  //  000A  GETMBR	R3	R0	K48
      0x4C100000,  //  000B  LDNIL	R4
      0x7C040600,  //  000C  CALL	R1	3
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_async_cb
********************************************************************/
be_local_closure(class_Matter_HTTP_remote_add_async_cb,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_remote,     /* shared constants */
    be_str_weak(add_async_cb),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x880C0107,  //  0000  GETMBR	R3	R0	K7
      0x980C0202,  //  0001  SETIDX	R3	R1	R2
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_timeout
********************************************************************/
be_local_closure(class_Matter_HTTP_remote_event_http_timeout,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_HTTP_remote,     /* shared constants */
    be_str_weak(event_http_timeout),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x88040109,  //  0000  GETMBR	R1	R0	K9
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060000,  //  0003  JMPF	R1	#0005
      0x80000200,  //  0004  RET	0
      0xB8062200,  //  0005  GETNGBL	R1	K17
      0x60080018,  //  0006  GETGBL	R2	G24
      0x580C0050,  //  0007  LDCONST	R3	K80
      0x88100130,  //  0008  GETMBR	R4	R0	K48
      0x88140151,  //  0009  GETMBR	R5	R0	K81
      0x88180152,  //  000A  GETMBR	R6	R0	K82
      0x601C000C,  //  000B  GETGBL	R7	G12
      0x88200129,  //  000C  GETMBR	R8	R0	K41
      0x7C1C0200,  //  000D  CALL	R7	1
      0x7C080A00,  //  000E  CALL	R2	5
      0x580C0014,  //  000F  LDCONST	R3	K20
      0x7C040400,  //  0010  CALL	R1	2
      0x8C04012F,  //  0011  GETMET	R1	R0	K47
      0x880C0130,  //  0012  GETMBR	R3	R0	K48
      0x4C100000,  //  0013  LDNIL	R4
      0x7C040600,  //  0014  CALL	R1	3
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_HTTP_remote
********************************************************************/
extern const bclass be_class_Matter_HTTP_async;
be_local_class(Matter_HTTP_remote,
    8,
    &be_class_Matter_HTTP_async,
    be_nested_map(32,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(add_schedule, -1), be_const_closure(class_Matter_HTTP_remote_add_schedule_closure) },
        { be_const_key_weak(event_http_timeout, 10), be_const_closure(class_Matter_HTTP_remote_event_http_timeout_closure) },
        { be_const_key_weak(UPDATE_TIME, -1), be_const_int(5000) },
        { be_const_key_weak(add_async_cb, 31), be_const_closure(class_Matter_HTTP_remote_add_async_cb_closure) },
        { be_const_key_weak(get_info, -1), be_const_closure(class_Matter_HTTP_remote_get_info_closure) },
        { be_const_key_weak(info, 21), be_const_var(7) },
        { be_const_key_weak(event_http_failed, -1), be_const_closure(class_Matter_HTTP_remote_event_http_failed_closure) },
        { be_const_key_weak(UPDATE_TIME2, -1), be_const_int(300000) },
        { be_const_key_weak(set_info, 2), be_const_closure(class_Matter_HTTP_remote_set_info_closure) },
        { be_const_key_weak(scheduler, 27), be_const_closure(class_Matter_HTTP_remote_scheduler_closure) },
        { be_const_key_weak(web_last_seen, -1), be_const_closure(class_Matter_HTTP_remote_web_last_seen_closure) },
        { be_const_key_weak(event_http_finished, -1), be_const_closure(class_Matter_HTTP_remote_event_http_finished_closure) },
        { be_const_key_weak(STATUS_PREFIX, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(14,
    ( (struct bvalue*) &(const bvalue[]) {
        be_nested_str_weak(Status),
        be_nested_str_weak(StatusPRM),
        be_nested_str_weak(StatusFWR),
        be_nested_str_weak(StatusLOG),
        be_nested_str_weak(StatusMEM),
        be_nested_str_weak(StatusNET),
        be_nested_str_weak(StatusMQT),
        be_nested_str_weak(StatusTIM),
        be_const_nil(),
        be_nested_str_weak(StatusPTH),
        be_nested_str_weak(StatusSNS),
        be_nested_str_weak(StatusSTS),
        be_nested_str_weak(StatusSTK),
        be_nested_str_weak(StatusSHT),
    }))    ) } )) },
        { be_const_key_weak(current_cmd, -1), be_const_var(4) },
        { be_const_key_weak(probe_async, 7), be_const_closure(class_Matter_HTTP_remote_probe_async_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_HTTP_remote_init_closure) },
        { be_const_key_weak(reachable, 1), be_const_var(5) },
        { be_const_key_weak(probe_next_timestamp_map, -1), be_const_var(2) },
        { be_const_key_weak(change_schedule, 15), be_const_closure(class_Matter_HTTP_remote_change_schedule_closure) },
        { be_const_key_weak(device, 18), be_const_var(0) },
        { be_const_key_weak(async_cb_map, -1), be_const_var(3) },
        { be_const_key_weak(reachable_utc, -1), be_const_var(6) },
        { be_const_key_weak(parse_status_response_and_call_method, -1), be_const_closure(class_Matter_HTTP_remote_parse_status_response_and_call_method_closure) },
        { be_const_key_weak(UPDATE_CMD2, -1), be_nested_str_weak(Status_X202) },
        { be_const_key_weak(probe_update_time_map, -1), be_const_var(1) },
        { be_const_key_weak(call_sync, -1), be_const_closure(class_Matter_HTTP_remote_call_sync_closure) },
        { be_const_key_weak(device_is_alive, 9), be_const_closure(class_Matter_HTTP_remote_device_is_alive_closure) },
        { be_const_key_weak(info_changed, -1), be_const_closure(class_Matter_HTTP_remote_info_changed_closure) },
        { be_const_key_weak(parse_status_http, 6), be_const_closure(class_Matter_HTTP_remote_parse_status_http_closure) },
        { be_const_key_weak(dispatch_cb, 3), be_const_closure(class_Matter_HTTP_remote_dispatch_cb_closure) },
        { be_const_key_weak(UPDATE_CMD5, -1), be_nested_str_weak(Status_X205) },
        { be_const_key_weak(UPDATE_CMD0, -1), be_nested_str_weak(Status) },
    })),
    be_str_weak(Matter_HTTP_remote)
);
/********************************************************************/
/* End of solidification */
