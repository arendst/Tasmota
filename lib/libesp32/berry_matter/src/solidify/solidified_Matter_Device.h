/* Solidification of Matter_Device.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Device;

/********************************************************************
** Solidified function: is_commissioning_open
********************************************************************/
be_local_closure(Matter_Device_is_commissioning_open,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 1]) {     /* constants */
    /* K0   */  be_nested_str_weak(commissioning_open),
    }),
    be_str_weak(is_commissioning_open),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _mdns_announce_hostname
********************************************************************/
be_local_closure(Matter_Device__mdns_announce_hostname,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[27]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(start),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(eth),
    /* K5   */  be_nested_str_weak(hostname_eth),
    /* K6   */  be_nested_str_weak(replace),
    /* K7   */  be_nested_str_weak(find),
    /* K8   */  be_nested_str_weak(mac),
    /* K9   */  be_nested_str_weak(_X3A),
    /* K10  */  be_nested_str_weak(),
    /* K11  */  be_nested_str_weak(ipv4only),
    /* K12  */  be_nested_str_weak(add_hostname),
    /* K13  */  be_nested_str_weak(ip6local),
    /* K14  */  be_nested_str_weak(ip),
    /* K15  */  be_nested_str_weak(ip6),
    /* K16  */  be_nested_str_weak(log),
    /* K17  */  be_nested_str_weak(format),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20calling_X20mdns_X2Eadd_hostname_X28_X25s_X2C_X20_X25s_X29),
    /* K19  */  be_const_int(3),
    /* K20  */  be_nested_str_weak(wifi),
    /* K21  */  be_nested_str_weak(hostname_wifi),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20start_X20mDNS_X20on_X20_X25s_X20host_X20_X27_X25s_X2Elocal_X27),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K24  */  be_nested_str_weak(_X7C),
    /* K25  */  be_const_int(2),
    /* K26  */  be_nested_str_weak(mdns_announce_op_discovery_all_fabrics),
    }),
    be_str_weak(_mdns_announce_hostname),
    &be_const_str_solidified,
    ( &(const binstruction[140]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x8C100502,  //  0002  GETMET	R4	R2	K2
      0x7C100200,  //  0003  CALL	R4	1
      0xA8020072,  //  0004  EXBLK	0	#0078
      0x78060030,  //  0005  JMPF	R1	#0037
      0xB8120600,  //  0006  GETNGBL	R4	K3
      0x8C100904,  //  0007  GETMET	R4	R4	K4
      0x7C100200,  //  0008  CALL	R4	1
      0x8C140706,  //  0009  GETMET	R5	R3	K6
      0x8C1C0907,  //  000A  GETMET	R7	R4	K7
      0x58240008,  //  000B  LDCONST	R9	K8
      0x7C1C0400,  //  000C  CALL	R7	2
      0x58200009,  //  000D  LDCONST	R8	K9
      0x5824000A,  //  000E  LDCONST	R9	K10
      0x7C140800,  //  000F  CALL	R5	4
      0x90020A05,  //  0010  SETMBR	R0	K5	R5
      0x8814010B,  //  0011  GETMBR	R5	R0	K11
      0x7416000F,  //  0012  JMPT	R5	#0023
      0x8C14050C,  //  0013  GETMET	R5	R2	K12
      0x881C0105,  //  0014  GETMBR	R7	R0	K5
      0x8C200907,  //  0015  GETMET	R8	R4	K7
      0x5828000D,  //  0016  LDCONST	R10	K13
      0x582C000A,  //  0017  LDCONST	R11	K10
      0x7C200600,  //  0018  CALL	R8	3
      0x8C240907,  //  0019  GETMET	R9	R4	K7
      0x582C000E,  //  001A  LDCONST	R11	K14
      0x5830000A,  //  001B  LDCONST	R12	K10
      0x7C240600,  //  001C  CALL	R9	3
      0x8C280907,  //  001D  GETMET	R10	R4	K7
      0x5830000F,  //  001E  LDCONST	R12	K15
      0x5834000A,  //  001F  LDCONST	R13	K10
      0x7C280600,  //  0020  CALL	R10	3
      0x7C140A00,  //  0021  CALL	R5	5
      0x70020012,  //  0022  JMP		#0036
      0xB8160600,  //  0023  GETNGBL	R5	K3
      0x8C140B10,  //  0024  GETMET	R5	R5	K16
      0x8C1C0711,  //  0025  GETMET	R7	R3	K17
      0x58240012,  //  0026  LDCONST	R9	K18
      0x88280105,  //  0027  GETMBR	R10	R0	K5
      0x8C2C0907,  //  0028  GETMET	R11	R4	K7
      0x5834000E,  //  0029  LDCONST	R13	K14
      0x5838000A,  //  002A  LDCONST	R14	K10
      0x7C2C0600,  //  002B  CALL	R11	3
      0x7C1C0800,  //  002C  CALL	R7	4
      0x58200013,  //  002D  LDCONST	R8	K19
      0x7C140600,  //  002E  CALL	R5	3
      0x8C14050C,  //  002F  GETMET	R5	R2	K12
      0x881C0105,  //  0030  GETMBR	R7	R0	K5
      0x8C200907,  //  0031  GETMET	R8	R4	K7
      0x5828000E,  //  0032  LDCONST	R10	K14
      0x582C000A,  //  0033  LDCONST	R11	K10
      0x7C200600,  //  0034  CALL	R8	3
      0x7C140600,  //  0035  CALL	R5	3
      0x7002002F,  //  0036  JMP		#0067
      0xB8120600,  //  0037  GETNGBL	R4	K3
      0x8C100914,  //  0038  GETMET	R4	R4	K20
      0x7C100200,  //  0039  CALL	R4	1
      0x8C140706,  //  003A  GETMET	R5	R3	K6
      0x8C1C0907,  //  003B  GETMET	R7	R4	K7
      0x58240008,  //  003C  LDCONST	R9	K8
      0x7C1C0400,  //  003D  CALL	R7	2
      0x58200009,  //  003E  LDCONST	R8	K9
      0x5824000A,  //  003F  LDCONST	R9	K10
      0x7C140800,  //  0040  CALL	R5	4
      0x90022A05,  //  0041  SETMBR	R0	K21	R5
      0x8814010B,  //  0042  GETMBR	R5	R0	K11
      0x7416000F,  //  0043  JMPT	R5	#0054
      0x8C14050C,  //  0044  GETMET	R5	R2	K12
      0x881C0115,  //  0045  GETMBR	R7	R0	K21
      0x8C200907,  //  0046  GETMET	R8	R4	K7
      0x5828000D,  //  0047  LDCONST	R10	K13
      0x582C000A,  //  0048  LDCONST	R11	K10
      0x7C200600,  //  0049  CALL	R8	3
      0x8C240907,  //  004A  GETMET	R9	R4	K7
      0x582C000E,  //  004B  LDCONST	R11	K14
      0x5830000A,  //  004C  LDCONST	R12	K10
      0x7C240600,  //  004D  CALL	R9	3
      0x8C280907,  //  004E  GETMET	R10	R4	K7
      0x5830000F,  //  004F  LDCONST	R12	K15
      0x5834000A,  //  0050  LDCONST	R13	K10
      0x7C280600,  //  0051  CALL	R10	3
      0x7C140A00,  //  0052  CALL	R5	5
      0x70020012,  //  0053  JMP		#0067
      0xB8160600,  //  0054  GETNGBL	R5	K3
      0x8C140B10,  //  0055  GETMET	R5	R5	K16
      0x8C1C0711,  //  0056  GETMET	R7	R3	K17
      0x58240012,  //  0057  LDCONST	R9	K18
      0x88280105,  //  0058  GETMBR	R10	R0	K5
      0x8C2C0907,  //  0059  GETMET	R11	R4	K7
      0x5834000E,  //  005A  LDCONST	R13	K14
      0x5838000A,  //  005B  LDCONST	R14	K10
      0x7C2C0600,  //  005C  CALL	R11	3
      0x7C1C0800,  //  005D  CALL	R7	4
      0x58200013,  //  005E  LDCONST	R8	K19
      0x7C140600,  //  005F  CALL	R5	3
      0x8C14050C,  //  0060  GETMET	R5	R2	K12
      0x881C0115,  //  0061  GETMBR	R7	R0	K21
      0x8C200907,  //  0062  GETMET	R8	R4	K7
      0x5828000E,  //  0063  LDCONST	R10	K14
      0x582C000A,  //  0064  LDCONST	R11	K10
      0x7C200600,  //  0065  CALL	R8	3
      0x7C140600,  //  0066  CALL	R5	3
      0xB8120600,  //  0067  GETNGBL	R4	K3
      0x8C100910,  //  0068  GETMET	R4	R4	K16
      0x8C180711,  //  0069  GETMET	R6	R3	K17
      0x58200016,  //  006A  LDCONST	R8	K22
      0x78060001,  //  006B  JMPF	R1	#006E
      0x58240004,  //  006C  LDCONST	R9	K4
      0x70020000,  //  006D  JMP		#006F
      0x58240014,  //  006E  LDCONST	R9	K20
      0x78060001,  //  006F  JMPF	R1	#0072
      0x88280105,  //  0070  GETMBR	R10	R0	K5
      0x70020000,  //  0071  JMP		#0073
      0x88280115,  //  0072  GETMBR	R10	R0	K21
      0x7C180800,  //  0073  CALL	R6	4
      0x581C0013,  //  0074  LDCONST	R7	K19
      0x7C100600,  //  0075  CALL	R4	3
      0xA8040001,  //  0076  EXBLK	1	1
      0x70020010,  //  0077  JMP		#0089
      0xAC100002,  //  0078  CATCH	R4	0	2
      0x7002000D,  //  0079  JMP		#0088
      0xB81A0600,  //  007A  GETNGBL	R6	K3
      0x8C180D10,  //  007B  GETMET	R6	R6	K16
      0x60200008,  //  007C  GETGBL	R8	G8
      0x5C240800,  //  007D  MOVE	R9	R4
      0x7C200200,  //  007E  CALL	R8	1
      0x00222E08,  //  007F  ADD	R8	K23	R8
      0x00201118,  //  0080  ADD	R8	R8	K24
      0x60240008,  //  0081  GETGBL	R9	G8
      0x5C280A00,  //  0082  MOVE	R10	R5
      0x7C240200,  //  0083  CALL	R9	1
      0x00201009,  //  0084  ADD	R8	R8	R9
      0x58240019,  //  0085  LDCONST	R9	K25
      0x7C180600,  //  0086  CALL	R6	3
      0x70020000,  //  0087  JMP		#0089
      0xB0080000,  //  0088  RAISE	2	R0	R0
      0x8C10011A,  //  0089  GETMET	R4	R0	K26
      0x7C100200,  //  008A  CALL	R4	1
      0x80000000,  //  008B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save_before_restart
********************************************************************/
be_local_closure(Matter_Device_save_before_restart,   /* name */
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
    /* K0   */  be_nested_str_weak(stop_basic_commissioning),
    /* K1   */  be_nested_str_weak(mdns_remove_op_discovery_all_fabrics),
    }),
    be_str_weak(save_before_restart),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x8C040101,  //  0002  GETMET	R1	R0	K1
      0x7C040200,  //  0003  CALL	R1	1
      0x80000000,  //  0004  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_remove_op_discovery
********************************************************************/
be_local_closure(Matter_Device_mdns_remove_op_discovery,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[25]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(get_device_id),
    /* K3   */  be_nested_str_weak(copy),
    /* K4   */  be_nested_str_weak(reverse),
    /* K5   */  be_nested_str_weak(get_fabric_compressed),
    /* K6   */  be_nested_str_weak(tohex),
    /* K7   */  be_nested_str_weak(_X2D),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(eth),
    /* K10  */  be_nested_str_weak(find),
    /* K11  */  be_nested_str_weak(up),
    /* K12  */  be_nested_str_weak(log),
    /* K13  */  be_nested_str_weak(format),
    /* K14  */  be_nested_str_weak(MTR_X3A_X20remove_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27),
    /* K15  */  be_const_int(3),
    /* K16  */  be_nested_str_weak(remove_service),
    /* K17  */  be_nested_str_weak(_matter),
    /* K18  */  be_nested_str_weak(_tcp),
    /* K19  */  be_nested_str_weak(hostname_eth),
    /* K20  */  be_nested_str_weak(wifi),
    /* K21  */  be_nested_str_weak(hostname_wifi),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K23  */  be_nested_str_weak(_X7C),
    /* K24  */  be_const_int(2),
    }),
    be_str_weak(mdns_remove_op_discovery),
    &be_const_str_solidified,
    ( &(const binstruction[81]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xA802003B,  //  0002  EXBLK	0	#003F
      0x8C100302,  //  0003  GETMET	R4	R1	K2
      0x7C100200,  //  0004  CALL	R4	1
      0x8C100903,  //  0005  GETMET	R4	R4	K3
      0x7C100200,  //  0006  CALL	R4	1
      0x8C100904,  //  0007  GETMET	R4	R4	K4
      0x7C100200,  //  0008  CALL	R4	1
      0x8C140305,  //  0009  GETMET	R5	R1	K5
      0x7C140200,  //  000A  CALL	R5	1
      0x8C180B06,  //  000B  GETMET	R6	R5	K6
      0x7C180200,  //  000C  CALL	R6	1
      0x00180D07,  //  000D  ADD	R6	R6	K7
      0x8C1C0906,  //  000E  GETMET	R7	R4	K6
      0x7C1C0200,  //  000F  CALL	R7	1
      0x00180C07,  //  0010  ADD	R6	R6	R7
      0xB81E1000,  //  0011  GETNGBL	R7	K8
      0x8C1C0F09,  //  0012  GETMET	R7	R7	K9
      0x7C1C0200,  //  0013  CALL	R7	1
      0x8C1C0F0A,  //  0014  GETMET	R7	R7	K10
      0x5824000B,  //  0015  LDCONST	R9	K11
      0x7C1C0400,  //  0016  CALL	R7	2
      0x781E000E,  //  0017  JMPF	R7	#0027
      0xB81E1000,  //  0018  GETNGBL	R7	K8
      0x8C1C0F0C,  //  0019  GETMET	R7	R7	K12
      0x8C24070D,  //  001A  GETMET	R9	R3	K13
      0x582C000E,  //  001B  LDCONST	R11	K14
      0x58300009,  //  001C  LDCONST	R12	K9
      0x5C340C00,  //  001D  MOVE	R13	R6
      0x7C240800,  //  001E  CALL	R9	4
      0x5828000F,  //  001F  LDCONST	R10	K15
      0x7C1C0600,  //  0020  CALL	R7	3
      0x8C1C0510,  //  0021  GETMET	R7	R2	K16
      0x58240011,  //  0022  LDCONST	R9	K17
      0x58280012,  //  0023  LDCONST	R10	K18
      0x5C2C0C00,  //  0024  MOVE	R11	R6
      0x88300113,  //  0025  GETMBR	R12	R0	K19
      0x7C1C0A00,  //  0026  CALL	R7	5
      0xB81E1000,  //  0027  GETNGBL	R7	K8
      0x8C1C0F14,  //  0028  GETMET	R7	R7	K20
      0x7C1C0200,  //  0029  CALL	R7	1
      0x8C1C0F0A,  //  002A  GETMET	R7	R7	K10
      0x5824000B,  //  002B  LDCONST	R9	K11
      0x7C1C0400,  //  002C  CALL	R7	2
      0x781E000E,  //  002D  JMPF	R7	#003D
      0xB81E1000,  //  002E  GETNGBL	R7	K8
      0x8C1C0F0C,  //  002F  GETMET	R7	R7	K12
      0x8C24070D,  //  0030  GETMET	R9	R3	K13
      0x582C000E,  //  0031  LDCONST	R11	K14
      0x58300014,  //  0032  LDCONST	R12	K20
      0x5C340C00,  //  0033  MOVE	R13	R6
      0x7C240800,  //  0034  CALL	R9	4
      0x5828000F,  //  0035  LDCONST	R10	K15
      0x7C1C0600,  //  0036  CALL	R7	3
      0x8C1C0510,  //  0037  GETMET	R7	R2	K16
      0x58240011,  //  0038  LDCONST	R9	K17
      0x58280012,  //  0039  LDCONST	R10	K18
      0x5C2C0C00,  //  003A  MOVE	R11	R6
      0x88300115,  //  003B  GETMBR	R12	R0	K21
      0x7C1C0A00,  //  003C  CALL	R7	5
      0xA8040001,  //  003D  EXBLK	1	1
      0x70020010,  //  003E  JMP		#0050
      0xAC100002,  //  003F  CATCH	R4	0	2
      0x7002000D,  //  0040  JMP		#004F
      0xB81A1000,  //  0041  GETNGBL	R6	K8
      0x8C180D0C,  //  0042  GETMET	R6	R6	K12
      0x60200008,  //  0043  GETGBL	R8	G8
      0x5C240800,  //  0044  MOVE	R9	R4
      0x7C200200,  //  0045  CALL	R8	1
      0x00222C08,  //  0046  ADD	R8	K22	R8
      0x00201117,  //  0047  ADD	R8	R8	K23
      0x60240008,  //  0048  GETGBL	R9	G8
      0x5C280A00,  //  0049  MOVE	R10	R5
      0x7C240200,  //  004A  CALL	R9	1
      0x00201009,  //  004B  ADD	R8	R8	R9
      0x58240018,  //  004C  LDCONST	R9	K24
      0x7C180600,  //  004D  CALL	R6	3
      0x70020000,  //  004E  JMP		#0050
      0xB0080000,  //  004F  RAISE	2	R0	R0
      0x80000000,  //  0050  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_Device_every_second,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(every_second),
    /* K2   */  be_nested_str_weak(message_handler),
    /* K3   */  be_nested_str_weak(commissioning_open),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(time_reached),
    }),
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x88040102,  //  0003  GETMBR	R1	R0	K2
      0x8C040301,  //  0004  GETMET	R1	R1	K1
      0x7C040200,  //  0005  CALL	R1	1
      0x88040103,  //  0006  GETMBR	R1	R0	K3
      0x4C080000,  //  0007  LDNIL	R2
      0x20040202,  //  0008  NE	R1	R1	R2
      0x78060006,  //  0009  JMPF	R1	#0011
      0xB8060800,  //  000A  GETNGBL	R1	K4
      0x8C040305,  //  000B  GETMET	R1	R1	K5
      0x880C0103,  //  000C  GETMBR	R3	R0	K3
      0x7C040400,  //  000D  CALL	R1	2
      0x78060001,  //  000E  JMPF	R1	#0011
      0x4C040000,  //  000F  LDNIL	R1
      0x90020601,  //  0010  SETMBR	R0	K3	R1
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_announce_op_discovery_all_fabrics
********************************************************************/
be_local_closure(Matter_Device_mdns_announce_op_discovery_all_fabrics,   /* name */
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
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(active_fabrics),
    /* K2   */  be_nested_str_weak(get_device_id),
    /* K3   */  be_nested_str_weak(get_fabric_id),
    /* K4   */  be_nested_str_weak(mdns_announce_op_discovery),
    /* K5   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(mdns_announce_op_discovery_all_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x8C080501,  //  0002  GETMET	R2	R2	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x7C040200,  //  0004  CALL	R1	1
      0xA802000B,  //  0005  EXBLK	0	#0012
      0x5C080200,  //  0006  MOVE	R2	R1
      0x7C080000,  //  0007  CALL	R2	0
      0x8C0C0502,  //  0008  GETMET	R3	R2	K2
      0x7C0C0200,  //  0009  CALL	R3	1
      0x780E0005,  //  000A  JMPF	R3	#0011
      0x8C0C0503,  //  000B  GETMET	R3	R2	K3
      0x7C0C0200,  //  000C  CALL	R3	1
      0x780E0002,  //  000D  JMPF	R3	#0011
      0x8C0C0104,  //  000E  GETMET	R3	R0	K4
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x7001FFF3,  //  0011  JMP		#0006
      0x58040005,  //  0012  LDCONST	R1	K5
      0xAC040200,  //  0013  CATCH	R1	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: autoconf_sensors_list
********************************************************************/
be_local_closure(Matter_Device_autoconf_sensors_list,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(k2l),
    /* K1   */  be_nested_str_weak(contains),
    /* K2   */  be_nested_str_weak(Temperature),
    /* K3   */  be_nested_str_weak(_X23Temperature),
    /* K4   */  be_nested_str_weak(push),
    /* K5   */  be_nested_str_weak(type),
    /* K6   */  be_nested_str_weak(temperature),
    /* K7   */  be_nested_str_weak(filter),
    /* K8   */  be_nested_str_weak(stop_iteration),
    /* K9   */  be_nested_str_weak(Pressure),
    /* K10  */  be_nested_str_weak(_X23Pressure),
    /* K11  */  be_nested_str_weak(pressure),
    /* K12  */  be_nested_str_weak(Illuminance),
    /* K13  */  be_nested_str_weak(_X23Illuminance),
    /* K14  */  be_nested_str_weak(illuminance),
    /* K15  */  be_nested_str_weak(Humidity),
    /* K16  */  be_nested_str_weak(_X23Humidity),
    /* K17  */  be_nested_str_weak(humidity),
    }),
    be_str_weak(autoconf_sensors_list),
    &be_const_str_solidified,
    ( &(const binstruction[119]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x600C0010,  //  0002  GETGBL	R3	G16
      0x8C100100,  //  0003  GETMET	R4	R0	K0
      0x5C180200,  //  0004  MOVE	R6	R1
      0x7C100400,  //  0005  CALL	R4	2
      0x7C0C0200,  //  0006  CALL	R3	1
      0xA8020013,  //  0007  EXBLK	0	#001C
      0x5C100600,  //  0008  MOVE	R4	R3
      0x7C100000,  //  0009  CALL	R4	0
      0x94140204,  //  000A  GETIDX	R5	R1	R4
      0x6018000F,  //  000B  GETGBL	R6	G15
      0x5C1C0A00,  //  000C  MOVE	R7	R5
      0x60200013,  //  000D  GETGBL	R8	G19
      0x7C180400,  //  000E  CALL	R6	2
      0x781A000A,  //  000F  JMPF	R6	#001B
      0x8C180B01,  //  0010  GETMET	R6	R5	K1
      0x58200002,  //  0011  LDCONST	R8	K2
      0x7C180400,  //  0012  CALL	R6	2
      0x781A0006,  //  0013  JMPF	R6	#001B
      0x00180903,  //  0014  ADD	R6	R4	K3
      0x8C1C0504,  //  0015  GETMET	R7	R2	K4
      0x60240013,  //  0016  GETGBL	R9	G19
      0x7C240000,  //  0017  CALL	R9	0
      0x98260B06,  //  0018  SETIDX	R9	K5	K6
      0x98260E06,  //  0019  SETIDX	R9	K7	R6
      0x7C1C0400,  //  001A  CALL	R7	2
      0x7001FFEB,  //  001B  JMP		#0008
      0x580C0008,  //  001C  LDCONST	R3	K8
      0xAC0C0200,  //  001D  CATCH	R3	1	0
      0xB0080000,  //  001E  RAISE	2	R0	R0
      0x600C0010,  //  001F  GETGBL	R3	G16
      0x8C100100,  //  0020  GETMET	R4	R0	K0
      0x5C180200,  //  0021  MOVE	R6	R1
      0x7C100400,  //  0022  CALL	R4	2
      0x7C0C0200,  //  0023  CALL	R3	1
      0xA8020013,  //  0024  EXBLK	0	#0039
      0x5C100600,  //  0025  MOVE	R4	R3
      0x7C100000,  //  0026  CALL	R4	0
      0x94140204,  //  0027  GETIDX	R5	R1	R4
      0x6018000F,  //  0028  GETGBL	R6	G15
      0x5C1C0A00,  //  0029  MOVE	R7	R5
      0x60200013,  //  002A  GETGBL	R8	G19
      0x7C180400,  //  002B  CALL	R6	2
      0x781A000A,  //  002C  JMPF	R6	#0038
      0x8C180B01,  //  002D  GETMET	R6	R5	K1
      0x58200009,  //  002E  LDCONST	R8	K9
      0x7C180400,  //  002F  CALL	R6	2
      0x781A0006,  //  0030  JMPF	R6	#0038
      0x0018090A,  //  0031  ADD	R6	R4	K10
      0x8C1C0504,  //  0032  GETMET	R7	R2	K4
      0x60240013,  //  0033  GETGBL	R9	G19
      0x7C240000,  //  0034  CALL	R9	0
      0x98260B0B,  //  0035  SETIDX	R9	K5	K11
      0x98260E06,  //  0036  SETIDX	R9	K7	R6
      0x7C1C0400,  //  0037  CALL	R7	2
      0x7001FFEB,  //  0038  JMP		#0025
      0x580C0008,  //  0039  LDCONST	R3	K8
      0xAC0C0200,  //  003A  CATCH	R3	1	0
      0xB0080000,  //  003B  RAISE	2	R0	R0
      0x600C0010,  //  003C  GETGBL	R3	G16
      0x8C100100,  //  003D  GETMET	R4	R0	K0
      0x5C180200,  //  003E  MOVE	R6	R1
      0x7C100400,  //  003F  CALL	R4	2
      0x7C0C0200,  //  0040  CALL	R3	1
      0xA8020013,  //  0041  EXBLK	0	#0056
      0x5C100600,  //  0042  MOVE	R4	R3
      0x7C100000,  //  0043  CALL	R4	0
      0x94140204,  //  0044  GETIDX	R5	R1	R4
      0x6018000F,  //  0045  GETGBL	R6	G15
      0x5C1C0A00,  //  0046  MOVE	R7	R5
      0x60200013,  //  0047  GETGBL	R8	G19
      0x7C180400,  //  0048  CALL	R6	2
      0x781A000A,  //  0049  JMPF	R6	#0055
      0x8C180B01,  //  004A  GETMET	R6	R5	K1
      0x5820000C,  //  004B  LDCONST	R8	K12
      0x7C180400,  //  004C  CALL	R6	2
      0x781A0006,  //  004D  JMPF	R6	#0055
      0x0018090D,  //  004E  ADD	R6	R4	K13
      0x8C1C0504,  //  004F  GETMET	R7	R2	K4
      0x60240013,  //  0050  GETGBL	R9	G19
      0x7C240000,  //  0051  CALL	R9	0
      0x98260B0E,  //  0052  SETIDX	R9	K5	K14
      0x98260E06,  //  0053  SETIDX	R9	K7	R6
      0x7C1C0400,  //  0054  CALL	R7	2
      0x7001FFEB,  //  0055  JMP		#0042
      0x580C0008,  //  0056  LDCONST	R3	K8
      0xAC0C0200,  //  0057  CATCH	R3	1	0
      0xB0080000,  //  0058  RAISE	2	R0	R0
      0x600C0010,  //  0059  GETGBL	R3	G16
      0x8C100100,  //  005A  GETMET	R4	R0	K0
      0x5C180200,  //  005B  MOVE	R6	R1
      0x7C100400,  //  005C  CALL	R4	2
      0x7C0C0200,  //  005D  CALL	R3	1
      0xA8020013,  //  005E  EXBLK	0	#0073
      0x5C100600,  //  005F  MOVE	R4	R3
      0x7C100000,  //  0060  CALL	R4	0
      0x94140204,  //  0061  GETIDX	R5	R1	R4
      0x6018000F,  //  0062  GETGBL	R6	G15
      0x5C1C0A00,  //  0063  MOVE	R7	R5
      0x60200013,  //  0064  GETGBL	R8	G19
      0x7C180400,  //  0065  CALL	R6	2
      0x781A000A,  //  0066  JMPF	R6	#0072
      0x8C180B01,  //  0067  GETMET	R6	R5	K1
      0x5820000F,  //  0068  LDCONST	R8	K15
      0x7C180400,  //  0069  CALL	R6	2
      0x781A0006,  //  006A  JMPF	R6	#0072
      0x00180910,  //  006B  ADD	R6	R4	K16
      0x8C1C0504,  //  006C  GETMET	R7	R2	K4
      0x60240013,  //  006D  GETGBL	R9	G19
      0x7C240000,  //  006E  CALL	R9	0
      0x98260B11,  //  006F  SETIDX	R9	K5	K17
      0x98260E06,  //  0070  SETIDX	R9	K7	R6
      0x7C1C0400,  //  0071  CALL	R7	2
      0x7001FFEB,  //  0072  JMP		#005F
      0x580C0008,  //  0073  LDCONST	R3	K8
      0xAC0C0200,  //  0074  CATCH	R3	1	0
      0xB0080000,  //  0075  RAISE	2	R0	R0
      0x80040400,  //  0076  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_250ms
********************************************************************/
be_local_closure(Matter_Device_every_250ms,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(message_handler),
    /* K1   */  be_nested_str_weak(every_250ms),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(plugins),
    /* K4   */  be_const_int(1),
    }),
    be_str_weak(every_250ms),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x58040002,  //  0003  LDCONST	R1	K2
      0x6008000C,  //  0004  GETGBL	R2	G12
      0x880C0103,  //  0005  GETMBR	R3	R0	K3
      0x7C080200,  //  0006  CALL	R2	1
      0x14080202,  //  0007  LT	R2	R1	R2
      0x780A0005,  //  0008  JMPF	R2	#000F
      0x88080103,  //  0009  GETMBR	R2	R0	K3
      0x94080401,  //  000A  GETIDX	R2	R2	R1
      0x8C080501,  //  000B  GETMET	R2	R2	K1
      0x7C080200,  //  000C  CALL	R2	1
      0x00040304,  //  000D  ADD	R1	R1	K4
      0x7001FFF4,  //  000E  JMP		#0004
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_attribute_expansion
********************************************************************/
be_local_closure(Matter_Device_process_attribute_expansion,   /* name */
  be_nested_proto(
    25,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        7,                          /* nstack */
        1,                          /* argc */
        0,                          /* varg */
        0,                          /* has upvals */
        NULL,                       /* no upvals */
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(keys),
        /* K1   */  be_nested_str_weak(push),
        /* K2   */  be_nested_str_weak(stop_iteration),
        /* K3   */  be_const_int(1),
        /* K4   */  be_const_int(0),
        }),
        be_str_weak(keys_sorted),
        &be_const_str_solidified,
        ( &(const binstruction[45]) {  /* code */
          0x60040012,  //  0000  GETGBL	R1	G18
          0x7C040000,  //  0001  CALL	R1	0
          0x60080010,  //  0002  GETGBL	R2	G16
          0x8C0C0100,  //  0003  GETMET	R3	R0	K0
          0x7C0C0200,  //  0004  CALL	R3	1
          0x7C080200,  //  0005  CALL	R2	1
          0xA8020005,  //  0006  EXBLK	0	#000D
          0x5C0C0400,  //  0007  MOVE	R3	R2
          0x7C0C0000,  //  0008  CALL	R3	0
          0x8C100301,  //  0009  GETMET	R4	R1	K1
          0x5C180600,  //  000A  MOVE	R6	R3
          0x7C100400,  //  000B  CALL	R4	2
          0x7001FFF9,  //  000C  JMP		#0007
          0x58080002,  //  000D  LDCONST	R2	K2
          0xAC080200,  //  000E  CATCH	R2	1	0
          0xB0080000,  //  000F  RAISE	2	R0	R0
          0x60080010,  //  0010  GETGBL	R2	G16
          0x600C000C,  //  0011  GETGBL	R3	G12
          0x5C100200,  //  0012  MOVE	R4	R1
          0x7C0C0200,  //  0013  CALL	R3	1
          0x040C0703,  //  0014  SUB	R3	R3	K3
          0x400E0603,  //  0015  CONNECT	R3	K3	R3
          0x7C080200,  //  0016  CALL	R2	1
          0xA8020010,  //  0017  EXBLK	0	#0029
          0x5C0C0400,  //  0018  MOVE	R3	R2
          0x7C0C0000,  //  0019  CALL	R3	0
          0x94100203,  //  001A  GETIDX	R4	R1	R3
          0x5C140600,  //  001B  MOVE	R5	R3
          0x24180B04,  //  001C  GT	R6	R5	K4
          0x781A0008,  //  001D  JMPF	R6	#0027
          0x04180B03,  //  001E  SUB	R6	R5	K3
          0x94180206,  //  001F  GETIDX	R6	R1	R6
          0x24180C04,  //  0020  GT	R6	R6	R4
          0x781A0004,  //  0021  JMPF	R6	#0027
          0x04180B03,  //  0022  SUB	R6	R5	K3
          0x94180206,  //  0023  GETIDX	R6	R1	R6
          0x98040A06,  //  0024  SETIDX	R1	R5	R6
          0x04140B03,  //  0025  SUB	R5	R5	K3
          0x7001FFF4,  //  0026  JMP		#001C
          0x98040A04,  //  0027  SETIDX	R1	R5	R4
          0x7001FFEE,  //  0028  JMP		#0018
          0x58080002,  //  0029  LDCONST	R2	K2
          0xAC080200,  //  002A  CATCH	R2	1	0
          0xB0080000,  //  002B  RAISE	2	R0	R0
          0x80040200,  //  002C  RET	1	R1
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(endpoint),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(plugins),
    /* K5   */  be_nested_str_weak(get_endpoint),
    /* K6   */  be_nested_str_weak(contains),
    /* K7   */  be_nested_str_weak(get_cluster_list),
    /* K8   */  be_nested_str_weak(get_attribute_list),
    /* K9   */  be_nested_str_weak(push),
    /* K10  */  be_nested_str_weak(stop_iteration),
    /* K11  */  be_nested_str_weak(status),
    /* K12  */  be_nested_str_weak(matter),
    /* K13  */  be_nested_str_weak(UNSUPPORTED_ENDPOINT),
    /* K14  */  be_nested_str_weak(UNSUPPORTED_CLUSTER),
    /* K15  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K16  */  be_nested_str_weak(UNREPORTABLE_ATTRIBUTE),
    }),
    be_str_weak(process_attribute_expansion),
    &be_const_str_solidified,
    ( &(const binstruction[206]) {  /* code */
      0x840C0000,  //  0000  CLOSURE	R3	P0
      0xA4120000,  //  0001  IMPORT	R4	K0
      0x88140301,  //  0002  GETMBR	R5	R1	K1
      0x50180000,  //  0003  LDBOOL	R6	0	0
      0x881C0302,  //  0004  GETMBR	R7	R1	K2
      0x50200000,  //  0005  LDBOOL	R8	0	0
      0x88240303,  //  0006  GETMBR	R9	R1	K3
      0x50280000,  //  0007  LDBOOL	R10	0	0
      0x882C0301,  //  0008  GETMBR	R11	R1	K1
      0x4C300000,  //  0009  LDNIL	R12
      0x202C160C,  //  000A  NE	R11	R11	R12
      0x782E0007,  //  000B  JMPF	R11	#0014
      0x882C0302,  //  000C  GETMBR	R11	R1	K2
      0x4C300000,  //  000D  LDNIL	R12
      0x202C160C,  //  000E  NE	R11	R11	R12
      0x782E0003,  //  000F  JMPF	R11	#0014
      0x882C0303,  //  0010  GETMBR	R11	R1	K3
      0x4C300000,  //  0011  LDNIL	R12
      0x202C160C,  //  0012  NE	R11	R11	R12
      0x742E0000,  //  0013  JMPT	R11	#0015
      0x502C0001,  //  0014  LDBOOL	R11	0	1
      0x502C0200,  //  0015  LDBOOL	R11	1	0
      0x60300013,  //  0016  GETGBL	R12	G19
      0x7C300000,  //  0017  CALL	R12	0
      0x60340010,  //  0018  GETGBL	R13	G16
      0x88380104,  //  0019  GETMBR	R14	R0	K4
      0x7C340200,  //  001A  CALL	R13	1
      0xA8020055,  //  001B  EXBLK	0	#0072
      0x5C381A00,  //  001C  MOVE	R14	R13
      0x7C380000,  //  001D  CALL	R14	0
      0x8C3C1D05,  //  001E  GETMET	R15	R14	K5
      0x7C3C0200,  //  001F  CALL	R15	1
      0x4C400000,  //  0020  LDNIL	R16
      0x20400A10,  //  0021  NE	R16	R5	R16
      0x78420002,  //  0022  JMPF	R16	#0026
      0x20401E05,  //  0023  NE	R16	R15	R5
      0x78420000,  //  0024  JMPF	R16	#0026
      0x7001FFF5,  //  0025  JMP		#001C
      0x8C401906,  //  0026  GETMET	R16	R12	K6
      0x5C481E00,  //  0027  MOVE	R18	R15
      0x7C400400,  //  0028  CALL	R16	2
      0x74420002,  //  0029  JMPT	R16	#002D
      0x60400013,  //  002A  GETGBL	R16	G19
      0x7C400000,  //  002B  CALL	R16	0
      0x98301E10,  //  002C  SETIDX	R12	R15	R16
      0x50180200,  //  002D  LDBOOL	R6	1	0
      0x8C401D07,  //  002E  GETMET	R16	R14	K7
      0x5C481E00,  //  002F  MOVE	R18	R15
      0x7C400400,  //  0030  CALL	R16	2
      0x60440010,  //  0031  GETGBL	R17	G16
      0x5C482000,  //  0032  MOVE	R18	R16
      0x7C440200,  //  0033  CALL	R17	1
      0xA8020038,  //  0034  EXBLK	0	#006E
      0x5C482200,  //  0035  MOVE	R18	R17
      0x7C480000,  //  0036  CALL	R18	0
      0x4C4C0000,  //  0037  LDNIL	R19
      0x204C0E13,  //  0038  NE	R19	R7	R19
      0x784E0002,  //  0039  JMPF	R19	#003D
      0x204C2407,  //  003A  NE	R19	R18	R7
      0x784E0000,  //  003B  JMPF	R19	#003D
      0x7001FFF7,  //  003C  JMP		#0035
      0x944C180F,  //  003D  GETIDX	R19	R12	R15
      0x8C4C2706,  //  003E  GETMET	R19	R19	K6
      0x5C542400,  //  003F  MOVE	R21	R18
      0x7C4C0400,  //  0040  CALL	R19	2
      0x744E0003,  //  0041  JMPT	R19	#0046
      0x944C180F,  //  0042  GETIDX	R19	R12	R15
      0x60500013,  //  0043  GETGBL	R20	G19
      0x7C500000,  //  0044  CALL	R20	0
      0x984C2414,  //  0045  SETIDX	R19	R18	R20
      0x50200200,  //  0046  LDBOOL	R8	1	0
      0x8C4C1D08,  //  0047  GETMET	R19	R14	K8
      0x5C541E00,  //  0048  MOVE	R21	R15
      0x5C582400,  //  0049  MOVE	R22	R18
      0x7C4C0600,  //  004A  CALL	R19	3
      0x60500010,  //  004B  GETGBL	R20	G16
      0x5C542600,  //  004C  MOVE	R21	R19
      0x7C500200,  //  004D  CALL	R20	1
      0xA802001A,  //  004E  EXBLK	0	#006A
      0x5C542800,  //  004F  MOVE	R21	R20
      0x7C540000,  //  0050  CALL	R21	0
      0x4C580000,  //  0051  LDNIL	R22
      0x20581216,  //  0052  NE	R22	R9	R22
      0x785A0002,  //  0053  JMPF	R22	#0057
      0x20582A09,  //  0054  NE	R22	R21	R9
      0x785A0000,  //  0055  JMPF	R22	#0057
      0x7001FFF7,  //  0056  JMP		#004F
      0x9458180F,  //  0057  GETIDX	R22	R12	R15
      0x94582C12,  //  0058  GETIDX	R22	R22	R18
      0x8C582D06,  //  0059  GETMET	R22	R22	K6
      0x5C602A00,  //  005A  MOVE	R24	R21
      0x7C580400,  //  005B  CALL	R22	2
      0x745A0004,  //  005C  JMPT	R22	#0062
      0x9458180F,  //  005D  GETIDX	R22	R12	R15
      0x94582C12,  //  005E  GETIDX	R22	R22	R18
      0x605C0012,  //  005F  GETGBL	R23	G18
      0x7C5C0000,  //  0060  CALL	R23	0
      0x98582A17,  //  0061  SETIDX	R22	R21	R23
      0x50280200,  //  0062  LDBOOL	R10	1	0
      0x9458180F,  //  0063  GETIDX	R22	R12	R15
      0x94582C12,  //  0064  GETIDX	R22	R22	R18
      0x94582C15,  //  0065  GETIDX	R22	R22	R21
      0x8C582D09,  //  0066  GETMET	R22	R22	K9
      0x5C601C00,  //  0067  MOVE	R24	R14
      0x7C580400,  //  0068  CALL	R22	2
      0x7001FFE4,  //  0069  JMP		#004F
      0x5850000A,  //  006A  LDCONST	R20	K10
      0xAC500200,  //  006B  CATCH	R20	1	0
      0xB0080000,  //  006C  RAISE	2	R0	R0
      0x7001FFC6,  //  006D  JMP		#0035
      0x5844000A,  //  006E  LDCONST	R17	K10
      0xAC440200,  //  006F  CATCH	R17	1	0
      0xB0080000,  //  0070  RAISE	2	R0	R0
      0x7001FFA9,  //  0071  JMP		#001C
      0x5834000A,  //  0072  LDCONST	R13	K10
      0xAC340200,  //  0073  CATCH	R13	1	0
      0xB0080000,  //  0074  RAISE	2	R0	R0
      0x60340010,  //  0075  GETGBL	R13	G16
      0x5C380600,  //  0076  MOVE	R14	R3
      0x5C3C1800,  //  0077  MOVE	R15	R12
      0x7C380200,  //  0078  CALL	R14	1
      0x7C340200,  //  0079  CALL	R13	1
      0xA8020033,  //  007A  EXBLK	0	#00AF
      0x5C381A00,  //  007B  MOVE	R14	R13
      0x7C380000,  //  007C  CALL	R14	0
      0x603C0010,  //  007D  GETGBL	R15	G16
      0x5C400600,  //  007E  MOVE	R16	R3
      0x9444180E,  //  007F  GETIDX	R17	R12	R14
      0x7C400200,  //  0080  CALL	R16	1
      0x7C3C0200,  //  0081  CALL	R15	1
      0xA8020027,  //  0082  EXBLK	0	#00AB
      0x5C401E00,  //  0083  MOVE	R16	R15
      0x7C400000,  //  0084  CALL	R16	0
      0x60440010,  //  0085  GETGBL	R17	G16
      0x5C480600,  //  0086  MOVE	R18	R3
      0x944C180E,  //  0087  GETIDX	R19	R12	R14
      0x944C2610,  //  0088  GETIDX	R19	R19	R16
      0x7C480200,  //  0089  CALL	R18	1
      0x7C440200,  //  008A  CALL	R17	1
      0xA802001A,  //  008B  EXBLK	0	#00A7
      0x5C482200,  //  008C  MOVE	R18	R17
      0x7C480000,  //  008D  CALL	R18	0
      0x604C0010,  //  008E  GETGBL	R19	G16
      0x9450180E,  //  008F  GETIDX	R20	R12	R14
      0x94502810,  //  0090  GETIDX	R20	R20	R16
      0x94502812,  //  0091  GETIDX	R20	R20	R18
      0x7C4C0200,  //  0092  CALL	R19	1
      0xA802000E,  //  0093  EXBLK	0	#00A3
      0x5C502600,  //  0094  MOVE	R20	R19
      0x7C500000,  //  0095  CALL	R20	0
      0x9006020E,  //  0096  SETMBR	R1	K1	R14
      0x90060410,  //  0097  SETMBR	R1	K2	R16
      0x90060612,  //  0098  SETMBR	R1	K3	R18
      0x5C540400,  //  0099  MOVE	R21	R2
      0x5C582800,  //  009A  MOVE	R22	R20
      0x5C5C0200,  //  009B  MOVE	R23	R1
      0x5C601600,  //  009C  MOVE	R24	R11
      0x7C540600,  //  009D  CALL	R21	3
      0x782E0002,  //  009E  JMPF	R11	#00A2
      0x78560001,  //  009F  JMPF	R21	#00A2
      0xA8040004,  //  00A0  EXBLK	1	4
      0x80002C00,  //  00A1  RET	0
      0x7001FFF0,  //  00A2  JMP		#0094
      0x584C000A,  //  00A3  LDCONST	R19	K10
      0xAC4C0200,  //  00A4  CATCH	R19	1	0
      0xB0080000,  //  00A5  RAISE	2	R0	R0
      0x7001FFE4,  //  00A6  JMP		#008C
      0x5844000A,  //  00A7  LDCONST	R17	K10
      0xAC440200,  //  00A8  CATCH	R17	1	0
      0xB0080000,  //  00A9  RAISE	2	R0	R0
      0x7001FFD7,  //  00AA  JMP		#0083
      0x583C000A,  //  00AB  LDCONST	R15	K10
      0xAC3C0200,  //  00AC  CATCH	R15	1	0
      0xB0080000,  //  00AD  RAISE	2	R0	R0
      0x7001FFCB,  //  00AE  JMP		#007B
      0x5834000A,  //  00AF  LDCONST	R13	K10
      0xAC340200,  //  00B0  CATCH	R13	1	0
      0xB0080000,  //  00B1  RAISE	2	R0	R0
      0x782E0019,  //  00B2  JMPF	R11	#00CD
      0x5C340C00,  //  00B3  MOVE	R13	R6
      0x74360003,  //  00B4  JMPT	R13	#00B9
      0xB8361800,  //  00B5  GETNGBL	R13	K12
      0x88341B0D,  //  00B6  GETMBR	R13	R13	K13
      0x9006160D,  //  00B7  SETMBR	R1	K11	R13
      0x7002000E,  //  00B8  JMP		#00C8
      0x5C341000,  //  00B9  MOVE	R13	R8
      0x74360003,  //  00BA  JMPT	R13	#00BF
      0xB8361800,  //  00BB  GETNGBL	R13	K12
      0x88341B0E,  //  00BC  GETMBR	R13	R13	K14
      0x9006160D,  //  00BD  SETMBR	R1	K11	R13
      0x70020008,  //  00BE  JMP		#00C8
      0x5C341400,  //  00BF  MOVE	R13	R10
      0x74360003,  //  00C0  JMPT	R13	#00C5
      0xB8361800,  //  00C1  GETNGBL	R13	K12
      0x88341B0F,  //  00C2  GETMBR	R13	R13	K15
      0x9006160D,  //  00C3  SETMBR	R1	K11	R13
      0x70020002,  //  00C4  JMP		#00C8
      0xB8361800,  //  00C5  GETNGBL	R13	K12
      0x88341B10,  //  00C6  GETMBR	R13	R13	K16
      0x9006160D,  //  00C7  SETMBR	R1	K11	R13
      0x5C340400,  //  00C8  MOVE	R13	R2
      0x4C380000,  //  00C9  LDNIL	R14
      0x5C3C0200,  //  00CA  MOVE	R15	R1
      0x50400200,  //  00CB  LDBOOL	R16	1	0
      0x7C340600,  //  00CC  CALL	R13	3
      0x80000000,  //  00CD  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _compute_pbkdf
********************************************************************/
be_local_closure(Matter_Device__compute_pbkdf,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(add),
    /* K3   */  be_nested_str_weak(PBKDF2_HMAC_SHA256),
    /* K4   */  be_nested_str_weak(derive),
    /* K5   */  be_const_int(0),
    /* K6   */  be_nested_str_weak(root_w0),
    /* K7   */  be_nested_str_weak(EC_P256),
    /* K8   */  be_nested_str_weak(mod),
    /* K9   */  be_nested_str_weak(root_L),
    /* K10  */  be_nested_str_weak(public_key),
    }),
    be_str_weak(_compute_pbkdf),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0xA4160200,  //  0001  IMPORT	R5	K1
      0x60180015,  //  0002  GETGBL	R6	G21
      0x7C180000,  //  0003  CALL	R6	0
      0x8C180D02,  //  0004  GETMET	R6	R6	K2
      0x5C200200,  //  0005  MOVE	R8	R1
      0x54260003,  //  0006  LDINT	R9	4
      0x7C180600,  //  0007  CALL	R6	3
      0x8C1C0903,  //  0008  GETMET	R7	R4	K3
      0x7C1C0200,  //  0009  CALL	R7	1
      0x8C1C0F04,  //  000A  GETMET	R7	R7	K4
      0x5C240C00,  //  000B  MOVE	R9	R6
      0x5C280600,  //  000C  MOVE	R10	R3
      0x5C2C0400,  //  000D  MOVE	R11	R2
      0x5432004F,  //  000E  LDINT	R12	80
      0x7C1C0A00,  //  000F  CALL	R7	5
      0x54220026,  //  0010  LDINT	R8	39
      0x40220A08,  //  0011  CONNECT	R8	K5	R8
      0x94200E08,  //  0012  GETIDX	R8	R7	R8
      0x54260027,  //  0013  LDINT	R9	40
      0x542A004E,  //  0014  LDINT	R10	79
      0x4024120A,  //  0015  CONNECT	R9	R9	R10
      0x94240E09,  //  0016  GETIDX	R9	R7	R9
      0x8C280907,  //  0017  GETMET	R10	R4	K7
      0x7C280200,  //  0018  CALL	R10	1
      0x8C281508,  //  0019  GETMET	R10	R10	K8
      0x5C301000,  //  001A  MOVE	R12	R8
      0x7C280400,  //  001B  CALL	R10	2
      0x90020C0A,  //  001C  SETMBR	R0	K6	R10
      0x8C280907,  //  001D  GETMET	R10	R4	K7
      0x7C280200,  //  001E  CALL	R10	1
      0x8C281508,  //  001F  GETMET	R10	R10	K8
      0x5C301200,  //  0020  MOVE	R12	R9
      0x7C280400,  //  0021  CALL	R10	2
      0x8C2C0907,  //  0022  GETMET	R11	R4	K7
      0x7C2C0200,  //  0023  CALL	R11	1
      0x8C2C170A,  //  0024  GETMET	R11	R11	K10
      0x5C341400,  //  0025  MOVE	R13	R10
      0x7C2C0400,  //  0026  CALL	R11	2
      0x9002120B,  //  0027  SETMBR	R0	K9	R11
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: bridge_remove_endpoint
********************************************************************/
be_local_closure(Matter_Device_bridge_remove_endpoint,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[20]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_nested_str_weak(plugins_config),
    /* K3   */  be_nested_str_weak(contains),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20Cannot_X20remove_X20an_X20enpoint_X20not_X20configured_X3A_X20),
    /* K7   */  be_const_int(3),
    /* K8   */  be_nested_str_weak(format),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20deleting_X20endpoint_X20_X3D_X20_X25i),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(remove),
    /* K12  */  be_nested_str_weak(plugins_persist),
    /* K13  */  be_nested_str_weak(save_param),
    /* K14  */  be_nested_str_weak(signal_endpoints_changed),
    /* K15  */  be_const_int(0),
    /* K16  */  be_nested_str_weak(plugins),
    /* K17  */  be_nested_str_weak(get_endpoint),
    /* K18  */  be_const_int(1),
    /* K19  */  be_nested_str_weak(clean_remotes),
    }),
    be_str_weak(bridge_remove_endpoint),
    &be_const_str_solidified,
    ( &(const binstruction[61]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x60100008,  //  0002  GETGBL	R4	G8
      0x5C140200,  //  0003  MOVE	R5	R1
      0x7C100200,  //  0004  CALL	R4	1
      0x4C140000,  //  0005  LDNIL	R5
      0x4C180000,  //  0006  LDNIL	R6
      0x881C0102,  //  0007  GETMBR	R7	R0	K2
      0x8C1C0F03,  //  0008  GETMET	R7	R7	K3
      0x5C240800,  //  0009  MOVE	R9	R4
      0x7C1C0400,  //  000A  CALL	R7	2
      0x741E0005,  //  000B  JMPT	R7	#0012
      0xB81E0800,  //  000C  GETNGBL	R7	K4
      0x8C1C0F05,  //  000D  GETMET	R7	R7	K5
      0x00260C04,  //  000E  ADD	R9	K6	R4
      0x58280007,  //  000F  LDCONST	R10	K7
      0x7C1C0600,  //  0010  CALL	R7	3
      0x80000E00,  //  0011  RET	0
      0xB81E0800,  //  0012  GETNGBL	R7	K4
      0x8C1C0F05,  //  0013  GETMET	R7	R7	K5
      0x8C240508,  //  0014  GETMET	R9	R2	K8
      0x582C0009,  //  0015  LDCONST	R11	K9
      0x5C300200,  //  0016  MOVE	R12	R1
      0x7C240600,  //  0017  CALL	R9	3
      0x5828000A,  //  0018  LDCONST	R10	K10
      0x7C1C0600,  //  0019  CALL	R7	3
      0x881C0102,  //  001A  GETMBR	R7	R0	K2
      0x8C1C0F0B,  //  001B  GETMET	R7	R7	K11
      0x5C240800,  //  001C  MOVE	R9	R4
      0x7C1C0400,  //  001D  CALL	R7	2
      0x501C0200,  //  001E  LDBOOL	R7	1	0
      0x90021807,  //  001F  SETMBR	R0	K12	R7
      0x8C1C010D,  //  0020  GETMET	R7	R0	K13
      0x7C1C0200,  //  0021  CALL	R7	1
      0x8C1C010E,  //  0022  GETMET	R7	R0	K14
      0x7C1C0200,  //  0023  CALL	R7	1
      0x581C000F,  //  0024  LDCONST	R7	K15
      0x6020000C,  //  0025  GETGBL	R8	G12
      0x88240110,  //  0026  GETMBR	R9	R0	K16
      0x7C200200,  //  0027  CALL	R8	1
      0x14200E08,  //  0028  LT	R8	R7	R8
      0x7822000F,  //  0029  JMPF	R8	#003A
      0x88200110,  //  002A  GETMBR	R8	R0	K16
      0x94201007,  //  002B  GETIDX	R8	R8	R7
      0x8C201111,  //  002C  GETMET	R8	R8	K17
      0x7C200200,  //  002D  CALL	R8	1
      0x1C200208,  //  002E  EQ	R8	R1	R8
      0x78220007,  //  002F  JMPF	R8	#0038
      0x88200110,  //  0030  GETMBR	R8	R0	K16
      0x8C20110B,  //  0031  GETMET	R8	R8	K11
      0x5C280E00,  //  0032  MOVE	R10	R7
      0x7C200400,  //  0033  CALL	R8	2
      0x8C20010E,  //  0034  GETMET	R8	R0	K14
      0x7C200200,  //  0035  CALL	R8	1
      0x70020002,  //  0036  JMP		#003A
      0x70020000,  //  0037  JMP		#0039
      0x001C0F12,  //  0038  ADD	R7	R7	K18
      0x7001FFEA,  //  0039  JMP		#0025
      0x8C200113,  //  003A  GETMET	R8	R0	K19
      0x7C200200,  //  003B  CALL	R8	1
      0x80000000,  //  003C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop_basic_commissioning
********************************************************************/
be_local_closure(Matter_Device_stop_basic_commissioning,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(is_root_commissioning_open),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(publish_result),
    /* K3   */  be_nested_str_weak(_X7B_X22Matter_X22_X3A_X7B_X22Commissioning_X22_X3A0_X7D_X7D),
    /* K4   */  be_nested_str_weak(Matter),
    /* K5   */  be_nested_str_weak(commissioning_open),
    /* K6   */  be_nested_str_weak(mdns_remove_PASE),
    /* K7   */  be_nested_str_weak(commissioning_iterations),
    /* K8   */  be_nested_str_weak(commissioning_discriminator),
    /* K9   */  be_nested_str_weak(commissioning_salt),
    /* K10  */  be_nested_str_weak(commissioning_w0),
    /* K11  */  be_nested_str_weak(commissioning_L),
    /* K12  */  be_nested_str_weak(commissioning_admin_fabric),
    }),
    be_str_weak(stop_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x7C040200,  //  0001  CALL	R1	1
      0x78060004,  //  0002  JMPF	R1	#0008
      0xB8060200,  //  0003  GETNGBL	R1	K1
      0x8C040302,  //  0004  GETMET	R1	R1	K2
      0x580C0003,  //  0005  LDCONST	R3	K3
      0x58100004,  //  0006  LDCONST	R4	K4
      0x7C040600,  //  0007  CALL	R1	3
      0x4C040000,  //  0008  LDNIL	R1
      0x90020A01,  //  0009  SETMBR	R0	K5	R1
      0x8C040106,  //  000A  GETMET	R1	R0	K6
      0x7C040200,  //  000B  CALL	R1	1
      0x4C040000,  //  000C  LDNIL	R1
      0x90020E01,  //  000D  SETMBR	R0	K7	R1
      0x4C040000,  //  000E  LDNIL	R1
      0x90021001,  //  000F  SETMBR	R0	K8	R1
      0x4C040000,  //  0010  LDNIL	R1
      0x90021201,  //  0011  SETMBR	R0	K9	R1
      0x4C040000,  //  0012  LDNIL	R1
      0x90021401,  //  0013  SETMBR	R0	K10	R1
      0x4C040000,  //  0014  LDNIL	R1
      0x90021601,  //  0015  SETMBR	R0	K11	R1
      0x4C040000,  //  0016  LDNIL	R1
      0x90021801,  //  0017  SETMBR	R0	K12	R1
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_fabric
********************************************************************/
be_local_closure(Matter_Device_remove_fabric,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[20]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(find_children_fabrics),
    /* K2   */  be_nested_str_weak(get_fabric_index),
    /* K3   */  be_nested_str_weak(find_fabric_by_index),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20removing_X20fabric_X20),
    /* K7   */  be_nested_str_weak(get_fabric_id),
    /* K8   */  be_nested_str_weak(copy),
    /* K9   */  be_nested_str_weak(reverse),
    /* K10  */  be_nested_str_weak(tohex),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(message_handler),
    /* K13  */  be_nested_str_weak(im),
    /* K14  */  be_nested_str_weak(subs_shop),
    /* K15  */  be_nested_str_weak(remove_by_fabric),
    /* K16  */  be_nested_str_weak(mdns_remove_op_discovery),
    /* K17  */  be_nested_str_weak(remove_fabric),
    /* K18  */  be_nested_str_weak(stop_iteration),
    /* K19  */  be_nested_str_weak(save_fabrics),
    }),
    be_str_weak(remove_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[56]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x8C100302,  //  0002  GETMET	R4	R1	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x7C080400,  //  0004  CALL	R2	2
      0x4C0C0000,  //  0005  LDNIL	R3
      0x1C0C0403,  //  0006  EQ	R3	R2	R3
      0x780E0000,  //  0007  JMPF	R3	#0009
      0x80000600,  //  0008  RET	0
      0x600C0010,  //  0009  GETGBL	R3	G16
      0x5C100400,  //  000A  MOVE	R4	R2
      0x7C0C0200,  //  000B  CALL	R3	1
      0xA8020023,  //  000C  EXBLK	0	#0031
      0x5C100600,  //  000D  MOVE	R4	R3
      0x7C100000,  //  000E  CALL	R4	0
      0x88140100,  //  000F  GETMBR	R5	R0	K0
      0x8C140B03,  //  0010  GETMET	R5	R5	K3
      0x5C1C0800,  //  0011  MOVE	R7	R4
      0x7C140400,  //  0012  CALL	R5	2
      0x4C180000,  //  0013  LDNIL	R6
      0x20180A06,  //  0014  NE	R6	R5	R6
      0x781A0019,  //  0015  JMPF	R6	#0030
      0xB81A0800,  //  0016  GETNGBL	R6	K4
      0x8C180D05,  //  0017  GETMET	R6	R6	K5
      0x8C200B07,  //  0018  GETMET	R8	R5	K7
      0x7C200200,  //  0019  CALL	R8	1
      0x8C201108,  //  001A  GETMET	R8	R8	K8
      0x7C200200,  //  001B  CALL	R8	1
      0x8C201109,  //  001C  GETMET	R8	R8	K9
      0x7C200200,  //  001D  CALL	R8	1
      0x8C20110A,  //  001E  GETMET	R8	R8	K10
      0x7C200200,  //  001F  CALL	R8	1
      0x00220C08,  //  0020  ADD	R8	K6	R8
      0x5824000B,  //  0021  LDCONST	R9	K11
      0x7C180600,  //  0022  CALL	R6	3
      0x8818010C,  //  0023  GETMBR	R6	R0	K12
      0x88180D0D,  //  0024  GETMBR	R6	R6	K13
      0x88180D0E,  //  0025  GETMBR	R6	R6	K14
      0x8C180D0F,  //  0026  GETMET	R6	R6	K15
      0x5C200A00,  //  0027  MOVE	R8	R5
      0x7C180400,  //  0028  CALL	R6	2
      0x8C180110,  //  0029  GETMET	R6	R0	K16
      0x5C200A00,  //  002A  MOVE	R8	R5
      0x7C180400,  //  002B  CALL	R6	2
      0x88180100,  //  002C  GETMBR	R6	R0	K0
      0x8C180D11,  //  002D  GETMET	R6	R6	K17
      0x5C200A00,  //  002E  MOVE	R8	R5
      0x7C180400,  //  002F  CALL	R6	2
      0x7001FFDB,  //  0030  JMP		#000D
      0x580C0012,  //  0031  LDCONST	R3	K18
      0xAC0C0200,  //  0032  CATCH	R3	1	0
      0xB0080000,  //  0033  RAISE	2	R0	R0
      0x880C0100,  //  0034  GETMBR	R3	R0	K0
      0x8C0C0713,  //  0035  GETMET	R3	R3	K19
      0x7C0C0200,  //  0036  CALL	R3	1
      0x80000000,  //  0037  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save_param
********************************************************************/
be_local_closure(Matter_Device_save_param,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[28]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_nested_str_weak(format),
    /* K3   */  be_nested_str_weak(_X7B_X22distinguish_X22_X3A_X25i_X2C_X22passcode_X22_X3A_X25i_X2C_X22ipv4only_X22_X3A_X25s_X2C_X22nextep_X22_X3A_X25i),
    /* K4   */  be_nested_str_weak(root_discriminator),
    /* K5   */  be_nested_str_weak(root_passcode),
    /* K6   */  be_nested_str_weak(ipv4only),
    /* K7   */  be_nested_str_weak(true),
    /* K8   */  be_nested_str_weak(false),
    /* K9   */  be_nested_str_weak(next_ep),
    /* K10  */  be_nested_str_weak(plugins_persist),
    /* K11  */  be_nested_str_weak(_X2C_X22config_X22_X3A),
    /* K12  */  be_nested_str_weak(dump),
    /* K13  */  be_nested_str_weak(plugins_config),
    /* K14  */  be_nested_str_weak(_X7D),
    /* K15  */  be_nested_str_weak(FILENAME),
    /* K16  */  be_nested_str_weak(w),
    /* K17  */  be_nested_str_weak(write),
    /* K18  */  be_nested_str_weak(close),
    /* K19  */  be_nested_str_weak(tasmota),
    /* K20  */  be_nested_str_weak(log),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20_X3DSaved_X20_X20_X20_X20_X20parameters_X25s),
    /* K22  */  be_nested_str_weak(_X20and_X20configuration),
    /* K23  */  be_nested_str_weak(),
    /* K24  */  be_const_int(3),
    /* K25  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Asave_X20Exception_X3A),
    /* K26  */  be_nested_str_weak(_X7C),
    /* K27  */  be_const_int(2),
    }),
    be_str_weak(save_param),
    &be_const_str_solidified,
    ( &(const binstruction[66]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x8C0C0302,  //  0002  GETMET	R3	R1	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x88180104,  //  0004  GETMBR	R6	R0	K4
      0x881C0105,  //  0005  GETMBR	R7	R0	K5
      0x88200106,  //  0006  GETMBR	R8	R0	K6
      0x78220001,  //  0007  JMPF	R8	#000A
      0x58200007,  //  0008  LDCONST	R8	K7
      0x70020000,  //  0009  JMP		#000B
      0x58200008,  //  000A  LDCONST	R8	K8
      0x88240109,  //  000B  GETMBR	R9	R0	K9
      0x7C0C0C00,  //  000C  CALL	R3	6
      0x8810010A,  //  000D  GETMBR	R4	R0	K10
      0x78120004,  //  000E  JMPF	R4	#0014
      0x000C070B,  //  000F  ADD	R3	R3	K11
      0x8C10050C,  //  0010  GETMET	R4	R2	K12
      0x8818010D,  //  0011  GETMBR	R6	R0	K13
      0x7C100400,  //  0012  CALL	R4	2
      0x000C0604,  //  0013  ADD	R3	R3	R4
      0x000C070E,  //  0014  ADD	R3	R3	K14
      0xA8020018,  //  0015  EXBLK	0	#002F
      0x60100011,  //  0016  GETGBL	R4	G17
      0x8814010F,  //  0017  GETMBR	R5	R0	K15
      0x58180010,  //  0018  LDCONST	R6	K16
      0x7C100400,  //  0019  CALL	R4	2
      0x8C140911,  //  001A  GETMET	R5	R4	K17
      0x5C1C0600,  //  001B  MOVE	R7	R3
      0x7C140400,  //  001C  CALL	R5	2
      0x8C140912,  //  001D  GETMET	R5	R4	K18
      0x7C140200,  //  001E  CALL	R5	1
      0xB8162600,  //  001F  GETNGBL	R5	K19
      0x8C140B14,  //  0020  GETMET	R5	R5	K20
      0x8C1C0302,  //  0021  GETMET	R7	R1	K2
      0x58240015,  //  0022  LDCONST	R9	K21
      0x8828010A,  //  0023  GETMBR	R10	R0	K10
      0x782A0001,  //  0024  JMPF	R10	#0027
      0x58280016,  //  0025  LDCONST	R10	K22
      0x70020000,  //  0026  JMP		#0028
      0x58280017,  //  0027  LDCONST	R10	K23
      0x7C1C0600,  //  0028  CALL	R7	3
      0x58200018,  //  0029  LDCONST	R8	K24
      0x7C140600,  //  002A  CALL	R5	3
      0xA8040001,  //  002B  EXBLK	1	1
      0x80040600,  //  002C  RET	1	R3
      0xA8040001,  //  002D  EXBLK	1	1
      0x70020011,  //  002E  JMP		#0041
      0xAC100002,  //  002F  CATCH	R4	0	2
      0x7002000E,  //  0030  JMP		#0040
      0xB81A2600,  //  0031  GETNGBL	R6	K19
      0x8C180D14,  //  0032  GETMET	R6	R6	K20
      0x60200008,  //  0033  GETGBL	R8	G8
      0x5C240800,  //  0034  MOVE	R9	R4
      0x7C200200,  //  0035  CALL	R8	1
      0x00223208,  //  0036  ADD	R8	K25	R8
      0x0020111A,  //  0037  ADD	R8	R8	K26
      0x60240008,  //  0038  GETGBL	R9	G8
      0x5C280A00,  //  0039  MOVE	R10	R5
      0x7C240200,  //  003A  CALL	R9	1
      0x00201009,  //  003B  ADD	R8	R8	R9
      0x5824001B,  //  003C  LDCONST	R9	K27
      0x7C180600,  //  003D  CALL	R6	3
      0x80040600,  //  003E  RET	1	R3
      0x70020000,  //  003F  JMP		#0041
      0xB0080000,  //  0040  RAISE	2	R0	R0
      0x80000000,  //  0041  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_plugin_class_arg
********************************************************************/
be_local_closure(Matter_Device_get_plugin_class_arg,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(plugins_classes),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(ARG),
    /* K3   */  be_nested_str_weak(),
    }),
    be_str_weak(get_plugin_class_arg),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x880C0502,  //  0005  GETMBR	R3	R2	K2
      0x70020000,  //  0006  JMP		#0008
      0x580C0003,  //  0007  LDCONST	R3	K3
      0x80040600,  //  0008  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_remove_PASE
********************************************************************/
be_local_closure(Matter_Device_mdns_remove_PASE,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(mdns_pase_eth),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20calling_X20mdns_X2Eremove_service_X28_X25s_X2C_X20_X25s_X2C_X20_X25s_X2C_X20_X25s_X29),
    /* K7   */  be_nested_str_weak(_matterc),
    /* K8   */  be_nested_str_weak(_udp),
    /* K9   */  be_nested_str_weak(commissioning_instance_eth),
    /* K10  */  be_nested_str_weak(hostname_eth),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str_weak(MTR_X3A_X20remove_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27),
    /* K13  */  be_nested_str_weak(eth),
    /* K14  */  be_nested_str_weak(remove_service),
    /* K15  */  be_nested_str_weak(mdns_pase_wifi),
    /* K16  */  be_nested_str_weak(commissioning_instance_wifi),
    /* K17  */  be_nested_str_weak(hostname_wifi),
    /* K18  */  be_nested_str_weak(wifi),
    /* K19  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K20  */  be_nested_str_weak(_X7C),
    /* K21  */  be_const_int(2),
    }),
    be_str_weak(mdns_remove_PASE),
    &be_const_str_solidified,
    ( &(const binstruction[83]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA802003D,  //  0002  EXBLK	0	#0041
      0x880C0102,  //  0003  GETMBR	R3	R0	K2
      0x780E001B,  //  0004  JMPF	R3	#0021
      0xB80E0600,  //  0005  GETNGBL	R3	K3
      0x8C0C0704,  //  0006  GETMET	R3	R3	K4
      0x8C140505,  //  0007  GETMET	R5	R2	K5
      0x581C0006,  //  0008  LDCONST	R7	K6
      0x58200007,  //  0009  LDCONST	R8	K7
      0x58240008,  //  000A  LDCONST	R9	K8
      0x88280109,  //  000B  GETMBR	R10	R0	K9
      0x882C010A,  //  000C  GETMBR	R11	R0	K10
      0x7C140C00,  //  000D  CALL	R5	6
      0x5818000B,  //  000E  LDCONST	R6	K11
      0x7C0C0600,  //  000F  CALL	R3	3
      0xB80E0600,  //  0010  GETNGBL	R3	K3
      0x8C0C0704,  //  0011  GETMET	R3	R3	K4
      0x8C140505,  //  0012  GETMET	R5	R2	K5
      0x581C000C,  //  0013  LDCONST	R7	K12
      0x5820000D,  //  0014  LDCONST	R8	K13
      0x88240109,  //  0015  GETMBR	R9	R0	K9
      0x7C140800,  //  0016  CALL	R5	4
      0x5818000B,  //  0017  LDCONST	R6	K11
      0x7C0C0600,  //  0018  CALL	R3	3
      0x500C0000,  //  0019  LDBOOL	R3	0	0
      0x90020403,  //  001A  SETMBR	R0	K2	R3
      0x8C0C030E,  //  001B  GETMET	R3	R1	K14
      0x58140007,  //  001C  LDCONST	R5	K7
      0x58180008,  //  001D  LDCONST	R6	K8
      0x881C0109,  //  001E  GETMBR	R7	R0	K9
      0x8820010A,  //  001F  GETMBR	R8	R0	K10
      0x7C0C0A00,  //  0020  CALL	R3	5
      0x880C010F,  //  0021  GETMBR	R3	R0	K15
      0x780E001B,  //  0022  JMPF	R3	#003F
      0xB80E0600,  //  0023  GETNGBL	R3	K3
      0x8C0C0704,  //  0024  GETMET	R3	R3	K4
      0x8C140505,  //  0025  GETMET	R5	R2	K5
      0x581C0006,  //  0026  LDCONST	R7	K6
      0x58200007,  //  0027  LDCONST	R8	K7
      0x58240008,  //  0028  LDCONST	R9	K8
      0x88280110,  //  0029  GETMBR	R10	R0	K16
      0x882C0111,  //  002A  GETMBR	R11	R0	K17
      0x7C140C00,  //  002B  CALL	R5	6
      0x5818000B,  //  002C  LDCONST	R6	K11
      0x7C0C0600,  //  002D  CALL	R3	3
      0xB80E0600,  //  002E  GETNGBL	R3	K3
      0x8C0C0704,  //  002F  GETMET	R3	R3	K4
      0x8C140505,  //  0030  GETMET	R5	R2	K5
      0x581C000C,  //  0031  LDCONST	R7	K12
      0x58200012,  //  0032  LDCONST	R8	K18
      0x88240110,  //  0033  GETMBR	R9	R0	K16
      0x7C140800,  //  0034  CALL	R5	4
      0x5818000B,  //  0035  LDCONST	R6	K11
      0x7C0C0600,  //  0036  CALL	R3	3
      0x500C0000,  //  0037  LDBOOL	R3	0	0
      0x90021E03,  //  0038  SETMBR	R0	K15	R3
      0x8C0C030E,  //  0039  GETMET	R3	R1	K14
      0x58140007,  //  003A  LDCONST	R5	K7
      0x58180008,  //  003B  LDCONST	R6	K8
      0x881C0110,  //  003C  GETMBR	R7	R0	K16
      0x88200111,  //  003D  GETMBR	R8	R0	K17
      0x7C0C0A00,  //  003E  CALL	R3	5
      0xA8040001,  //  003F  EXBLK	1	1
      0x70020010,  //  0040  JMP		#0052
      0xAC0C0002,  //  0041  CATCH	R3	0	2
      0x7002000D,  //  0042  JMP		#0051
      0xB8160600,  //  0043  GETNGBL	R5	K3
      0x8C140B04,  //  0044  GETMET	R5	R5	K4
      0x601C0008,  //  0045  GETGBL	R7	G8
      0x5C200600,  //  0046  MOVE	R8	R3
      0x7C1C0200,  //  0047  CALL	R7	1
      0x001E2607,  //  0048  ADD	R7	K19	R7
      0x001C0F14,  //  0049  ADD	R7	R7	K20
      0x60200008,  //  004A  GETGBL	R8	G8
      0x5C240800,  //  004B  MOVE	R9	R4
      0x7C200200,  //  004C  CALL	R8	1
      0x001C0E08,  //  004D  ADD	R7	R7	R8
      0x58200015,  //  004E  LDCONST	R8	K21
      0x7C140600,  //  004F  CALL	R5	3
      0x70020000,  //  0050  JMP		#0052
      0xB0080000,  //  0051  RAISE	2	R0	R0
      0x80000000,  //  0052  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_fabrics_saved
********************************************************************/
be_local_closure(Matter_Device_event_fabrics_saved,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(count_active_fabrics),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(plugins_persist),
    /* K4   */  be_nested_str_weak(save_param),
    }),
    be_str_weak(event_fabrics_saved),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x24040302,  //  0003  GT	R1	R1	K2
      0x78060005,  //  0004  JMPF	R1	#000B
      0x88040103,  //  0005  GETMBR	R1	R0	K3
      0x74060003,  //  0006  JMPT	R1	#000B
      0x50040200,  //  0007  LDBOOL	R1	1	0
      0x90020601,  //  0008  SETMBR	R0	K3	R1
      0x8C040104,  //  0009  GETMET	R1	R0	K4
      0x7C040200,  //  000A  CALL	R1	1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_commissioning_complete
********************************************************************/
be_local_closure(Matter_Device_start_commissioning_complete,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(get_fabric),
    /* K2   */  be_nested_str_weak(get_fabric_id),
    /* K3   */  be_nested_str_weak(copy),
    /* K4   */  be_nested_str_weak(reverse),
    /* K5   */  be_nested_str_weak(tohex),
    /* K6   */  be_nested_str_weak(get_admin_vendor_name),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(format),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20_X2D_X2D_X2D_X20Commissioning_X20complete_X20for_X20Fabric_X20_X27_X25s_X27_X20_X28Vendor_X20_X25s_X29_X20_X2D_X2D_X2D),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(stop_basic_commissioning),
    }),
    be_str_weak(start_commissioning_complete),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0301,  //  0001  GETMET	R3	R1	K1
      0x7C0C0200,  //  0002  CALL	R3	1
      0x8C100702,  //  0003  GETMET	R4	R3	K2
      0x7C100200,  //  0004  CALL	R4	1
      0x8C100903,  //  0005  GETMET	R4	R4	K3
      0x7C100200,  //  0006  CALL	R4	1
      0x8C100904,  //  0007  GETMET	R4	R4	K4
      0x7C100200,  //  0008  CALL	R4	1
      0x8C100905,  //  0009  GETMET	R4	R4	K5
      0x7C100200,  //  000A  CALL	R4	1
      0x8C140706,  //  000B  GETMET	R5	R3	K6
      0x7C140200,  //  000C  CALL	R5	1
      0xB81A0E00,  //  000D  GETNGBL	R6	K7
      0x8C180D08,  //  000E  GETMET	R6	R6	K8
      0x8C200509,  //  000F  GETMET	R8	R2	K9
      0x5828000A,  //  0010  LDCONST	R10	K10
      0x5C2C0800,  //  0011  MOVE	R11	R4
      0x5C300A00,  //  0012  MOVE	R12	R5
      0x7C200800,  //  0013  CALL	R8	4
      0x5824000B,  //  0014  LDCONST	R9	K11
      0x7C180600,  //  0015  CALL	R6	3
      0x8C18010C,  //  0016  GETMET	R6	R0	K12
      0x7C180200,  //  0017  CALL	R6	1
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: msg_received
********************************************************************/
be_local_closure(Matter_Device_msg_received,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(message_handler),
    /* K1   */  be_nested_str_weak(msg_received),
    }),
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88100100,  //  0000  GETMBR	R4	R0	K0
      0x8C100901,  //  0001  GETMET	R4	R4	K1
      0x5C180200,  //  0002  MOVE	R6	R1
      0x5C1C0400,  //  0003  MOVE	R7	R2
      0x5C200600,  //  0004  MOVE	R8	R3
      0x7C100800,  //  0005  CALL	R4	4
      0x80040800,  //  0006  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _instantiate_plugins_from_config
