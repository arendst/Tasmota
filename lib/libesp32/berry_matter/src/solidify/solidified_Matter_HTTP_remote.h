/* Solidification of Matter_HTTP_remote.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_HTTP_remote;

/********************************************************************
** Solidified function: device_is_alive
********************************************************************/
be_local_closure(Matter_HTTP_remote_device_is_alive,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(reachable),
    /* K1   */  be_nested_str_weak(reachable_utc),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(rtc_utc),
    }),
    be_str_weak(device_is_alive),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x78060006,  //  0000  JMPF	R1	#0008
      0x50080200,  //  0001  LDBOOL	R2	1	0
      0x90020002,  //  0002  SETMBR	R0	K0	R2
      0xB80A0400,  //  0003  GETNGBL	R2	K2
      0x8C080503,  //  0004  GETMET	R2	R2	K3
      0x7C080200,  //  0005  CALL	R2	1
      0x90020202,  //  0006  SETMBR	R0	K1	R2
      0x70020001,  //  0007  JMP		#000A
      0x50080000,  //  0008  LDBOOL	R2	0	0
      0x90020002,  //  0009  SETMBR	R0	K0	R2
      0x80000000,  //  000A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_update
********************************************************************/
be_local_closure(Matter_HTTP_remote_parse_update,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[28]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(DeviceName),
    /* K3   */  be_nested_str_weak(Tasmota),
    /* K4   */  be_nested_str_weak(info),
    /* K5   */  be_nested_str_weak(name),
    /* K6   */  be_nested_str_weak(remove),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20update_X20_X27_X25s_X27_X20name_X3D_X27_X25s_X27),
    /* K10  */  be_nested_str_weak(addr),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str_weak(change_schedule),
    /* K13  */  be_nested_str_weak(UPDATE_CMD0),
    /* K14  */  be_nested_str_weak(UPDATE_TIME2),
    /* K15  */  be_const_int(2),
    /* K16  */  be_nested_str_weak(Version),
    /* K17  */  be_nested_str_weak(Hardware),
    /* K18  */  be_nested_str_weak(version),
    /* K19  */  be_nested_str_weak(MTR_X3A_X20update_X20_X27_X25s_X27_X20version_X3D_X27_X25s_X27),
    /* K20  */  be_nested_str_weak(hardware),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20update_X20_X27_X25s_X27_X20hardware_X3D_X27_X25s_X27),
    /* K22  */  be_nested_str_weak(UPDATE_CMD2),
    /* K23  */  be_nested_str_weak(Mac),
    /* K24  */  be_nested_str_weak(mac),
    /* K25  */  be_nested_str_weak(MTR_X3A_X20update_X20_X27_X25s_X27_X20mac_X3D_X27_X25s_X27),
    /* K26  */  be_nested_str_weak(UPDATE_CMD5),
    /* K27  */  be_nested_str_weak(info_changed),
    }),
    be_str_weak(parse_update),
    &be_const_str_solidified,
    ( &(const binstruction[145]) {  /* code */
      0x500C0000,  //  0000  LDBOOL	R3	0	0
      0x1C100500,  //  0001  EQ	R4	R2	K0
      0x78120024,  //  0002  JMPF	R4	#0028
      0x8C100301,  //  0003  GETMET	R4	R1	K1
      0x58180002,  //  0004  LDCONST	R6	K2
      0x7C100400,  //  0005  CALL	R4	2
      0x1C140903,  //  0006  EQ	R5	R4	K3
      0x78160000,  //  0007  JMPF	R5	#0009
      0x4C100000,  //  0008  LDNIL	R4
      0x88140104,  //  0009  GETMBR	R5	R0	K4
      0x8C140B01,  //  000A  GETMET	R5	R5	K1
      0x581C0005,  //  000B  LDCONST	R7	K5
      0x7C140400,  //  000C  CALL	R5	2
      0x20140A04,  //  000D  NE	R5	R5	R4
      0x78160013,  //  000E  JMPF	R5	#0023
      0x4C140000,  //  000F  LDNIL	R5
      0x20140805,  //  0010  NE	R5	R4	R5
      0x78160002,  //  0011  JMPF	R5	#0015
      0x88140104,  //  0012  GETMBR	R5	R0	K4
      0x98160A04,  //  0013  SETIDX	R5	K5	R4
      0x70020003,  //  0014  JMP		#0019
      0x88140104,  //  0015  GETMBR	R5	R0	K4
      0x8C140B06,  //  0016  GETMET	R5	R5	K6
      0x581C0005,  //  0017  LDCONST	R7	K5
      0x7C140400,  //  0018  CALL	R5	2
      0xB8160E00,  //  0019  GETNGBL	R5	K7
      0x8C140B08,  //  001A  GETMET	R5	R5	K8
      0x601C0018,  //  001B  GETGBL	R7	G24
      0x58200009,  //  001C  LDCONST	R8	K9
      0x8824010A,  //  001D  GETMBR	R9	R0	K10
      0x5C280800,  //  001E  MOVE	R10	R4
      0x7C1C0600,  //  001F  CALL	R7	3
      0x5820000B,  //  0020  LDCONST	R8	K11
      0x7C140600,  //  0021  CALL	R5	3
      0x500C0200,  //  0022  LDBOOL	R3	1	0
      0x8C14010C,  //  0023  GETMET	R5	R0	K12
      0x881C010D,  //  0024  GETMBR	R7	R0	K13
      0x8820010E,  //  0025  GETMBR	R8	R0	K14
      0x7C140600,  //  0026  CALL	R5	3
      0x70020064,  //  0027  JMP		#008D
      0x1C10050F,  //  0028  EQ	R4	R2	K15
      0x7812003E,  //  0029  JMPF	R4	#0069
      0x8C100301,  //  002A  GETMET	R4	R1	K1
      0x58180010,  //  002B  LDCONST	R6	K16
      0x7C100400,  //  002C  CALL	R4	2
      0x8C140301,  //  002D  GETMET	R5	R1	K1
      0x581C0011,  //  002E  LDCONST	R7	K17
      0x7C140400,  //  002F  CALL	R5	2
      0x88180104,  //  0030  GETMBR	R6	R0	K4
      0x8C180D01,  //  0031  GETMET	R6	R6	K1
      0x58200012,  //  0032  LDCONST	R8	K18
      0x7C180400,  //  0033  CALL	R6	2
      0x20180C04,  //  0034  NE	R6	R6	R4
      0x781A0013,  //  0035  JMPF	R6	#004A
      0x4C180000,  //  0036  LDNIL	R6
      0x20180806,  //  0037  NE	R6	R4	R6
      0x781A0002,  //  0038  JMPF	R6	#003C
      0x88180104,  //  0039  GETMBR	R6	R0	K4
      0x981A2404,  //  003A  SETIDX	R6	K18	R4
      0x70020003,  //  003B  JMP		#0040
      0x88180104,  //  003C  GETMBR	R6	R0	K4
      0x8C180D06,  //  003D  GETMET	R6	R6	K6
      0x58200012,  //  003E  LDCONST	R8	K18
      0x7C180400,  //  003F  CALL	R6	2
      0xB81A0E00,  //  0040  GETNGBL	R6	K7
      0x8C180D08,  //  0041  GETMET	R6	R6	K8
      0x60200018,  //  0042  GETGBL	R8	G24
      0x58240013,  //  0043  LDCONST	R9	K19
      0x8828010A,  //  0044  GETMBR	R10	R0	K10
      0x5C2C0800,  //  0045  MOVE	R11	R4
      0x7C200600,  //  0046  CALL	R8	3
      0x5824000B,  //  0047  LDCONST	R9	K11
      0x7C180600,  //  0048  CALL	R6	3
      0x500C0200,  //  0049  LDBOOL	R3	1	0
      0x88180104,  //  004A  GETMBR	R6	R0	K4
      0x8C180D01,  //  004B  GETMET	R6	R6	K1
      0x58200014,  //  004C  LDCONST	R8	K20
      0x7C180400,  //  004D  CALL	R6	2
      0x20180C05,  //  004E  NE	R6	R6	R5
      0x781A0013,  //  004F  JMPF	R6	#0064
      0x4C180000,  //  0050  LDNIL	R6
      0x20180A06,  //  0051  NE	R6	R5	R6
      0x781A0002,  //  0052  JMPF	R6	#0056
      0x88180104,  //  0053  GETMBR	R6	R0	K4
      0x981A2805,  //  0054  SETIDX	R6	K20	R5
      0x70020003,  //  0055  JMP		#005A
      0x88180104,  //  0056  GETMBR	R6	R0	K4
      0x8C180D06,  //  0057  GETMET	R6	R6	K6
      0x58200014,  //  0058  LDCONST	R8	K20
      0x7C180400,  //  0059  CALL	R6	2
      0xB81A0E00,  //  005A  GETNGBL	R6	K7
      0x8C180D08,  //  005B  GETMET	R6	R6	K8
      0x60200018,  //  005C  GETGBL	R8	G24
      0x58240015,  //  005D  LDCONST	R9	K21
      0x8828010A,  //  005E  GETMBR	R10	R0	K10
      0x5C2C0A00,  //  005F  MOVE	R11	R5
      0x7C200600,  //  0060  CALL	R8	3
      0x5824000B,  //  0061  LDCONST	R9	K11
      0x7C180600,  //  0062  CALL	R6	3
      0x500C0200,  //  0063  LDBOOL	R3	1	0
      0x8C18010C,  //  0064  GETMET	R6	R0	K12
      0x88200116,  //  0065  GETMBR	R8	R0	K22
      0x8824010E,  //  0066  GETMBR	R9	R0	K14
      0x7C180600,  //  0067  CALL	R6	3
      0x70020023,  //  0068  JMP		#008D
      0x54120004,  //  0069  LDINT	R4	5
      0x1C100404,  //  006A  EQ	R4	R2	R4
      0x78120020,  //  006B  JMPF	R4	#008D
      0x8C100301,  //  006C  GETMET	R4	R1	K1
      0x58180017,  //  006D  LDCONST	R6	K23
      0x7C100400,  //  006E  CALL	R4	2
      0x88140104,  //  006F  GETMBR	R5	R0	K4
      0x8C140B01,  //  0070  GETMET	R5	R5	K1
      0x581C0018,  //  0071  LDCONST	R7	K24
      0x7C140400,  //  0072  CALL	R5	2
      0x20140A04,  //  0073  NE	R5	R5	R4
      0x78160013,  //  0074  JMPF	R5	#0089
      0x4C140000,  //  0075  LDNIL	R5
      0x20140805,  //  0076  NE	R5	R4	R5
      0x78160002,  //  0077  JMPF	R5	#007B
      0x88140104,  //  0078  GETMBR	R5	R0	K4
      0x98163004,  //  0079  SETIDX	R5	K24	R4
      0x70020003,  //  007A  JMP		#007F
      0x88140104,  //  007B  GETMBR	R5	R0	K4
      0x8C140B06,  //  007C  GETMET	R5	R5	K6
      0x581C0018,  //  007D  LDCONST	R7	K24
      0x7C140400,  //  007E  CALL	R5	2
      0xB8160E00,  //  007F  GETNGBL	R5	K7
      0x8C140B08,  //  0080  GETMET	R5	R5	K8
      0x601C0018,  //  0081  GETGBL	R7	G24
      0x58200019,  //  0082  LDCONST	R8	K25
      0x8824010A,  //  0083  GETMBR	R9	R0	K10
      0x5C280800,  //  0084  MOVE	R10	R4
      0x7C1C0600,  //  0085  CALL	R7	3
      0x5820000B,  //  0086  LDCONST	R8	K11
      0x7C140600,  //  0087  CALL	R5	3
      0x500C0200,  //  0088  LDBOOL	R3	1	0
      0x8C14010C,  //  0089  GETMET	R5	R0	K12
      0x881C011A,  //  008A  GETMBR	R7	R0	K26
      0x8820010E,  //  008B  GETMBR	R8	R0	K14
      0x7C140600,  //  008C  CALL	R5	3
      0x780E0001,  //  008D  JMPF	R3	#0090
      0x8C10011B,  //  008E  GETMET	R4	R0	K27
      0x7C100200,  //  008F  CALL	R4	1
      0x80000000,  //  0090  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: call_sync
********************************************************************/
be_local_closure(Matter_HTTP_remote_call_sync,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(webserver),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(wifi),
    /* K4   */  be_nested_str_weak(up),
    /* K5   */  be_nested_str_weak(eth),
    /* K6   */  be_nested_str_weak(current_cmd),
    /* K7   */  be_nested_str_weak(_X2Fcm_X3Fcmnd_X3D),
    /* K8   */  be_nested_str_weak(tr),
    /* K9   */  be_nested_str_weak(_X20),
    /* K10  */  be_nested_str_weak(_X2B),
    /* K11  */  be_nested_str_weak(log),
    /* K12  */  be_nested_str_weak(MTR_X3A_X20HTTP_X20sync_X20request_X20_X27http_X3A_X2F_X2F_X25s_X3A_X25i_X25s_X27),
    /* K13  */  be_nested_str_weak(addr),
    /* K14  */  be_nested_str_weak(port),
    /* K15  */  be_const_int(3),
    /* K16  */  be_nested_str_weak(begin_sync),
    /* K17  */  be_nested_str_weak(nil),
    /* K18  */  be_const_int(0),
    /* K19  */  be_nested_str_weak(_X2E_X2E_X2E),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20HTTP_X20sync_X2Dresp_X20_X20in_X20_X25i_X20ms_X20from_X20_X25s_X3A_X20_X5B_X25i_X5D_X20_X27_X25s_X27),
    /* K21  */  be_nested_str_weak(millis),
    /* K22  */  be_nested_str_weak(time_start),
    /* K23  */  be_nested_str_weak(payload),
    }),
    be_str_weak(call_sync),
    &be_const_str_solidified,
    ( &(const binstruction[72]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xA4120200,  //  0001  IMPORT	R4	K1
      0xB8160400,  //  0002  GETNGBL	R5	K2
      0x8C140B03,  //  0003  GETMET	R5	R5	K3
      0x7C140200,  //  0004  CALL	R5	1
      0x94140B04,  //  0005  GETIDX	R5	R5	K4
      0x74160006,  //  0006  JMPT	R5	#000E
      0xB8160400,  //  0007  GETNGBL	R5	K2
      0x8C140B05,  //  0008  GETMET	R5	R5	K5
      0x7C140200,  //  0009  CALL	R5	1
      0x94140B04,  //  000A  GETIDX	R5	R5	K4
      0x74160001,  //  000B  JMPT	R5	#000E
      0x4C140000,  //  000C  LDNIL	R5
      0x80040A00,  //  000D  RET	1	R5
      0x4C140000,  //  000E  LDNIL	R5
      0x90020C05,  //  000F  SETMBR	R0	K6	R5
      0x8C140708,  //  0010  GETMET	R5	R3	K8
      0x5C1C0200,  //  0011  MOVE	R7	R1
      0x58200009,  //  0012  LDCONST	R8	K9
      0x5824000A,  //  0013  LDCONST	R9	K10
      0x7C140800,  //  0014  CALL	R5	4
      0x00160E05,  //  0015  ADD	R5	K7	R5
      0xB81A0400,  //  0016  GETNGBL	R6	K2
      0x8C180D0B,  //  0017  GETMET	R6	R6	K11
      0x60200018,  //  0018  GETGBL	R8	G24
      0x5824000C,  //  0019  LDCONST	R9	K12
      0x8828010D,  //  001A  GETMBR	R10	R0	K13
      0x882C010E,  //  001B  GETMBR	R11	R0	K14
      0x5C300A00,  //  001C  MOVE	R12	R5
      0x7C200800,  //  001D  CALL	R8	4
      0x5824000F,  //  001E  LDCONST	R9	K15
      0x7C180600,  //  001F  CALL	R6	3
      0x60180003,  //  0020  GETGBL	R6	G3
      0x5C1C0000,  //  0021  MOVE	R7	R0
      0x7C180200,  //  0022  CALL	R6	1
      0x8C180D10,  //  0023  GETMET	R6	R6	K16
      0x5C200A00,  //  0024  MOVE	R8	R5
      0x5C240400,  //  0025  MOVE	R9	R2
      0x7C180600,  //  0026  CALL	R6	3
      0x781A0001,  //  0027  JMPF	R6	#002A
      0x5C1C0C00,  //  0028  MOVE	R7	R6
      0x70020000,  //  0029  JMP		#002B
      0x581C0011,  //  002A  LDCONST	R7	K17
      0x6020000C,  //  002B  GETGBL	R8	G12
      0x5C240E00,  //  002C  MOVE	R9	R7
      0x7C200200,  //  002D  CALL	R8	1
      0x5426001D,  //  002E  LDINT	R9	30
      0x24201009,  //  002F  GT	R8	R8	R9
      0x78220004,  //  0030  JMPF	R8	#0036
      0x5422001C,  //  0031  LDINT	R8	29
      0x40222408,  //  0032  CONNECT	R8	K18	R8
      0x94200E08,  //  0033  GETIDX	R8	R7	R8
      0x00201113,  //  0034  ADD	R8	R8	K19
      0x5C1C1000,  //  0035  MOVE	R7	R8
      0xB8220400,  //  0036  GETNGBL	R8	K2
      0x8C20110B,  //  0037  GETMET	R8	R8	K11
      0x60280018,  //  0038  GETGBL	R10	G24
      0x582C0014,  //  0039  LDCONST	R11	K20
      0xB8320400,  //  003A  GETNGBL	R12	K2
      0x8C301915,  //  003B  GETMET	R12	R12	K21
      0x7C300200,  //  003C  CALL	R12	1
      0x88340116,  //  003D  GETMBR	R13	R0	K22
      0x0430180D,  //  003E  SUB	R12	R12	R13
      0x8834010D,  //  003F  GETMBR	R13	R0	K13
      0x6038000C,  //  0040  GETGBL	R14	G12
      0x883C0117,  //  0041  GETMBR	R15	R0	K23
      0x7C380200,  //  0042  CALL	R14	1
      0x5C3C0E00,  //  0043  MOVE	R15	R7
      0x7C280A00,  //  0044  CALL	R10	5
      0x582C000F,  //  0045  LDCONST	R11	K15
      0x7C200600,  //  0046  CALL	R8	3
      0x80040C00,  //  0047  RET	1	R6
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_finished
********************************************************************/
be_local_closure(Matter_HTTP_remote_event_http_finished,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[14]) {     /* constants */
    /* K0   */  be_nested_str_weak(current_cmd),
    /* K1   */  be_nested_str_weak(payload),
    /* K2   */  be_nested_str_weak(nil),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(_X2E_X2E_X2E),
    /* K5   */  be_nested_str_weak(tasmota),
    /* K6   */  be_nested_str_weak(log),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20HTTP_X20async_X2Dresp_X20in_X20_X25i_X20ms_X20from_X20_X25s_X3A_X20_X5B_X25i_X5D_X20_X27_X25s_X27),
    /* K8   */  be_nested_str_weak(millis),
    /* K9   */  be_nested_str_weak(time_start),
    /* K10  */  be_nested_str_weak(addr),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str_weak(dispatch_cb),
    /* K13  */  be_nested_str_weak(http_status),
    }),
    be_str_weak(event_http_finished),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060000,  //  0003  JMPF	R1	#0005
      0x80000200,  //  0004  RET	0
      0x88040101,  //  0005  GETMBR	R1	R0	K1
      0x4C080000,  //  0006  LDNIL	R2
      0x20040202,  //  0007  NE	R1	R1	R2
      0x78060001,  //  0008  JMPF	R1	#000B
      0x88040101,  //  0009  GETMBR	R1	R0	K1
      0x70020000,  //  000A  JMP		#000C
      0x58040002,  //  000B  LDCONST	R1	K2
      0x6008000C,  //  000C  GETGBL	R2	G12
      0x5C0C0200,  //  000D  MOVE	R3	R1
      0x7C080200,  //  000E  CALL	R2	1
      0x540E001D,  //  000F  LDINT	R3	30
      0x24080403,  //  0010  GT	R2	R2	R3
      0x780A0004,  //  0011  JMPF	R2	#0017
      0x540A001C,  //  0012  LDINT	R2	29
      0x400A0602,  //  0013  CONNECT	R2	K3	R2
      0x94080202,  //  0014  GETIDX	R2	R1	R2
      0x00080504,  //  0015  ADD	R2	R2	K4
      0x5C040400,  //  0016  MOVE	R1	R2
      0xB80A0A00,  //  0017  GETNGBL	R2	K5
      0x8C080506,  //  0018  GETMET	R2	R2	K6
      0x60100018,  //  0019  GETGBL	R4	G24
      0x58140007,  //  001A  LDCONST	R5	K7
      0xB81A0A00,  //  001B  GETNGBL	R6	K5
      0x8C180D08,  //  001C  GETMET	R6	R6	K8
      0x7C180200,  //  001D  CALL	R6	1
      0x881C0109,  //  001E  GETMBR	R7	R0	K9
      0x04180C07,  //  001F  SUB	R6	R6	R7
      0x881C010A,  //  0020  GETMBR	R7	R0	K10
      0x6020000C,  //  0021  GETGBL	R8	G12
      0x88240101,  //  0022  GETMBR	R9	R0	K1
      0x7C200200,  //  0023  CALL	R8	1
      0x5C240200,  //  0024  MOVE	R9	R1
      0x7C100A00,  //  0025  CALL	R4	5
      0x5814000B,  //  0026  LDCONST	R5	K11
      0x7C080600,  //  0027  CALL	R2	3
      0x8C08010C,  //  0028  GETMET	R2	R0	K12
      0x8810010D,  //  0029  GETMBR	R4	R0	K13
      0x88140101,  //  002A  GETMBR	R5	R0	K1
      0x7C080600,  //  002B  CALL	R2	3
      0x80000000,  //  002C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_last_seen