********************************************************************/
be_local_closure(Matter_Device__instantiate_plugins_from_config,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[32]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(k2l_num),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(log),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20Configuring_X20endpoints),
    /* K5   */  be_const_int(2),
    /* K6   */  be_nested_str_weak(plugins),
    /* K7   */  be_nested_str_weak(push),
    /* K8   */  be_nested_str_weak(matter),
    /* K9   */  be_nested_str_weak(Plugin_Root),
    /* K10  */  be_const_int(0),
    /* K11  */  be_nested_str_weak(format),
    /* K12  */  be_nested_str_weak(MTR_X3A_X20_X20_X20endpoint_X20_X3D_X20_X255i_X20type_X20_X3D_X20_X25s_X25s),
    /* K13  */  be_nested_str_weak(root),
    /* K14  */  be_nested_str_weak(),
    /* K15  */  be_nested_str_weak(Plugin_Aggregator),
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
    /* K28  */  be_nested_str_weak(aggregator),
    /* K29  */  be_nested_str_weak(publish_result),
    /* K30  */  be_nested_str_weak(_X7B_X22Matter_X22_X3A_X7B_X22Initialized_X22_X3A1_X7D_X7D),
    /* K31  */  be_nested_str_weak(Matter),
    }),
    be_str_weak(_instantiate_plugins_from_config),
    &be_const_str_solidified,
    ( &(const binstruction[153]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0101,  //  0001  GETMET	R3	R0	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0xB8120400,  //  0004  GETNGBL	R4	K2
      0x8C100903,  //  0005  GETMET	R4	R4	K3
      0x58180004,  //  0006  LDCONST	R6	K4
      0x581C0005,  //  0007  LDCONST	R7	K5
      0x7C100600,  //  0008  CALL	R4	3
      0x88100106,  //  0009  GETMBR	R4	R0	K6
      0x8C100907,  //  000A  GETMET	R4	R4	K7
      0xB81A1000,  //  000B  GETNGBL	R6	K8
      0x8C180D09,  //  000C  GETMET	R6	R6	K9
      0x5C200000,  //  000D  MOVE	R8	R0
      0x5824000A,  //  000E  LDCONST	R9	K10
      0x60280013,  //  000F  GETGBL	R10	G19
      0x7C280000,  //  0010  CALL	R10	0
      0x7C180800,  //  0011  CALL	R6	4
      0x7C100400,  //  0012  CALL	R4	2
      0xB8120400,  //  0013  GETNGBL	R4	K2
      0x8C100903,  //  0014  GETMET	R4	R4	K3
      0x8C18050B,  //  0015  GETMET	R6	R2	K11
      0x5820000C,  //  0016  LDCONST	R8	K12
      0x5824000A,  //  0017  LDCONST	R9	K10
      0x5828000D,  //  0018  LDCONST	R10	K13
      0x582C000E,  //  0019  LDCONST	R11	K14
      0x7C180A00,  //  001A  CALL	R6	5
      0x581C0005,  //  001B  LDCONST	R7	K5
      0x7C100600,  //  001C  CALL	R4	3
      0x88100106,  //  001D  GETMBR	R4	R0	K6
      0x8C100907,  //  001E  GETMET	R4	R4	K7
      0xB81A1000,  //  001F  GETNGBL	R6	K8
      0x8C180D0F,  //  0020  GETMET	R6	R6	K15
      0x5C200000,  //  0021  MOVE	R8	R0
      0x5426FEFF,  //  0022  LDINT	R9	65280
      0x60280013,  //  0023  GETGBL	R10	G19
      0x7C280000,  //  0024  CALL	R10	0
      0x7C180800,  //  0025  CALL	R6	4
      0x7C100400,  //  0026  CALL	R4	2
      0x60100010,  //  0027  GETGBL	R4	G16
      0x5C140600,  //  0028  MOVE	R5	R3
      0x7C100200,  //  0029  CALL	R4	1
      0xA802005A,  //  002A  EXBLK	0	#0086
      0x5C140800,  //  002B  MOVE	R5	R4
      0x7C140000,  //  002C  CALL	R5	0
      0x1C180B0A,  //  002D  EQ	R6	R5	K10
      0x781A0000,  //  002E  JMPF	R6	#0030
      0x7001FFFA,  //  002F  JMP		#002B
      0xA8020042,  //  0030  EXBLK	0	#0074
      0x60180008,  //  0031  GETGBL	R6	G8
      0x5C1C0A00,  //  0032  MOVE	R7	R5
      0x7C180200,  //  0033  CALL	R6	1
      0x94180206,  //  0034  GETIDX	R6	R1	R6
      0x8C1C0D10,  //  0035  GETMET	R7	R6	K16
      0x58240011,  //  0036  LDCONST	R9	K17
      0x7C1C0400,  //  0037  CALL	R7	2
      0x4C200000,  //  0038  LDNIL	R8
      0x1C200E08,  //  0039  EQ	R8	R7	R8
      0x78220006,  //  003A  JMPF	R8	#0042
      0xB8220400,  //  003B  GETNGBL	R8	K2
      0x8C201103,  //  003C  GETMET	R8	R8	K3
      0x58280012,  //  003D  LDCONST	R10	K18
      0x582C0013,  //  003E  LDCONST	R11	K19
      0x7C200600,  //  003F  CALL	R8	3
      0xA8040001,  //  0040  EXBLK	1	1
      0x7001FFE8,  //  0041  JMP		#002B
      0x1C200F0D,  //  0042  EQ	R8	R7	K13
      0x78220006,  //  0043  JMPF	R8	#004B
      0xB8220400,  //  0044  GETNGBL	R8	K2
      0x8C201103,  //  0045  GETMET	R8	R8	K3
      0x58280014,  //  0046  LDCONST	R10	K20
      0x582C0013,  //  0047  LDCONST	R11	K19
      0x7C200600,  //  0048  CALL	R8	3
      0xA8040001,  //  0049  EXBLK	1	1
      0x7001FFDF,  //  004A  JMP		#002B
      0x88200115,  //  004B  GETMBR	R8	R0	K21
      0x8C201110,  //  004C  GETMET	R8	R8	K16
      0x5C280E00,  //  004D  MOVE	R10	R7
      0x7C200400,  //  004E  CALL	R8	2
      0x4C240000,  //  004F  LDNIL	R9
      0x1C241009,  //  0050  EQ	R9	R8	R9
      0x7826000A,  //  0051  JMPF	R9	#005D
      0xB8260400,  //  0052  GETNGBL	R9	K2
      0x8C241303,  //  0053  GETMET	R9	R9	K3
      0x602C0008,  //  0054  GETGBL	R11	G8
      0x5C300E00,  //  0055  MOVE	R12	R7
      0x7C2C0200,  //  0056  CALL	R11	1
      0x002E2C0B,  //  0057  ADD	R11	K22	R11
      0x002C1717,  //  0058  ADD	R11	R11	K23
      0x58300005,  //  0059  LDCONST	R12	K5
      0x7C240600,  //  005A  CALL	R9	3
      0xA8040001,  //  005B  EXBLK	1	1
      0x7001FFCD,  //  005C  JMP		#002B
      0x5C241000,  //  005D  MOVE	R9	R8
      0x5C280000,  //  005E  MOVE	R10	R0
      0x5C2C0A00,  //  005F  MOVE	R11	R5
      0x5C300C00,  //  0060  MOVE	R12	R6
      0x7C240600,  //  0061  CALL	R9	3
      0x88280106,  //  0062  GETMBR	R10	R0	K6
      0x8C281507,  //  0063  GETMET	R10	R10	K7
      0x5C301200,  //  0064  MOVE	R12	R9
      0x7C280400,  //  0065  CALL	R10	2
      0xB82A0400,  //  0066  GETNGBL	R10	K2
      0x8C281503,  //  0067  GETMET	R10	R10	K3
      0x8C30050B,  //  0068  GETMET	R12	R2	K11
      0x5838000C,  //  0069  LDCONST	R14	K12
      0x5C3C0A00,  //  006A  MOVE	R15	R5
      0x5C400E00,  //  006B  MOVE	R16	R7
      0x8C440118,  //  006C  GETMET	R17	R0	K24
      0x5C4C0C00,  //  006D  MOVE	R19	R6
      0x7C440400,  //  006E  CALL	R17	2
      0x7C300A00,  //  006F  CALL	R12	5
      0x58340005,  //  0070  LDCONST	R13	K5
      0x7C280600,  //  0071  CALL	R10	3
      0xA8040001,  //  0072  EXBLK	1	1
      0x70020010,  //  0073  JMP		#0085
      0xAC180002,  //  0074  CATCH	R6	0	2
      0x7002000D,  //  0075  JMP		#0084
      0xB8220400,  //  0076  GETNGBL	R8	K2
      0x8C201103,  //  0077  GETMET	R8	R8	K3
      0x60280008,  //  0078  GETGBL	R10	G8
      0x5C2C0C00,  //  0079  MOVE	R11	R6
      0x7C280200,  //  007A  CALL	R10	1
      0x002A320A,  //  007B  ADD	R10	K25	R10
      0x0028151A,  //  007C  ADD	R10	R10	K26
      0x602C0008,  //  007D  GETGBL	R11	G8
      0x5C300E00,  //  007E  MOVE	R12	R7
      0x7C2C0200,  //  007F  CALL	R11	1
      0x0028140B,  //  0080  ADD	R10	R10	R11
      0x582C0005,  //  0081  LDCONST	R11	K5
      0x7C200600,  //  0082  CALL	R8	3
      0x70020000,  //  0083  JMP		#0085
      0xB0080000,  //  0084  RAISE	2	R0	R0
      0x7001FFA4,  //  0085  JMP		#002B
      0x5810001B,  //  0086  LDCONST	R4	K27
      0xAC100200,  //  0087  CATCH	R4	1	0
      0xB0080000,  //  0088  RAISE	2	R0	R0
      0xB8120400,  //  0089  GETNGBL	R4	K2
      0x8C100903,  //  008A  GETMET	R4	R4	K3
      0x8C18050B,  //  008B  GETMET	R6	R2	K11
      0x5820000C,  //  008C  LDCONST	R8	K12
      0x5426FEFF,  //  008D  LDINT	R9	65280
      0x5828001C,  //  008E  LDCONST	R10	K28
      0x582C000E,  //  008F  LDCONST	R11	K14
      0x7C180A00,  //  0090  CALL	R6	5
      0x581C0005,  //  0091  LDCONST	R7	K5
      0x7C100600,  //  0092  CALL	R4	3
      0xB8120400,  //  0093  GETNGBL	R4	K2
      0x8C10091D,  //  0094  GETMET	R4	R4	K29
      0x5818001E,  //  0095  LDCONST	R6	K30
      0x581C001F,  //  0096  LDCONST	R7	K31
      0x7C100600,  //  0097  CALL	R4	3
      0x80000000,  //  0098  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_active_endpoints
********************************************************************/
be_local_closure(Matter_Device_get_active_endpoints,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(plugins),
    /* K1   */  be_nested_str_weak(get_endpoint),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(push),
    /* K5   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(get_active_endpoints),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x600C0010,  //  0002  GETGBL	R3	G16
      0x88100100,  //  0003  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0004  CALL	R3	1
      0xA8020011,  //  0005  EXBLK	0	#0018
      0x5C100600,  //  0006  MOVE	R4	R3
      0x7C100000,  //  0007  CALL	R4	0
      0x8C140901,  //  0008  GETMET	R5	R4	K1
      0x7C140200,  //  0009  CALL	R5	1
      0x78060002,  //  000A  JMPF	R1	#000E
      0x1C180B02,  //  000B  EQ	R6	R5	K2
      0x781A0000,  //  000C  JMPF	R6	#000E
      0x7001FFF7,  //  000D  JMP		#0006
      0x8C180503,  //  000E  GETMET	R6	R2	K3
      0x5C200A00,  //  000F  MOVE	R8	R5
      0x7C180400,  //  0010  CALL	R6	2
      0x4C1C0000,  //  0011  LDNIL	R7
      0x1C180C07,  //  0012  EQ	R6	R6	R7
      0x781A0002,  //  0013  JMPF	R6	#0017
      0x8C180504,  //  0014  GETMET	R6	R2	K4
      0x5C200A00,  //  0015  MOVE	R8	R5
      0x7C180400,  //  0016  CALL	R6	2
      0x7001FFED,  //  0017  JMP		#0006
      0x580C0005,  //  0018  LDCONST	R3	K5
      0xAC0C0200,  //  0019  CATCH	R3	1	0
      0xB0080000,  //  001A  RAISE	2	R0	R0
      0x80040400,  //  001B  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _start_udp
********************************************************************/
be_local_closure(Matter_Device__start_udp,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
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
        /* K0   */  be_nested_str_weak(msg_received),
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
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(udp_server),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20Starting_X20UDP_X20server_X20on_X20port_X3A_X20),
    /* K4   */  be_const_int(2),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(UDPServer),
    /* K7   */  be_nested_str_weak(),
    /* K8   */  be_nested_str_weak(start),
    }),
    be_str_weak(_start_udp),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x80000400,  //  0002  RET	0
      0x4C080000,  //  0003  LDNIL	R2
      0x1C080202,  //  0004  EQ	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x540615A3,  //  0006  LDINT	R1	5540
      0xB80A0200,  //  0007  GETNGBL	R2	K1
      0x8C080502,  //  0008  GETMET	R2	R2	K2
      0x60100008,  //  0009  GETGBL	R4	G8
      0x5C140200,  //  000A  MOVE	R5	R1
      0x7C100200,  //  000B  CALL	R4	1
      0x00120604,  //  000C  ADD	R4	K3	R4
      0x58140004,  //  000D  LDCONST	R5	K4
      0x7C080600,  //  000E  CALL	R2	3
      0xB80A0A00,  //  000F  GETNGBL	R2	K5
      0x8C080506,  //  0010  GETMET	R2	R2	K6
      0x58100007,  //  0011  LDCONST	R4	K7
      0x5C140200,  //  0012  MOVE	R5	R1
      0x7C080600,  //  0013  CALL	R2	3
      0x90020002,  //  0014  SETMBR	R0	K0	R2
      0x88080100,  //  0015  GETMBR	R2	R0	K0
      0x8C080508,  //  0016  GETMET	R2	R2	K8
      0x84100000,  //  0017  CLOSURE	R4	P0
      0x7C080400,  //  0018  CALL	R2	2
      0xA0000000,  //  0019  CLOSE	R0
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_root_commissioning_open
********************************************************************/
be_local_closure(Matter_Device_is_root_commissioning_open,   /* name */
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
    /* K0   */  be_nested_str_weak(commissioning_open),
    /* K1   */  be_nested_str_weak(commissioning_admin_fabric),
    }),
    be_str_weak(is_root_commissioning_open),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060003,  //  0003  JMPF	R1	#0008
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x4C080000,  //  0005  LDNIL	R2
      0x1C040202,  //  0006  EQ	R1	R1	R2
      0x74060000,  //  0007  JMPT	R1	#0009
      0x50040001,  //  0008  LDBOOL	R1	0	1
      0x50040200,  //  0009  LDBOOL	R1	1	0
      0x80040200,  //  000A  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_root_basic_commissioning
********************************************************************/
be_local_closure(Matter_Device_start_root_basic_commissioning,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[20]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(PASE_TIMEOUT),
    /* K2   */  be_nested_str_weak(compute_manual_pairing_code),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(MTR_X3A_X20Manual_X20pairing_X20code_X3A_X20_X25s),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(compute_qrcode_content),
    /* K9   */  be_nested_str_weak(publish_result),
    /* K10  */  be_nested_str_weak(_X7B_X22Matter_X22_X3A_X7B_X22Commissioning_X22_X3A1_X2C_X22PairingCode_X22_X3A_X22_X25s_X22_X2C_X22QRCode_X22_X3A_X22_X25s_X22_X7D_X7D),
    /* K11  */  be_nested_str_weak(Matter),
    /* K12  */  be_nested_str_weak(_compute_pbkdf),
    /* K13  */  be_nested_str_weak(root_passcode),
    /* K14  */  be_nested_str_weak(root_iterations),
    /* K15  */  be_nested_str_weak(root_salt),
    /* K16  */  be_nested_str_weak(start_basic_commissioning),
    /* K17  */  be_nested_str_weak(root_discriminator),
    /* K18  */  be_nested_str_weak(root_w0),
    /* K19  */  be_nested_str_weak(root_L),
    }),
    be_str_weak(start_root_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[41]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C0C0203,  //  0002  EQ	R3	R1	R3
      0x780E0000,  //  0003  JMPF	R3	#0005
      0x88040101,  //  0004  GETMBR	R1	R0	K1
      0x8C0C0102,  //  0005  GETMET	R3	R0	K2
      0x7C0C0200,  //  0006  CALL	R3	1
      0xB8120600,  //  0007  GETNGBL	R4	K3
      0x8C100904,  //  0008  GETMET	R4	R4	K4
      0x8C180505,  //  0009  GETMET	R6	R2	K5
      0x58200006,  //  000A  LDCONST	R8	K6
      0x5C240600,  //  000B  MOVE	R9	R3
      0x7C180600,  //  000C  CALL	R6	3
      0x581C0007,  //  000D  LDCONST	R7	K7
      0x7C100600,  //  000E  CALL	R4	3
      0x8C100108,  //  000F  GETMET	R4	R0	K8
      0x7C100200,  //  0010  CALL	R4	1
      0xB8160600,  //  0011  GETNGBL	R5	K3
      0x8C140B09,  //  0012  GETMET	R5	R5	K9
      0x8C1C0505,  //  0013  GETMET	R7	R2	K5
      0x5824000A,  //  0014  LDCONST	R9	K10
      0x5C280600,  //  0015  MOVE	R10	R3
      0x5C2C0800,  //  0016  MOVE	R11	R4
      0x7C1C0800,  //  0017  CALL	R7	4
      0x5820000B,  //  0018  LDCONST	R8	K11
      0x7C140600,  //  0019  CALL	R5	3
      0x8C14010C,  //  001A  GETMET	R5	R0	K12
      0x881C010D,  //  001B  GETMBR	R7	R0	K13
      0x8820010E,  //  001C  GETMBR	R8	R0	K14
      0x8824010F,  //  001D  GETMBR	R9	R0	K15
      0x7C140800,  //  001E  CALL	R5	4
      0x8C140110,  //  001F  GETMET	R5	R0	K16
      0x5C1C0200,  //  0020  MOVE	R7	R1
      0x8820010E,  //  0021  GETMBR	R8	R0	K14
      0x88240111,  //  0022  GETMBR	R9	R0	K17
      0x8828010F,  //  0023  GETMBR	R10	R0	K15
      0x882C0112,  //  0024  GETMBR	R11	R0	K18
      0x88300113,  //  0025  GETMBR	R12	R0	K19
      0x4C340000,  //  0026  LDNIL	R13
      0x7C141000,  //  0027  CALL	R5	8
      0x80000000,  //  0028  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_commissioning_complete_deferred
********************************************************************/
be_local_closure(Matter_Device_start_commissioning_complete_deferred,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        3,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 1),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(start_commissioning_complete),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 5]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x68080001,  //  0002  GETUPV	R2	U1
          0x7C000400,  //  0003  CALL	R0	2
          0x80040000,  //  0004  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(set_timer),
    /* K2   */  be_const_int(0),
    }),
    be_str_weak(start_commissioning_complete_deferred),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x84140000,  //  0003  CLOSURE	R5	P0
      0x7C080600,  //  0004  CALL	R2	3
      0xA0000000,  //  0005  CLOSE	R0
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: MtrJoin
********************************************************************/
be_local_closure(Matter_Device_MtrJoin,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(start_root_basic_commissioning),
    /* K1   */  be_nested_str_weak(stop_basic_commissioning),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(resp_cmnd_done),
    }),
    be_str_weak(MtrJoin),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x60140009,  //  0000  GETGBL	R5	G9
      0x5C180600,  //  0001  MOVE	R6	R3
      0x7C140200,  //  0002  CALL	R5	1
      0x78160002,  //  0003  JMPF	R5	#0007
      0x8C180100,  //  0004  GETMET	R6	R0	K0
      0x7C180200,  //  0005  CALL	R6	1
      0x70020001,  //  0006  JMP		#0009
      0x8C180101,  //  0007  GETMET	R6	R0	K1
      0x7C180200,  //  0008  CALL	R6	1
      0xB81A0400,  //  0009  GETNGBL	R6	K2
      0x8C180D03,  //  000A  GETMET	R6	R6	K3
      0x7C180200,  //  000B  CALL	R6	1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(Matter_Device_start,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
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
        /* K0   */  be_nested_str_weak(_trigger_read_sensors),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 4]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0x80000000,  //  0003  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(started),
    /* K1   */  be_nested_str_weak(autoconf_device),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(add_cron),
    /* K4   */  be_nested_str_weak(_X2A_X2F30_X20_X2A_X20_X2A_X20_X2A_X20_X2A_X20_X2A),
    /* K5   */  be_nested_str_weak(matter_sensors_30s),
    /* K6   */  be_nested_str_weak(_start_udp),
    /* K7   */  be_nested_str_weak(UDP_PORT),
    /* K8   */  be_nested_str_weak(start_mdns_announce_hostnames),
    }),
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060000,  //  0001  JMPF	R1	#0003
      0x80000200,  //  0002  RET	0
      0x8C040101,  //  0003  GETMET	R1	R0	K1
      0x7C040200,  //  0004  CALL	R1	1
      0xB8060400,  //  0005  GETNGBL	R1	K2
      0x8C040303,  //  0006  GETMET	R1	R1	K3
      0x580C0004,  //  0007  LDCONST	R3	K4
      0x84100000,  //  0008  CLOSURE	R4	P0
      0x58140005,  //  0009  LDCONST	R5	K5
      0x7C040800,  //  000A  CALL	R1	4
      0x8C040106,  //  000B  GETMET	R1	R0	K6
      0x880C0107,  //  000C  GETMBR	R3	R0	K7
      0x7C040400,  //  000D  CALL	R1	2
      0x8C040108,  //  000E  GETMET	R1	R0	K8
      0x7C040200,  //  000F  CALL	R1	1
      0x50040200,  //  0010  LDBOOL	R1	1	0
      0x90020001,  //  0011  SETMBR	R0	K0	R1
      0xA0000000,  //  0012  CLOSE	R0
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: conf_to_log
********************************************************************/
be_local_closure(Matter_Device_conf_to_log,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Device),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(),
    /* K3   */  be_nested_str_weak(k2l),
    /* K4   */  be_nested_str_weak(type),
    /* K5   */  be_nested_str_weak(format),
    /* K6   */  be_nested_str_weak(_X20_X25s_X20_X3D_X20_X25s),
    /* K7   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(conf_to_log),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x580C0002,  //  0002  LDCONST	R3	K2
      0x60100010,  //  0003  GETGBL	R4	G16
      0x8C140303,  //  0004  GETMET	R5	R1	K3
      0x5C1C0000,  //  0005  MOVE	R7	R0
      0x7C140400,  //  0006  CALL	R5	2
      0x7C100200,  //  0007  CALL	R4	1
      0xA802000B,  //  0008  EXBLK	0	#0015
      0x5C140800,  //  0009  MOVE	R5	R4
      0x7C140000,  //  000A  CALL	R5	0
      0x1C180B04,  //  000B  EQ	R6	R5	K4
      0x781A0000,  //  000C  JMPF	R6	#000E
      0x7001FFFA,  //  000D  JMP		#0009
      0x8C180505,  //  000E  GETMET	R6	R2	K5
      0x58200006,  //  000F  LDCONST	R8	K6
      0x5C240A00,  //  0010  MOVE	R9	R5
      0x94280005,  //  0011  GETIDX	R10	R0	R5
      0x7C180800,  //  0012  CALL	R6	4
      0x000C0606,  //  0013  ADD	R3	R3	R6
      0x7001FFF3,  //  0014  JMP		#0009
      0x58100007,  //  0015  LDCONST	R4	K7
      0xAC100200,  //  0016  CATCH	R4	1	0
      0xB0080000,  //  0017  RAISE	2	R0	R0
      0x80040600,  //  0018  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: attribute_updated
********************************************************************/
be_local_closure(Matter_Device_attribute_updated,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(matter),
    /* K1   */  be_nested_str_weak(Path),
    /* K2   */  be_nested_str_weak(endpoint),
    /* K3   */  be_nested_str_weak(cluster),
    /* K4   */  be_nested_str_weak(attribute),
    /* K5   */  be_nested_str_weak(message_handler),
    /* K6   */  be_nested_str_weak(im),
    /* K7   */  be_nested_str_weak(subs_shop),
    /* K8   */  be_nested_str_weak(attribute_updated_ctx),
    }),
    be_str_weak(attribute_updated),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140805,  //  0001  EQ	R5	R4	R5
      0x78160000,  //  0002  JMPF	R5	#0004
      0x50100000,  //  0003  LDBOOL	R4	0	0
      0xB8160000,  //  0004  GETNGBL	R5	K0
      0x8C140B01,  //  0005  GETMET	R5	R5	K1
      0x7C140200,  //  0006  CALL	R5	1
      0x90160401,  //  0007  SETMBR	R5	K2	R1
      0x90160602,  //  0008  SETMBR	R5	K3	R2
      0x90160803,  //  0009  SETMBR	R5	K4	R3
      0x88180105,  //  000A  GETMBR	R6	R0	K5
      0x88180D06,  //  000B  GETMBR	R6	R6	K6
      0x88180D07,  //  000C  GETMBR	R6	R6	K7
      0x8C180D08,  //  000D  GETMET	R6	R6	K8
      0x5C200A00,  //  000E  MOVE	R8	R5
      0x5C240800,  //  000F  MOVE	R9	R4
      0x7C180600,  //  0010  CALL	R6	3
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _init_basic_commissioning
********************************************************************/
be_local_closure(Matter_Device__init_basic_commissioning,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(count_active_fabrics),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(start_root_basic_commissioning),
    }),
    be_str_weak(_init_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x1C040302,  //  0003  EQ	R1	R1	K2
      0x78060001,  //  0004  JMPF	R1	#0007
      0x8C040103,  //  0005  GETMET	R1	R0	K3
      0x7C040200,  //  0006  CALL	R1	1
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Device_init,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(start),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Wifi_X23Connected),
        /* K4   */  be_nested_str_weak(matter_start),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0xB8020200,  //  0003  GETNGBL	R0	K1
          0x8C000102,  //  0004  GETMET	R0	R0	K2
          0x58080003,  //  0005  LDCONST	R2	K3
          0x580C0004,  //  0006  LDCONST	R3	K4
          0x7C000600,  //  0007  CALL	R0	3
          0x80000000,  //  0008  RET	0
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(start),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Eth_X23Connected),
        /* K4   */  be_nested_str_weak(matter_start),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0xB8020200,  //  0003  GETNGBL	R0	K1
          0x8C000102,  //  0004  GETMET	R0	R0	K2
          0x58080003,  //  0005  LDCONST	R2	K3
          0x580C0004,  //  0006  LDCONST	R3	K4
          0x7C000600,  //  0007  CALL	R0	3
          0x80000000,  //  0008  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[43]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(get_option),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(MATTER_OPTION),
    /* K6   */  be_nested_str_weak(UI),
    /* K7   */  be_nested_str_weak(started),
    /* K8   */  be_nested_str_weak(tick),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(plugins),
    /* K11  */  be_nested_str_weak(plugins_persist),
    /* K12  */  be_nested_str_weak(plugins_classes),
    /* K13  */  be_nested_str_weak(register_native_classes),
    /* K14  */  be_nested_str_weak(vendorid),
    /* K15  */  be_nested_str_weak(VENDOR_ID),
    /* K16  */  be_nested_str_weak(productid),
    /* K17  */  be_nested_str_weak(PRODUCT_ID),
    /* K18  */  be_nested_str_weak(root_iterations),
    /* K19  */  be_nested_str_weak(PBKDF_ITERATIONS),
    /* K20  */  be_nested_str_weak(next_ep),
    /* K21  */  be_const_int(1),
    /* K22  */  be_nested_str_weak(root_salt),
    /* K23  */  be_nested_str_weak(random),
    /* K24  */  be_nested_str_weak(ipv4only),
    /* K25  */  be_nested_str_weak(load_param),
    /* K26  */  be_nested_str_weak(sessions),
    /* K27  */  be_nested_str_weak(Session_Store),
    /* K28  */  be_nested_str_weak(load_fabrics),
    /* K29  */  be_nested_str_weak(message_handler),
    /* K30  */  be_nested_str_weak(MessageHandler),
    /* K31  */  be_nested_str_weak(ui),
    /* K32  */  be_nested_str_weak(wifi),
    /* K33  */  be_nested_str_weak(up),
    /* K34  */  be_nested_str_weak(eth),
    /* K35  */  be_nested_str_weak(start),
    /* K36  */  be_nested_str_weak(add_rule),
    /* K37  */  be_nested_str_weak(Wifi_X23Connected),
    /* K38  */  be_nested_str_weak(matter_start),
    /* K39  */  be_nested_str_weak(Eth_X23Connected),
    /* K40  */  be_nested_str_weak(_init_basic_commissioning),
    /* K41  */  be_nested_str_weak(add_driver),
    /* K42  */  be_nested_str_weak(register_commands),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[103]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xB80E0400,  //  0002  GETNGBL	R3	K2
      0x8C0C0703,  //  0003  GETMET	R3	R3	K3
      0xB8160800,  //  0004  GETNGBL	R5	K4
      0x88140B05,  //  0005  GETMBR	R5	R5	K5
      0x7C0C0400,  //  0006  CALL	R3	2
      0x740E0004,  //  0007  JMPT	R3	#000D
      0xB80E0800,  //  0008  GETNGBL	R3	K4
      0x8C0C0706,  //  0009  GETMET	R3	R3	K6
      0x5C140000,  //  000A  MOVE	R5	R0
      0x7C0C0400,  //  000B  CALL	R3	2
      0x80000600,  //  000C  RET	0
      0x500C0000,  //  000D  LDBOOL	R3	0	0
      0x90020E03,  //  000E  SETMBR	R0	K7	R3
      0x90021109,  //  000F  SETMBR	R0	K8	K9
      0x600C0012,  //  0010  GETGBL	R3	G18
      0x7C0C0000,  //  0011  CALL	R3	0
      0x90021403,  //  0012  SETMBR	R0	K10	R3
      0x500C0000,  //  0013  LDBOOL	R3	0	0
      0x90021603,  //  0014  SETMBR	R0	K11	R3
      0x600C0013,  //  0015  GETGBL	R3	G19
      0x7C0C0000,  //  0016  CALL	R3	0
      0x90021803,  //  0017  SETMBR	R0	K12	R3
      0x8C0C010D,  //  0018  GETMET	R3	R0	K13
      0x7C0C0200,  //  0019  CALL	R3	1
      0x880C010F,  //  001A  GETMBR	R3	R0	K15
      0x90021C03,  //  001B  SETMBR	R0	K14	R3
      0x880C0111,  //  001C  GETMBR	R3	R0	K17
      0x90022003,  //  001D  SETMBR	R0	K16	R3
      0x880C0113,  //  001E  GETMBR	R3	R0	K19
      0x90022403,  //  001F  SETMBR	R0	K18	R3
      0x90022915,  //  0020  SETMBR	R0	K20	K21
      0x8C0C0317,  //  0021  GETMET	R3	R1	K23
      0x5416000F,  //  0022  LDINT	R5	16
      0x7C0C0400,  //  0023  CALL	R3	2
      0x90022C03,  //  0024  SETMBR	R0	K22	R3
      0x500C0000,  //  0025  LDBOOL	R3	0	0
      0x90023003,  //  0026  SETMBR	R0	K24	R3
      0x8C0C0119,  //  0027  GETMET	R3	R0	K25
      0x7C0C0200,  //  0028  CALL	R3	1
      0xB80E0800,  //  0029  GETNGBL	R3	K4
      0x8C0C071B,  //  002A  GETMET	R3	R3	K27
      0x5C140000,  //  002B  MOVE	R5	R0
      0x7C0C0400,  //  002C  CALL	R3	2
      0x90023403,  //  002D  SETMBR	R0	K26	R3
      0x880C011A,  //  002E  GETMBR	R3	R0	K26
      0x8C0C071C,  //  002F  GETMET	R3	R3	K28
      0x7C0C0200,  //  0030  CALL	R3	1
      0xB80E0800,  //  0031  GETNGBL	R3	K4
      0x8C0C071E,  //  0032  GETMET	R3	R3	K30
      0x5C140000,  //  0033  MOVE	R5	R0
      0x7C0C0400,  //  0034  CALL	R3	2
      0x90023A03,  //  0035  SETMBR	R0	K29	R3
      0xB80E0800,  //  0036  GETNGBL	R3	K4
      0x8C0C0706,  //  0037  GETMET	R3	R3	K6
      0x5C140000,  //  0038  MOVE	R5	R0
      0x7C0C0400,  //  0039  CALL	R3	2
      0x90023E03,  //  003A  SETMBR	R0	K31	R3
      0xB80E0400,  //  003B  GETNGBL	R3	K2
      0x8C0C0720,  //  003C  GETMET	R3	R3	K32
      0x7C0C0200,  //  003D  CALL	R3	1
      0x940C0721,  //  003E  GETIDX	R3	R3	K33
      0x740E0004,  //  003F  JMPT	R3	#0045
      0xB80E0400,  //  0040  GETNGBL	R3	K2
      0x8C0C0722,  //  0041  GETMET	R3	R3	K34
      0x7C0C0200,  //  0042  CALL	R3	1
      0x940C0721,  //  0043  GETIDX	R3	R3	K33
      0x780E0001,  //  0044  JMPF	R3	#0047
      0x8C0C0123,  //  0045  GETMET	R3	R0	K35
      0x7C0C0200,  //  0046  CALL	R3	1
      0xB80E0400,  //  0047  GETNGBL	R3	K2
      0x8C0C0720,  //  0048  GETMET	R3	R3	K32
      0x7C0C0200,  //  0049  CALL	R3	1
      0x940C0721,  //  004A  GETIDX	R3	R3	K33
      0x740E0005,  //  004B  JMPT	R3	#0052
      0xB80E0400,  //  004C  GETNGBL	R3	K2
      0x8C0C0724,  //  004D  GETMET	R3	R3	K36
      0x58140025,  //  004E  LDCONST	R5	K37
      0x84180000,  //  004F  CLOSURE	R6	P0
      0x581C0026,  //  0050  LDCONST	R7	K38
      0x7C0C0800,  //  0051  CALL	R3	4
      0xB80E0400,  //  0052  GETNGBL	R3	K2
      0x8C0C0722,  //  0053  GETMET	R3	R3	K34
      0x7C0C0200,  //  0054  CALL	R3	1
      0x940C0721,  //  0055  GETIDX	R3	R3	K33
      0x740E0005,  //  0056  JMPT	R3	#005D
      0xB80E0400,  //  0057  GETNGBL	R3	K2
      0x8C0C0724,  //  0058  GETMET	R3	R3	K36
      0x58140027,  //  0059  LDCONST	R5	K39
      0x84180001,  //  005A  CLOSURE	R6	P1
      0x581C0026,  //  005B  LDCONST	R7	K38
      0x7C0C0800,  //  005C  CALL	R3	4
      0x8C0C0128,  //  005D  GETMET	R3	R0	K40
      0x7C0C0200,  //  005E  CALL	R3	1
      0xB80E0400,  //  005F  GETNGBL	R3	K2
      0x8C0C0729,  //  0060  GETMET	R3	R3	K41
      0x5C140000,  //  0061  MOVE	R5	R0
      0x7C0C0400,  //  0062  CALL	R3	2
      0x8C0C012A,  //  0063  GETMET	R3	R0	K42
      0x7C0C0200,  //  0064  CALL	R3	1
      0xA0000000,  //  0065  CLOSE	R0
      0x80000000,  //  0066  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: k2l