********************************************************************/
be_local_closure(Matter_HTTP_remote_web_last_seen,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(webserver),
    /* K1   */  be_nested_str_weak(reachable_utc),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(rtc_utc),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(seconds_to_dhm),
    }),
    be_str_weak(web_last_seen),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x5409FFFE,  //  0001  LDINT	R2	-1
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x4C100000,  //  0003  LDNIL	R4
      0x200C0604,  //  0004  NE	R3	R3	R4
      0x780E0005,  //  0005  JMPF	R3	#000C
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x8C0C0703,  //  0007  GETMET	R3	R3	K3
      0x7C0C0200,  //  0008  CALL	R3	1
      0x88100101,  //  0009  GETMBR	R4	R0	K1
      0x040C0604,  //  000A  SUB	R3	R3	R4
      0x5C080600,  //  000B  MOVE	R2	R3
      0xB80E0800,  //  000C  GETNGBL	R3	K4
      0x8C0C0705,  //  000D  GETMET	R3	R3	K5
      0x5C140400,  //  000E  MOVE	R5	R2
      0x7C0C0400,  //  000F  CALL	R3	2
      0x80040600,  //  0010  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_failed
********************************************************************/
be_local_closure(Matter_HTTP_remote_event_http_failed,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(current_cmd),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20HTTP_X20failed),
    /* K4   */  be_const_int(3),
    /* K5   */  be_nested_str_weak(dispatch_cb),
    /* K6   */  be_nested_str_weak(http_status),
    }),
    be_str_weak(event_http_failed),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060000,  //  0003  JMPF	R1	#0005
      0x80000200,  //  0004  RET	0
      0xB8060200,  //  0005  GETNGBL	R1	K1
      0x8C040302,  //  0006  GETMET	R1	R1	K2
      0x580C0003,  //  0007  LDCONST	R3	K3
      0x58100004,  //  0008  LDCONST	R4	K4
      0x7C040600,  //  0009  CALL	R1	3
      0x8C040105,  //  000A  GETMET	R1	R0	K5
      0x880C0106,  //  000B  GETMBR	R3	R0	K6
      0x4C100000,  //  000C  LDNIL	R4
      0x7C040600,  //  000D  CALL	R1	3
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_status_response
********************************************************************/
be_local_closure(Matter_HTTP_remote_parse_status_response,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(device_is_alive),
    /* K2   */  be_nested_str_weak(json),
    /* K3   */  be_nested_str_weak(load),
    /* K4   */  be_nested_str_weak(contains),
    /* K5   */  be_nested_str_weak(Status),
    /* K6   */  be_nested_str_weak(StatusFWR),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(StatusNET),
    /* K9   */  be_nested_str_weak(parse_update),
    }),
    be_str_weak(parse_status_response),
    &be_const_str_solidified,
    ( &(const binstruction[39]) {  /* code */
      0x4C100000,  //  0000  LDNIL	R4
      0x20100204,  //  0001  NE	R4	R1	R4
      0x78120022,  //  0002  JMPF	R4	#0026
      0x24100300,  //  0003  GT	R4	R1	K0
      0x78120020,  //  0004  JMPF	R4	#0026
      0x8C100101,  //  0005  GETMET	R4	R0	K1
      0x50180200,  //  0006  LDBOOL	R6	1	0
      0x7C100400,  //  0007  CALL	R4	2
      0xA4120400,  //  0008  IMPORT	R4	K2
      0x8C140903,  //  0009  GETMET	R5	R4	K3
      0x5C1C0400,  //  000A  MOVE	R7	R2
      0x7C140400,  //  000B  CALL	R5	2
      0x4C180000,  //  000C  LDNIL	R6
      0x78160017,  //  000D  JMPF	R5	#0026
      0x8C1C0B04,  //  000E  GETMET	R7	R5	K4
      0x58240005,  //  000F  LDCONST	R9	K5
      0x7C1C0400,  //  0010  CALL	R7	2
      0x781E0002,  //  0011  JMPF	R7	#0015
      0x94140B05,  //  0012  GETIDX	R5	R5	K5
      0x58180000,  //  0013  LDCONST	R6	K0
      0x7002000C,  //  0014  JMP		#0022
      0x8C1C0B04,  //  0015  GETMET	R7	R5	K4
      0x58240006,  //  0016  LDCONST	R9	K6
      0x7C1C0400,  //  0017  CALL	R7	2
      0x781E0002,  //  0018  JMPF	R7	#001C
      0x94140B06,  //  0019  GETIDX	R5	R5	K6
      0x58180007,  //  001A  LDCONST	R6	K7
      0x70020005,  //  001B  JMP		#0022
      0x8C1C0B04,  //  001C  GETMET	R7	R5	K4
      0x58240008,  //  001D  LDCONST	R9	K8
      0x7C1C0400,  //  001E  CALL	R7	2
      0x781E0001,  //  001F  JMPF	R7	#0022
      0x94140B08,  //  0020  GETIDX	R5	R5	K8
      0x541A0004,  //  0021  LDINT	R6	5
      0x8C1C0109,  //  0022  GETMET	R7	R0	K9
      0x5C240A00,  //  0023  MOVE	R9	R5
      0x5C280C00,  //  0024  MOVE	R10	R6
      0x7C1C0600,  //  0025  CALL	R7	3
      0x80000000,  //  0026  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_info
********************************************************************/
be_local_closure(Matter_HTTP_remote_get_info,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(info),
    }),
    be_str_weak(get_info),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x80040200,  //  0001  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_http_timeout
********************************************************************/
be_local_closure(Matter_HTTP_remote_event_http_timeout,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(current_cmd),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20HTTP_X20timeout_X20http_status_X3D_X25i_X20phase_X3D_X25i_X20tcp_status_X3D_X25i_X20size_payload_X3D_X25i),
    /* K4   */  be_nested_str_weak(http_status),
    /* K5   */  be_nested_str_weak(phase),
    /* K6   */  be_nested_str_weak(status),
    /* K7   */  be_nested_str_weak(payload),
    /* K8   */  be_const_int(3),
    /* K9   */  be_nested_str_weak(dispatch_cb),
    }),
    be_str_weak(event_http_timeout),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060000,  //  0003  JMPF	R1	#0005
      0x80000200,  //  0004  RET	0
      0xB8060200,  //  0005  GETNGBL	R1	K1
      0x8C040302,  //  0006  GETMET	R1	R1	K2
      0x600C0018,  //  0007  GETGBL	R3	G24
      0x58100003,  //  0008  LDCONST	R4	K3
      0x88140104,  //  0009  GETMBR	R5	R0	K4
      0x88180105,  //  000A  GETMBR	R6	R0	K5
      0x881C0106,  //  000B  GETMBR	R7	R0	K6
      0x6020000C,  //  000C  GETGBL	R8	G12
      0x88240107,  //  000D  GETMBR	R9	R0	K7
      0x7C200200,  //  000E  CALL	R8	1
      0x7C0C0A00,  //  000F  CALL	R3	5
      0x58100008,  //  0010  LDCONST	R4	K8
      0x7C040600,  //  0011  CALL	R1	3
      0x8C040109,  //  0012  GETMET	R1	R0	K9
      0x880C0104,  //  0013  GETMBR	R3	R0	K4
      0x4C100000,  //  0014  LDNIL	R4
      0x7C040600,  //  0015  CALL	R1	3
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: info_changed
********************************************************************/
be_local_closure(Matter_HTTP_remote_info_changed,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(save_param),
    }),
    be_str_weak(info_changed),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: scheduler