********************************************************************/
be_local_closure(Matter_Device_k2l,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Device),
    /* K1   */  be_nested_str_weak(keys),
    /* K2   */  be_nested_str_weak(push),
    /* K3   */  be_nested_str_weak(stop_iteration),
    /* K4   */  be_const_int(1),
    /* K5   */  be_const_int(0),
    }),
    be_str_weak(k2l),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0003,  //  0004  EQ	R3	R0	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x80040400,  //  0006  RET	1	R2
      0x600C0010,  //  0007  GETGBL	R3	G16
      0x8C100101,  //  0008  GETMET	R4	R0	K1
      0x7C100200,  //  0009  CALL	R4	1
      0x7C0C0200,  //  000A  CALL	R3	1
      0xA8020005,  //  000B  EXBLK	0	#0012
      0x5C100600,  //  000C  MOVE	R4	R3
      0x7C100000,  //  000D  CALL	R4	0
      0x8C140502,  //  000E  GETMET	R5	R2	K2
      0x5C1C0800,  //  000F  MOVE	R7	R4
      0x7C140400,  //  0010  CALL	R5	2
      0x7001FFF9,  //  0011  JMP		#000C
      0x580C0003,  //  0012  LDCONST	R3	K3
      0xAC0C0200,  //  0013  CATCH	R3	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x600C0010,  //  0015  GETGBL	R3	G16
      0x6010000C,  //  0016  GETGBL	R4	G12
      0x5C140400,  //  0017  MOVE	R5	R2
      0x7C100200,  //  0018  CALL	R4	1
      0x04100904,  //  0019  SUB	R4	R4	K4
      0x40120804,  //  001A  CONNECT	R4	K4	R4
      0x7C0C0200,  //  001B  CALL	R3	1
      0xA8020010,  //  001C  EXBLK	0	#002E
      0x5C100600,  //  001D  MOVE	R4	R3
      0x7C100000,  //  001E  CALL	R4	0
      0x94140404,  //  001F  GETIDX	R5	R2	R4
      0x5C180800,  //  0020  MOVE	R6	R4
      0x241C0D05,  //  0021  GT	R7	R6	K5
      0x781E0008,  //  0022  JMPF	R7	#002C
      0x041C0D04,  //  0023  SUB	R7	R6	K4
      0x941C0407,  //  0024  GETIDX	R7	R2	R7
      0x241C0E05,  //  0025  GT	R7	R7	R5
      0x781E0004,  //  0026  JMPF	R7	#002C
      0x041C0D04,  //  0027  SUB	R7	R6	K4
      0x941C0407,  //  0028  GETIDX	R7	R2	R7
      0x98080C07,  //  0029  SETIDX	R2	R6	R7
      0x04180D04,  //  002A  SUB	R6	R6	K4
      0x7001FFF4,  //  002B  JMP		#0021
      0x98080C05,  //  002C  SETIDX	R2	R6	R5
      0x7001FFEE,  //  002D  JMP		#001D
      0x580C0003,  //  002E  LDCONST	R3	K3
      0xAC0C0200,  //  002F  CATCH	R3	1	0
      0xB0080000,  //  0030  RAISE	2	R0	R0
      0x80040400,  //  0031  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: adjust_next_ep
********************************************************************/
be_local_closure(Matter_Device_adjust_next_ep,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(plugins_config),
    /* K1   */  be_nested_str_weak(keys),
    /* K2   */  be_nested_str_weak(next_ep),
    /* K3   */  be_const_int(1),
    /* K4   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(adjust_next_ep),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x8C080501,  //  0002  GETMET	R2	R2	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x7C040200,  //  0004  CALL	R1	1
      0xA802000A,  //  0005  EXBLK	0	#0011
      0x5C080200,  //  0006  MOVE	R2	R1
      0x7C080000,  //  0007  CALL	R2	0
      0x600C0009,  //  0008  GETGBL	R3	G9
      0x5C100400,  //  0009  MOVE	R4	R2
      0x7C0C0200,  //  000A  CALL	R3	1
      0x88100102,  //  000B  GETMBR	R4	R0	K2
      0x28100604,  //  000C  GE	R4	R3	R4
      0x78120001,  //  000D  JMPF	R4	#0010
      0x00100703,  //  000E  ADD	R4	R3	K3
      0x90020404,  //  000F  SETMBR	R0	K2	R4
      0x7001FFF4,  //  0010  JMP		#0006
      0x58040004,  //  0011  LDCONST	R1	K4
      0xAC040200,  //  0012  CATCH	R1	1	0
      0xB0080000,  //  0013  RAISE	2	R0	R0
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_plugin_class_displayname
********************************************************************/
be_local_closure(Matter_Device_get_plugin_class_displayname,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(plugins_classes),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(NAME),
    /* K3   */  be_nested_str_weak(),
    }),
    be_str_weak(get_plugin_class_displayname),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x880C0502,  //  0005  GETMBR	R3	R2	K2
      0x70020000,  //  0006  JMP		#0008
      0x580C0003,  //  0007  LDCONST	R3	K3
      0x80040600,  //  0008  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_announce_op_discovery
********************************************************************/
be_local_closure(Matter_Device_mdns_announce_op_discovery,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[29]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(get_device_id),
    /* K3   */  be_nested_str_weak(copy),
    /* K4   */  be_nested_str_weak(reverse),
    /* K5   */  be_nested_str_weak(get_fabric_compressed),
    /* K6   */  be_nested_str_weak(tohex),
    /* K7   */  be_nested_str_weak(_X2D),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20Operational_X20Discovery_X20node_X20_X3D_X20),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str_weak(eth),
    /* K13  */  be_nested_str_weak(find),
    /* K14  */  be_nested_str_weak(up),
    /* K15  */  be_nested_str_weak(format),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20adding_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
    /* K17  */  be_nested_str_weak(hostname_eth),
    /* K18  */  be_nested_str_weak(add_service),
    /* K19  */  be_nested_str_weak(_matter),
    /* K20  */  be_nested_str_weak(_tcp),
    /* K21  */  be_nested_str_weak(_I),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20adding_X20subtype_X3A_X20),
    /* K23  */  be_nested_str_weak(add_subtype),
    /* K24  */  be_nested_str_weak(wifi),
    /* K25  */  be_nested_str_weak(hostname_wifi),
    /* K26  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K27  */  be_nested_str_weak(_X7C),
    /* K28  */  be_const_int(2),
    }),
    be_str_weak(mdns_announce_op_discovery),
    &be_const_str_solidified,
    ( &(const binstruction[122]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xA8020064,  //  0002  EXBLK	0	#0068
      0x8C100302,  //  0003  GETMET	R4	R1	K2
      0x7C100200,  //  0004  CALL	R4	1
      0x8C100903,  //  0005  GETMET	R4	R4	K3
      0x7C100200,  //  0006  CALL	R4	1
      0x8C100904,  //  0007  GETMET	R4	R4	K4
      0x7C100200,  //  0008  CALL	R4	1
      0x8C140305,  //  0009  GETMET	R5	R1	K5
      0x7C140200,  //  000A  CALL	R5	1
      0x8C180B06,  //  000B  GETMET	R6	R5	K6
      0x7C180200,  //  000C  CALL	R6	1
      0x00180D07,  //  000D  ADD	R6	R6	K7
      0x8C1C0906,  //  000E  GETMET	R7	R4	K6
      0x7C1C0200,  //  000F  CALL	R7	1
      0x00180C07,  //  0010  ADD	R6	R6	R7
      0xB81E1000,  //  0011  GETNGBL	R7	K8
      0x8C1C0F09,  //  0012  GETMET	R7	R7	K9
      0x00261406,  //  0013  ADD	R9	K10	R6
      0x5828000B,  //  0014  LDCONST	R10	K11
      0x7C1C0600,  //  0015  CALL	R7	3
      0xB81E1000,  //  0016  GETNGBL	R7	K8
      0x8C1C0F0C,  //  0017  GETMET	R7	R7	K12
      0x7C1C0200,  //  0018  CALL	R7	1
      0x8C1C0F0D,  //  0019  GETMET	R7	R7	K13
      0x5824000E,  //  001A  LDCONST	R9	K14
      0x7C1C0400,  //  001B  CALL	R7	2
      0x781E0020,  //  001C  JMPF	R7	#003E
      0xB81E1000,  //  001D  GETNGBL	R7	K8
      0x8C1C0F09,  //  001E  GETMET	R7	R7	K9
      0x8C24070F,  //  001F  GETMET	R9	R3	K15
      0x582C0010,  //  0020  LDCONST	R11	K16
      0x5830000C,  //  0021  LDCONST	R12	K12
      0x5C340C00,  //  0022  MOVE	R13	R6
      0x88380111,  //  0023  GETMBR	R14	R0	K17
      0x7C240A00,  //  0024  CALL	R9	5
      0x5828000B,  //  0025  LDCONST	R10	K11
      0x7C1C0600,  //  0026  CALL	R7	3
      0x8C1C0512,  //  0027  GETMET	R7	R2	K18
      0x58240013,  //  0028  LDCONST	R9	K19
      0x58280014,  //  0029  LDCONST	R10	K20
      0x542E15A3,  //  002A  LDINT	R11	5540
      0x4C300000,  //  002B  LDNIL	R12
      0x5C340C00,  //  002C  MOVE	R13	R6
      0x88380111,  //  002D  GETMBR	R14	R0	K17
      0x7C1C0E00,  //  002E  CALL	R7	7
      0x8C1C0B06,  //  002F  GETMET	R7	R5	K6
      0x7C1C0200,  //  0030  CALL	R7	1
      0x001E2A07,  //  0031  ADD	R7	K21	R7
      0xB8221000,  //  0032  GETNGBL	R8	K8
      0x8C201109,  //  0033  GETMET	R8	R8	K9
      0x002A2C07,  //  0034  ADD	R10	K22	R7
      0x582C000B,  //  0035  LDCONST	R11	K11
      0x7C200600,  //  0036  CALL	R8	3
      0x8C200517,  //  0037  GETMET	R8	R2	K23
      0x58280013,  //  0038  LDCONST	R10	K19
      0x582C0014,  //  0039  LDCONST	R11	K20
      0x5C300C00,  //  003A  MOVE	R12	R6
      0x88340111,  //  003B  GETMBR	R13	R0	K17
      0x5C380E00,  //  003C  MOVE	R14	R7
      0x7C200C00,  //  003D  CALL	R8	6
      0xB81E1000,  //  003E  GETNGBL	R7	K8
      0x8C1C0F18,  //  003F  GETMET	R7	R7	K24
      0x7C1C0200,  //  0040  CALL	R7	1
      0x8C1C0F0D,  //  0041  GETMET	R7	R7	K13
      0x5824000E,  //  0042  LDCONST	R9	K14
      0x7C1C0400,  //  0043  CALL	R7	2
      0x781E0020,  //  0044  JMPF	R7	#0066
      0xB81E1000,  //  0045  GETNGBL	R7	K8
      0x8C1C0F09,  //  0046  GETMET	R7	R7	K9
      0x8C24070F,  //  0047  GETMET	R9	R3	K15
      0x582C0010,  //  0048  LDCONST	R11	K16
      0x58300018,  //  0049  LDCONST	R12	K24
      0x5C340C00,  //  004A  MOVE	R13	R6
      0x88380119,  //  004B  GETMBR	R14	R0	K25
      0x7C240A00,  //  004C  CALL	R9	5
      0x5828000B,  //  004D  LDCONST	R10	K11
      0x7C1C0600,  //  004E  CALL	R7	3
      0x8C1C0512,  //  004F  GETMET	R7	R2	K18
      0x58240013,  //  0050  LDCONST	R9	K19
      0x58280014,  //  0051  LDCONST	R10	K20
      0x542E15A3,  //  0052  LDINT	R11	5540
      0x4C300000,  //  0053  LDNIL	R12
      0x5C340C00,  //  0054  MOVE	R13	R6
      0x88380119,  //  0055  GETMBR	R14	R0	K25
      0x7C1C0E00,  //  0056  CALL	R7	7
      0x8C1C0B06,  //  0057  GETMET	R7	R5	K6
      0x7C1C0200,  //  0058  CALL	R7	1
      0x001E2A07,  //  0059  ADD	R7	K21	R7
      0xB8221000,  //  005A  GETNGBL	R8	K8
      0x8C201109,  //  005B  GETMET	R8	R8	K9
      0x002A2C07,  //  005C  ADD	R10	K22	R7
      0x582C000B,  //  005D  LDCONST	R11	K11
      0x7C200600,  //  005E  CALL	R8	3
      0x8C200517,  //  005F  GETMET	R8	R2	K23
      0x58280013,  //  0060  LDCONST	R10	K19
      0x582C0014,  //  0061  LDCONST	R11	K20
      0x5C300C00,  //  0062  MOVE	R12	R6
      0x88340119,  //  0063  GETMBR	R13	R0	K25
      0x5C380E00,  //  0064  MOVE	R14	R7
      0x7C200C00,  //  0065  CALL	R8	6
      0xA8040001,  //  0066  EXBLK	1	1
      0x70020010,  //  0067  JMP		#0079
      0xAC100002,  //  0068  CATCH	R4	0	2
      0x7002000D,  //  0069  JMP		#0078
      0xB81A1000,  //  006A  GETNGBL	R6	K8
      0x8C180D09,  //  006B  GETMET	R6	R6	K9
      0x60200008,  //  006C  GETGBL	R8	G8
      0x5C240800,  //  006D  MOVE	R9	R4
      0x7C200200,  //  006E  CALL	R8	1
      0x00223408,  //  006F  ADD	R8	K26	R8
      0x0020111B,  //  0070  ADD	R8	R8	K27
      0x60240008,  //  0071  GETGBL	R9	G8
      0x5C280A00,  //  0072  MOVE	R10	R5
      0x7C240200,  //  0073  CALL	R9	1
      0x00201009,  //  0074  ADD	R8	R8	R9
      0x5824001C,  //  0075  LDCONST	R9	K28
      0x7C180600,  //  0076  CALL	R6	3
      0x70020000,  //  0077  JMP		#0079
      0xB0080000,  //  0078  RAISE	2	R0	R0
      0x80000000,  //  0079  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clean_remotes
********************************************************************/
be_local_closure(Matter_Device_clean_remotes,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(http_remotes),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(stop_iteration),
    /* K5   */  be_nested_str_weak(plugins),
    /* K6   */  be_nested_str_weak(get),
    /* K7   */  be_nested_str_weak(http_remote),
    /* K8   */  be_nested_str_weak(find),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(keys),
    /* K11  */  be_nested_str_weak(tasmota),
    /* K12  */  be_nested_str_weak(log),
    /* K13  */  be_nested_str_weak(MTR_X3A_X20remove_X20unused_X20remote_X3A_X20),
    /* K14  */  be_nested_str_weak(addr),
    /* K15  */  be_const_int(3),
    /* K16  */  be_nested_str_weak(close),
    /* K17  */  be_nested_str_weak(remove),
    }),
    be_str_weak(clean_remotes),
    &be_const_str_solidified,
    ( &(const binstruction[67]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x780E003D,  //  0003  JMPF	R3	#0042
      0x600C0013,  //  0004  GETGBL	R3	G19
      0x7C0C0000,  //  0005  CALL	R3	0
      0x60100010,  //  0006  GETGBL	R4	G16
      0x88140102,  //  0007  GETMBR	R5	R0	K2
      0x7C100200,  //  0008  CALL	R4	1
      0xA8020003,  //  0009  EXBLK	0	#000E
      0x5C140800,  //  000A  MOVE	R5	R4
      0x7C140000,  //  000B  CALL	R5	0
      0x980C0B03,  //  000C  SETIDX	R3	R5	K3
      0x7001FFFB,  //  000D  JMP		#000A
      0x58100004,  //  000E  LDCONST	R4	K4
      0xAC100200,  //  000F  CATCH	R4	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x60100010,  //  0011  GETGBL	R4	G16
      0x88140105,  //  0012  GETMBR	R5	R0	K5
      0x7C100200,  //  0013  CALL	R4	1
      0xA802000F,  //  0014  EXBLK	0	#0025
      0x5C140800,  //  0015  MOVE	R5	R4
      0x7C140000,  //  0016  CALL	R5	0
      0x8C180306,  //  0017  GETMET	R6	R1	K6
      0x5C200A00,  //  0018  MOVE	R8	R5
      0x58240007,  //  0019  LDCONST	R9	K7
      0x7C180600,  //  001A  CALL	R6	3
      0x4C1C0000,  //  001B  LDNIL	R7
      0x201C0C07,  //  001C  NE	R7	R6	R7
      0x781E0005,  //  001D  JMPF	R7	#0024
      0x8C1C0708,  //  001E  GETMET	R7	R3	K8
      0x5C240C00,  //  001F  MOVE	R9	R6
      0x58280003,  //  0020  LDCONST	R10	K3
      0x7C1C0600,  //  0021  CALL	R7	3
      0x001C0F09,  //  0022  ADD	R7	R7	K9
      0x980C0C07,  //  0023  SETIDX	R3	R6	R7
      0x7001FFEF,  //  0024  JMP		#0015
      0x58100004,  //  0025  LDCONST	R4	K4
      0xAC100200,  //  0026  CATCH	R4	1	0
      0xB0080000,  //  0027  RAISE	2	R0	R0
      0x60100010,  //  0028  GETGBL	R4	G16
      0x8C14070A,  //  0029  GETMET	R5	R3	K10
      0x7C140200,  //  002A  CALL	R5	1
      0x7C100200,  //  002B  CALL	R4	1
      0xA8020011,  //  002C  EXBLK	0	#003F
      0x5C140800,  //  002D  MOVE	R5	R4
      0x7C140000,  //  002E  CALL	R5	0
      0x94180605,  //  002F  GETIDX	R6	R3	R5
      0x1C180D03,  //  0030  EQ	R6	R6	K3
      0x781A000B,  //  0031  JMPF	R6	#003E
      0xB81A1600,  //  0032  GETNGBL	R6	K11
      0x8C180D0C,  //  0033  GETMET	R6	R6	K12
      0x88200B0E,  //  0034  GETMBR	R8	R5	K14
      0x00221A08,  //  0035  ADD	R8	K13	R8
      0x5824000F,  //  0036  LDCONST	R9	K15
      0x7C180600,  //  0037  CALL	R6	3
      0x8C180B10,  //  0038  GETMET	R6	R5	K16
      0x7C180200,  //  0039  CALL	R6	1
      0x88180102,  //  003A  GETMBR	R6	R0	K2
      0x8C180D11,  //  003B  GETMET	R6	R6	K17
      0x5C200A00,  //  003C  MOVE	R8	R5
      0x7C180400,  //  003D  CALL	R6	2
      0x7001FFED,  //  003E  JMP		#002D
      0x58100004,  //  003F  LDCONST	R4	K4
      0xAC100200,  //  0040  CATCH	R4	1	0
      0xB0080000,  //  0041  RAISE	2	R0	R0
      0x80000000,  //  0042  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: k2l_num
********************************************************************/
be_local_closure(Matter_Device_k2l_num,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Device),
    /* K1   */  be_nested_str_weak(keys),
    /* K2   */  be_nested_str_weak(push),
    /* K3   */  be_nested_str_weak(stop_iteration),
    /* K4   */  be_const_int(1),
    /* K5   */  be_const_int(0),
    }),
    be_str_weak(k2l_num),
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0003,  //  0004  EQ	R3	R0	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x80040400,  //  0006  RET	1	R2
      0x600C0010,  //  0007  GETGBL	R3	G16
      0x8C100101,  //  0008  GETMET	R4	R0	K1
      0x7C100200,  //  0009  CALL	R4	1
      0x7C0C0200,  //  000A  CALL	R3	1
      0xA8020007,  //  000B  EXBLK	0	#0014
      0x5C100600,  //  000C  MOVE	R4	R3
      0x7C100000,  //  000D  CALL	R4	0
      0x8C140502,  //  000E  GETMET	R5	R2	K2
      0x601C0009,  //  000F  GETGBL	R7	G9
      0x5C200800,  //  0010  MOVE	R8	R4
      0x7C1C0200,  //  0011  CALL	R7	1
      0x7C140400,  //  0012  CALL	R5	2
      0x7001FFF7,  //  0013  JMP		#000C
      0x580C0003,  //  0014  LDCONST	R3	K3
      0xAC0C0200,  //  0015  CATCH	R3	1	0
      0xB0080000,  //  0016  RAISE	2	R0	R0
      0x600C0010,  //  0017  GETGBL	R3	G16
      0x6010000C,  //  0018  GETGBL	R4	G12
      0x5C140400,  //  0019  MOVE	R5	R2
      0x7C100200,  //  001A  CALL	R4	1
      0x04100904,  //  001B  SUB	R4	R4	K4
      0x40120804,  //  001C  CONNECT	R4	K4	R4
      0x7C0C0200,  //  001D  CALL	R3	1
      0xA8020010,  //  001E  EXBLK	0	#0030
      0x5C100600,  //  001F  MOVE	R4	R3
      0x7C100000,  //  0020  CALL	R4	0
      0x94140404,  //  0021  GETIDX	R5	R2	R4
      0x5C180800,  //  0022  MOVE	R6	R4
      0x241C0D05,  //  0023  GT	R7	R6	K5
      0x781E0008,  //  0024  JMPF	R7	#002E
      0x041C0D04,  //  0025  SUB	R7	R6	K4
      0x941C0407,  //  0026  GETIDX	R7	R2	R7
      0x241C0E05,  //  0027  GT	R7	R7	R5
      0x781E0004,  //  0028  JMPF	R7	#002E
      0x041C0D04,  //  0029  SUB	R7	R6	K4
      0x941C0407,  //  002A  GETIDX	R7	R2	R7
      0x98080C07,  //  002B  SETIDX	R2	R6	R7
      0x04180D04,  //  002C  SUB	R6	R6	K4
      0x7001FFF4,  //  002D  JMP		#0023
      0x98080C05,  //  002E  SETIDX	R2	R6	R5
      0x7001FFEE,  //  002F  JMP		#001F
      0x580C0003,  //  0030  LDCONST	R3	K3
      0xAC0C0200,  //  0031  CATCH	R3	1	0
      0xB0080000,  //  0032  RAISE	2	R0	R0
      0x80040400,  //  0033  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_50ms
********************************************************************/
be_local_closure(Matter_Device_every_50ms,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(tick),
    /* K1   */  be_const_int(1),
    }),
    be_str_weak(every_50ms),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x00040301,  //  0001  ADD	R1	R1	K1
      0x90020001,  //  0002  SETMBR	R0	K0	R1
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(Matter_Device_stop,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(remove_driver),
    /* K2   */  be_nested_str_weak(udp_server),
    /* K3   */  be_nested_str_weak(stop),
    }),
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x88040102,  //  0004  GETMBR	R1	R0	K2
      0x78060002,  //  0005  JMPF	R1	#0009
      0x88040102,  //  0006  GETMBR	R1	R0	K2
      0x8C040303,  //  0007  GETMET	R1	R1	K3
      0x7C040200,  //  0008  CALL	R1	1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_remove_op_discovery_all_fabrics
********************************************************************/
be_local_closure(Matter_Device_mdns_remove_op_discovery_all_fabrics,   /* name */
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
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(active_fabrics),
    /* K2   */  be_nested_str_weak(get_device_id),
    /* K3   */  be_nested_str_weak(get_fabric_id),
    /* K4   */  be_nested_str_weak(mdns_remove_op_discovery),
    /* K5   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(mdns_remove_op_discovery_all_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x8C080501,  //  0002  GETMET	R2	R2	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x7C040200,  //  0004  CALL	R1	1
      0xA802000B,  //  0005  EXBLK	0	#0012
      0x5C080200,  //  0006  MOVE	R2	R1
      0x7C080000,  //  0007  CALL	R2	0
      0x8C0C0502,  //  0008  GETMET	R3	R2	K2
      0x7C0C0200,  //  0009  CALL	R3	1
      0x780E0005,  //  000A  JMPF	R3	#0011
      0x8C0C0503,  //  000B  GETMET	R3	R2	K3
      0x7C0C0200,  //  000C  CALL	R3	1
      0x780E0002,  //  000D  JMPF	R3	#0011
      0x8C0C0104,  //  000E  GETMET	R3	R0	K4
      0x5C140400,  //  000F  MOVE	R5	R2
      0x7C0C0400,  //  0010  CALL	R3	2
      0x7001FFF3,  //  0011  JMP		#0006
      0x58040005,  //  0012  LDCONST	R1	K5
      0xAC040200,  //  0013  CATCH	R1	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_commands
********************************************************************/
be_local_closure(Matter_Device_register_commands,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        10,                          /* nstack */
        4,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(MtrJoin),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 8]) {  /* code */
          0x68100000,  //  0000  GETUPV	R4	U0
          0x8C100900,  //  0001  GETMET	R4	R4	K0
          0x5C180000,  //  0002  MOVE	R6	R0
          0x5C1C0200,  //  0003  MOVE	R7	R1
          0x5C200400,  //  0004  MOVE	R8	R2
          0x5C240600,  //  0005  MOVE	R9	R3
          0x7C100A00,  //  0006  CALL	R4	5
          0x80040800,  //  0007  RET	1	R4
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(add_cmd),
    /* K2   */  be_nested_str_weak(MtrJoin),
    }),
    be_str_weak(register_commands),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x580C0002,  //  0002  LDCONST	R3	K2
      0x84100000,  //  0003  CLOSURE	R4	P0
      0x7C040600,  //  0004  CALL	R1	3
      0xA0000000,  //  0005  CLOSE	R0
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_http_remote
********************************************************************/
be_local_closure(Matter_Device_register_http_remote,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(http_remotes),
    /* K1   */  be_nested_str_weak(contains),
    /* K2   */  be_nested_str_weak(get_timeout),
    /* K3   */  be_nested_str_weak(set_timeout),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(HTTP_remote),
    }),
    be_str_weak(register_http_remote),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x4C100000,  //  0001  LDNIL	R4
      0x1C0C0604,  //  0002  EQ	R3	R3	R4
      0x780E0002,  //  0003  JMPF	R3	#0007
      0x600C0013,  //  0004  GETGBL	R3	G19
      0x7C0C0000,  //  0005  CALL	R3	0
      0x90020003,  //  0006  SETMBR	R0	K0	R3
      0x4C0C0000,  //  0007  LDNIL	R3
      0x88100100,  //  0008  GETMBR	R4	R0	K0
      0x8C100901,  //  0009  GETMET	R4	R4	K1
      0x5C180200,  //  000A  MOVE	R6	R1
      0x7C100400,  //  000B  CALL	R4	2
      0x78120009,  //  000C  JMPF	R4	#0017
      0x88100100,  //  000D  GETMBR	R4	R0	K0
      0x940C0801,  //  000E  GETIDX	R3	R4	R1
      0x8C140702,  //  000F  GETMET	R5	R3	K2
      0x7C140200,  //  0010  CALL	R5	1
      0x14140405,  //  0011  LT	R5	R2	R5
      0x78160002,  //  0012  JMPF	R5	#0016
      0x8C140703,  //  0013  GETMET	R5	R3	K3
      0x5C1C0400,  //  0014  MOVE	R7	R2
      0x7C140400,  //  0015  CALL	R5	2
      0x70020007,  //  0016  JMP		#001F
      0xB8120800,  //  0017  GETNGBL	R4	K4
      0x8C100905,  //  0018  GETMET	R4	R4	K5
      0x5C180200,  //  0019  MOVE	R6	R1
      0x5C1C0400,  //  001A  MOVE	R7	R2
      0x7C100600,  //  001B  CALL	R4	3
      0x5C0C0800,  //  001C  MOVE	R3	R4
      0x88100100,  //  001D  GETMBR	R4	R0	K0
      0x98100203,  //  001E  SETIDX	R4	R1	R3
      0x80040600,  //  001F  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compute_qrcode_content
********************************************************************/
be_local_closure(Matter_Device_compute_qrcode_content,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[12]) {     /* constants */
    /* K0   */  be_nested_str_weak(resize),
    /* K1   */  be_nested_str_weak(setbits),
    /* K2   */  be_const_int(3),
    /* K3   */  be_nested_str_weak(vendorid),
    /* K4   */  be_nested_str_weak(productid),
    /* K5   */  be_nested_str_weak(root_discriminator),
    /* K6   */  be_nested_str_weak(root_passcode),
    /* K7   */  be_const_int(134217727),
    /* K8   */  be_nested_str_weak(MT_X3A),
    /* K9   */  be_nested_str_weak(matter),
    /* K10  */  be_nested_str_weak(Base38),
    /* K11  */  be_nested_str_weak(encode),
    }),
    be_str_weak(compute_qrcode_content),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x60040015,  //  0000  GETGBL	R1	G21
      0x7C040000,  //  0001  CALL	R1	0
      0x8C040300,  //  0002  GETMET	R1	R1	K0
      0x540E000A,  //  0003  LDINT	R3	11
      0x7C040400,  //  0004  CALL	R1	2
      0x8C080301,  //  0005  GETMET	R2	R1	K1
      0x58100002,  //  0006  LDCONST	R4	K2
      0x5416000F,  //  0007  LDINT	R5	16
      0x88180103,  //  0008  GETMBR	R6	R0	K3
      0x7C080800,  //  0009  CALL	R2	4
      0x8C080301,  //  000A  GETMET	R2	R1	K1
      0x54120012,  //  000B  LDINT	R4	19
      0x5416000F,  //  000C  LDINT	R5	16
      0x88180104,  //  000D  GETMBR	R6	R0	K4
      0x7C080800,  //  000E  CALL	R2	4
      0x8C080301,  //  000F  GETMET	R2	R1	K1
      0x54120024,  //  0010  LDINT	R4	37
      0x54160007,  //  0011  LDINT	R5	8
      0x541A0003,  //  0012  LDINT	R6	4
      0x7C080800,  //  0013  CALL	R2	4
      0x8C080301,  //  0014  GETMET	R2	R1	K1
      0x5412002C,  //  0015  LDINT	R4	45
      0x5416000B,  //  0016  LDINT	R5	12
      0x88180105,  //  0017  GETMBR	R6	R0	K5
      0x541E0FFE,  //  0018  LDINT	R7	4095
      0x2C180C07,  //  0019  AND	R6	R6	R7
      0x7C080800,  //  001A  CALL	R2	4
      0x8C080301,  //  001B  GETMET	R2	R1	K1
      0x54120038,  //  001C  LDINT	R4	57
      0x5416001A,  //  001D  LDINT	R5	27
      0x88180106,  //  001E  GETMBR	R6	R0	K6
      0x2C180D07,  //  001F  AND	R6	R6	K7
      0x7C080800,  //  0020  CALL	R2	4
      0xB80A1200,  //  0021  GETNGBL	R2	K9
      0x8808050A,  //  0022  GETMBR	R2	R2	K10
      0x8C08050B,  //  0023  GETMET	R2	R2	K11
      0x5C100200,  //  0024  MOVE	R4	R1
      0x7C080400,  //  0025  CALL	R2	2
      0x000A1002,  //  0026  ADD	R2	K8	R2
      0x80040400,  //  0027  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_operational_discovery
********************************************************************/
be_local_closure(Matter_Device_start_operational_discovery,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(mdns),
    /* K2   */  be_nested_str_weak(string),
    /* K3   */  be_nested_str_weak(stop_basic_commissioning),
    /* K4   */  be_nested_str_weak(root_w0),
    /* K5   */  be_nested_str_weak(root_L),
    /* K6   */  be_nested_str_weak(mdns_announce_op_discovery),
    }),
    be_str_weak(start_operational_discovery),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xA4120400,  //  0002  IMPORT	R4	K2
      0x8C140103,  //  0003  GETMET	R5	R0	K3
      0x7C140200,  //  0004  CALL	R5	1
      0x4C140000,  //  0005  LDNIL	R5
      0x90020805,  //  0006  SETMBR	R0	K4	R5
      0x4C140000,  //  0007  LDNIL	R5
      0x90020A05,  //  0008  SETMBR	R0	K5	R5
      0x8C140106,  //  0009  GETMET	R5	R0	K6
      0x5C1C0200,  //  000A  MOVE	R7	R1
      0x7C140400,  //  000B  CALL	R5	2
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_plugin_class
********************************************************************/
be_local_closure(Matter_Device_register_plugin_class,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(get),
    /* K2   */  be_nested_str_weak(TYPE),
    /* K3   */  be_nested_str_weak(plugins_classes),
    }),
    be_str_weak(register_plugin_class),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0501,  //  0001  GETMET	R3	R2	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x58180002,  //  0003  LDCONST	R6	K2
      0x7C0C0600,  //  0004  CALL	R3	3
      0x780E0001,  //  0005  JMPF	R3	#0008
      0x88100103,  //  0006  GETMBR	R4	R0	K3
      0x98100601,  //  0007  SETIDX	R4	R3	R1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _trigger_read_sensors