********************************************************************/
be_local_closure(Matter_HTTP_remote_scheduler,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(probe_next_timestamp_map),
    /* K1   */  be_nested_str_weak(keys),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(stop_iteration),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(time_reached),
    /* K6   */  be_nested_str_weak(tcp_connected),
    /* K7   */  be_nested_str_weak(millis),
    /* K8   */  be_nested_str_weak(probe_update_time_map),
    /* K9   */  be_nested_str_weak(probe_async),
    }),
    be_str_weak(scheduler),
    &be_const_str_solidified,
    ( &(const binstruction[66]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x60080010,  //  0001  GETGBL	R2	G16
      0x880C0100,  //  0002  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0003  GETMET	R3	R3	K1
      0x7C0C0200,  //  0004  CALL	R3	1
      0x7C080200,  //  0005  CALL	R2	1
      0xA802000A,  //  0006  EXBLK	0	#0012
      0x5C0C0400,  //  0007  MOVE	R3	R2
      0x7C0C0000,  //  0008  CALL	R3	0
      0x88100100,  //  0009  GETMBR	R4	R0	K0
      0x94100803,  //  000A  GETIDX	R4	R4	R3
      0x1C100902,  //  000B  EQ	R4	R4	K2
      0x78120001,  //  000C  JMPF	R4	#000F
      0x5C040600,  //  000D  MOVE	R1	R3
      0x70020000,  //  000E  JMP		#0010
      0x7001FFF6,  //  000F  JMP		#0007
      0xA8040001,  //  0010  EXBLK	1	1
      0x70020002,  //  0011  JMP		#0015
      0x58080003,  //  0012  LDCONST	R2	K3
      0xAC080200,  //  0013  CATCH	R2	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x4C080000,  //  0015  LDNIL	R2
      0x1C080202,  //  0016  EQ	R2	R1	R2
      0x780A0015,  //  0017  JMPF	R2	#002E
      0x60080010,  //  0018  GETGBL	R2	G16
      0x880C0100,  //  0019  GETMBR	R3	R0	K0
      0x8C0C0701,  //  001A  GETMET	R3	R3	K1
      0x7C0C0200,  //  001B  CALL	R3	1
      0x7C080200,  //  001C  CALL	R2	1
      0xA802000C,  //  001D  EXBLK	0	#002B
      0x5C0C0400,  //  001E  MOVE	R3	R2
      0x7C0C0000,  //  001F  CALL	R3	0
      0xB8120800,  //  0020  GETNGBL	R4	K4
      0x8C100905,  //  0021  GETMET	R4	R4	K5
      0x88180100,  //  0022  GETMBR	R6	R0	K0
      0x94180C03,  //  0023  GETIDX	R6	R6	R3
      0x7C100400,  //  0024  CALL	R4	2
      0x78120001,  //  0025  JMPF	R4	#0028
      0x5C040600,  //  0026  MOVE	R1	R3
      0x70020000,  //  0027  JMP		#0029
      0x7001FFF4,  //  0028  JMP		#001E
      0xA8040001,  //  0029  EXBLK	1	1
      0x70020002,  //  002A  JMP		#002E
      0x58080003,  //  002B  LDCONST	R2	K3
      0xAC080200,  //  002C  CATCH	R2	1	0
      0xB0080000,  //  002D  RAISE	2	R0	R0
      0x4C080000,  //  002E  LDNIL	R2
      0x1C080202,  //  002F  EQ	R2	R1	R2
      0x780A0000,  //  0030  JMPF	R2	#0032
      0x80000400,  //  0031  RET	0
      0x88080106,  //  0032  GETMBR	R2	R0	K6
      0x780A0002,  //  0033  JMPF	R2	#0037
      0x88080100,  //  0034  GETMBR	R2	R0	K0
      0x98080302,  //  0035  SETIDX	R2	R1	K2
      0x80000400,  //  0036  RET	0
      0x88080100,  //  0037  GETMBR	R2	R0	K0
      0xB80E0800,  //  0038  GETNGBL	R3	K4
      0x8C0C0707,  //  0039  GETMET	R3	R3	K7
      0x88140108,  //  003A  GETMBR	R5	R0	K8
      0x94140A01,  //  003B  GETIDX	R5	R5	R1
      0x7C0C0400,  //  003C  CALL	R3	2
      0x98080203,  //  003D  SETIDX	R2	R1	R3
      0x8C080109,  //  003E  GETMET	R2	R0	K9
      0x5C100200,  //  003F  MOVE	R4	R1
      0x7C080400,  //  0040  CALL	R2	2
      0x80000000,  //  0041  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_schedule
********************************************************************/
be_local_closure(Matter_HTTP_remote_add_schedule,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(probe_update_time_map),
    /* K1   */  be_nested_str_weak(contains),
    /* K2   */  be_nested_str_weak(probe_next_timestamp_map),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(jitter),
    /* K5   */  be_nested_str_weak(add_async_cb),
    }),
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
** Solidified function: probe_async
********************************************************************/
be_local_closure(Matter_HTTP_remote_probe_async,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(webserver),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(wifi),
    /* K4   */  be_nested_str_weak(up),
    /* K5   */  be_nested_str_weak(eth),
    /* K6   */  be_nested_str_weak(current_cmd),
    /* K7   */  be_nested_str_weak(_X2Fcm_X3Fcmnd_X3D),
    /* K8   */  be_nested_str_weak(tr),
    /* K9   */  be_nested_str_weak(_X20),
    /* K10  */  be_nested_str_weak(_X2B),
    /* K11  */  be_nested_str_weak(log),
    /* K12  */  be_nested_str_weak(MTR_X3A_X20HTTP_X20async_X20request_X20_X27http_X3A_X2F_X2F_X25s_X3A_X25i_X25s_X27),
    /* K13  */  be_nested_str_weak(addr),
    /* K14  */  be_nested_str_weak(port),
    /* K15  */  be_const_int(3),
    /* K16  */  be_nested_str_weak(begin),
    }),
    be_str_weak(probe_async),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xB8120400,  //  0002  GETNGBL	R4	K2
      0x8C100903,  //  0003  GETMET	R4	R4	K3
      0x7C100200,  //  0004  CALL	R4	1
      0x94100904,  //  0005  GETIDX	R4	R4	K4
      0x74120006,  //  0006  JMPT	R4	#000E
      0xB8120400,  //  0007  GETNGBL	R4	K2
      0x8C100905,  //  0008  GETMET	R4	R4	K5
      0x7C100200,  //  0009  CALL	R4	1
      0x94100904,  //  000A  GETIDX	R4	R4	K4
      0x74120001,  //  000B  JMPT	R4	#000E
      0x4C100000,  //  000C  LDNIL	R4
      0x80040800,  //  000D  RET	1	R4
      0x90020C01,  //  000E  SETMBR	R0	K6	R1
      0x8C100508,  //  000F  GETMET	R4	R2	K8
      0x5C180200,  //  0010  MOVE	R6	R1
      0x581C0009,  //  0011  LDCONST	R7	K9
      0x5820000A,  //  0012  LDCONST	R8	K10
      0x7C100800,  //  0013  CALL	R4	4
      0x00120E04,  //  0014  ADD	R4	K7	R4
      0xB8160400,  //  0015  GETNGBL	R5	K2
      0x8C140B0B,  //  0016  GETMET	R5	R5	K11
      0x601C0018,  //  0017  GETGBL	R7	G24
      0x5820000C,  //  0018  LDCONST	R8	K12
      0x8824010D,  //  0019  GETMBR	R9	R0	K13
      0x8828010E,  //  001A  GETMBR	R10	R0	K14
      0x5C2C0800,  //  001B  MOVE	R11	R4
      0x7C1C0800,  //  001C  CALL	R7	4
      0x5820000F,  //  001D  LDCONST	R8	K15
      0x7C140600,  //  001E  CALL	R5	3
      0x8C140110,  //  001F  GETMET	R5	R0	K16
      0x5C1C0800,  //  0020  MOVE	R7	R4
      0x7C140400,  //  0021  CALL	R5	2
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_async_cb
********************************************************************/
be_local_closure(Matter_HTTP_remote_add_async_cb,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(async_cb_map),
    }),
    be_str_weak(add_async_cb),
    &be_const_str_solidified,
    ( &(const binstruction[ 3]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x980C0202,  //  0001  SETIDX	R3	R1	R2
      0x80000000,  //  0002  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: set_info
********************************************************************/
be_local_closure(Matter_HTTP_remote_set_info,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(info),
    }),
    be_str_weak(set_info),
    &be_const_str_solidified,
    ( &(const binstruction[ 2]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x80000000,  //  0001  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: dispatch_cb
********************************************************************/
be_local_closure(Matter_HTTP_remote_dispatch_cb,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(async_cb_map),
    /* K2   */  be_nested_str_weak(keys),
    /* K3   */  be_nested_str_weak(current_cmd),
    /* K4   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(dispatch_cb),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x580C0000,  //  0000  LDCONST	R3	K0
      0x60100010,  //  0001  GETGBL	R4	G16
      0x88140101,  //  0002  GETMBR	R5	R0	K1
      0x8C140B02,  //  0003  GETMET	R5	R5	K2
      0x7C140200,  //  0004  CALL	R5	1
      0x7C100200,  //  0005  CALL	R4	1
      0xA802000F,  //  0006  EXBLK	0	#0017
      0x5C140800,  //  0007  MOVE	R5	R4
      0x7C140000,  //  0008  CALL	R5	0
      0x88180101,  //  0009  GETMBR	R6	R0	K1
      0x94180C05,  //  000A  GETIDX	R6	R6	R5
      0x881C0103,  //  000B  GETMBR	R7	R0	K3
      0x1C1C0C07,  //  000C  EQ	R7	R6	R7
      0x741E0002,  //  000D  JMPT	R7	#0011
      0x4C1C0000,  //  000E  LDNIL	R7
      0x1C1C0C07,  //  000F  EQ	R7	R6	R7
      0x781E0004,  //  0010  JMPF	R7	#0016
      0x5C1C0A00,  //  0011  MOVE	R7	R5
      0x5C200200,  //  0012  MOVE	R8	R1
      0x5C240400,  //  0013  MOVE	R9	R2
      0x88280103,  //  0014  GETMBR	R10	R0	K3
      0x7C1C0600,  //  0015  CALL	R7	3
      0x7001FFEF,  //  0016  JMP		#0007
      0x58100004,  //  0017  LDCONST	R4	K4
      0xAC100200,  //  0018  CATCH	R4	1	0
      0xB0080000,  //  0019  RAISE	2	R0	R0
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: change_schedule
********************************************************************/
be_local_closure(Matter_HTTP_remote_change_schedule,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(probe_update_time_map),
    /* K1   */  be_nested_str_weak(contains),
    /* K2   */  be_nested_str_weak(probe_next_timestamp_map),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(jitter),
    }),
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
** Solidified function: init
********************************************************************/
be_local_closure(Matter_HTTP_remote_init,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 3]) {
      be_nested_proto(
        8,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(parse_status_response),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 7]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x8C0C0700,  //  0001  GETMET	R3	R3	K0
          0x5C140000,  //  0002  MOVE	R5	R0
          0x5C180200,  //  0003  MOVE	R6	R1
          0x5C1C0400,  //  0004  MOVE	R7	R2
          0x7C0C0800,  //  0005  CALL	R3	4
          0x80040600,  //  0006  RET	1	R3
        })
      ),
      be_nested_proto(
        8,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(parse_status_response),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 7]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x8C0C0700,  //  0001  GETMET	R3	R3	K0
          0x5C140000,  //  0002  MOVE	R5	R0
          0x5C180200,  //  0003  MOVE	R6	R1
          0x5C1C0400,  //  0004  MOVE	R7	R2
          0x7C0C0800,  //  0005  CALL	R3	4
          0x80040600,  //  0006  RET	1	R3
        })
      ),
      be_nested_proto(
        8,                          /* nstack */
        3,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(parse_status_response),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 7]) {  /* code */
          0x680C0000,  //  0000  GETUPV	R3	U0
          0x8C0C0700,  //  0001  GETMET	R3	R3	K0
          0x5C140000,  //  0002  MOVE	R5	R0
          0x5C180200,  //  0003  MOVE	R6	R1
          0x5C1C0400,  //  0004  MOVE	R7	R2
          0x7C0C0800,  //  0005  CALL	R3	4
          0x80040600,  //  0006  RET	1	R3
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(device),
    /* K1   */  be_nested_str_weak(probe_update_time_map),
    /* K2   */  be_nested_str_weak(probe_next_timestamp_map),
    /* K3   */  be_nested_str_weak(async_cb_map),
    /* K4   */  be_nested_str_weak(current_cmd),
    /* K5   */  be_nested_str_weak(reachable),
    /* K6   */  be_nested_str_weak(init),
    /* K7   */  be_nested_str_weak(info),
    /* K8   */  be_nested_str_weak(add_schedule),
    /* K9   */  be_nested_str_weak(UPDATE_CMD0),
    /* K10  */  be_nested_str_weak(UPDATE_TIME),
    /* K11  */  be_nested_str_weak(UPDATE_CMD2),
    /* K12  */  be_nested_str_weak(UPDATE_CMD5),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x90020001,  //  0000  SETMBR	R0	K0	R1
      0x60140013,  //  0001  GETGBL	R5	G19
      0x7C140000,  //  0002  CALL	R5	0
      0x90020205,  //  0003  SETMBR	R0	K1	R5
      0x60140013,  //  0004  GETGBL	R5	G19
      0x7C140000,  //  0005  CALL	R5	0
      0x90020405,  //  0006  SETMBR	R0	K2	R5
      0x60140013,  //  0007  GETGBL	R5	G19
      0x7C140000,  //  0008  CALL	R5	0
      0x90020605,  //  0009  SETMBR	R0	K3	R5
      0x4C140000,  //  000A  LDNIL	R5
      0x90020805,  //  000B  SETMBR	R0	K4	R5
      0x50140000,  //  000C  LDBOOL	R5	0	0
      0x90020A05,  //  000D  SETMBR	R0	K5	R5
      0x60140003,  //  000E  GETGBL	R5	G3
      0x5C180000,  //  000F  MOVE	R6	R0
      0x7C140200,  //  0010  CALL	R5	1
      0x8C140B06,  //  0011  GETMET	R5	R5	K6
      0x5C1C0400,  //  0012  MOVE	R7	R2
      0x5422004F,  //  0013  LDINT	R8	80
      0x5C240600,  //  0014  MOVE	R9	R3
      0x5C280800,  //  0015  MOVE	R10	R4
      0x7C140A00,  //  0016  CALL	R5	5
      0x60140013,  //  0017  GETGBL	R5	G19
      0x7C140000,  //  0018  CALL	R5	0
      0x90020E05,  //  0019  SETMBR	R0	K7	R5
      0x88140100,  //  001A  GETMBR	R5	R0	K0
      0x7816000E,  //  001B  JMPF	R5	#002B
      0x8C140108,  //  001C  GETMET	R5	R0	K8
      0x881C0109,  //  001D  GETMBR	R7	R0	K9
      0x8820010A,  //  001E  GETMBR	R8	R0	K10
      0x84240000,  //  001F  CLOSURE	R9	P0
      0x7C140800,  //  0020  CALL	R5	4
      0x8C140108,  //  0021  GETMET	R5	R0	K8
      0x881C010B,  //  0022  GETMBR	R7	R0	K11
      0x8820010A,  //  0023  GETMBR	R8	R0	K10
      0x84240001,  //  0024  CLOSURE	R9	P1
      0x7C140800,  //  0025  CALL	R5	4
      0x8C140108,  //  0026  GETMET	R5	R0	K8
      0x881C010C,  //  0027  GETMBR	R7	R0	K12
      0x8820010A,  //  0028  GETMBR	R8	R0	K10
      0x84240002,  //  0029  CLOSURE	R9	P2
      0x7C140800,  //  002A  CALL	R5	4
      0xA0000000,  //  002B  CLOSE	R0
      0x80000000,  //  002C  RET	0
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
    be_nested_map(31,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(device_is_alive, -1), be_const_closure(Matter_HTTP_remote_device_is_alive_closure) },
        { be_const_key_weak(reachable, 21), be_const_var(5) },
        { be_const_key_weak(parse_update, -1), be_const_closure(Matter_HTTP_remote_parse_update_closure) },
        { be_const_key_weak(UPDATE_TIME, 7), be_const_int(5000) },
        { be_const_key_weak(web_last_seen, 30), be_const_closure(Matter_HTTP_remote_web_last_seen_closure) },
        { be_const_key_weak(call_sync, 4), be_const_closure(Matter_HTTP_remote_call_sync_closure) },
        { be_const_key_weak(change_schedule, 19), be_const_closure(Matter_HTTP_remote_change_schedule_closure) },
        { be_const_key_weak(device, -1), be_const_var(0) },
        { be_const_key_weak(UPDATE_CMD5, -1), be_nested_str_weak(Status_X205) },
        { be_const_key_weak(get_info, 17), be_const_closure(Matter_HTTP_remote_get_info_closure) },
        { be_const_key_weak(info, -1), be_const_var(7) },
        { be_const_key_weak(event_http_timeout, -1), be_const_closure(Matter_HTTP_remote_event_http_timeout_closure) },
        { be_const_key_weak(info_changed, -1), be_const_closure(Matter_HTTP_remote_info_changed_closure) },
        { be_const_key_weak(add_schedule, -1), be_const_closure(Matter_HTTP_remote_add_schedule_closure) },
        { be_const_key_weak(UPDATE_TIME2, -1), be_const_int(300000) },
        { be_const_key_weak(scheduler, -1), be_const_closure(Matter_HTTP_remote_scheduler_closure) },
        { be_const_key_weak(parse_status_response, 13), be_const_closure(Matter_HTTP_remote_parse_status_response_closure) },
        { be_const_key_weak(probe_async, 18), be_const_closure(Matter_HTTP_remote_probe_async_closure) },
        { be_const_key_weak(probe_update_time_map, -1), be_const_var(1) },
        { be_const_key_weak(reachable_utc, -1), be_const_var(6) },
        { be_const_key_weak(probe_next_timestamp_map, -1), be_const_var(2) },
        { be_const_key_weak(async_cb_map, -1), be_const_var(3) },
        { be_const_key_weak(current_cmd, -1), be_const_var(4) },
        { be_const_key_weak(set_info, -1), be_const_closure(Matter_HTTP_remote_set_info_closure) },
        { be_const_key_weak(dispatch_cb, -1), be_const_closure(Matter_HTTP_remote_dispatch_cb_closure) },
        { be_const_key_weak(UPDATE_CMD2, -1), be_nested_str_weak(Status_X202) },
        { be_const_key_weak(UPDATE_CMD0, -1), be_nested_str_weak(Status) },
        { be_const_key_weak(event_http_failed, 3), be_const_closure(Matter_HTTP_remote_event_http_failed_closure) },
        { be_const_key_weak(add_async_cb, 6), be_const_closure(Matter_HTTP_remote_add_async_cb_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_HTTP_remote_init_closure) },
        { be_const_key_weak(event_http_finished, -1), be_const_closure(Matter_HTTP_remote_event_http_finished_closure) },
    })),
    be_str_weak(Matter_HTTP_remote)
);
/*******************************************************************/

void be_load_Matter_HTTP_remote_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_HTTP_remote);
    be_setglobal(vm, "Matter_HTTP_remote");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