********************************************************************/
be_local_closure(Matter_Device__trigger_read_sensors,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(read_sensors),
    /* K3   */  be_nested_str_weak(load),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(plugins),
    /* K6   */  be_nested_str_weak(parse_sensors),
    /* K7   */  be_const_int(1),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20unable_X20to_X20parse_X20read_sensors_X3A_X20),
    /* K10  */  be_const_int(3),
    }),
    be_str_weak(_trigger_read_sensors),
    &be_const_str_solidified,
    ( &(const binstruction[37]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xB80A0200,  //  0001  GETNGBL	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0x7C080200,  //  0003  CALL	R2	1
      0x4C0C0000,  //  0004  LDNIL	R3
      0x1C0C0403,  //  0005  EQ	R3	R2	R3
      0x780E0000,  //  0006  JMPF	R3	#0008
      0x80000600,  //  0007  RET	0
      0x8C0C0303,  //  0008  GETMET	R3	R1	K3
      0x5C140400,  //  0009  MOVE	R5	R2
      0x7C0C0400,  //  000A  CALL	R3	2
      0x4C100000,  //  000B  LDNIL	R4
      0x20100604,  //  000C  NE	R4	R3	R4
      0x7812000D,  //  000D  JMPF	R4	#001C
      0x58100004,  //  000E  LDCONST	R4	K4
      0x6014000C,  //  000F  GETGBL	R5	G12
      0x88180105,  //  0010  GETMBR	R6	R0	K5
      0x7C140200,  //  0011  CALL	R5	1
      0x14140805,  //  0012  LT	R5	R4	R5
      0x78160006,  //  0013  JMPF	R5	#001B
      0x88140105,  //  0014  GETMBR	R5	R0	K5
      0x94140A04,  //  0015  GETIDX	R5	R5	R4
      0x8C140B06,  //  0016  GETMET	R5	R5	K6
      0x5C1C0600,  //  0017  MOVE	R7	R3
      0x7C140400,  //  0018  CALL	R5	2
      0x00100907,  //  0019  ADD	R4	R4	K7
      0x7001FFF3,  //  001A  JMP		#000F
      0x70020007,  //  001B  JMP		#0024
      0xB8120200,  //  001C  GETNGBL	R4	K1
      0x8C100908,  //  001D  GETMET	R4	R4	K8
      0x60180008,  //  001E  GETGBL	R6	G8
      0x5C1C0400,  //  001F  MOVE	R7	R2
      0x7C180200,  //  0020  CALL	R6	1
      0x001A1206,  //  0021  ADD	R6	K9	R6
      0x581C000A,  //  0022  LDCONST	R7	K10
      0x7C100600,  //  0023  CALL	R4	3
      0x80000000,  //  0024  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: bridge_add_endpoint
********************************************************************/
be_local_closure(Matter_Device_bridge_add_endpoint,   /* name */
  be_nested_proto(
    19,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(plugins_classes),
    /* K2   */  be_nested_str_weak(find),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(MTR_X3A_X20unknown_X20class_X20name_X20_X27),
    /* K6   */  be_nested_str_weak(_X27_X20skipping),
    /* K7   */  be_const_int(3),
    /* K8   */  be_nested_str_weak(next_ep),
    /* K9   */  be_nested_str_weak(plugins),
    /* K10  */  be_nested_str_weak(push),
    /* K11  */  be_nested_str_weak(type),
    /* K12  */  be_nested_str_weak(keys),
    /* K13  */  be_nested_str_weak(stop_iteration),
    /* K14  */  be_nested_str_weak(format),
    /* K15  */  be_nested_str_weak(MTR_X3A_X20adding_X20endpoint_X20_X3D_X20_X25i_X20type_X20_X3D_X20_X25s_X25s),
    /* K16  */  be_nested_str_weak(conf_to_log),
    /* K17  */  be_const_int(2),
    /* K18  */  be_nested_str_weak(plugins_config),
    /* K19  */  be_nested_str_weak(plugins_persist),
    /* K20  */  be_const_int(1),
    /* K21  */  be_nested_str_weak(save_param),
    /* K22  */  be_nested_str_weak(signal_endpoints_changed),
    }),
    be_str_weak(bridge_add_endpoint),
    &be_const_str_solidified,
    ( &(const binstruction[71]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0x88100101,  //  0001  GETMBR	R4	R0	K1
      0x8C100902,  //  0002  GETMET	R4	R4	K2
      0x5C180200,  //  0003  MOVE	R6	R1
      0x7C100400,  //  0004  CALL	R4	2
      0x4C140000,  //  0005  LDNIL	R5
      0x1C140805,  //  0006  EQ	R5	R4	R5
      0x78160009,  //  0007  JMPF	R5	#0012
      0xB8160600,  //  0008  GETNGBL	R5	K3
      0x8C140B04,  //  0009  GETMET	R5	R5	K4
      0x601C0008,  //  000A  GETGBL	R7	G8
      0x5C200200,  //  000B  MOVE	R8	R1
      0x7C1C0200,  //  000C  CALL	R7	1
      0x001E0A07,  //  000D  ADD	R7	K5	R7
      0x001C0F06,  //  000E  ADD	R7	R7	K6
      0x58200007,  //  000F  LDCONST	R8	K7
      0x7C140600,  //  0010  CALL	R5	3
      0x80000A00,  //  0011  RET	0
      0x88140108,  //  0012  GETMBR	R5	R0	K8
      0x60180008,  //  0013  GETGBL	R6	G8
      0x5C1C0A00,  //  0014  MOVE	R7	R5
      0x7C180200,  //  0015  CALL	R6	1
      0x5C1C0800,  //  0016  MOVE	R7	R4
      0x5C200000,  //  0017  MOVE	R8	R0
      0x5C240A00,  //  0018  MOVE	R9	R5
      0x5C280400,  //  0019  MOVE	R10	R2
      0x7C1C0600,  //  001A  CALL	R7	3
      0x88200109,  //  001B  GETMBR	R8	R0	K9
      0x8C20110A,  //  001C  GETMET	R8	R8	K10
      0x5C280E00,  //  001D  MOVE	R10	R7
      0x7C200400,  //  001E  CALL	R8	2
      0x60200013,  //  001F  GETGBL	R8	G19
      0x7C200000,  //  0020  CALL	R8	0
      0x98221601,  //  0021  SETIDX	R8	K11	R1
      0x60240010,  //  0022  GETGBL	R9	G16
      0x8C28050C,  //  0023  GETMET	R10	R2	K12
      0x7C280200,  //  0024  CALL	R10	1
      0x7C240200,  //  0025  CALL	R9	1
      0xA8020004,  //  0026  EXBLK	0	#002C
      0x5C281200,  //  0027  MOVE	R10	R9
      0x7C280000,  //  0028  CALL	R10	0
      0x942C040A,  //  0029  GETIDX	R11	R2	R10
      0x9820140B,  //  002A  SETIDX	R8	R10	R11
      0x7001FFFA,  //  002B  JMP		#0027
      0x5824000D,  //  002C  LDCONST	R9	K13
      0xAC240200,  //  002D  CATCH	R9	1	0
      0xB0080000,  //  002E  RAISE	2	R0	R0
      0xB8260600,  //  002F  GETNGBL	R9	K3
      0x8C241304,  //  0030  GETMET	R9	R9	K4
      0x8C2C070E,  //  0031  GETMET	R11	R3	K14
      0x5834000F,  //  0032  LDCONST	R13	K15
      0x5C380A00,  //  0033  MOVE	R14	R5
      0x5C3C0200,  //  0034  MOVE	R15	R1
      0x8C400110,  //  0035  GETMET	R16	R0	K16
      0x5C480400,  //  0036  MOVE	R18	R2
      0x7C400400,  //  0037  CALL	R16	2
      0x7C2C0A00,  //  0038  CALL	R11	5
      0x58300011,  //  0039  LDCONST	R12	K17
      0x7C240600,  //  003A  CALL	R9	3
      0x88240112,  //  003B  GETMBR	R9	R0	K18
      0x98240C08,  //  003C  SETIDX	R9	R6	R8
      0x50240200,  //  003D  LDBOOL	R9	1	0
      0x90022609,  //  003E  SETMBR	R0	K19	R9
      0x88240108,  //  003F  GETMBR	R9	R0	K8
      0x00241314,  //  0040  ADD	R9	R9	K20
      0x90021009,  //  0041  SETMBR	R0	K8	R9
      0x8C240115,  //  0042  GETMET	R9	R0	K21
      0x7C240200,  //  0043  CALL	R9	1
      0x8C240116,  //  0044  GETMET	R9	R0	K22
      0x7C240200,  //  0045  CALL	R9	1
      0x80040A00,  //  0046  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_param
********************************************************************/
be_local_closure(Matter_Device_load_param,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[32]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(crypto),
    /* K2   */  be_nested_str_weak(FILENAME),
    /* K3   */  be_nested_str_weak(read),
    /* K4   */  be_nested_str_weak(close),
    /* K5   */  be_nested_str_weak(json),
    /* K6   */  be_nested_str_weak(load),
    /* K7   */  be_nested_str_weak(root_discriminator),
    /* K8   */  be_nested_str_weak(find),
    /* K9   */  be_nested_str_weak(distinguish),
    /* K10  */  be_nested_str_weak(root_passcode),
    /* K11  */  be_nested_str_weak(passcode),
    /* K12  */  be_nested_str_weak(ipv4only),
    /* K13  */  be_nested_str_weak(next_ep),
    /* K14  */  be_nested_str_weak(nextep),
    /* K15  */  be_nested_str_weak(plugins_config),
    /* K16  */  be_nested_str_weak(config),
    /* K17  */  be_nested_str_weak(tasmota),
    /* K18  */  be_nested_str_weak(log),
    /* K19  */  be_nested_str_weak(MTR_X3A_X20load_config_X20_X3D_X20),
    /* K20  */  be_const_int(3),
    /* K21  */  be_nested_str_weak(adjust_next_ep),
    /* K22  */  be_nested_str_weak(plugins_persist),
    /* K23  */  be_nested_str_weak(io_error),
    /* K24  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Aload_X20Exception_X3A),
    /* K25  */  be_nested_str_weak(_X7C),
    /* K26  */  be_const_int(2),
    /* K27  */  be_nested_str_weak(random),
    /* K28  */  be_nested_str_weak(get),
    /* K29  */  be_const_int(0),
    /* K30  */  be_nested_str_weak(generate_random_passcode),
    /* K31  */  be_nested_str_weak(save_param),
    }),
    be_str_weak(load_param),
    &be_const_str_solidified,
    ( &(const binstruction[105]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA8020036,  //  0002  EXBLK	0	#003A
      0x600C0011,  //  0003  GETGBL	R3	G17
      0x88100102,  //  0004  GETMBR	R4	R0	K2
      0x7C0C0200,  //  0005  CALL	R3	1
      0x8C100703,  //  0006  GETMET	R4	R3	K3
      0x7C100200,  //  0007  CALL	R4	1
      0x8C140704,  //  0008  GETMET	R5	R3	K4
      0x7C140200,  //  0009  CALL	R5	1
      0xA4160A00,  //  000A  IMPORT	R5	K5
      0x8C180B06,  //  000B  GETMET	R6	R5	K6
      0x5C200800,  //  000C  MOVE	R8	R4
      0x7C180400,  //  000D  CALL	R6	2
      0x8C1C0D08,  //  000E  GETMET	R7	R6	K8
      0x58240009,  //  000F  LDCONST	R9	K9
      0x88280107,  //  0010  GETMBR	R10	R0	K7
      0x7C1C0600,  //  0011  CALL	R7	3
      0x90020E07,  //  0012  SETMBR	R0	K7	R7
      0x8C1C0D08,  //  0013  GETMET	R7	R6	K8
      0x5824000B,  //  0014  LDCONST	R9	K11
      0x8828010A,  //  0015  GETMBR	R10	R0	K10
      0x7C1C0600,  //  0016  CALL	R7	3
      0x90021407,  //  0017  SETMBR	R0	K10	R7
      0x601C0017,  //  0018  GETGBL	R7	G23
      0x8C200D08,  //  0019  GETMET	R8	R6	K8
      0x5828000C,  //  001A  LDCONST	R10	K12
      0x502C0000,  //  001B  LDBOOL	R11	0	0
      0x7C200600,  //  001C  CALL	R8	3
      0x7C1C0200,  //  001D  CALL	R7	1
      0x90021807,  //  001E  SETMBR	R0	K12	R7
      0x8C1C0D08,  //  001F  GETMET	R7	R6	K8
      0x5824000E,  //  0020  LDCONST	R9	K14
      0x8828010D,  //  0021  GETMBR	R10	R0	K13
      0x7C1C0600,  //  0022  CALL	R7	3
      0x90021A07,  //  0023  SETMBR	R0	K13	R7
      0x8C1C0D08,  //  0024  GETMET	R7	R6	K8
      0x58240010,  //  0025  LDCONST	R9	K16
      0x7C1C0400,  //  0026  CALL	R7	2
      0x90021E07,  //  0027  SETMBR	R0	K15	R7
      0x881C010F,  //  0028  GETMBR	R7	R0	K15
      0x4C200000,  //  0029  LDNIL	R8
      0x201C0E08,  //  002A  NE	R7	R7	R8
      0x781E000B,  //  002B  JMPF	R7	#0038
      0xB81E2200,  //  002C  GETNGBL	R7	K17
      0x8C1C0F12,  //  002D  GETMET	R7	R7	K18
      0x60240008,  //  002E  GETGBL	R9	G8
      0x8828010F,  //  002F  GETMBR	R10	R0	K15
      0x7C240200,  //  0030  CALL	R9	1
      0x00262609,  //  0031  ADD	R9	K19	R9
      0x58280014,  //  0032  LDCONST	R10	K20
      0x7C1C0600,  //  0033  CALL	R7	3
      0x8C1C0115,  //  0034  GETMET	R7	R0	K21
      0x7C1C0200,  //  0035  CALL	R7	1
      0x501C0200,  //  0036  LDBOOL	R7	1	0
      0x90022C07,  //  0037  SETMBR	R0	K22	R7
      0xA8040001,  //  0038  EXBLK	1	1
      0x70020012,  //  0039  JMP		#004D
      0xAC0C0002,  //  003A  CATCH	R3	0	2
      0x7002000F,  //  003B  JMP		#004C
      0x20140717,  //  003C  NE	R5	R3	K23
      0x7816000C,  //  003D  JMPF	R5	#004B
      0xB8162200,  //  003E  GETNGBL	R5	K17
      0x8C140B12,  //  003F  GETMET	R5	R5	K18
      0x601C0008,  //  0040  GETGBL	R7	G8
      0x5C200600,  //  0041  MOVE	R8	R3
      0x7C1C0200,  //  0042  CALL	R7	1
      0x001E3007,  //  0043  ADD	R7	K24	R7
      0x001C0F19,  //  0044  ADD	R7	R7	K25
      0x60200008,  //  0045  GETGBL	R8	G8
      0x5C240800,  //  0046  MOVE	R9	R4
      0x7C200200,  //  0047  CALL	R8	1
      0x001C0E08,  //  0048  ADD	R7	R7	R8
      0x5820001A,  //  0049  LDCONST	R8	K26
      0x7C140600,  //  004A  CALL	R5	3
      0x70020000,  //  004B  JMP		#004D
      0xB0080000,  //  004C  RAISE	2	R0	R0
      0x500C0000,  //  004D  LDBOOL	R3	0	0
      0x88100107,  //  004E  GETMBR	R4	R0	K7
      0x4C140000,  //  004F  LDNIL	R5
      0x1C100805,  //  0050  EQ	R4	R4	R5
      0x7812000A,  //  0051  JMPF	R4	#005D
      0x8C10051B,  //  0052  GETMET	R4	R2	K27
      0x5818001A,  //  0053  LDCONST	R6	K26
      0x7C100400,  //  0054  CALL	R4	2
      0x8C10091C,  //  0055  GETMET	R4	R4	K28
      0x5818001D,  //  0056  LDCONST	R6	K29
      0x581C001A,  //  0057  LDCONST	R7	K26
      0x7C100600,  //  0058  CALL	R4	3
      0x54160FFE,  //  0059  LDINT	R5	4095
      0x2C100805,  //  005A  AND	R4	R4	R5
      0x90020E04,  //  005B  SETMBR	R0	K7	R4
      0x500C0200,  //  005C  LDBOOL	R3	1	0
      0x8810010A,  //  005D  GETMBR	R4	R0	K10
      0x4C140000,  //  005E  LDNIL	R5
      0x1C100805,  //  005F  EQ	R4	R4	R5
      0x78120003,  //  0060  JMPF	R4	#0065
      0x8C10011E,  //  0061  GETMET	R4	R0	K30
      0x7C100200,  //  0062  CALL	R4	1
      0x90021404,  //  0063  SETMBR	R0	K10	R4
      0x500C0200,  //  0064  LDBOOL	R3	1	0
      0x780E0001,  //  0065  JMPF	R3	#0068
      0x8C10011F,  //  0066  GETMET	R4	R0	K31
      0x7C100200,  //  0067  CALL	R4	1
      0x80000000,  //  0068  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compute_manual_pairing_code
********************************************************************/
be_local_closure(Matter_Device_compute_manual_pairing_code,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(root_discriminator),
    /* K2   */  be_nested_str_weak(root_passcode),
    /* K3   */  be_nested_str_weak(format),
    /* K4   */  be_nested_str_weak(_X251i_X2505i_X2504i),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(Verhoeff),
    /* K7   */  be_nested_str_weak(checksum),
    }),
    be_str_weak(compute_manual_pairing_code),
    &be_const_str_solidified,
    ( &(const binstruction[31]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x540E0FFE,  //  0002  LDINT	R3	4095
      0x2C080403,  //  0003  AND	R2	R2	R3
      0x540E0009,  //  0004  LDINT	R3	10
      0x3C080403,  //  0005  SHR	R2	R2	R3
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x541202FF,  //  0007  LDINT	R4	768
      0x2C0C0604,  //  0008  AND	R3	R3	R4
      0x54120005,  //  0009  LDINT	R4	6
      0x380C0604,  //  000A  SHL	R3	R3	R4
      0x88100102,  //  000B  GETMBR	R4	R0	K2
      0x54163FFE,  //  000C  LDINT	R5	16383
      0x2C100805,  //  000D  AND	R4	R4	R5
      0x300C0604,  //  000E  OR	R3	R3	R4
      0x88100102,  //  000F  GETMBR	R4	R0	K2
      0x5416000D,  //  0010  LDINT	R5	14
      0x3C100805,  //  0011  SHR	R4	R4	R5
      0x8C140303,  //  0012  GETMET	R5	R1	K3
      0x581C0004,  //  0013  LDCONST	R7	K4
      0x5C200400,  //  0014  MOVE	R8	R2
      0x5C240600,  //  0015  MOVE	R9	R3
      0x5C280800,  //  0016  MOVE	R10	R4
      0x7C140A00,  //  0017  CALL	R5	5
      0xB81A0A00,  //  0018  GETNGBL	R6	K5
      0x88180D06,  //  0019  GETMBR	R6	R6	K6
      0x8C180D07,  //  001A  GETMET	R6	R6	K7
      0x5C200A00,  //  001B  MOVE	R8	R5
      0x7C180400,  //  001C  CALL	R6	2
      0x00140A06,  //  001D  ADD	R5	R5	R6
      0x80040A00,  //  001E  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Device_invoke_request,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(endpoint),
    /* K2   */  be_nested_str_weak(plugins),
    /* K3   */  be_nested_str_weak(invoke_request),
    /* K4   */  be_const_int(1),
    /* K5   */  be_nested_str_weak(status),
    /* K6   */  be_nested_str_weak(matter),
    /* K7   */  be_nested_str_weak(UNSUPPORTED_ENDPOINT),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x58100000,  //  0000  LDCONST	R4	K0
      0x88140701,  //  0001  GETMBR	R5	R3	K1
      0x6018000C,  //  0002  GETGBL	R6	G12
      0x881C0102,  //  0003  GETMBR	R7	R0	K2
      0x7C180200,  //  0004  CALL	R6	1
      0x14180806,  //  0005  LT	R6	R4	R6
      0x781A000C,  //  0006  JMPF	R6	#0014
      0x88180102,  //  0007  GETMBR	R6	R0	K2
      0x94180C04,  //  0008  GETIDX	R6	R6	R4
      0x881C0D01,  //  0009  GETMBR	R7	R6	K1
      0x1C1C0E05,  //  000A  EQ	R7	R7	R5
      0x781E0005,  //  000B  JMPF	R7	#0012
      0x8C1C0D03,  //  000C  GETMET	R7	R6	K3
      0x5C240200,  //  000D  MOVE	R9	R1
      0x5C280400,  //  000E  MOVE	R10	R2
      0x5C2C0600,  //  000F  MOVE	R11	R3
      0x7C1C0800,  //  0010  CALL	R7	4
      0x80040E00,  //  0011  RET	1	R7
      0x00100904,  //  0012  ADD	R4	R4	K4
      0x7001FFED,  //  0013  JMP		#0002
      0xB81A0C00,  //  0014  GETNGBL	R6	K6
      0x88180D07,  //  0015  GETMBR	R6	R6	K7
      0x900E0A06,  //  0016  SETMBR	R3	K5	R6
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_operational_discovery_deferred
********************************************************************/
be_local_closure(Matter_Device_start_operational_discovery_deferred,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        3,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 2]) {  /* upvals */
          be_local_const_upval(1, 0),
          be_local_const_upval(1, 1),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 1]) {     /* constants */
        /* K0   */  be_nested_str_weak(start_operational_discovery),
        }),
        be_str_weak(_X3Clambda_X3E),
        &be_const_str_solidified,
        ( &(const binstruction[ 5]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x68080001,  //  0002  GETUPV	R2	U1
          0x7C000400,  //  0003  CALL	R0	2
          0x80040000,  //  0004  RET	1	R0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(set_timer),
    /* K2   */  be_const_int(0),
    }),
    be_str_weak(start_operational_discovery_deferred),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x84140000,  //  0003  CLOSURE	R5	P0
      0x7C080600,  //  0004  CALL	R2	3
      0xA0000000,  //  0005  CLOSE	R0
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: autoconf_device_map
********************************************************************/
be_local_closure(Matter_Device_autoconf_device_map,   /* name */
  be_nested_proto(
    22,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[38]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_const_int(1),
    /* K3   */  be_nested_str_weak(light),
    /* K4   */  be_nested_str_weak(get),
    /* K5   */  be_nested_str_weak(find),
    /* K6   */  be_nested_str_weak(channels),
    /* K7   */  be_nested_str_weak(),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(type),
    /* K10  */  be_nested_str_weak(light1),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(light2),
    /* K13  */  be_nested_str_weak(light3),
    /* K14  */  be_nested_str_weak(tasmota),
    /* K15  */  be_nested_str_weak(cmd),
    /* K16  */  be_nested_str_weak(Status_X2013),
    /* K17  */  be_nested_str_weak(log),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20Status_X2013_X20_X3D_X20),
    /* K19  */  be_const_int(3),
    /* K20  */  be_nested_str_weak(contains),
    /* K21  */  be_nested_str_weak(StatusSHT),
    /* K22  */  be_nested_str_weak(SHT),
    /* K23  */  be_nested_str_weak(format),
    /* K24  */  be_nested_str_weak(MTR_X3A_X20_X27_X25s_X27_X20_X3D_X20_X25s),
    /* K25  */  be_nested_str_weak(Relay1),
    /* K26  */  be_nested_str_weak(Relay2),
    /* K27  */  be_nested_str_weak(push),
    /* K28  */  be_nested_str_weak(MTR_X3A_X20relay1_X20_X3D_X20_X25s_X2C_X20relay2_X20_X3D_X20_X25s),
    /* K29  */  be_nested_str_weak(TiltConfig),
    /* K30  */  be_nested_str_weak(shutter_X2Btilt),
    /* K31  */  be_nested_str_weak(shutter),
    /* K32  */  be_nested_str_weak(get_power),
    /* K33  */  be_nested_str_weak(relay),
    /* K34  */  be_nested_str_weak(load),
    /* K35  */  be_nested_str_weak(read_sensors),
    /* K36  */  be_nested_str_weak(autoconf_sensors_list),
    /* K37  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(autoconf_device_map),
    &be_const_str_solidified,
    ( &(const binstruction[200]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x600C0013,  //  0002  GETGBL	R3	G19
      0x7C0C0000,  //  0003  CALL	R3	0
      0x58100002,  //  0004  LDCONST	R4	K2
      0x50140000,  //  0005  LDBOOL	R5	0	0
      0xA41A0600,  //  0006  IMPORT	R6	K3
      0x8C1C0D04,  //  0007  GETMET	R7	R6	K4
      0x7C1C0200,  //  0008  CALL	R7	1
      0x4C200000,  //  0009  LDNIL	R8
      0x20200E08,  //  000A  NE	R8	R7	R8
      0x78220024,  //  000B  JMPF	R8	#0031
      0x6020000C,  //  000C  GETGBL	R8	G12
      0x8C240F05,  //  000D  GETMET	R9	R7	K5
      0x582C0006,  //  000E  LDCONST	R11	K6
      0x58300007,  //  000F  LDCONST	R12	K7
      0x7C240600,  //  0010  CALL	R9	3
      0x7C200200,  //  0011  CALL	R8	1
      0x24241108,  //  0012  GT	R9	R8	K8
      0x7826001C,  //  0013  JMPF	R9	#0031
      0x1C241102,  //  0014  EQ	R9	R8	K2
      0x78260007,  //  0015  JMPF	R9	#001E
      0x60240008,  //  0016  GETGBL	R9	G8
      0x5C280800,  //  0017  MOVE	R10	R4
      0x7C240200,  //  0018  CALL	R9	1
      0x60280013,  //  0019  GETGBL	R10	G19
      0x7C280000,  //  001A  CALL	R10	0
      0x982A130A,  //  001B  SETIDX	R10	K9	K10
      0x980C120A,  //  001C  SETIDX	R3	R9	R10
      0x70020010,  //  001D  JMP		#002F
      0x1C24110B,  //  001E  EQ	R9	R8	K11
      0x78260007,  //  001F  JMPF	R9	#0028
      0x60240008,  //  0020  GETGBL	R9	G8
      0x5C280800,  //  0021  MOVE	R10	R4
      0x7C240200,  //  0022  CALL	R9	1
      0x60280013,  //  0023  GETGBL	R10	G19
      0x7C280000,  //  0024  CALL	R10	0
      0x982A130C,  //  0025  SETIDX	R10	K9	K12
      0x980C120A,  //  0026  SETIDX	R3	R9	R10
      0x70020006,  //  0027  JMP		#002F
      0x60240008,  //  0028  GETGBL	R9	G8
      0x5C280800,  //  0029  MOVE	R10	R4
      0x7C240200,  //  002A  CALL	R9	1
      0x60280013,  //  002B  GETGBL	R10	G19
      0x7C280000,  //  002C  CALL	R10	0
      0x982A130D,  //  002D  SETIDX	R10	K9	K13
      0x980C120A,  //  002E  SETIDX	R3	R9	R10
      0x50140200,  //  002F  LDBOOL	R5	1	0
      0x00100902,  //  0030  ADD	R4	R4	K2
      0xB8221C00,  //  0031  GETNGBL	R8	K14
      0x8C20110F,  //  0032  GETMET	R8	R8	K15
      0x58280010,  //  0033  LDCONST	R10	K16
      0x502C0200,  //  0034  LDBOOL	R11	1	0
      0x7C200600,  //  0035  CALL	R8	3
      0x60240012,  //  0036  GETGBL	R9	G18
      0x7C240000,  //  0037  CALL	R9	0
      0xB82A1C00,  //  0038  GETNGBL	R10	K14
      0x8C281511,  //  0039  GETMET	R10	R10	K17
      0x60300008,  //  003A  GETGBL	R12	G8
      0x5C341000,  //  003B  MOVE	R13	R8
      0x7C300200,  //  003C  CALL	R12	1
      0x0032240C,  //  003D  ADD	R12	K18	R12
      0x58340013,  //  003E  LDCONST	R13	K19
      0x7C280600,  //  003F  CALL	R10	3
      0x4C280000,  //  0040  LDNIL	R10
      0x2028100A,  //  0041  NE	R10	R8	R10
      0x782A0051,  //  0042  JMPF	R10	#0095
      0x8C281114,  //  0043  GETMET	R10	R8	K20
      0x58300015,  //  0044  LDCONST	R12	K21
      0x7C280400,  //  0045  CALL	R10	2
      0x782A004D,  //  0046  JMPF	R10	#0095
      0x94201115,  //  0047  GETIDX	R8	R8	K21
      0x58280008,  //  0048  LDCONST	R10	K8
      0x502C0200,  //  0049  LDBOOL	R11	1	0
      0x782E0049,  //  004A  JMPF	R11	#0095
      0x602C0008,  //  004B  GETGBL	R11	G8
      0x5C301400,  //  004C  MOVE	R12	R10
      0x7C2C0200,  //  004D  CALL	R11	1
      0x002E2C0B,  //  004E  ADD	R11	K22	R11
      0x8C301114,  //  004F  GETMET	R12	R8	K20
      0x5C381600,  //  0050  MOVE	R14	R11
      0x7C300400,  //  0051  CALL	R12	2
      0x74320000,  //  0052  JMPT	R12	#0054
      0x70020040,  //  0053  JMP		#0095
      0x9430100B,  //  0054  GETIDX	R12	R8	R11
      0xB8361C00,  //  0055  GETNGBL	R13	K14
      0x8C341B11,  //  0056  GETMET	R13	R13	K17
      0x8C3C0317,  //  0057  GETMET	R15	R1	K23
      0x58440018,  //  0058  LDCONST	R17	K24
      0x5C481600,  //  0059  MOVE	R18	R11
      0x604C0008,  //  005A  GETGBL	R19	G8
      0x5C501800,  //  005B  MOVE	R20	R12
      0x7C4C0200,  //  005C  CALL	R19	1
      0x7C3C0800,  //  005D  CALL	R15	4
      0x58400013,  //  005E  LDCONST	R16	K19
      0x7C340600,  //  005F  CALL	R13	3
      0x8C341905,  //  0060  GETMET	R13	R12	K5
      0x583C0019,  //  0061  LDCONST	R15	K25
      0x58400008,  //  0062  LDCONST	R16	K8
      0x7C340600,  //  0063  CALL	R13	3
      0x04341B02,  //  0064  SUB	R13	R13	K2
      0x8C381905,  //  0065  GETMET	R14	R12	K5
      0x5840001A,  //  0066  LDCONST	R16	K26
      0x58440008,  //  0067  LDCONST	R17	K8
      0x7C380600,  //  0068  CALL	R14	3
      0x04381D02,  //  0069  SUB	R14	R14	K2
      0x283C1B08,  //  006A  GE	R15	R13	K8
      0x783E0002,  //  006B  JMPF	R15	#006F
      0x8C3C131B,  //  006C  GETMET	R15	R9	K27
      0x5C441A00,  //  006D  MOVE	R17	R13
      0x7C3C0400,  //  006E  CALL	R15	2
      0x283C1D08,  //  006F  GE	R15	R14	K8
      0x783E0002,  //  0070  JMPF	R15	#0074
      0x8C3C131B,  //  0071  GETMET	R15	R9	K27
      0x5C441C00,  //  0072  MOVE	R17	R14
      0x7C3C0400,  //  0073  CALL	R15	2
      0xB83E1C00,  //  0074  GETNGBL	R15	K14
      0x8C3C1F11,  //  0075  GETMET	R15	R15	K17
      0x8C440317,  //  0076  GETMET	R17	R1	K23
      0x584C001C,  //  0077  LDCONST	R19	K28
      0x5C501A00,  //  0078  MOVE	R20	R13
      0x5C541C00,  //  0079  MOVE	R21	R14
      0x7C440800,  //  007A  CALL	R17	4
      0x58480013,  //  007B  LDCONST	R18	K19
      0x7C3C0600,  //  007C  CALL	R15	3
      0x8C3C1905,  //  007D  GETMET	R15	R12	K5
      0x5844001D,  //  007E  LDCONST	R17	K29
      0x7C3C0400,  //  007F  CALL	R15	2
      0x783E0002,  //  0080  JMPF	R15	#0084
      0x94401F0B,  //  0081  GETIDX	R16	R15	K11
      0x24402108,  //  0082  GT	R16	R16	K8
      0x74420000,  //  0083  JMPT	R16	#0085
      0x50400001,  //  0084  LDBOOL	R16	0	1
      0x50400200,  //  0085  LDBOOL	R16	1	0
      0x60440008,  //  0086  GETGBL	R17	G8
      0x5C480800,  //  0087  MOVE	R18	R4
      0x7C440200,  //  0088  CALL	R17	1
      0x60480013,  //  0089  GETGBL	R18	G19
      0x7C480000,  //  008A  CALL	R18	0
      0x78420001,  //  008B  JMPF	R16	#008E
      0x584C001E,  //  008C  LDCONST	R19	K30
      0x70020000,  //  008D  JMP		#008F
      0x584C001F,  //  008E  LDCONST	R19	K31
      0x984A1213,  //  008F  SETIDX	R18	K9	R19
      0x984A3E0A,  //  0090  SETIDX	R18	K31	R10
      0x980C2212,  //  0091  SETIDX	R3	R17	R18
      0x00100902,  //  0092  ADD	R4	R4	K2
      0x00281502,  //  0093  ADD	R10	R10	K2
      0x7001FFB3,  //  0094  JMP		#0049
      0x6028000C,  //  0095  GETGBL	R10	G12
      0xB82E1C00,  //  0096  GETNGBL	R11	K14
      0x8C2C1720,  //  0097  GETMET	R11	R11	K32
      0x7C2C0200,  //  0098  CALL	R11	1
      0x7C280200,  //  0099  CALL	R10	1
      0x582C0008,  //  009A  LDCONST	R11	K8
      0x78160000,  //  009B  JMPF	R5	#009D
      0x04281502,  //  009C  SUB	R10	R10	K2
      0x1430160A,  //  009D  LT	R12	R11	R10
      0x78320010,  //  009E  JMPF	R12	#00B0
      0x8C301305,  //  009F  GETMET	R12	R9	K5
      0x5C381600,  //  00A0  MOVE	R14	R11
      0x7C300400,  //  00A1  CALL	R12	2
      0x4C340000,  //  00A2  LDNIL	R13
      0x1C30180D,  //  00A3  EQ	R12	R12	R13
      0x78320008,  //  00A4  JMPF	R12	#00AE
      0x60300008,  //  00A5  GETGBL	R12	G8
      0x5C340800,  //  00A6  MOVE	R13	R4
      0x7C300200,  //  00A7  CALL	R12	1
      0x60340013,  //  00A8  GETGBL	R13	G19
      0x7C340000,  //  00A9  CALL	R13	0
      0x98361321,  //  00AA  SETIDX	R13	K9	K33
      0x9836420B,  //  00AB  SETIDX	R13	K33	R11
      0x980C180D,  //  00AC  SETIDX	R3	R12	R13
      0x00100902,  //  00AD  ADD	R4	R4	K2
      0x002C1702,  //  00AE  ADD	R11	R11	K2
      0x7001FFEC,  //  00AF  JMP		#009D
      0x8C300522,  //  00B0  GETMET	R12	R2	K34
      0xB83A1C00,  //  00B1  GETNGBL	R14	K14
      0x8C381D23,  //  00B2  GETMET	R14	R14	K35
      0x7C380200,  //  00B3  CALL	R14	1
      0x7C300400,  //  00B4  CALL	R12	2
      0x8C340124,  //  00B5  GETMET	R13	R0	K36
      0x5C3C1800,  //  00B6  MOVE	R15	R12
      0x7C340400,  //  00B7  CALL	R13	2
      0x60380010,  //  00B8  GETGBL	R14	G16
      0x5C3C1A00,  //  00B9  MOVE	R15	R13
      0x7C380200,  //  00BA  CALL	R14	1
      0xA8020007,  //  00BB  EXBLK	0	#00C4
      0x5C3C1C00,  //  00BC  MOVE	R15	R14
      0x7C3C0000,  //  00BD  CALL	R15	0
      0x60400008,  //  00BE  GETGBL	R16	G8
      0x5C440800,  //  00BF  MOVE	R17	R4
      0x7C400200,  //  00C0  CALL	R16	1
      0x980C200F,  //  00C1  SETIDX	R3	R16	R15
      0x00100902,  //  00C2  ADD	R4	R4	K2
      0x7001FFF7,  //  00C3  JMP		#00BC
      0x58380025,  //  00C4  LDCONST	R14	K37
      0xAC380200,  //  00C5  CATCH	R14	1	0
      0xB0080000,  //  00C6  RAISE	2	R0	R0
      0x80040600,  //  00C7  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: autoconf_device
********************************************************************/
be_local_closure(Matter_Device_autoconf_device,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(json),
    /* K2   */  be_nested_str_weak(plugins),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(plugins_persist),
    /* K5   */  be_nested_str_weak(plugins_config),
    /* K6   */  be_nested_str_weak(autoconf_device_map),
    /* K7   */  be_nested_str_weak(adjust_next_ep),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20autoconfig_X20_X3D_X20),
    /* K11  */  be_const_int(3),
    /* K12  */  be_nested_str_weak(_instantiate_plugins_from_config),
    /* K13  */  be_nested_str_weak(sessions),
    /* K14  */  be_nested_str_weak(count_active_fabrics),
    /* K15  */  be_nested_str_weak(save_param),
    }),
    be_str_weak(autoconf_device),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x88100102,  //  0003  GETMBR	R4	R0	K2
      0x7C0C0200,  //  0004  CALL	R3	1
      0x240C0703,  //  0005  GT	R3	R3	K3
      0x780E0000,  //  0006  JMPF	R3	#0008
      0x80000600,  //  0007  RET	0
      0x880C0104,  //  0008  GETMBR	R3	R0	K4
      0x740E000C,  //  0009  JMPT	R3	#0017
      0x8C0C0106,  //  000A  GETMET	R3	R0	K6
      0x7C0C0200,  //  000B  CALL	R3	1
      0x90020A03,  //  000C  SETMBR	R0	K5	R3
      0x8C0C0107,  //  000D  GETMET	R3	R0	K7
      0x7C0C0200,  //  000E  CALL	R3	1
      0xB80E1000,  //  000F  GETNGBL	R3	K8
      0x8C0C0709,  //  0010  GETMET	R3	R3	K9
      0x60140008,  //  0011  GETGBL	R5	G8
      0x88180105,  //  0012  GETMBR	R6	R0	K5
      0x7C140200,  //  0013  CALL	R5	1
      0x00161405,  //  0014  ADD	R5	K10	R5
      0x5818000B,  //  0015  LDCONST	R6	K11
      0x7C0C0600,  //  0016  CALL	R3	3
      0x8C0C010C,  //  0017  GETMET	R3	R0	K12
      0x88140105,  //  0018  GETMBR	R5	R0	K5
      0x7C0C0400,  //  0019  CALL	R3	2
      0x880C0104,  //  001A  GETMBR	R3	R0	K4
      0x740E0008,  //  001B  JMPT	R3	#0025
      0x880C010D,  //  001C  GETMBR	R3	R0	K13
      0x8C0C070E,  //  001D  GETMET	R3	R3	K14
      0x7C0C0200,  //  001E  CALL	R3	1
      0x240C0703,  //  001F  GT	R3	R3	K3
      0x780E0003,  //  0020  JMPF	R3	#0025
      0x500C0200,  //  0021  LDBOOL	R3	1	0
      0x90020803,  //  0022  SETMBR	R0	K4	R3
      0x8C0C010F,  //  0023  GETMET	R3	R0	K15
      0x7C0C0200,  //  0024  CALL	R3	1
      0x80000000,  //  0025  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: received_ack
********************************************************************/
be_local_closure(Matter_Device_received_ack,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(udp_server),
    /* K1   */  be_nested_str_weak(received_ack),
    }),
    be_str_weak(received_ack),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: msg_send
********************************************************************/
be_local_closure(Matter_Device_msg_send,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(udp_server),
    /* K1   */  be_nested_str_weak(send_UDP),
    }),
    be_str_weak(msg_send),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_basic_commissioning
********************************************************************/
be_local_closure(Matter_Device_start_basic_commissioning,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    8,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 4]) {     /* constants */
        /* K0   */  be_nested_str_weak(mdns_announce_PASE),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Wifi_X23Connected),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0xB8020200,  //  0003  GETNGBL	R0	K1
          0x8C000102,  //  0004  GETMET	R0	R0	K2
          0x58080003,  //  0005  LDCONST	R2	K3
          0x580C0000,  //  0006  LDCONST	R3	K0
          0x7C000600,  //  0007  CALL	R0	3
          0x80000000,  //  0008  RET	0
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 4]) {     /* constants */
        /* K0   */  be_nested_str_weak(mdns_announce_PASE),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Eth_X23Connected),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[ 9]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x7C000200,  //  0002  CALL	R0	1
          0xB8020200,  //  0003  GETNGBL	R0	K1
          0x8C000102,  //  0004  GETMET	R0	R0	K2
          0x58080003,  //  0005  LDCONST	R2	K3
          0x580C0000,  //  0006  LDCONST	R3	K0
          0x7C000600,  //  0007  CALL	R0	3
          0x80000000,  //  0008  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(commissioning_open),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(millis),
    /* K3   */  be_nested_str_weak(commissioning_iterations),
    /* K4   */  be_nested_str_weak(commissioning_discriminator),
    /* K5   */  be_nested_str_weak(commissioning_salt),
    /* K6   */  be_nested_str_weak(commissioning_w0),
    /* K7   */  be_nested_str_weak(commissioning_L),
    /* K8   */  be_nested_str_weak(commissioning_admin_fabric),
    /* K9   */  be_nested_str_weak(wifi),
    /* K10  */  be_nested_str_weak(up),
    /* K11  */  be_nested_str_weak(eth),
    /* K12  */  be_nested_str_weak(mdns_announce_PASE),
    /* K13  */  be_nested_str_weak(add_rule),
    /* K14  */  be_nested_str_weak(Wifi_X23Connected),
    /* K15  */  be_nested_str_weak(Eth_X23Connected),
    }),
    be_str_weak(start_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0xB8220200,  //  0000  GETNGBL	R8	K1
      0x8C201102,  //  0001  GETMET	R8	R8	K2
      0x7C200200,  //  0002  CALL	R8	1
      0x542603E7,  //  0003  LDINT	R9	1000
      0x08240209,  //  0004  MUL	R9	R1	R9
      0x00201009,  //  0005  ADD	R8	R8	R9
      0x90020008,  //  0006  SETMBR	R0	K0	R8
      0x90020602,  //  0007  SETMBR	R0	K3	R2
      0x90020803,  //  0008  SETMBR	R0	K4	R3
      0x90020A04,  //  0009  SETMBR	R0	K5	R4
      0x90020C05,  //  000A  SETMBR	R0	K6	R5
      0x90020E06,  //  000B  SETMBR	R0	K7	R6
      0x90021007,  //  000C  SETMBR	R0	K8	R7
      0xB8220200,  //  000D  GETNGBL	R8	K1
      0x8C201109,  //  000E  GETMET	R8	R8	K9
      0x7C200200,  //  000F  CALL	R8	1
      0x9420110A,  //  0010  GETIDX	R8	R8	K10
      0x74220004,  //  0011  JMPT	R8	#0017
      0xB8220200,  //  0012  GETNGBL	R8	K1
      0x8C20110B,  //  0013  GETMET	R8	R8	K11
      0x7C200200,  //  0014  CALL	R8	1
      0x9420110A,  //  0015  GETIDX	R8	R8	K10
      0x78220002,  //  0016  JMPF	R8	#001A
      0x8C20010C,  //  0017  GETMET	R8	R0	K12
      0x7C200200,  //  0018  CALL	R8	1
      0x7002000B,  //  0019  JMP		#0026
      0xB8220200,  //  001A  GETNGBL	R8	K1
      0x8C20110D,  //  001B  GETMET	R8	R8	K13
      0x5828000E,  //  001C  LDCONST	R10	K14
      0x842C0000,  //  001D  CLOSURE	R11	P0
      0x5830000C,  //  001E  LDCONST	R12	K12
      0x7C200800,  //  001F  CALL	R8	4
      0xB8220200,  //  0020  GETNGBL	R8	K1
      0x8C20110D,  //  0021  GETMET	R8	R8	K13
      0x5828000F,  //  0022  LDCONST	R10	K15
      0x842C0001,  //  0023  CLOSURE	R11	P1
      0x5830000C,  //  0024  LDCONST	R12	K12
      0x7C200800,  //  0025  CALL	R8	4
      0xA0000000,  //  0026  CLOSE	R0
      0x80000000,  //  0027  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_plugin_by_endpoint
********************************************************************/
be_local_closure(Matter_Device_find_plugin_by_endpoint,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 4]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(plugins),
    /* K2   */  be_nested_str_weak(get_endpoint),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(find_plugin_by_endpoint),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x58080000,  //  0000  LDCONST	R2	K0
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x88100101,  //  0002  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E0008,  //  0005  JMPF	R3	#000F
      0x880C0101,  //  0006  GETMBR	R3	R0	K1
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x8C100702,  //  0008  GETMET	R4	R3	K2
      0x7C100200,  //  0009  CALL	R4	1
      0x1C100801,  //  000A  EQ	R4	R4	R1
      0x78120000,  //  000B  JMPF	R4	#000D
      0x80040600,  //  000C  RET	1	R3
      0x00080503,  //  000D  ADD	R2	R2	K3
      0x7001FFF1,  //  000E  JMP		#0001
      0x4C0C0000,  //  000F  LDNIL	R3
      0x80040600,  //  0010  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_mdns_announce_hostnames
********************************************************************/
be_local_closure(Matter_Device_start_mdns_announce_hostnames,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 2]) {
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(_mdns_announce_hostname),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Wifi_X23Connected),
        /* K4   */  be_nested_str_weak(matter_mdns_host),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[10]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x50080000,  //  0002  LDBOOL	R2	0	0
          0x7C000400,  //  0003  CALL	R0	2
          0xB8020200,  //  0004  GETNGBL	R0	K1
          0x8C000102,  //  0005  GETMET	R0	R0	K2
          0x58080003,  //  0006  LDCONST	R2	K3
          0x580C0004,  //  0007  LDCONST	R3	K4
          0x7C000600,  //  0008  CALL	R0	3
          0x80000000,  //  0009  RET	0
        })
      ),
      be_nested_proto(
        4,                          /* nstack */
        0,                          /* argc */
        0,                          /* varg */
        1,                          /* has upvals */
        ( &(const bupvaldesc[ 1]) {  /* upvals */
          be_local_const_upval(1, 0),
        }),
        0,                          /* has sup protos */
        NULL,                       /* no sub protos */
        1,                          /* has constants */
        ( &(const bvalue[ 5]) {     /* constants */
        /* K0   */  be_nested_str_weak(_mdns_announce_hostname),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Eth_X23Connected),
        /* K4   */  be_nested_str_weak(matter_mdns_host),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[10]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x50080200,  //  0002  LDBOOL	R2	1	0
          0x7C000400,  //  0003  CALL	R0	2
          0xB8020200,  //  0004  GETNGBL	R0	K1
          0x8C000102,  //  0005  GETMET	R0	R0	K2
          0x58080003,  //  0006  LDCONST	R2	K3
          0x580C0004,  //  0007  LDCONST	R3	K4
          0x7C000600,  //  0008  CALL	R0	3
          0x80000000,  //  0009  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[ 9]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(wifi),
    /* K2   */  be_nested_str_weak(up),
    /* K3   */  be_nested_str_weak(_mdns_announce_hostname),
    /* K4   */  be_nested_str_weak(add_rule),
    /* K5   */  be_nested_str_weak(Wifi_X23Connected),
    /* K6   */  be_nested_str_weak(matter_mdns_host),
    /* K7   */  be_nested_str_weak(eth),
    /* K8   */  be_nested_str_weak(Eth_X23Connected),
    }),
    be_str_weak(start_mdns_announce_hostnames),
    &be_const_str_solidified,
    ( &(const binstruction[32]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x94040302,  //  0003  GETIDX	R1	R1	K2
      0x78060003,  //  0004  JMPF	R1	#0009
      0x8C040103,  //  0005  GETMET	R1	R0	K3
      0x500C0000,  //  0006  LDBOOL	R3	0	0
      0x7C040400,  //  0007  CALL	R1	2
      0x70020005,  //  0008  JMP		#000F
      0xB8060000,  //  0009  GETNGBL	R1	K0
      0x8C040304,  //  000A  GETMET	R1	R1	K4
      0x580C0005,  //  000B  LDCONST	R3	K5
      0x84100000,  //  000C  CLOSURE	R4	P0
      0x58140006,  //  000D  LDCONST	R5	K6
      0x7C040800,  //  000E  CALL	R1	4
      0xB8060000,  //  000F  GETNGBL	R1	K0
      0x8C040307,  //  0010  GETMET	R1	R1	K7
      0x7C040200,  //  0011  CALL	R1	1
      0x94040302,  //  0012  GETIDX	R1	R1	K2
      0x78060003,  //  0013  JMPF	R1	#0018
      0x8C040103,  //  0014  GETMET	R1	R0	K3
      0x500C0200,  //  0015  LDBOOL	R3	1	0
      0x7C040400,  //  0016  CALL	R1	2
      0x70020005,  //  0017  JMP		#001E
      0xB8060000,  //  0018  GETNGBL	R1	K0
      0x8C040304,  //  0019  GETMET	R1	R1	K4
      0x580C0008,  //  001A  LDCONST	R3	K8
      0x84100001,  //  001B  CLOSURE	R4	P1
      0x58140006,  //  001C  LDCONST	R5	K6
      0x7C040800,  //  001D  CALL	R1	4
      0xA0000000,  //  001E  CLOSE	R0
      0x80000000,  //  001F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_announce_PASE
********************************************************************/
be_local_closure(Matter_Device_mdns_announce_PASE,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[43]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(crypto),
    /* K3   */  be_nested_str_weak(VP),
    /* K4   */  be_nested_str_weak(vendorid),
    /* K5   */  be_nested_str_weak(_X2B),
    /* K6   */  be_nested_str_weak(productid),
    /* K7   */  be_nested_str_weak(D),
    /* K8   */  be_nested_str_weak(commissioning_discriminator),
    /* K9   */  be_nested_str_weak(CM),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_str_weak(T),
    /* K12  */  be_const_int(0),
    /* K13  */  be_nested_str_weak(SII),
    /* K14  */  be_nested_str_weak(SAI),
    /* K15  */  be_nested_str_weak(commissioning_instance_wifi),
    /* K16  */  be_nested_str_weak(random),
    /* K17  */  be_nested_str_weak(tohex),
    /* K18  */  be_nested_str_weak(commissioning_instance_eth),
    /* K19  */  be_nested_str_weak(hostname_eth),
    /* K20  */  be_nested_str_weak(add_service),
    /* K21  */  be_nested_str_weak(_matterc),
    /* K22  */  be_nested_str_weak(_udp),
    /* K23  */  be_nested_str_weak(mdns_pase_eth),
    /* K24  */  be_nested_str_weak(tasmota),
    /* K25  */  be_nested_str_weak(log),
    /* K26  */  be_nested_str_weak(format),
    /* K27  */  be_nested_str_weak(MTR_X3A_X20announce_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
    /* K28  */  be_nested_str_weak(eth),
    /* K29  */  be_const_int(2),
    /* K30  */  be_nested_str_weak(_L),
    /* K31  */  be_nested_str_weak(MTR_X3A_X20adding_X20subtype_X3A_X20),
    /* K32  */  be_const_int(3),
    /* K33  */  be_nested_str_weak(add_subtype),
    /* K34  */  be_nested_str_weak(_S),
    /* K35  */  be_nested_str_weak(_V),
    /* K36  */  be_nested_str_weak(_CM1),
    /* K37  */  be_nested_str_weak(hostname_wifi),
    /* K38  */  be_nested_str_weak(mdns_pase_wifi),
    /* K39  */  be_nested_str_weak(MTR_X3A_X20starting_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
    /* K40  */  be_nested_str_weak(wifi),
    /* K41  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K42  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(mdns_announce_PASE),
    &be_const_str_solidified,
    ( &(const binstruction[237]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA40E0400,  //  0002  IMPORT	R3	K2
      0x60100013,  //  0003  GETGBL	R4	G19
      0x7C100000,  //  0004  CALL	R4	0
      0x60140008,  //  0005  GETGBL	R5	G8
      0x88180104,  //  0006  GETMBR	R6	R0	K4
      0x7C140200,  //  0007  CALL	R5	1
      0x00140B05,  //  0008  ADD	R5	R5	K5
      0x60180008,  //  0009  GETGBL	R6	G8
      0x881C0106,  //  000A  GETMBR	R7	R0	K6
      0x7C180200,  //  000B  CALL	R6	1
      0x00140A06,  //  000C  ADD	R5	R5	R6
      0x98120605,  //  000D  SETIDX	R4	K3	R5
      0x88140108,  //  000E  GETMBR	R5	R0	K8
      0x98120E05,  //  000F  SETIDX	R4	K7	R5
      0x9812130A,  //  0010  SETIDX	R4	K9	K10
      0x9812170C,  //  0011  SETIDX	R4	K11	K12
      0x54161387,  //  0012  LDINT	R5	5000
      0x98121A05,  //  0013  SETIDX	R4	K13	R5
      0x5416012B,  //  0014  LDINT	R5	300
      0x98121C05,  //  0015  SETIDX	R4	K14	R5
      0x8C140710,  //  0016  GETMET	R5	R3	K16
      0x541E0007,  //  0017  LDINT	R7	8
      0x7C140400,  //  0018  CALL	R5	2
      0x8C140B11,  //  0019  GETMET	R5	R5	K17
      0x7C140200,  //  001A  CALL	R5	1
      0x90021E05,  //  001B  SETMBR	R0	K15	R5
      0x8C140710,  //  001C  GETMET	R5	R3	K16
      0x541E0007,  //  001D  LDINT	R7	8
      0x7C140400,  //  001E  CALL	R5	2
      0x8C140B11,  //  001F  GETMET	R5	R5	K17
      0x7C140200,  //  0020  CALL	R5	1
      0x90022405,  //  0021  SETMBR	R0	K18	R5
      0xA80200B7,  //  0022  EXBLK	0	#00DB
      0x88140113,  //  0023  GETMBR	R5	R0	K19
      0x78160058,  //  0024  JMPF	R5	#007E
      0x8C140314,  //  0025  GETMET	R5	R1	K20
      0x581C0015,  //  0026  LDCONST	R7	K21
      0x58200016,  //  0027  LDCONST	R8	K22
      0x542615A3,  //  0028  LDINT	R9	5540
      0x5C280800,  //  0029  MOVE	R10	R4
      0x882C0112,  //  002A  GETMBR	R11	R0	K18
      0x88300113,  //  002B  GETMBR	R12	R0	K19
      0x7C140E00,  //  002C  CALL	R5	7
      0x50140200,  //  002D  LDBOOL	R5	1	0
      0x90022E05,  //  002E  SETMBR	R0	K23	R5
      0xB8163000,  //  002F  GETNGBL	R5	K24
      0x8C140B19,  //  0030  GETMET	R5	R5	K25
      0x8C1C051A,  //  0031  GETMET	R7	R2	K26
      0x5824001B,  //  0032  LDCONST	R9	K27
      0x5828001C,  //  0033  LDCONST	R10	K28
      0x882C0112,  //  0034  GETMBR	R11	R0	K18
      0x88300113,  //  0035  GETMBR	R12	R0	K19
      0x7C1C0A00,  //  0036  CALL	R7	5
      0x5820001D,  //  0037  LDCONST	R8	K29
      0x7C140600,  //  0038  CALL	R5	3
      0x60140008,  //  0039  GETGBL	R5	G8
      0x88180108,  //  003A  GETMBR	R6	R0	K8
      0x541E0FFE,  //  003B  LDINT	R7	4095
      0x2C180C07,  //  003C  AND	R6	R6	R7
      0x7C140200,  //  003D  CALL	R5	1
      0x00163C05,  //  003E  ADD	R5	K30	R5
      0xB81A3000,  //  003F  GETNGBL	R6	K24
      0x8C180D19,  //  0040  GETMET	R6	R6	K25
      0x00223E05,  //  0041  ADD	R8	K31	R5
      0x58240020,  //  0042  LDCONST	R9	K32
      0x7C180600,  //  0043  CALL	R6	3
      0x8C180321,  //  0044  GETMET	R6	R1	K33
      0x58200015,  //  0045  LDCONST	R8	K21
      0x58240016,  //  0046  LDCONST	R9	K22
      0x88280112,  //  0047  GETMBR	R10	R0	K18
      0x882C0113,  //  0048  GETMBR	R11	R0	K19
      0x5C300A00,  //  0049  MOVE	R12	R5
      0x7C180C00,  //  004A  CALL	R6	6
      0x60180008,  //  004B  GETGBL	R6	G8
      0x881C0108,  //  004C  GETMBR	R7	R0	K8
      0x54220EFF,  //  004D  LDINT	R8	3840
      0x2C1C0E08,  //  004E  AND	R7	R7	R8
      0x54220007,  //  004F  LDINT	R8	8
      0x3C1C0E08,  //  0050  SHR	R7	R7	R8
      0x7C180200,  //  0051  CALL	R6	1
      0x001A4406,  //  0052  ADD	R6	K34	R6
      0x5C140C00,  //  0053  MOVE	R5	R6
      0xB81A3000,  //  0054  GETNGBL	R6	K24
      0x8C180D19,  //  0055  GETMET	R6	R6	K25
      0x00223E05,  //  0056  ADD	R8	K31	R5
      0x58240020,  //  0057  LDCONST	R9	K32
      0x7C180600,  //  0058  CALL	R6	3
      0x8C180321,  //  0059  GETMET	R6	R1	K33
      0x58200015,  //  005A  LDCONST	R8	K21
      0x58240016,  //  005B  LDCONST	R9	K22
      0x88280112,  //  005C  GETMBR	R10	R0	K18
      0x882C0113,  //  005D  GETMBR	R11	R0	K19
      0x5C300A00,  //  005E  MOVE	R12	R5
      0x7C180C00,  //  005F  CALL	R6	6
      0x60180008,  //  0060  GETGBL	R6	G8
      0x881C0104,  //  0061  GETMBR	R7	R0	K4
      0x7C180200,  //  0062  CALL	R6	1
      0x001A4606,  //  0063  ADD	R6	K35	R6
      0x5C140C00,  //  0064  MOVE	R5	R6
      0xB81A3000,  //  0065  GETNGBL	R6	K24
      0x8C180D19,  //  0066  GETMET	R6	R6	K25
      0x00223E05,  //  0067  ADD	R8	K31	R5
      0x58240020,  //  0068  LDCONST	R9	K32
      0x7C180600,  //  0069  CALL	R6	3
      0x8C180321,  //  006A  GETMET	R6	R1	K33
      0x58200015,  //  006B  LDCONST	R8	K21
      0x58240016,  //  006C  LDCONST	R9	K22
      0x88280112,  //  006D  GETMBR	R10	R0	K18
      0x882C0113,  //  006E  GETMBR	R11	R0	K19
      0x5C300A00,  //  006F  MOVE	R12	R5
      0x7C180C00,  //  0070  CALL	R6	6
      0x58140024,  //  0071  LDCONST	R5	K36
      0xB81A3000,  //  0072  GETNGBL	R6	K24
      0x8C180D19,  //  0073  GETMET	R6	R6	K25
      0x00223E05,  //  0074  ADD	R8	K31	R5
      0x58240020,  //  0075  LDCONST	R9	K32
      0x7C180600,  //  0076  CALL	R6	3
      0x8C180321,  //  0077  GETMET	R6	R1	K33
      0x58200015,  //  0078  LDCONST	R8	K21
      0x58240016,  //  0079  LDCONST	R9	K22
      0x88280112,  //  007A  GETMBR	R10	R0	K18
      0x882C0113,  //  007B  GETMBR	R11	R0	K19
      0x5C300A00,  //  007C  MOVE	R12	R5
      0x7C180C00,  //  007D  CALL	R6	6
      0x88140125,  //  007E  GETMBR	R5	R0	K37
      0x78160058,  //  007F  JMPF	R5	#00D9
      0x8C140314,  //  0080  GETMET	R5	R1	K20
      0x581C0015,  //  0081  LDCONST	R7	K21
      0x58200016,  //  0082  LDCONST	R8	K22
      0x542615A3,  //  0083  LDINT	R9	5540
      0x5C280800,  //  0084  MOVE	R10	R4
      0x882C010F,  //  0085  GETMBR	R11	R0	K15
      0x88300125,  //  0086  GETMBR	R12	R0	K37
      0x7C140E00,  //  0087  CALL	R5	7
      0x50140200,  //  0088  LDBOOL	R5	1	0
      0x90024C05,  //  0089  SETMBR	R0	K38	R5
      0xB8163000,  //  008A  GETNGBL	R5	K24
      0x8C140B19,  //  008B  GETMET	R5	R5	K25
      0x8C1C051A,  //  008C  GETMET	R7	R2	K26
      0x58240027,  //  008D  LDCONST	R9	K39
      0x58280028,  //  008E  LDCONST	R10	K40
      0x882C010F,  //  008F  GETMBR	R11	R0	K15
      0x88300125,  //  0090  GETMBR	R12	R0	K37
      0x7C1C0A00,  //  0091  CALL	R7	5
      0x58200020,  //  0092  LDCONST	R8	K32
      0x7C140600,  //  0093  CALL	R5	3
      0x60140008,  //  0094  GETGBL	R5	G8
      0x88180108,  //  0095  GETMBR	R6	R0	K8
      0x541E0FFE,  //  0096  LDINT	R7	4095
      0x2C180C07,  //  0097  AND	R6	R6	R7
      0x7C140200,  //  0098  CALL	R5	1
      0x00163C05,  //  0099  ADD	R5	K30	R5
      0xB81A3000,  //  009A  GETNGBL	R6	K24
      0x8C180D19,  //  009B  GETMET	R6	R6	K25
      0x00223E05,  //  009C  ADD	R8	K31	R5
      0x58240020,  //  009D  LDCONST	R9	K32
      0x7C180600,  //  009E  CALL	R6	3
      0x8C180321,  //  009F  GETMET	R6	R1	K33
      0x58200015,  //  00A0  LDCONST	R8	K21
      0x58240016,  //  00A1  LDCONST	R9	K22
      0x8828010F,  //  00A2  GETMBR	R10	R0	K15
      0x882C0125,  //  00A3  GETMBR	R11	R0	K37
      0x5C300A00,  //  00A4  MOVE	R12	R5
      0x7C180C00,  //  00A5  CALL	R6	6
      0x60180008,  //  00A6  GETGBL	R6	G8
      0x881C0108,  //  00A7  GETMBR	R7	R0	K8
      0x54220EFF,  //  00A8  LDINT	R8	3840
      0x2C1C0E08,  //  00A9  AND	R7	R7	R8
      0x54220007,  //  00AA  LDINT	R8	8
      0x3C1C0E08,  //  00AB  SHR	R7	R7	R8
      0x7C180200,  //  00AC  CALL	R6	1
      0x001A4406,  //  00AD  ADD	R6	K34	R6
      0x5C140C00,  //  00AE  MOVE	R5	R6
      0xB81A3000,  //  00AF  GETNGBL	R6	K24
      0x8C180D19,  //  00B0  GETMET	R6	R6	K25
      0x00223E05,  //  00B1  ADD	R8	K31	R5
      0x58240020,  //  00B2  LDCONST	R9	K32
      0x7C180600,  //  00B3  CALL	R6	3
      0x8C180321,  //  00B4  GETMET	R6	R1	K33
      0x58200015,  //  00B5  LDCONST	R8	K21
      0x58240016,  //  00B6  LDCONST	R9	K22
      0x8828010F,  //  00B7  GETMBR	R10	R0	K15
      0x882C0125,  //  00B8  GETMBR	R11	R0	K37
      0x5C300A00,  //  00B9  MOVE	R12	R5
      0x7C180C00,  //  00BA  CALL	R6	6
      0x60180008,  //  00BB  GETGBL	R6	G8
      0x881C0104,  //  00BC  GETMBR	R7	R0	K4
      0x7C180200,  //  00BD  CALL	R6	1
      0x001A4606,  //  00BE  ADD	R6	K35	R6
      0x5C140C00,  //  00BF  MOVE	R5	R6
      0xB81A3000,  //  00C0  GETNGBL	R6	K24
      0x8C180D19,  //  00C1  GETMET	R6	R6	K25
      0x00223E05,  //  00C2  ADD	R8	K31	R5
      0x58240020,  //  00C3  LDCONST	R9	K32
      0x7C180600,  //  00C4  CALL	R6	3
      0x8C180321,  //  00C5  GETMET	R6	R1	K33
      0x58200015,  //  00C6  LDCONST	R8	K21
      0x58240016,  //  00C7  LDCONST	R9	K22
      0x8828010F,  //  00C8  GETMBR	R10	R0	K15
      0x882C0125,  //  00C9  GETMBR	R11	R0	K37
      0x5C300A00,  //  00CA  MOVE	R12	R5
      0x7C180C00,  //  00CB  CALL	R6	6
      0x58140024,  //  00CC  LDCONST	R5	K36
      0xB81A3000,  //  00CD  GETNGBL	R6	K24
      0x8C180D19,  //  00CE  GETMET	R6	R6	K25
      0x00223E05,  //  00CF  ADD	R8	K31	R5
      0x58240020,  //  00D0  LDCONST	R9	K32
      0x7C180600,  //  00D1  CALL	R6	3
      0x8C180321,  //  00D2  GETMET	R6	R1	K33
      0x58200015,  //  00D3  LDCONST	R8	K21
      0x58240016,  //  00D4  LDCONST	R9	K22
      0x8828010F,  //  00D5  GETMBR	R10	R0	K15
      0x882C0125,  //  00D6  GETMBR	R11	R0	K37
      0x5C300A00,  //  00D7  MOVE	R12	R5
      0x7C180C00,  //  00D8  CALL	R6	6
      0xA8040001,  //  00D9  EXBLK	1	1
      0x70020010,  //  00DA  JMP		#00EC
      0xAC140002,  //  00DB  CATCH	R5	0	2
      0x7002000D,  //  00DC  JMP		#00EB
      0xB81E3000,  //  00DD  GETNGBL	R7	K24
      0x8C1C0F19,  //  00DE  GETMET	R7	R7	K25
      0x60240008,  //  00DF  GETGBL	R9	G8
      0x5C280A00,  //  00E0  MOVE	R10	R5
      0x7C240200,  //  00E1  CALL	R9	1
      0x00265209,  //  00E2  ADD	R9	K41	R9
      0x0024132A,  //  00E3  ADD	R9	R9	K42
      0x60280008,  //  00E4  GETGBL	R10	G8
      0x5C2C0C00,  //  00E5  MOVE	R11	R6
      0x7C280200,  //  00E6  CALL	R10	1
      0x0024120A,  //  00E7  ADD	R9	R9	R10
      0x5828001D,  //  00E8  LDCONST	R10	K29
      0x7C1C0600,  //  00E9  CALL	R7	3
      0x70020000,  //  00EA  JMP		#00EC
      0xB0080000,  //  00EB  RAISE	2	R0	R0
      0x80000000,  //  00EC  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: generate_random_passcode
********************************************************************/
be_local_closure(Matter_Device_generate_random_passcode,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(random),
    /* K2   */  be_nested_str_weak(get),
    /* K3   */  be_const_int(0),
    /* K4   */  be_const_int(134217727),
    /* K5   */  be_const_int(99999998),
    /* K6   */  be_nested_str_weak(PASSCODE_INVALID),
    /* K7   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(generate_random_passcode),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x4C080000,  //  0001  LDNIL	R2
      0x500C0200,  //  0002  LDBOOL	R3	1	0
      0x780E001D,  //  0003  JMPF	R3	#0022
      0x8C0C0301,  //  0004  GETMET	R3	R1	K1
      0x54160003,  //  0005  LDINT	R5	4
      0x7C0C0400,  //  0006  CALL	R3	2
      0x8C0C0702,  //  0007  GETMET	R3	R3	K2
      0x58140003,  //  0008  LDCONST	R5	K3
      0x541A0003,  //  0009  LDINT	R6	4
      0x7C0C0600,  //  000A  CALL	R3	3
      0x2C0C0704,  //  000B  AND	R3	R3	K4
      0x5C080600,  //  000C  MOVE	R2	R3
      0x240C0505,  //  000D  GT	R3	R2	K5
      0x780E0000,  //  000E  JMPF	R3	#0010
      0x7001FFF1,  //  000F  JMP		#0002
      0x600C0010,  //  0010  GETGBL	R3	G16
      0x88100106,  //  0011  GETMBR	R4	R0	K6
      0x7C0C0200,  //  0012  CALL	R3	1
      0xA8020005,  //  0013  EXBLK	0	#001A
      0x5C100600,  //  0014  MOVE	R4	R3
      0x7C100000,  //  0015  CALL	R4	0
      0x1C140404,  //  0016  EQ	R5	R2	R4
      0x78160000,  //  0017  JMPF	R5	#0019
      0x4C080000,  //  0018  LDNIL	R2
      0x7001FFF9,  //  0019  JMP		#0014
      0x580C0007,  //  001A  LDCONST	R3	K7
      0xAC0C0200,  //  001B  CATCH	R3	1	0
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0x4C0C0000,  //  001D  LDNIL	R3
      0x200C0403,  //  001E  NE	R3	R2	R3
      0x780E0000,  //  001F  JMPF	R3	#0021
      0x80040400,  //  0020  RET	1	R2
      0x7001FFDF,  //  0021  JMP		#0002
      0x80000000,  //  0022  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_native_classes
********************************************************************/
be_local_closure(Matter_Device_register_native_classes,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[17]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(members),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(get),
    /* K5   */  be_nested_str_weak(class),
    /* K6   */  be_nested_str_weak(find),
    /* K7   */  be_nested_str_weak(Plugin_),
    /* K8   */  be_const_int(0),
    /* K9   */  be_nested_str_weak(register_plugin_class),
    /* K10  */  be_nested_str_weak(stop_iteration),
    /* K11  */  be_nested_str_weak(tasmota),
    /* K12  */  be_nested_str_weak(log),
    /* K13  */  be_nested_str_weak(MTR_X3A_X20registered_X20classes_X20),
    /* K14  */  be_nested_str_weak(k2l),
    /* K15  */  be_nested_str_weak(plugins_classes),
    /* K16  */  be_const_int(3),
    }),
    be_str_weak(register_native_classes),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0xA40E0000,  //  0000  IMPORT	R3	K0
      0xA4120200,  //  0001  IMPORT	R4	K1
      0x60140010,  //  0002  GETGBL	R5	G16
      0x8C180702,  //  0003  GETMET	R6	R3	K2
      0xB8220600,  //  0004  GETNGBL	R8	K3
      0x7C180400,  //  0005  CALL	R6	2
      0x7C140200,  //  0006  CALL	R5	1
      0xA8020014,  //  0007  EXBLK	0	#001D
      0x5C180A00,  //  0008  MOVE	R6	R5
      0x7C180000,  //  0009  CALL	R6	0
      0x8C1C0704,  //  000A  GETMET	R7	R3	K4
      0xB8260600,  //  000B  GETNGBL	R9	K3
      0x5C280C00,  //  000C  MOVE	R10	R6
      0x7C1C0600,  //  000D  CALL	R7	3
      0x60200004,  //  000E  GETGBL	R8	G4
      0x5C240E00,  //  000F  MOVE	R9	R7
      0x7C200200,  //  0010  CALL	R8	1
      0x1C201105,  //  0011  EQ	R8	R8	K5
      0x78220008,  //  0012  JMPF	R8	#001C
      0x8C200906,  //  0013  GETMET	R8	R4	K6
      0x5C280C00,  //  0014  MOVE	R10	R6
      0x582C0007,  //  0015  LDCONST	R11	K7
      0x7C200600,  //  0016  CALL	R8	3
      0x1C201108,  //  0017  EQ	R8	R8	K8
      0x78220002,  //  0018  JMPF	R8	#001C
      0x8C200109,  //  0019  GETMET	R8	R0	K9
      0x5C280E00,  //  001A  MOVE	R10	R7
      0x7C200400,  //  001B  CALL	R8	2
      0x7001FFEA,  //  001C  JMP		#0008
      0x5814000A,  //  001D  LDCONST	R5	K10
      0xAC140200,  //  001E  CATCH	R5	1	0
      0xB0080000,  //  001F  RAISE	2	R0	R0
      0xB8161600,  //  0020  GETNGBL	R5	K11
      0x8C140B0C,  //  0021  GETMET	R5	R5	K12
      0x601C0008,  //  0022  GETGBL	R7	G8
      0x8C20010E,  //  0023  GETMET	R8	R0	K14
      0x8828010F,  //  0024  GETMBR	R10	R0	K15
      0x7C200400,  //  0025  CALL	R8	2
      0x7C1C0200,  //  0026  CALL	R7	1
      0x001E1A07,  //  0027  ADD	R7	K13	R7
      0x58200010,  //  0028  LDCONST	R8	K16
      0x7C140600,  //  0029  CALL	R5	3
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sort_distinct
********************************************************************/
be_local_closure(Matter_Device_sort_distinct,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    4,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_const_class(be_class_Matter_Device),
    /* K1   */  be_const_int(1),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(stop_iteration),
    /* K4   */  be_nested_str_weak(remove),
    }),
    be_str_weak(sort_distinct),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x60080010,  //  0001  GETGBL	R2	G16
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C0C0200,  //  0004  CALL	R3	1
      0x040C0701,  //  0005  SUB	R3	R3	K1
      0x400E0203,  //  0006  CONNECT	R3	K1	R3
      0x7C080200,  //  0007  CALL	R2	1
      0xA8020010,  //  0008  EXBLK	0	#001A
      0x5C0C0400,  //  0009  MOVE	R3	R2
      0x7C0C0000,  //  000A  CALL	R3	0
      0x94100003,  //  000B  GETIDX	R4	R0	R3
      0x5C140600,  //  000C  MOVE	R5	R3
      0x24180B02,  //  000D  GT	R6	R5	K2
      0x781A0008,  //  000E  JMPF	R6	#0018
      0x04180B01,  //  000F  SUB	R6	R5	K1
      0x94180006,  //  0010  GETIDX	R6	R0	R6
      0x24180C04,  //  0011  GT	R6	R6	R4
      0x781A0004,  //  0012  JMPF	R6	#0018
      0x04180B01,  //  0013  SUB	R6	R5	K1
      0x94180006,  //  0014  GETIDX	R6	R0	R6
      0x98000A06,  //  0015  SETIDX	R0	R5	R6
      0x04140B01,  //  0016  SUB	R5	R5	K1
      0x7001FFF4,  //  0017  JMP		#000D
      0x98000A04,  //  0018  SETIDX	R0	R5	R4
      0x7001FFEE,  //  0019  JMP		#0009
      0x58080003,  //  001A  LDCONST	R2	K3
      0xAC080200,  //  001B  CATCH	R2	1	0
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0x58080001,  //  001D  LDCONST	R2	K1
      0x600C000C,  //  001E  GETGBL	R3	G12
      0x5C100000,  //  001F  MOVE	R4	R0
      0x7C0C0200,  //  0020  CALL	R3	1
      0x180C0701,  //  0021  LE	R3	R3	K1
      0x780E0000,  //  0022  JMPF	R3	#0024
      0x80040000,  //  0023  RET	1	R0
      0x940C0102,  //  0024  GETIDX	R3	R0	K2
      0x6010000C,  //  0025  GETGBL	R4	G12
      0x5C140000,  //  0026  MOVE	R5	R0
      0x7C100200,  //  0027  CALL	R4	1
      0x14100404,  //  0028  LT	R4	R2	R4
      0x78120009,  //  0029  JMPF	R4	#0034
      0x94100002,  //  002A  GETIDX	R4	R0	R2
      0x1C100803,  //  002B  EQ	R4	R4	R3
      0x78120003,  //  002C  JMPF	R4	#0031
      0x8C100104,  //  002D  GETMET	R4	R0	K4
      0x5C180400,  //  002E  MOVE	R6	R2
      0x7C100400,  //  002F  CALL	R4	2
      0x70020001,  //  0030  JMP		#0033
      0x940C0002,  //  0031  GETIDX	R3	R0	R2
      0x00080501,  //  0032  ADD	R2	R2	K1
      0x7001FFF0,  //  0033  JMP		#0025
      0x80040000,  //  0034  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: signal_endpoints_changed
********************************************************************/
be_local_closure(Matter_Device_signal_endpoints_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(attribute_updated),
    /* K1   */  be_const_int(0),
    /* K2   */  be_const_int(3),
    }),
    be_str_weak(signal_endpoints_changed),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x580C0001,  //  0001  LDCONST	R3	K1
      0x5412001C,  //  0002  LDINT	R4	29
      0x58140002,  //  0003  LDCONST	R5	K2
      0x50180000,  //  0004  LDBOOL	R6	0	0
      0x7C040A00,  //  0005  CALL	R1	5
      0x8C040100,  //  0006  GETMET	R1	R0	K0
      0x540EFEFF,  //  0007  LDINT	R3	65280
      0x5412001C,  //  0008  LDINT	R4	29
      0x58140002,  //  0009  LDCONST	R5	K2
      0x50180000,  //  000A  LDBOOL	R6	0	0
      0x7C040A00,  //  000B  CALL	R1	5
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Device
********************************************************************/
be_local_class(Matter_Device,
    34,
    NULL,
    be_nested_map(104,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(hostname_eth, -1), be_const_var(20) },
        { be_const_key_weak(save_before_restart, -1), be_const_closure(Matter_Device_save_before_restart_closure) },
        { be_const_key_weak(is_commissioning_open, 65), be_const_closure(Matter_Device_is_commissioning_open_closure) },
        { be_const_key_weak(plugins, 94), be_const_var(1) },
        { be_const_key_weak(plugins_config, 1), be_const_var(4) },
        { be_const_key_weak(save_param, 4), be_const_closure(Matter_Device_save_param_closure) },
        { be_const_key_weak(root_L, 24), be_const_var(33) },
        { be_const_key_weak(mdns_remove_op_discovery, 86), be_const_closure(Matter_Device_mdns_remove_op_discovery_closure) },
        { be_const_key_weak(plugins_classes, 36), be_const_var(3) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_Device_every_second_closure) },
        { be_const_key_weak(root_salt, -1), be_const_var(31) },
        { be_const_key_weak(mdns_announce_op_discovery_all_fabrics, -1), be_const_closure(Matter_Device_mdns_announce_op_discovery_all_fabrics_closure) },
        { be_const_key_weak(sessions, -1), be_const_var(7) },
        { be_const_key_weak(autoconf_sensors_list, 29), be_const_closure(Matter_Device_autoconf_sensors_list_closure) },
        { be_const_key_weak(mdns_remove_PASE, -1), be_const_closure(Matter_Device_mdns_remove_PASE_closure) },
        { be_const_key_weak(process_attribute_expansion, 60), be_const_closure(Matter_Device_process_attribute_expansion_closure) },
        { be_const_key_weak(mdns_pase_eth, -1), be_const_var(23) },
        { be_const_key_weak(_compute_pbkdf, 76), be_const_closure(Matter_Device__compute_pbkdf_closure) },
        { be_const_key_weak(commissioning_w0, -1), be_const_var(14) },
        { be_const_key_weak(bridge_remove_endpoint, 48), be_const_closure(Matter_Device_bridge_remove_endpoint_closure) },
        { be_const_key_weak(stop_basic_commissioning, -1), be_const_closure(Matter_Device_stop_basic_commissioning_closure) },
        { be_const_key_weak(http_remotes, -1), be_const_var(25) },
        { be_const_key_weak(vendorid, 56), be_const_var(21) },
        { be_const_key_weak(remove_fabric, -1), be_const_closure(Matter_Device_remove_fabric_closure) },
        { be_const_key_weak(_start_udp, -1), be_const_closure(Matter_Device__start_udp_closure) },
        { be_const_key_weak(productid, -1), be_const_var(22) },
        { be_const_key_weak(_mdns_announce_hostname, 5), be_const_closure(Matter_Device__mdns_announce_hostname_closure) },
        { be_const_key_weak(get_plugin_class_arg, -1), be_const_closure(Matter_Device_get_plugin_class_arg_closure) },
        { be_const_key_weak(plugins_persist, -1), be_const_var(2) },
        { be_const_key_weak(generate_random_passcode, -1), be_const_closure(Matter_Device_generate_random_passcode_closure) },
        { be_const_key_weak(event_fabrics_saved, -1), be_const_closure(Matter_Device_event_fabrics_saved_closure) },
        { be_const_key_weak(start_commissioning_complete, 82), be_const_closure(Matter_Device_start_commissioning_complete_closure) },
        { be_const_key_weak(msg_received, 54), be_const_closure(Matter_Device_msg_received_closure) },
        { be_const_key_weak(_instantiate_plugins_from_config, -1), be_const_closure(Matter_Device__instantiate_plugins_from_config_closure) },
        { be_const_key_weak(commissioning_instance_wifi, 74), be_const_var(17) },
        { be_const_key_weak(get_active_endpoints, 69), be_const_closure(Matter_Device_get_active_endpoints_closure) },
        { be_const_key_weak(commissioning_admin_fabric, -1), be_const_var(16) },
        { be_const_key_weak(mdns_announce_PASE, -1), be_const_closure(Matter_Device_mdns_announce_PASE_closure) },
        { be_const_key_weak(root_passcode, 88), be_const_var(27) },
        { be_const_key_weak(message_handler, -1), be_const_var(6) },
        { be_const_key_weak(FILENAME, 95), be_nested_str_weak(_matter_device_X2Ejson) },
        { be_const_key_weak(VENDOR_ID, 42), be_const_int(65521) },
        { be_const_key_weak(root_w0, 96), be_const_var(32) },
        { be_const_key_weak(MtrJoin, -1), be_const_closure(Matter_Device_MtrJoin_closure) },
        { be_const_key_weak(attribute_updated, -1), be_const_closure(Matter_Device_attribute_updated_closure) },
        { be_const_key_weak(PASSCODE_INVALID, 62), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(12,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(0),
        be_const_int(11111111),
        be_const_int(22222222),
        be_const_int(33333333),
        be_const_int(44444444),
        be_const_int(55555555),
        be_const_int(66666666),
        be_const_int(77777777),
        be_const_int(88888888),
        be_const_int(99999999),
        be_const_int(12345678),
        be_const_int(87654321),
    }))    ) } )) },
        { be_const_key_weak(started, 55), be_const_var(0) },
        { be_const_key_weak(start, -1), be_const_closure(Matter_Device_start_closure) },
        { be_const_key_weak(init, -1), be_const_closure(Matter_Device_init_closure) },
        { be_const_key_weak(commissioning_salt, -1), be_const_var(13) },
        { be_const_key_weak(commissioning_instance_eth, 75), be_const_var(18) },
        { be_const_key_weak(k2l, -1), be_const_static_closure(Matter_Device_k2l_closure) },
        { be_const_key_weak(hostname_wifi, 51), be_const_var(19) },
        { be_const_key_weak(_init_basic_commissioning, -1), be_const_closure(Matter_Device__init_basic_commissioning_closure) },
        { be_const_key_weak(received_ack, -1), be_const_closure(Matter_Device_received_ack_closure) },
        { be_const_key_weak(clean_remotes, 43), be_const_closure(Matter_Device_clean_remotes_closure) },
        { be_const_key_weak(autoconf_device_map, -1), be_const_closure(Matter_Device_autoconf_device_map_closure) },
        { be_const_key_weak(commissioning_open, -1), be_const_var(10) },
        { be_const_key_weak(get_plugin_class_displayname, -1), be_const_closure(Matter_Device_get_plugin_class_displayname_closure) },
        { be_const_key_weak(commissioning_discriminator, 37), be_const_var(12) },
        { be_const_key_weak(is_root_commissioning_open, -1), be_const_closure(Matter_Device_is_root_commissioning_open_closure) },
        { be_const_key_weak(mdns_pase_wifi, -1), be_const_var(24) },
        { be_const_key_weak(stop, -1), be_const_closure(Matter_Device_stop_closure) },
        { be_const_key_weak(mdns_remove_op_discovery_all_fabrics, -1), be_const_closure(Matter_Device_mdns_remove_op_discovery_all_fabrics_closure) },
        { be_const_key_weak(every_250ms, 85), be_const_closure(Matter_Device_every_250ms_closure) },
        { be_const_key_weak(compute_manual_pairing_code, -1), be_const_closure(Matter_Device_compute_manual_pairing_code_closure) },
        { be_const_key_weak(adjust_next_ep, 63), be_const_closure(Matter_Device_adjust_next_ep_closure) },
        { be_const_key_weak(register_commands, -1), be_const_closure(Matter_Device_register_commands_closure) },
        { be_const_key_weak(register_http_remote, -1), be_const_closure(Matter_Device_register_http_remote_closure) },
        { be_const_key_weak(load_param, -1), be_const_closure(Matter_Device_load_param_closure) },
        { be_const_key_weak(root_discriminator, -1), be_const_var(26) },
        { be_const_key_weak(compute_qrcode_content, -1), be_const_closure(Matter_Device_compute_qrcode_content_closure) },
        { be_const_key_weak(conf_to_log, 81), be_const_static_closure(Matter_Device_conf_to_log_closure) },
        { be_const_key_weak(start_operational_discovery, -1), be_const_closure(Matter_Device_start_operational_discovery_closure) },
        { be_const_key_weak(UDP_PORT, 14), be_const_int(5540) },
        { be_const_key_weak(root_iterations, -1), be_const_var(30) },
        { be_const_key_weak(register_plugin_class, -1), be_const_closure(Matter_Device_register_plugin_class_closure) },
        { be_const_key_weak(commissioning_iterations, -1), be_const_var(11) },
        { be_const_key_weak(udp_server, -1), be_const_var(5) },
        { be_const_key_weak(_trigger_read_sensors, -1), be_const_closure(Matter_Device__trigger_read_sensors_closure) },
        { be_const_key_weak(bridge_add_endpoint, -1), be_const_closure(Matter_Device_bridge_add_endpoint_closure) },
        { be_const_key_weak(ipv4only, -1), be_const_var(28) },
        { be_const_key_weak(PBKDF_ITERATIONS, 44), be_const_int(1000) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Device_invoke_request_closure) },
        { be_const_key_weak(PRODUCT_ID, -1), be_const_int(32768) },
        { be_const_key_weak(every_50ms, -1), be_const_closure(Matter_Device_every_50ms_closure) },
        { be_const_key_weak(k2l_num, -1), be_const_static_closure(Matter_Device_k2l_num_closure) },
        { be_const_key_weak(autoconf_device, -1), be_const_closure(Matter_Device_autoconf_device_closure) },
        { be_const_key_weak(mdns_announce_op_discovery, -1), be_const_closure(Matter_Device_mdns_announce_op_discovery_closure) },
        { be_const_key_weak(msg_send, -1), be_const_closure(Matter_Device_msg_send_closure) },
        { be_const_key_weak(start_basic_commissioning, -1), be_const_closure(Matter_Device_start_basic_commissioning_closure) },
        { be_const_key_weak(commissioning_L, -1), be_const_var(15) },
        { be_const_key_weak(find_plugin_by_endpoint, -1), be_const_closure(Matter_Device_find_plugin_by_endpoint_closure) },
        { be_const_key_weak(start_mdns_announce_hostnames, -1), be_const_closure(Matter_Device_start_mdns_announce_hostnames_closure) },
        { be_const_key_weak(start_commissioning_complete_deferred, -1), be_const_closure(Matter_Device_start_commissioning_complete_deferred_closure) },
        { be_const_key_weak(next_ep, -1), be_const_var(29) },
        { be_const_key_weak(start_root_basic_commissioning, -1), be_const_closure(Matter_Device_start_root_basic_commissioning_closure) },
        { be_const_key_weak(start_operational_discovery_deferred, 25), be_const_closure(Matter_Device_start_operational_discovery_deferred_closure) },
        { be_const_key_weak(PASE_TIMEOUT, -1), be_const_int(600) },
        { be_const_key_weak(ui, 16), be_const_var(8) },
        { be_const_key_weak(tick, -1), be_const_var(9) },
        { be_const_key_weak(register_native_classes, -1), be_const_closure(Matter_Device_register_native_classes_closure) },
        { be_const_key_weak(sort_distinct, -1), be_const_static_closure(Matter_Device_sort_distinct_closure) },
        { be_const_key_weak(signal_endpoints_changed, -1), be_const_closure(Matter_Device_signal_endpoints_changed_closure) },
    })),
    be_str_weak(Matter_Device)
);
/*******************************************************************/

void be_load_Matter_Device_class(bvm *vm) {
    be_pushntvclass(vm, &be_class_Matter_Device);
    be_setglobal(vm, "Matter_Device");
    be_pop(vm, 1);
}
/********************************************************************/
/* End of solidification */
