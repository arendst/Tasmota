/* Solidification of Matter_Device.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Device;

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
    /* K2   */  be_nested_str_weak(DISPLAY_NAME),
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
** Solidified function: mdns_remove_PASE
********************************************************************/
be_local_closure(Matter_Device_mdns_remove_PASE,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[20]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(mdns_pase_eth),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(log),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20calling_X20mdns_X2Eremove_service_X28_X25s_X2C_X20_X25s_X2C_X20_X25s_X2C_X20_X25s_X29),
    /* K5   */  be_nested_str_weak(_matterc),
    /* K6   */  be_nested_str_weak(_udp),
    /* K7   */  be_nested_str_weak(commissioning_instance_eth),
    /* K8   */  be_nested_str_weak(hostname_eth),
    /* K9   */  be_const_int(3),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20remove_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27),
    /* K11  */  be_nested_str_weak(eth),
    /* K12  */  be_nested_str_weak(remove_service),
    /* K13  */  be_nested_str_weak(mdns_pase_wifi),
    /* K14  */  be_nested_str_weak(commissioning_instance_wifi),
    /* K15  */  be_nested_str_weak(hostname_wifi),
    /* K16  */  be_nested_str_weak(wifi),
    /* K17  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K18  */  be_nested_str_weak(_X7C),
    /* K19  */  be_const_int(2),
    }),
    be_str_weak(mdns_remove_PASE),
    &be_const_str_solidified,
    ( &(const binstruction[82]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA802003D,  //  0001  EXBLK	0	#0040
      0x88080101,  //  0002  GETMBR	R2	R0	K1
      0x780A001B,  //  0003  JMPF	R2	#0020
      0xB80A0400,  //  0004  GETNGBL	R2	K2
      0x8C080503,  //  0005  GETMET	R2	R2	K3
      0x60100018,  //  0006  GETGBL	R4	G24
      0x58140004,  //  0007  LDCONST	R5	K4
      0x58180005,  //  0008  LDCONST	R6	K5
      0x581C0006,  //  0009  LDCONST	R7	K6
      0x88200107,  //  000A  GETMBR	R8	R0	K7
      0x88240108,  //  000B  GETMBR	R9	R0	K8
      0x7C100A00,  //  000C  CALL	R4	5
      0x58140009,  //  000D  LDCONST	R5	K9
      0x7C080600,  //  000E  CALL	R2	3
      0xB80A0400,  //  000F  GETNGBL	R2	K2
      0x8C080503,  //  0010  GETMET	R2	R2	K3
      0x60100018,  //  0011  GETGBL	R4	G24
      0x5814000A,  //  0012  LDCONST	R5	K10
      0x5818000B,  //  0013  LDCONST	R6	K11
      0x881C0107,  //  0014  GETMBR	R7	R0	K7
      0x7C100600,  //  0015  CALL	R4	3
      0x58140009,  //  0016  LDCONST	R5	K9
      0x7C080600,  //  0017  CALL	R2	3
      0x50080000,  //  0018  LDBOOL	R2	0	0
      0x90020202,  //  0019  SETMBR	R0	K1	R2
      0x8C08030C,  //  001A  GETMET	R2	R1	K12
      0x58100005,  //  001B  LDCONST	R4	K5
      0x58140006,  //  001C  LDCONST	R5	K6
      0x88180107,  //  001D  GETMBR	R6	R0	K7
      0x881C0108,  //  001E  GETMBR	R7	R0	K8
      0x7C080A00,  //  001F  CALL	R2	5
      0x8808010D,  //  0020  GETMBR	R2	R0	K13
      0x780A001B,  //  0021  JMPF	R2	#003E
      0xB80A0400,  //  0022  GETNGBL	R2	K2
      0x8C080503,  //  0023  GETMET	R2	R2	K3
      0x60100018,  //  0024  GETGBL	R4	G24
      0x58140004,  //  0025  LDCONST	R5	K4
      0x58180005,  //  0026  LDCONST	R6	K5
      0x581C0006,  //  0027  LDCONST	R7	K6
      0x8820010E,  //  0028  GETMBR	R8	R0	K14
      0x8824010F,  //  0029  GETMBR	R9	R0	K15
      0x7C100A00,  //  002A  CALL	R4	5
      0x58140009,  //  002B  LDCONST	R5	K9
      0x7C080600,  //  002C  CALL	R2	3
      0xB80A0400,  //  002D  GETNGBL	R2	K2
      0x8C080503,  //  002E  GETMET	R2	R2	K3
      0x60100018,  //  002F  GETGBL	R4	G24
      0x5814000A,  //  0030  LDCONST	R5	K10
      0x58180010,  //  0031  LDCONST	R6	K16
      0x881C010E,  //  0032  GETMBR	R7	R0	K14
      0x7C100600,  //  0033  CALL	R4	3
      0x58140009,  //  0034  LDCONST	R5	K9
      0x7C080600,  //  0035  CALL	R2	3
      0x50080000,  //  0036  LDBOOL	R2	0	0
      0x90021A02,  //  0037  SETMBR	R0	K13	R2
      0x8C08030C,  //  0038  GETMET	R2	R1	K12
      0x58100005,  //  0039  LDCONST	R4	K5
      0x58140006,  //  003A  LDCONST	R5	K6
      0x8818010E,  //  003B  GETMBR	R6	R0	K14
      0x881C010F,  //  003C  GETMBR	R7	R0	K15
      0x7C080A00,  //  003D  CALL	R2	5
      0xA8040001,  //  003E  EXBLK	1	1
      0x70020010,  //  003F  JMP		#0051
      0xAC080002,  //  0040  CATCH	R2	0	2
      0x7002000D,  //  0041  JMP		#0050
      0xB8120400,  //  0042  GETNGBL	R4	K2
      0x8C100903,  //  0043  GETMET	R4	R4	K3
      0x60180008,  //  0044  GETGBL	R6	G8
      0x5C1C0400,  //  0045  MOVE	R7	R2
      0x7C180200,  //  0046  CALL	R6	1
      0x001A2206,  //  0047  ADD	R6	K17	R6
      0x00180D12,  //  0048  ADD	R6	R6	K18
      0x601C0008,  //  0049  GETGBL	R7	G8
      0x5C200600,  //  004A  MOVE	R8	R3
      0x7C1C0200,  //  004B  CALL	R7	1
      0x00180C07,  //  004C  ADD	R6	R6	R7
      0x581C0013,  //  004D  LDCONST	R7	K19
      0x7C100600,  //  004E  CALL	R4	3
      0x70020000,  //  004F  JMP		#0051
      0xB0080000,  //  0050  RAISE	2	R0	R0
      0x80000000,  //  0051  RET	0
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
** Solidified function: compute_manual_pairing_code
********************************************************************/
be_local_closure(Matter_Device_compute_manual_pairing_code,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(root_discriminator),
    /* K1   */  be_nested_str_weak(root_passcode),
    /* K2   */  be_nested_str_weak(_X251i_X2505i_X2504i),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(Verhoeff),
    /* K5   */  be_nested_str_weak(checksum),
    }),
    be_str_weak(compute_manual_pairing_code),
    &be_const_str_solidified,
    ( &(const binstruction[30]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x540A0FFE,  //  0001  LDINT	R2	4095
      0x2C040202,  //  0002  AND	R1	R1	R2
      0x540A0009,  //  0003  LDINT	R2	10
      0x3C040202,  //  0004  SHR	R1	R1	R2
      0x88080100,  //  0005  GETMBR	R2	R0	K0
      0x540E02FF,  //  0006  LDINT	R3	768
      0x2C080403,  //  0007  AND	R2	R2	R3
      0x540E0005,  //  0008  LDINT	R3	6
      0x38080403,  //  0009  SHL	R2	R2	R3
      0x880C0101,  //  000A  GETMBR	R3	R0	K1
      0x54123FFE,  //  000B  LDINT	R4	16383
      0x2C0C0604,  //  000C  AND	R3	R3	R4
      0x30080403,  //  000D  OR	R2	R2	R3
      0x880C0101,  //  000E  GETMBR	R3	R0	K1
      0x5412000D,  //  000F  LDINT	R4	14
      0x3C0C0604,  //  0010  SHR	R3	R3	R4
      0x60100018,  //  0011  GETGBL	R4	G24
      0x58140002,  //  0012  LDCONST	R5	K2
      0x5C180200,  //  0013  MOVE	R6	R1
      0x5C1C0400,  //  0014  MOVE	R7	R2
      0x5C200600,  //  0015  MOVE	R8	R3
      0x7C100800,  //  0016  CALL	R4	4
      0xB8160600,  //  0017  GETNGBL	R5	K3
      0x88140B04,  //  0018  GETMBR	R5	R5	K4
      0x8C140B05,  //  0019  GETMET	R5	R5	K5
      0x5C1C0800,  //  001A  MOVE	R7	R4
      0x7C140400,  //  001B  CALL	R5	2
      0x00100805,  //  001C  ADD	R4	R4	R5
      0x80040800,  //  001D  RET	1	R4
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
** Solidified function: bridge_add_endpoint
********************************************************************/
be_local_closure(Matter_Device_bridge_add_endpoint,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[21]) {     /* constants */
    /* K0   */  be_nested_str_weak(plugins_classes),
    /* K1   */  be_nested_str_weak(find),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(log),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20unknown_X20class_X20name_X20_X27),
    /* K5   */  be_nested_str_weak(_X27_X20skipping),
    /* K6   */  be_const_int(3),
    /* K7   */  be_nested_str_weak(next_ep),
    /* K8   */  be_nested_str_weak(plugins),
    /* K9   */  be_nested_str_weak(push),
    /* K10  */  be_nested_str_weak(type),
    /* K11  */  be_nested_str_weak(keys),
    /* K12  */  be_nested_str_weak(stop_iteration),
    /* K13  */  be_nested_str_weak(MTR_X3A_X20adding_X20endpoint_X20_X3D_X20_X25i_X20type_X3A_X25s_X25s),
    /* K14  */  be_nested_str_weak(conf_to_log),
    /* K15  */  be_const_int(2),
    /* K16  */  be_nested_str_weak(plugins_config),
    /* K17  */  be_nested_str_weak(plugins_persist),
    /* K18  */  be_const_int(1),
    /* K19  */  be_nested_str_weak(save_param),
    /* K20  */  be_nested_str_weak(signal_endpoints_changed),
    }),
    be_str_weak(bridge_add_endpoint),
    &be_const_str_solidified,
    ( &(const binstruction[70]) {  /* code */
      0x880C0100,  //  0000  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0001  GETMET	R3	R3	K1
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x4C100000,  //  0004  LDNIL	R4
      0x1C100604,  //  0005  EQ	R4	R3	R4
      0x78120009,  //  0006  JMPF	R4	#0011
      0xB8120400,  //  0007  GETNGBL	R4	K2
      0x8C100903,  //  0008  GETMET	R4	R4	K3
      0x60180008,  //  0009  GETGBL	R6	G8
      0x5C1C0200,  //  000A  MOVE	R7	R1
      0x7C180200,  //  000B  CALL	R6	1
      0x001A0806,  //  000C  ADD	R6	K4	R6
      0x00180D05,  //  000D  ADD	R6	R6	K5
      0x581C0006,  //  000E  LDCONST	R7	K6
      0x7C100600,  //  000F  CALL	R4	3
      0x80000800,  //  0010  RET	0
      0x88100107,  //  0011  GETMBR	R4	R0	K7
      0x60140008,  //  0012  GETGBL	R5	G8
      0x5C180800,  //  0013  MOVE	R6	R4
      0x7C140200,  //  0014  CALL	R5	1
      0x5C180600,  //  0015  MOVE	R6	R3
      0x5C1C0000,  //  0016  MOVE	R7	R0
      0x5C200800,  //  0017  MOVE	R8	R4
      0x5C240400,  //  0018  MOVE	R9	R2
      0x7C180600,  //  0019  CALL	R6	3
      0x881C0108,  //  001A  GETMBR	R7	R0	K8
      0x8C1C0F09,  //  001B  GETMET	R7	R7	K9
      0x5C240C00,  //  001C  MOVE	R9	R6
      0x7C1C0400,  //  001D  CALL	R7	2
      0x601C0013,  //  001E  GETGBL	R7	G19
      0x7C1C0000,  //  001F  CALL	R7	0
      0x981E1401,  //  0020  SETIDX	R7	K10	R1
      0x60200010,  //  0021  GETGBL	R8	G16
      0x8C24050B,  //  0022  GETMET	R9	R2	K11
      0x7C240200,  //  0023  CALL	R9	1
      0x7C200200,  //  0024  CALL	R8	1
      0xA8020004,  //  0025  EXBLK	0	#002B
      0x5C241000,  //  0026  MOVE	R9	R8
      0x7C240000,  //  0027  CALL	R9	0
      0x94280409,  //  0028  GETIDX	R10	R2	R9
      0x981C120A,  //  0029  SETIDX	R7	R9	R10
      0x7001FFFA,  //  002A  JMP		#0026
      0x5820000C,  //  002B  LDCONST	R8	K12
      0xAC200200,  //  002C  CATCH	R8	1	0
      0xB0080000,  //  002D  RAISE	2	R0	R0
      0xB8220400,  //  002E  GETNGBL	R8	K2
      0x8C201103,  //  002F  GETMET	R8	R8	K3
      0x60280018,  //  0030  GETGBL	R10	G24
      0x582C000D,  //  0031  LDCONST	R11	K13
      0x5C300800,  //  0032  MOVE	R12	R4
      0x5C340200,  //  0033  MOVE	R13	R1
      0x8C38010E,  //  0034  GETMET	R14	R0	K14
      0x5C400400,  //  0035  MOVE	R16	R2
      0x7C380400,  //  0036  CALL	R14	2
      0x7C280800,  //  0037  CALL	R10	4
      0x582C000F,  //  0038  LDCONST	R11	K15
      0x7C200600,  //  0039  CALL	R8	3
      0x88200110,  //  003A  GETMBR	R8	R0	K16
      0x98200A07,  //  003B  SETIDX	R8	R5	R7
      0x50200200,  //  003C  LDBOOL	R8	1	0
      0x90022208,  //  003D  SETMBR	R0	K17	R8
      0x88200107,  //  003E  GETMBR	R8	R0	K7
      0x00201112,  //  003F  ADD	R8	R8	K18
      0x90020E08,  //  0040  SETMBR	R0	K7	R8
      0x8C200113,  //  0041  GETMET	R8	R0	K19
      0x7C200200,  //  0042  CALL	R8	1
      0x8C200114,  //  0043  GETMET	R8	R0	K20
      0x7C200200,  //  0044  CALL	R8	1
      0x80040800,  //  0045  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_remove_op_discovery
********************************************************************/
be_local_closure(Matter_Device_mdns_remove_op_discovery,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[23]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(get_device_id),
    /* K2   */  be_nested_str_weak(copy),
    /* K3   */  be_nested_str_weak(reverse),
    /* K4   */  be_nested_str_weak(get_fabric_compressed),
    /* K5   */  be_nested_str_weak(tohex),
    /* K6   */  be_nested_str_weak(_X2D),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(eth),
    /* K9   */  be_nested_str_weak(find),
    /* K10  */  be_nested_str_weak(up),
    /* K11  */  be_nested_str_weak(log),
    /* K12  */  be_nested_str_weak(MTR_X3A_X20remove_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27),
    /* K13  */  be_const_int(3),
    /* K14  */  be_nested_str_weak(remove_service),
    /* K15  */  be_nested_str_weak(_matter),
    /* K16  */  be_nested_str_weak(_tcp),
    /* K17  */  be_nested_str_weak(hostname_eth),
    /* K18  */  be_nested_str_weak(wifi),
    /* K19  */  be_nested_str_weak(hostname_wifi),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K21  */  be_nested_str_weak(_X7C),
    /* K22  */  be_const_int(2),
    }),
    be_str_weak(mdns_remove_op_discovery),
    &be_const_str_solidified,
    ( &(const binstruction[80]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA802003B,  //  0001  EXBLK	0	#003E
      0x8C0C0301,  //  0002  GETMET	R3	R1	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8C0C0702,  //  0004  GETMET	R3	R3	K2
      0x7C0C0200,  //  0005  CALL	R3	1
      0x8C0C0703,  //  0006  GETMET	R3	R3	K3
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C100304,  //  0008  GETMET	R4	R1	K4
      0x7C100200,  //  0009  CALL	R4	1
      0x8C140905,  //  000A  GETMET	R5	R4	K5
      0x7C140200,  //  000B  CALL	R5	1
      0x00140B06,  //  000C  ADD	R5	R5	K6
      0x8C180705,  //  000D  GETMET	R6	R3	K5
      0x7C180200,  //  000E  CALL	R6	1
      0x00140A06,  //  000F  ADD	R5	R5	R6
      0xB81A0E00,  //  0010  GETNGBL	R6	K7
      0x8C180D08,  //  0011  GETMET	R6	R6	K8
      0x7C180200,  //  0012  CALL	R6	1
      0x8C180D09,  //  0013  GETMET	R6	R6	K9
      0x5820000A,  //  0014  LDCONST	R8	K10
      0x7C180400,  //  0015  CALL	R6	2
      0x781A000E,  //  0016  JMPF	R6	#0026
      0xB81A0E00,  //  0017  GETNGBL	R6	K7
      0x8C180D0B,  //  0018  GETMET	R6	R6	K11
      0x60200018,  //  0019  GETGBL	R8	G24
      0x5824000C,  //  001A  LDCONST	R9	K12
      0x58280008,  //  001B  LDCONST	R10	K8
      0x5C2C0A00,  //  001C  MOVE	R11	R5
      0x7C200600,  //  001D  CALL	R8	3
      0x5824000D,  //  001E  LDCONST	R9	K13
      0x7C180600,  //  001F  CALL	R6	3
      0x8C18050E,  //  0020  GETMET	R6	R2	K14
      0x5820000F,  //  0021  LDCONST	R8	K15
      0x58240010,  //  0022  LDCONST	R9	K16
      0x5C280A00,  //  0023  MOVE	R10	R5
      0x882C0111,  //  0024  GETMBR	R11	R0	K17
      0x7C180A00,  //  0025  CALL	R6	5
      0xB81A0E00,  //  0026  GETNGBL	R6	K7
      0x8C180D12,  //  0027  GETMET	R6	R6	K18
      0x7C180200,  //  0028  CALL	R6	1
      0x8C180D09,  //  0029  GETMET	R6	R6	K9
      0x5820000A,  //  002A  LDCONST	R8	K10
      0x7C180400,  //  002B  CALL	R6	2
      0x781A000E,  //  002C  JMPF	R6	#003C
      0xB81A0E00,  //  002D  GETNGBL	R6	K7
      0x8C180D0B,  //  002E  GETMET	R6	R6	K11
      0x60200018,  //  002F  GETGBL	R8	G24
      0x5824000C,  //  0030  LDCONST	R9	K12
      0x58280012,  //  0031  LDCONST	R10	K18
      0x5C2C0A00,  //  0032  MOVE	R11	R5
      0x7C200600,  //  0033  CALL	R8	3
      0x5824000D,  //  0034  LDCONST	R9	K13
      0x7C180600,  //  0035  CALL	R6	3
      0x8C18050E,  //  0036  GETMET	R6	R2	K14
      0x5820000F,  //  0037  LDCONST	R8	K15
      0x58240010,  //  0038  LDCONST	R9	K16
      0x5C280A00,  //  0039  MOVE	R10	R5
      0x882C0113,  //  003A  GETMBR	R11	R0	K19
      0x7C180A00,  //  003B  CALL	R6	5
      0xA8040001,  //  003C  EXBLK	1	1
      0x70020010,  //  003D  JMP		#004F
      0xAC0C0002,  //  003E  CATCH	R3	0	2
      0x7002000D,  //  003F  JMP		#004E
      0xB8160E00,  //  0040  GETNGBL	R5	K7
      0x8C140B0B,  //  0041  GETMET	R5	R5	K11
      0x601C0008,  //  0042  GETGBL	R7	G8
      0x5C200600,  //  0043  MOVE	R8	R3
      0x7C1C0200,  //  0044  CALL	R7	1
      0x001E2807,  //  0045  ADD	R7	K20	R7
      0x001C0F15,  //  0046  ADD	R7	R7	K21
      0x60200008,  //  0047  GETGBL	R8	G8
      0x5C240800,  //  0048  MOVE	R9	R4
      0x7C200200,  //  0049  CALL	R8	1
      0x001C0E08,  //  004A  ADD	R7	R7	R8
      0x58200016,  //  004B  LDCONST	R8	K22
      0x7C140600,  //  004C  CALL	R5	3
      0x70020000,  //  004D  JMP		#004F
      0xB0080000,  //  004E  RAISE	2	R0	R0
      0x80000000,  //  004F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_plugin_remote_info
********************************************************************/
be_local_closure(Matter_Device_get_plugin_remote_info,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(plugins_config_remotes),
    /* K1   */  be_nested_str_weak(find),
    }),
    be_str_weak(get_plugin_remote_info),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88080100,  //  0000  GETMBR	R2	R0	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x5C100200,  //  0002  MOVE	R4	R1
      0x60140013,  //  0003  GETGBL	R5	G19
      0x7C140000,  //  0004  CALL	R5	0
      0x7C080600,  //  0005  CALL	R2	3
      0x80040400,  //  0006  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: autoconf_device_map
********************************************************************/
be_local_closure(Matter_Device_autoconf_device_map,   /* name */
  be_nested_proto(
    20,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[36]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_const_int(1),
    /* K2   */  be_nested_str_weak(light),
    /* K3   */  be_nested_str_weak(get),
    /* K4   */  be_nested_str_weak(find),
    /* K5   */  be_nested_str_weak(channels),
    /* K6   */  be_nested_str_weak(),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(type),
    /* K9   */  be_nested_str_weak(light1),
    /* K10  */  be_const_int(2),
    /* K11  */  be_nested_str_weak(light2),
    /* K12  */  be_nested_str_weak(light3),
    /* K13  */  be_nested_str_weak(tasmota),
    /* K14  */  be_nested_str_weak(cmd),
    /* K15  */  be_nested_str_weak(Status_X2013),
    /* K16  */  be_nested_str_weak(log),
    /* K17  */  be_nested_str_weak(MTR_X3A_X20Status_X2013_X20_X3D_X20),
    /* K18  */  be_const_int(3),
    /* K19  */  be_nested_str_weak(contains),
    /* K20  */  be_nested_str_weak(StatusSHT),
    /* K21  */  be_nested_str_weak(SHT),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20_X27_X25s_X27_X20_X3D_X20_X25s),
    /* K23  */  be_nested_str_weak(Relay1),
    /* K24  */  be_nested_str_weak(Relay2),
    /* K25  */  be_nested_str_weak(push),
    /* K26  */  be_nested_str_weak(MTR_X3A_X20relay1_X3D_X25s_X20relay2_X3D_X25s),
    /* K27  */  be_nested_str_weak(TiltConfig),
    /* K28  */  be_nested_str_weak(shutter_X2Btilt),
    /* K29  */  be_nested_str_weak(shutter),
    /* K30  */  be_nested_str_weak(get_power),
    /* K31  */  be_nested_str_weak(relay),
    /* K32  */  be_nested_str_weak(load),
    /* K33  */  be_nested_str_weak(read_sensors),
    /* K34  */  be_nested_str_weak(autoconf_sensors_list),
    /* K35  */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(autoconf_device_map),
    &be_const_str_solidified,
    ( &(const binstruction[198]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x60080013,  //  0001  GETGBL	R2	G19
      0x7C080000,  //  0002  CALL	R2	0
      0x580C0001,  //  0003  LDCONST	R3	K1
      0x50100000,  //  0004  LDBOOL	R4	0	0
      0xA4160400,  //  0005  IMPORT	R5	K2
      0x8C180B03,  //  0006  GETMET	R6	R5	K3
      0x7C180200,  //  0007  CALL	R6	1
      0x4C1C0000,  //  0008  LDNIL	R7
      0x201C0C07,  //  0009  NE	R7	R6	R7
      0x781E0024,  //  000A  JMPF	R7	#0030
      0x601C000C,  //  000B  GETGBL	R7	G12
      0x8C200D04,  //  000C  GETMET	R8	R6	K4
      0x58280005,  //  000D  LDCONST	R10	K5
      0x582C0006,  //  000E  LDCONST	R11	K6
      0x7C200600,  //  000F  CALL	R8	3
      0x7C1C0200,  //  0010  CALL	R7	1
      0x24200F07,  //  0011  GT	R8	R7	K7
      0x7822001C,  //  0012  JMPF	R8	#0030
      0x1C200F01,  //  0013  EQ	R8	R7	K1
      0x78220007,  //  0014  JMPF	R8	#001D
      0x60200008,  //  0015  GETGBL	R8	G8
      0x5C240600,  //  0016  MOVE	R9	R3
      0x7C200200,  //  0017  CALL	R8	1
      0x60240013,  //  0018  GETGBL	R9	G19
      0x7C240000,  //  0019  CALL	R9	0
      0x98261109,  //  001A  SETIDX	R9	K8	K9
      0x98081009,  //  001B  SETIDX	R2	R8	R9
      0x70020010,  //  001C  JMP		#002E
      0x1C200F0A,  //  001D  EQ	R8	R7	K10
      0x78220007,  //  001E  JMPF	R8	#0027
      0x60200008,  //  001F  GETGBL	R8	G8
      0x5C240600,  //  0020  MOVE	R9	R3
      0x7C200200,  //  0021  CALL	R8	1
      0x60240013,  //  0022  GETGBL	R9	G19
      0x7C240000,  //  0023  CALL	R9	0
      0x9826110B,  //  0024  SETIDX	R9	K8	K11
      0x98081009,  //  0025  SETIDX	R2	R8	R9
      0x70020006,  //  0026  JMP		#002E
      0x60200008,  //  0027  GETGBL	R8	G8
      0x5C240600,  //  0028  MOVE	R9	R3
      0x7C200200,  //  0029  CALL	R8	1
      0x60240013,  //  002A  GETGBL	R9	G19
      0x7C240000,  //  002B  CALL	R9	0
      0x9826110C,  //  002C  SETIDX	R9	K8	K12
      0x98081009,  //  002D  SETIDX	R2	R8	R9
      0x50100200,  //  002E  LDBOOL	R4	1	0
      0x000C0701,  //  002F  ADD	R3	R3	K1
      0xB81E1A00,  //  0030  GETNGBL	R7	K13
      0x8C1C0F0E,  //  0031  GETMET	R7	R7	K14
      0x5824000F,  //  0032  LDCONST	R9	K15
      0x50280200,  //  0033  LDBOOL	R10	1	0
      0x7C1C0600,  //  0034  CALL	R7	3
      0x60200012,  //  0035  GETGBL	R8	G18
      0x7C200000,  //  0036  CALL	R8	0
      0xB8261A00,  //  0037  GETNGBL	R9	K13
      0x8C241310,  //  0038  GETMET	R9	R9	K16
      0x602C0008,  //  0039  GETGBL	R11	G8
      0x5C300E00,  //  003A  MOVE	R12	R7
      0x7C2C0200,  //  003B  CALL	R11	1
      0x002E220B,  //  003C  ADD	R11	K17	R11
      0x58300012,  //  003D  LDCONST	R12	K18
      0x7C240600,  //  003E  CALL	R9	3
      0x4C240000,  //  003F  LDNIL	R9
      0x20240E09,  //  0040  NE	R9	R7	R9
      0x7826004F,  //  0041  JMPF	R9	#0092
      0x8C240F13,  //  0042  GETMET	R9	R7	K19
      0x582C0014,  //  0043  LDCONST	R11	K20
      0x7C240400,  //  0044  CALL	R9	2
      0x7826004B,  //  0045  JMPF	R9	#0092
      0x941C0F14,  //  0046  GETIDX	R7	R7	K20
      0x58240007,  //  0047  LDCONST	R9	K7
      0x50280200,  //  0048  LDBOOL	R10	1	0
      0x782A0047,  //  0049  JMPF	R10	#0092
      0x60280008,  //  004A  GETGBL	R10	G8
      0x5C2C1200,  //  004B  MOVE	R11	R9
      0x7C280200,  //  004C  CALL	R10	1
      0x002A2A0A,  //  004D  ADD	R10	K21	R10
      0x8C2C0F13,  //  004E  GETMET	R11	R7	K19
      0x5C341400,  //  004F  MOVE	R13	R10
      0x7C2C0400,  //  0050  CALL	R11	2
      0x742E0000,  //  0051  JMPT	R11	#0053
      0x7002003E,  //  0052  JMP		#0092
      0x942C0E0A,  //  0053  GETIDX	R11	R7	R10
      0xB8321A00,  //  0054  GETNGBL	R12	K13
      0x8C301910,  //  0055  GETMET	R12	R12	K16
      0x60380018,  //  0056  GETGBL	R14	G24
      0x583C0016,  //  0057  LDCONST	R15	K22
      0x5C401400,  //  0058  MOVE	R16	R10
      0x60440008,  //  0059  GETGBL	R17	G8
      0x5C481600,  //  005A  MOVE	R18	R11
      0x7C440200,  //  005B  CALL	R17	1
      0x7C380600,  //  005C  CALL	R14	3
      0x583C0012,  //  005D  LDCONST	R15	K18
      0x7C300600,  //  005E  CALL	R12	3
      0x8C301704,  //  005F  GETMET	R12	R11	K4
      0x58380017,  //  0060  LDCONST	R14	K23
      0x543DFFFE,  //  0061  LDINT	R15	-1
      0x7C300600,  //  0062  CALL	R12	3
      0x8C341704,  //  0063  GETMET	R13	R11	K4
      0x583C0018,  //  0064  LDCONST	R15	K24
      0x5441FFFE,  //  0065  LDINT	R16	-1
      0x7C340600,  //  0066  CALL	R13	3
      0x24381907,  //  0067  GT	R14	R12	K7
      0x783A0002,  //  0068  JMPF	R14	#006C
      0x8C381119,  //  0069  GETMET	R14	R8	K25
      0x04401901,  //  006A  SUB	R16	R12	K1
      0x7C380400,  //  006B  CALL	R14	2
      0x24381B07,  //  006C  GT	R14	R13	K7
      0x783A0002,  //  006D  JMPF	R14	#0071
      0x8C381119,  //  006E  GETMET	R14	R8	K25
      0x04401B01,  //  006F  SUB	R16	R13	K1
      0x7C380400,  //  0070  CALL	R14	2
      0xB83A1A00,  //  0071  GETNGBL	R14	K13
      0x8C381D10,  //  0072  GETMET	R14	R14	K16
      0x60400018,  //  0073  GETGBL	R16	G24
      0x5844001A,  //  0074  LDCONST	R17	K26
      0x5C481800,  //  0075  MOVE	R18	R12
      0x5C4C1A00,  //  0076  MOVE	R19	R13
      0x7C400600,  //  0077  CALL	R16	3
      0x58440012,  //  0078  LDCONST	R17	K18
      0x7C380600,  //  0079  CALL	R14	3
      0x8C381704,  //  007A  GETMET	R14	R11	K4
      0x5840001B,  //  007B  LDCONST	R16	K27
      0x7C380400,  //  007C  CALL	R14	2
      0x783A0002,  //  007D  JMPF	R14	#0081
      0x943C1D0A,  //  007E  GETIDX	R15	R14	K10
      0x243C1F07,  //  007F  GT	R15	R15	K7
      0x743E0000,  //  0080  JMPT	R15	#0082
      0x503C0001,  //  0081  LDBOOL	R15	0	1
      0x503C0200,  //  0082  LDBOOL	R15	1	0
      0x60400008,  //  0083  GETGBL	R16	G8
      0x5C440600,  //  0084  MOVE	R17	R3
      0x7C400200,  //  0085  CALL	R16	1
      0x60440013,  //  0086  GETGBL	R17	G19
      0x7C440000,  //  0087  CALL	R17	0
      0x783E0001,  //  0088  JMPF	R15	#008B
      0x5848001C,  //  0089  LDCONST	R18	K28
      0x70020000,  //  008A  JMP		#008C
      0x5848001D,  //  008B  LDCONST	R18	K29
      0x98461012,  //  008C  SETIDX	R17	K8	R18
      0x98463A09,  //  008D  SETIDX	R17	K29	R9
      0x98082011,  //  008E  SETIDX	R2	R16	R17
      0x000C0701,  //  008F  ADD	R3	R3	K1
      0x00241301,  //  0090  ADD	R9	R9	K1
      0x7001FFB5,  //  0091  JMP		#0048
      0x6024000C,  //  0092  GETGBL	R9	G12
      0xB82A1A00,  //  0093  GETNGBL	R10	K13
      0x8C28151E,  //  0094  GETMET	R10	R10	K30
      0x7C280200,  //  0095  CALL	R10	1
      0x7C240200,  //  0096  CALL	R9	1
      0x58280007,  //  0097  LDCONST	R10	K7
      0x78120000,  //  0098  JMPF	R4	#009A
      0x04241301,  //  0099  SUB	R9	R9	K1
      0x142C1409,  //  009A  LT	R11	R10	R9
      0x782E0011,  //  009B  JMPF	R11	#00AE
      0x8C2C1104,  //  009C  GETMET	R11	R8	K4
      0x5C341400,  //  009D  MOVE	R13	R10
      0x7C2C0400,  //  009E  CALL	R11	2
      0x4C300000,  //  009F  LDNIL	R12
      0x1C2C160C,  //  00A0  EQ	R11	R11	R12
      0x782E0009,  //  00A1  JMPF	R11	#00AC
      0x602C0008,  //  00A2  GETGBL	R11	G8
      0x5C300600,  //  00A3  MOVE	R12	R3
      0x7C2C0200,  //  00A4  CALL	R11	1
      0x60300013,  //  00A5  GETGBL	R12	G19
      0x7C300000,  //  00A6  CALL	R12	0
      0x9832111F,  //  00A7  SETIDX	R12	K8	K31
      0x00341501,  //  00A8  ADD	R13	R10	K1
      0x98323E0D,  //  00A9  SETIDX	R12	K31	R13
      0x9808160C,  //  00AA  SETIDX	R2	R11	R12
      0x000C0701,  //  00AB  ADD	R3	R3	K1
      0x00281501,  //  00AC  ADD	R10	R10	K1
      0x7001FFEB,  //  00AD  JMP		#009A
      0x8C2C0320,  //  00AE  GETMET	R11	R1	K32
      0xB8361A00,  //  00AF  GETNGBL	R13	K13
      0x8C341B21,  //  00B0  GETMET	R13	R13	K33
      0x7C340200,  //  00B1  CALL	R13	1
      0x7C2C0400,  //  00B2  CALL	R11	2
      0x8C300122,  //  00B3  GETMET	R12	R0	K34
      0x5C381600,  //  00B4  MOVE	R14	R11
      0x7C300400,  //  00B5  CALL	R12	2
      0x60340010,  //  00B6  GETGBL	R13	G16
      0x5C381800,  //  00B7  MOVE	R14	R12
      0x7C340200,  //  00B8  CALL	R13	1
      0xA8020007,  //  00B9  EXBLK	0	#00C2
      0x5C381A00,  //  00BA  MOVE	R14	R13
      0x7C380000,  //  00BB  CALL	R14	0
      0x603C0008,  //  00BC  GETGBL	R15	G8
      0x5C400600,  //  00BD  MOVE	R16	R3
      0x7C3C0200,  //  00BE  CALL	R15	1
      0x98081E0E,  //  00BF  SETIDX	R2	R15	R14
      0x000C0701,  //  00C0  ADD	R3	R3	K1
      0x7001FFF7,  //  00C1  JMP		#00BA
      0x58340023,  //  00C2  LDCONST	R13	K35
      0xAC340200,  //  00C3  CATCH	R13	1	0
      0xB0080000,  //  00C4  RAISE	2	R0	R0
      0x80040400,  //  00C5  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clean_remotes
********************************************************************/
be_local_closure(Matter_Device_clean_remotes,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(introspect),
    /* K1   */  be_nested_str_weak(http_remotes),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(stop_iteration),
    /* K4   */  be_nested_str_weak(plugins),
    /* K5   */  be_nested_str_weak(get),
    /* K6   */  be_nested_str_weak(http_remote),
    /* K7   */  be_nested_str_weak(find),
    /* K8   */  be_const_int(1),
    /* K9   */  be_nested_str_weak(keys),
    /* K10  */  be_nested_str_weak(push),
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
    ( &(const binstruction[81]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x88080101,  //  0001  GETMBR	R2	R0	K1
      0x780A004C,  //  0002  JMPF	R2	#0050
      0x60080013,  //  0003  GETGBL	R2	G19
      0x7C080000,  //  0004  CALL	R2	0
      0x600C0010,  //  0005  GETGBL	R3	G16
      0x88100101,  //  0006  GETMBR	R4	R0	K1
      0x7C0C0200,  //  0007  CALL	R3	1
      0xA8020003,  //  0008  EXBLK	0	#000D
      0x5C100600,  //  0009  MOVE	R4	R3
      0x7C100000,  //  000A  CALL	R4	0
      0x98080902,  //  000B  SETIDX	R2	R4	K2
      0x7001FFFB,  //  000C  JMP		#0009
      0x580C0003,  //  000D  LDCONST	R3	K3
      0xAC0C0200,  //  000E  CATCH	R3	1	0
      0xB0080000,  //  000F  RAISE	2	R0	R0
      0x600C0010,  //  0010  GETGBL	R3	G16
      0x88100104,  //  0011  GETMBR	R4	R0	K4
      0x7C0C0200,  //  0012  CALL	R3	1
      0xA802000F,  //  0013  EXBLK	0	#0024
      0x5C100600,  //  0014  MOVE	R4	R3
      0x7C100000,  //  0015  CALL	R4	0
      0x8C140305,  //  0016  GETMET	R5	R1	K5
      0x5C1C0800,  //  0017  MOVE	R7	R4
      0x58200006,  //  0018  LDCONST	R8	K6
      0x7C140600,  //  0019  CALL	R5	3
      0x4C180000,  //  001A  LDNIL	R6
      0x20180A06,  //  001B  NE	R6	R5	R6
      0x781A0005,  //  001C  JMPF	R6	#0023
      0x8C180507,  //  001D  GETMET	R6	R2	K7
      0x5C200A00,  //  001E  MOVE	R8	R5
      0x58240002,  //  001F  LDCONST	R9	K2
      0x7C180600,  //  0020  CALL	R6	3
      0x00180D08,  //  0021  ADD	R6	R6	K8
      0x98080A06,  //  0022  SETIDX	R2	R5	R6
      0x7001FFEF,  //  0023  JMP		#0014
      0x580C0003,  //  0024  LDCONST	R3	K3
      0xAC0C0200,  //  0025  CATCH	R3	1	0
      0xB0080000,  //  0026  RAISE	2	R0	R0
      0x600C0012,  //  0027  GETGBL	R3	G18
      0x7C0C0000,  //  0028  CALL	R3	0
      0x60100010,  //  0029  GETGBL	R4	G16
      0x8C140509,  //  002A  GETMET	R5	R2	K9
      0x7C140200,  //  002B  CALL	R5	1
      0x7C100200,  //  002C  CALL	R4	1
      0xA8020008,  //  002D  EXBLK	0	#0037
      0x5C140800,  //  002E  MOVE	R5	R4
      0x7C140000,  //  002F  CALL	R5	0
      0x94180405,  //  0030  GETIDX	R6	R2	R5
      0x1C180D02,  //  0031  EQ	R6	R6	K2
      0x781A0002,  //  0032  JMPF	R6	#0036
      0x8C18070A,  //  0033  GETMET	R6	R3	K10
      0x5C200A00,  //  0034  MOVE	R8	R5
      0x7C180400,  //  0035  CALL	R6	2
      0x7001FFF6,  //  0036  JMP		#002E
      0x58100003,  //  0037  LDCONST	R4	K3
      0xAC100200,  //  0038  CATCH	R4	1	0
      0xB0080000,  //  0039  RAISE	2	R0	R0
      0x60100010,  //  003A  GETGBL	R4	G16
      0x5C140600,  //  003B  MOVE	R5	R3
      0x7C100200,  //  003C  CALL	R4	1
      0xA802000E,  //  003D  EXBLK	0	#004D
      0x5C140800,  //  003E  MOVE	R5	R4
      0x7C140000,  //  003F  CALL	R5	0
      0xB81A1600,  //  0040  GETNGBL	R6	K11
      0x8C180D0C,  //  0041  GETMET	R6	R6	K12
      0x88200B0E,  //  0042  GETMBR	R8	R5	K14
      0x00221A08,  //  0043  ADD	R8	K13	R8
      0x5824000F,  //  0044  LDCONST	R9	K15
      0x7C180600,  //  0045  CALL	R6	3
      0x8C180B10,  //  0046  GETMET	R6	R5	K16
      0x7C180200,  //  0047  CALL	R6	1
      0x88180101,  //  0048  GETMBR	R6	R0	K1
      0x8C180D11,  //  0049  GETMET	R6	R6	K17
      0x88200B0E,  //  004A  GETMBR	R8	R5	K14
      0x7C180400,  //  004B  CALL	R6	2
      0x7001FFF0,  //  004C  JMP		#003E
      0x58100003,  //  004D  LDCONST	R4	K3
      0xAC100200,  //  004E  CATCH	R4	1	0
      0xB0080000,  //  004F  RAISE	2	R0	R0
      0x80000000,  //  0050  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_fabric
********************************************************************/
be_local_closure(Matter_Device_remove_fabric,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(MTR_X3A_X20removing_X20fabric_X20),
    /* K3   */  be_nested_str_weak(get_fabric_id),
    /* K4   */  be_nested_str_weak(copy),
    /* K5   */  be_nested_str_weak(reverse),
    /* K6   */  be_nested_str_weak(tohex),
    /* K7   */  be_const_int(2),
    /* K8   */  be_nested_str_weak(message_handler),
    /* K9   */  be_nested_str_weak(im),
    /* K10  */  be_nested_str_weak(subs_shop),
    /* K11  */  be_nested_str_weak(remove_by_fabric),
    /* K12  */  be_nested_str_weak(mdns_remove_op_discovery),
    /* K13  */  be_nested_str_weak(sessions),
    /* K14  */  be_nested_str_weak(remove_fabric),
    /* K15  */  be_nested_str_weak(save_fabrics),
    }),
    be_str_weak(remove_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0019,  //  0002  JMPF	R2	#001D
      0xB80A0000,  //  0003  GETNGBL	R2	K0
      0x8C080501,  //  0004  GETMET	R2	R2	K1
      0x8C100303,  //  0005  GETMET	R4	R1	K3
      0x7C100200,  //  0006  CALL	R4	1
      0x8C100904,  //  0007  GETMET	R4	R4	K4
      0x7C100200,  //  0008  CALL	R4	1
      0x8C100905,  //  0009  GETMET	R4	R4	K5
      0x7C100200,  //  000A  CALL	R4	1
      0x8C100906,  //  000B  GETMET	R4	R4	K6
      0x7C100200,  //  000C  CALL	R4	1
      0x00120404,  //  000D  ADD	R4	K2	R4
      0x58140007,  //  000E  LDCONST	R5	K7
      0x7C080600,  //  000F  CALL	R2	3
      0x88080108,  //  0010  GETMBR	R2	R0	K8
      0x88080509,  //  0011  GETMBR	R2	R2	K9
      0x8808050A,  //  0012  GETMBR	R2	R2	K10
      0x8C08050B,  //  0013  GETMET	R2	R2	K11
      0x5C100200,  //  0014  MOVE	R4	R1
      0x7C080400,  //  0015  CALL	R2	2
      0x8C08010C,  //  0016  GETMET	R2	R0	K12
      0x5C100200,  //  0017  MOVE	R4	R1
      0x7C080400,  //  0018  CALL	R2	2
      0x8808010D,  //  0019  GETMBR	R2	R0	K13
      0x8C08050E,  //  001A  GETMET	R2	R2	K14
      0x5C100200,  //  001B  MOVE	R4	R1
      0x7C080400,  //  001C  CALL	R2	2
      0x8808010D,  //  001D  GETMBR	R2	R0	K13
      0x8C08050F,  //  001E  GETMET	R2	R2	K15
      0x7C080200,  //  001F  CALL	R2	1
      0x80000000,  //  0020  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _mdns_announce_hostname
********************************************************************/
be_local_closure(Matter_Device__mdns_announce_hostname,   /* name */
  be_nested_proto(
    14,                          /* nstack */
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
    /* K12  */  be_nested_str_weak(contains),
    /* K13  */  be_nested_str_weak(ip6local),
    /* K14  */  be_nested_str_weak(add_hostname),
    /* K15  */  be_nested_str_weak(ip),
    /* K16  */  be_nested_str_weak(ip6),
    /* K17  */  be_nested_str_weak(log),
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
    ( &(const binstruction[148]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x8C100502,  //  0002  GETMET	R4	R2	K2
      0x7C100200,  //  0003  CALL	R4	1
      0xA802007A,  //  0004  EXBLK	0	#0080
      0x78060034,  //  0005  JMPF	R1	#003B
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
      0x78160003,  //  0012  JMPF	R5	#0017
      0x8C14090C,  //  0013  GETMET	R5	R4	K12
      0x581C000D,  //  0014  LDCONST	R7	K13
      0x7C140400,  //  0015  CALL	R5	2
      0x7416000F,  //  0016  JMPT	R5	#0027
      0x8C14050E,  //  0017  GETMET	R5	R2	K14
      0x881C0105,  //  0018  GETMBR	R7	R0	K5
      0x8C200907,  //  0019  GETMET	R8	R4	K7
      0x5828000D,  //  001A  LDCONST	R10	K13
      0x582C000A,  //  001B  LDCONST	R11	K10
      0x7C200600,  //  001C  CALL	R8	3
      0x8C240907,  //  001D  GETMET	R9	R4	K7
      0x582C000F,  //  001E  LDCONST	R11	K15
      0x5830000A,  //  001F  LDCONST	R12	K10
      0x7C240600,  //  0020  CALL	R9	3
      0x8C280907,  //  0021  GETMET	R10	R4	K7
      0x58300010,  //  0022  LDCONST	R12	K16
      0x5834000A,  //  0023  LDCONST	R13	K10
      0x7C280600,  //  0024  CALL	R10	3
      0x7C140A00,  //  0025  CALL	R5	5
      0x70020012,  //  0026  JMP		#003A
      0xB8160600,  //  0027  GETNGBL	R5	K3
      0x8C140B11,  //  0028  GETMET	R5	R5	K17
      0x601C0018,  //  0029  GETGBL	R7	G24
      0x58200012,  //  002A  LDCONST	R8	K18
      0x88240105,  //  002B  GETMBR	R9	R0	K5
      0x8C280907,  //  002C  GETMET	R10	R4	K7
      0x5830000F,  //  002D  LDCONST	R12	K15
      0x5834000A,  //  002E  LDCONST	R13	K10
      0x7C280600,  //  002F  CALL	R10	3
      0x7C1C0600,  //  0030  CALL	R7	3
      0x58200013,  //  0031  LDCONST	R8	K19
      0x7C140600,  //  0032  CALL	R5	3
      0x8C14050E,  //  0033  GETMET	R5	R2	K14
      0x881C0105,  //  0034  GETMBR	R7	R0	K5
      0x8C200907,  //  0035  GETMET	R8	R4	K7
      0x5828000F,  //  0036  LDCONST	R10	K15
      0x582C000A,  //  0037  LDCONST	R11	K10
      0x7C200600,  //  0038  CALL	R8	3
      0x7C140600,  //  0039  CALL	R5	3
      0x70020033,  //  003A  JMP		#006F
      0xB8120600,  //  003B  GETNGBL	R4	K3
      0x8C100914,  //  003C  GETMET	R4	R4	K20
      0x7C100200,  //  003D  CALL	R4	1
      0x8C140706,  //  003E  GETMET	R5	R3	K6
      0x8C1C0907,  //  003F  GETMET	R7	R4	K7
      0x58240008,  //  0040  LDCONST	R9	K8
      0x7C1C0400,  //  0041  CALL	R7	2
      0x58200009,  //  0042  LDCONST	R8	K9
      0x5824000A,  //  0043  LDCONST	R9	K10
      0x7C140800,  //  0044  CALL	R5	4
      0x90022A05,  //  0045  SETMBR	R0	K21	R5
      0x8814010B,  //  0046  GETMBR	R5	R0	K11
      0x78160003,  //  0047  JMPF	R5	#004C
      0x8C14090C,  //  0048  GETMET	R5	R4	K12
      0x581C000D,  //  0049  LDCONST	R7	K13
      0x7C140400,  //  004A  CALL	R5	2
      0x7416000F,  //  004B  JMPT	R5	#005C
      0x8C14050E,  //  004C  GETMET	R5	R2	K14
      0x881C0115,  //  004D  GETMBR	R7	R0	K21
      0x8C200907,  //  004E  GETMET	R8	R4	K7
      0x5828000D,  //  004F  LDCONST	R10	K13
      0x582C000A,  //  0050  LDCONST	R11	K10
      0x7C200600,  //  0051  CALL	R8	3
      0x8C240907,  //  0052  GETMET	R9	R4	K7
      0x582C000F,  //  0053  LDCONST	R11	K15
      0x5830000A,  //  0054  LDCONST	R12	K10
      0x7C240600,  //  0055  CALL	R9	3
      0x8C280907,  //  0056  GETMET	R10	R4	K7
      0x58300010,  //  0057  LDCONST	R12	K16
      0x5834000A,  //  0058  LDCONST	R13	K10
      0x7C280600,  //  0059  CALL	R10	3
      0x7C140A00,  //  005A  CALL	R5	5
      0x70020012,  //  005B  JMP		#006F
      0xB8160600,  //  005C  GETNGBL	R5	K3
      0x8C140B11,  //  005D  GETMET	R5	R5	K17
      0x601C0018,  //  005E  GETGBL	R7	G24
      0x58200012,  //  005F  LDCONST	R8	K18
      0x88240115,  //  0060  GETMBR	R9	R0	K21
      0x8C280907,  //  0061  GETMET	R10	R4	K7
      0x5830000F,  //  0062  LDCONST	R12	K15
      0x5834000A,  //  0063  LDCONST	R13	K10
      0x7C280600,  //  0064  CALL	R10	3
      0x7C1C0600,  //  0065  CALL	R7	3
      0x58200013,  //  0066  LDCONST	R8	K19
      0x7C140600,  //  0067  CALL	R5	3
      0x8C14050E,  //  0068  GETMET	R5	R2	K14
      0x881C0115,  //  0069  GETMBR	R7	R0	K21
      0x8C200907,  //  006A  GETMET	R8	R4	K7
      0x5828000F,  //  006B  LDCONST	R10	K15
      0x582C000A,  //  006C  LDCONST	R11	K10
      0x7C200600,  //  006D  CALL	R8	3
      0x7C140600,  //  006E  CALL	R5	3
      0xB8120600,  //  006F  GETNGBL	R4	K3
      0x8C100911,  //  0070  GETMET	R4	R4	K17
      0x60180018,  //  0071  GETGBL	R6	G24
      0x581C0016,  //  0072  LDCONST	R7	K22
      0x78060001,  //  0073  JMPF	R1	#0076
      0x58200004,  //  0074  LDCONST	R8	K4
      0x70020000,  //  0075  JMP		#0077
      0x58200014,  //  0076  LDCONST	R8	K20
      0x78060001,  //  0077  JMPF	R1	#007A
      0x88240105,  //  0078  GETMBR	R9	R0	K5
      0x70020000,  //  0079  JMP		#007B
      0x88240115,  //  007A  GETMBR	R9	R0	K21
      0x7C180600,  //  007B  CALL	R6	3
      0x581C0013,  //  007C  LDCONST	R7	K19
      0x7C100600,  //  007D  CALL	R4	3
      0xA8040001,  //  007E  EXBLK	1	1
      0x70020010,  //  007F  JMP		#0091
      0xAC100002,  //  0080  CATCH	R4	0	2
      0x7002000D,  //  0081  JMP		#0090
      0xB81A0600,  //  0082  GETNGBL	R6	K3
      0x8C180D11,  //  0083  GETMET	R6	R6	K17
      0x60200008,  //  0084  GETGBL	R8	G8
      0x5C240800,  //  0085  MOVE	R9	R4
      0x7C200200,  //  0086  CALL	R8	1
      0x00222E08,  //  0087  ADD	R8	K23	R8
      0x00201118,  //  0088  ADD	R8	R8	K24
      0x60240008,  //  0089  GETGBL	R9	G8
      0x5C280A00,  //  008A  MOVE	R10	R5
      0x7C240200,  //  008B  CALL	R9	1
      0x00201009,  //  008C  ADD	R8	R8	R9
      0x58240019,  //  008D  LDCONST	R9	K25
      0x7C180600,  //  008E  CALL	R6	3
      0x70020000,  //  008F  JMP		#0091
      0xB0080000,  //  0090  RAISE	2	R0	R0
      0x8C10011A,  //  0091  GETMET	R4	R0	K26
      0x7C100200,  //  0092  CALL	R4	1
      0x80000000,  //  0093  RET	0
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
    ( &(const bvalue[11]) {     /* constants */
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
    }),
    be_str_weak(register_native_classes),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
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
      0x80000000,  //  0020  RET	0
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
** Solidified function: process_attribute_read_solo
********************************************************************/
be_local_closure(Matter_Device_process_attribute_read_solo,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(endpoint),
    /* K1   */  be_nested_str_weak(cluster),
    /* K2   */  be_nested_str_weak(attribute),
    /* K3   */  be_nested_str_weak(find_plugin_by_endpoint),
    /* K4   */  be_nested_str_weak(status),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(UNSUPPORTED_ENDPOINT),
    /* K7   */  be_nested_str_weak(contains_cluster),
    /* K8   */  be_nested_str_weak(UNSUPPORTED_CLUSTER),
    /* K9   */  be_nested_str_weak(contains_attribute),
    /* K10  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    }),
    be_str_weak(process_attribute_read_solo),
    &be_const_str_solidified,
    ( &(const binstruction[45]) {  /* code */
      0x88080300,  //  0000  GETMBR	R2	R1	K0
      0x880C0301,  //  0001  GETMBR	R3	R1	K1
      0x88100302,  //  0002  GETMBR	R4	R1	K2
      0x4C140000,  //  0003  LDNIL	R5
      0x1C140405,  //  0004  EQ	R5	R2	R5
      0x74160005,  //  0005  JMPT	R5	#000C
      0x4C140000,  //  0006  LDNIL	R5
      0x1C140605,  //  0007  EQ	R5	R3	R5
      0x74160002,  //  0008  JMPT	R5	#000C
      0x4C140000,  //  0009  LDNIL	R5
      0x1C140805,  //  000A  EQ	R5	R4	R5
      0x78160001,  //  000B  JMPF	R5	#000E
      0x4C140000,  //  000C  LDNIL	R5
      0x80040A00,  //  000D  RET	1	R5
      0x8C140103,  //  000E  GETMET	R5	R0	K3
      0x5C1C0400,  //  000F  MOVE	R7	R2
      0x7C140400,  //  0010  CALL	R5	2
      0x4C180000,  //  0011  LDNIL	R6
      0x1C180A06,  //  0012  EQ	R6	R5	R6
      0x781A0004,  //  0013  JMPF	R6	#0019
      0xB81A0A00,  //  0014  GETNGBL	R6	K5
      0x88180D06,  //  0015  GETMBR	R6	R6	K6
      0x90060806,  //  0016  SETMBR	R1	K4	R6
      0x4C180000,  //  0017  LDNIL	R6
      0x80040C00,  //  0018  RET	1	R6
      0x8C180B07,  //  0019  GETMET	R6	R5	K7
      0x5C200600,  //  001A  MOVE	R8	R3
      0x7C180400,  //  001B  CALL	R6	2
      0x741A0004,  //  001C  JMPT	R6	#0022
      0xB81A0A00,  //  001D  GETNGBL	R6	K5
      0x88180D08,  //  001E  GETMBR	R6	R6	K8
      0x90060806,  //  001F  SETMBR	R1	K4	R6
      0x4C180000,  //  0020  LDNIL	R6
      0x80040C00,  //  0021  RET	1	R6
      0x8C180B09,  //  0022  GETMET	R6	R5	K9
      0x5C200600,  //  0023  MOVE	R8	R3
      0x5C240800,  //  0024  MOVE	R9	R4
      0x7C180600,  //  0025  CALL	R6	3
      0x741A0004,  //  0026  JMPT	R6	#002C
      0xB81A0A00,  //  0027  GETNGBL	R6	K5
      0x88180D0A,  //  0028  GETMBR	R6	R6	K10
      0x90060806,  //  0029  SETMBR	R1	K4	R6
      0x4C180000,  //  002A  LDNIL	R6
      0x80040C00,  //  002B  RET	1	R6
      0x80040A00,  //  002C  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: autoconf_device
********************************************************************/
be_local_closure(Matter_Device_autoconf_device,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(plugins),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(plugins_persist),
    /* K4   */  be_nested_str_weak(plugins_config),
    /* K5   */  be_nested_str_weak(autoconf_device_map),
    /* K6   */  be_nested_str_weak(plugins_config_remotes),
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
    ( &(const binstruction[40]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C0101,  //  0002  GETMBR	R3	R0	K1
      0x7C080200,  //  0003  CALL	R2	1
      0x24080502,  //  0004  GT	R2	R2	K2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x80000400,  //  0006  RET	0
      0x88080103,  //  0007  GETMBR	R2	R0	K3
      0x740A000F,  //  0008  JMPT	R2	#0019
      0x8C080105,  //  0009  GETMET	R2	R0	K5
      0x7C080200,  //  000A  CALL	R2	1
      0x90020802,  //  000B  SETMBR	R0	K4	R2
      0x60080013,  //  000C  GETGBL	R2	G19
      0x7C080000,  //  000D  CALL	R2	0
      0x90020C02,  //  000E  SETMBR	R0	K6	R2
      0x8C080107,  //  000F  GETMET	R2	R0	K7
      0x7C080200,  //  0010  CALL	R2	1
      0xB80A1000,  //  0011  GETNGBL	R2	K8
      0x8C080509,  //  0012  GETMET	R2	R2	K9
      0x60100008,  //  0013  GETGBL	R4	G8
      0x88140104,  //  0014  GETMBR	R5	R0	K4
      0x7C100200,  //  0015  CALL	R4	1
      0x00121404,  //  0016  ADD	R4	K10	R4
      0x5814000B,  //  0017  LDCONST	R5	K11
      0x7C080600,  //  0018  CALL	R2	3
      0x8C08010C,  //  0019  GETMET	R2	R0	K12
      0x88100104,  //  001A  GETMBR	R4	R0	K4
      0x7C080400,  //  001B  CALL	R2	2
      0x88080103,  //  001C  GETMBR	R2	R0	K3
      0x740A0008,  //  001D  JMPT	R2	#0027
      0x8808010D,  //  001E  GETMBR	R2	R0	K13
      0x8C08050E,  //  001F  GETMET	R2	R2	K14
      0x7C080200,  //  0020  CALL	R2	1
      0x24080502,  //  0021  GT	R2	R2	K2
      0x780A0003,  //  0022  JMPF	R2	#0027
      0x50080200,  //  0023  LDBOOL	R2	1	0
      0x90020602,  //  0024  SETMBR	R0	K3	R2
      0x8C08010F,  //  0025  GETMET	R2	R0	K15
      0x7C080200,  //  0026  CALL	R2	1
      0x80000000,  //  0027  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_http_remote
********************************************************************/
be_local_closure(Matter_Device_register_http_remote,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(http_remotes),
    /* K1   */  be_nested_str_weak(contains),
    /* K2   */  be_nested_str_weak(get_timeout),
    /* K3   */  be_nested_str_weak(set_timeout),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(HTTP_remote),
    /* K6   */  be_nested_str_weak(plugins_config_remotes),
    /* K7   */  be_nested_str_weak(set_info),
    }),
    be_str_weak(register_http_remote),
    &be_const_str_solidified,
    ( &(const binstruction[42]) {  /* code */
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
      0x70020011,  //  0016  JMP		#0029
      0xB8120800,  //  0017  GETNGBL	R4	K4
      0x8C100905,  //  0018  GETMET	R4	R4	K5
      0x5C180000,  //  0019  MOVE	R6	R0
      0x5C1C0200,  //  001A  MOVE	R7	R1
      0x5C200400,  //  001B  MOVE	R8	R2
      0x7C100800,  //  001C  CALL	R4	4
      0x5C0C0800,  //  001D  MOVE	R3	R4
      0x88100106,  //  001E  GETMBR	R4	R0	K6
      0x8C100901,  //  001F  GETMET	R4	R4	K1
      0x5C180200,  //  0020  MOVE	R6	R1
      0x7C100400,  //  0021  CALL	R4	2
      0x78120003,  //  0022  JMPF	R4	#0027
      0x8C100707,  //  0023  GETMET	R4	R3	K7
      0x88180106,  //  0024  GETMBR	R6	R0	K6
      0x94180C01,  //  0025  GETIDX	R6	R6	R1
      0x7C100400,  //  0026  CALL	R4	2
      0x88100100,  //  0027  GETMBR	R4	R0	K0
      0x98100203,  //  0028  SETIDX	R4	R1	R3
      0x80040600,  //  0029  RET	1	R3
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
    7,                          /* nstack */
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
    ( &(const binstruction[28]) {  /* code */
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
      0x5C100000,  //  0011  MOVE	R4	R0
      0x58140007,  //  0012  LDCONST	R5	K7
      0x5C180200,  //  0013  MOVE	R6	R1
      0x7C080800,  //  0014  CALL	R2	4
      0x90020002,  //  0015  SETMBR	R0	K0	R2
      0x88080100,  //  0016  GETMBR	R2	R0	K0
      0x8C080508,  //  0017  GETMET	R2	R2	K8
      0x84100000,  //  0018  CLOSURE	R4	P0
      0x7C080400,  //  0019  CALL	R2	2
      0xA0000000,  //  001A  CLOSE	R0
      0x80000000,  //  001B  RET	0
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
** Solidified function: bridge_remove_endpoint
********************************************************************/
be_local_closure(Matter_Device_bridge_remove_endpoint,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(plugins_config),
    /* K2   */  be_nested_str_weak(contains),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(log),
    /* K5   */  be_nested_str_weak(MTR_X3A_X20Cannot_X20remove_X20an_X20enpoint_X20not_X20configured_X3A_X20),
    /* K6   */  be_const_int(3),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20deleting_X20endpoint_X20_X3D_X20_X25i),
    /* K8   */  be_const_int(2),
    /* K9   */  be_nested_str_weak(remove),
    /* K10  */  be_nested_str_weak(plugins_persist),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_weak(plugins),
    /* K13  */  be_nested_str_weak(get_endpoint),
    /* K14  */  be_const_int(1),
    /* K15  */  be_nested_str_weak(clean_remotes),
    /* K16  */  be_nested_str_weak(save_param),
    /* K17  */  be_nested_str_weak(signal_endpoints_changed),
    }),
    be_str_weak(bridge_remove_endpoint),
    &be_const_str_solidified,
    ( &(const binstruction[58]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x600C0008,  //  0001  GETGBL	R3	G8
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x4C100000,  //  0004  LDNIL	R4
      0x4C140000,  //  0005  LDNIL	R5
      0x88180101,  //  0006  GETMBR	R6	R0	K1
      0x8C180D02,  //  0007  GETMET	R6	R6	K2
      0x5C200600,  //  0008  MOVE	R8	R3
      0x7C180400,  //  0009  CALL	R6	2
      0x741A0005,  //  000A  JMPT	R6	#0011
      0xB81A0600,  //  000B  GETNGBL	R6	K3
      0x8C180D04,  //  000C  GETMET	R6	R6	K4
      0x00220A03,  //  000D  ADD	R8	K5	R3
      0x58240006,  //  000E  LDCONST	R9	K6
      0x7C180600,  //  000F  CALL	R6	3
      0x80000C00,  //  0010  RET	0
      0xB81A0600,  //  0011  GETNGBL	R6	K3
      0x8C180D04,  //  0012  GETMET	R6	R6	K4
      0x60200018,  //  0013  GETGBL	R8	G24
      0x58240007,  //  0014  LDCONST	R9	K7
      0x5C280200,  //  0015  MOVE	R10	R1
      0x7C200400,  //  0016  CALL	R8	2
      0x58240008,  //  0017  LDCONST	R9	K8
      0x7C180600,  //  0018  CALL	R6	3
      0x88180101,  //  0019  GETMBR	R6	R0	K1
      0x8C180D09,  //  001A  GETMET	R6	R6	K9
      0x5C200600,  //  001B  MOVE	R8	R3
      0x7C180400,  //  001C  CALL	R6	2
      0x50180200,  //  001D  LDBOOL	R6	1	0
      0x90021406,  //  001E  SETMBR	R0	K10	R6
      0x5818000B,  //  001F  LDCONST	R6	K11
      0x601C000C,  //  0020  GETGBL	R7	G12
      0x8820010C,  //  0021  GETMBR	R8	R0	K12
      0x7C1C0200,  //  0022  CALL	R7	1
      0x141C0C07,  //  0023  LT	R7	R6	R7
      0x781E000D,  //  0024  JMPF	R7	#0033
      0x881C010C,  //  0025  GETMBR	R7	R0	K12
      0x941C0E06,  //  0026  GETIDX	R7	R7	R6
      0x8C1C0F0D,  //  0027  GETMET	R7	R7	K13
      0x7C1C0200,  //  0028  CALL	R7	1
      0x1C1C0207,  //  0029  EQ	R7	R1	R7
      0x781E0005,  //  002A  JMPF	R7	#0031
      0x881C010C,  //  002B  GETMBR	R7	R0	K12
      0x8C1C0F09,  //  002C  GETMET	R7	R7	K9
      0x5C240C00,  //  002D  MOVE	R9	R6
      0x7C1C0400,  //  002E  CALL	R7	2
      0x70020002,  //  002F  JMP		#0033
      0x70020000,  //  0030  JMP		#0032
      0x00180D0E,  //  0031  ADD	R6	R6	K14
      0x7001FFEC,  //  0032  JMP		#0020
      0x8C1C010F,  //  0033  GETMET	R7	R0	K15
      0x7C1C0200,  //  0034  CALL	R7	1
      0x8C1C0110,  //  0035  GETMET	R7	R0	K16
      0x7C1C0200,  //  0036  CALL	R7	1
      0x8C1C0111,  //  0037  GETMET	R7	R0	K17
      0x7C1C0200,  //  0038  CALL	R7	1
      0x80000000,  //  0039  RET	0
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
** Solidified function: init
********************************************************************/
be_local_closure(Matter_Device_init,   /* name */
  be_nested_proto(
    7,                          /* nstack */
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
    ( &(const bvalue[46]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(get_option),
    /* K3   */  be_nested_str_weak(matter),
    /* K4   */  be_nested_str_weak(MATTER_OPTION),
    /* K5   */  be_nested_str_weak(UI),
    /* K6   */  be_nested_str_weak(profiler),
    /* K7   */  be_nested_str_weak(Profiler),
    /* K8   */  be_nested_str_weak(started),
    /* K9   */  be_nested_str_weak(tick),
    /* K10  */  be_const_int(0),
    /* K11  */  be_nested_str_weak(plugins),
    /* K12  */  be_nested_str_weak(plugins_persist),
    /* K13  */  be_nested_str_weak(plugins_classes),
    /* K14  */  be_nested_str_weak(plugins_config_remotes),
    /* K15  */  be_nested_str_weak(register_native_classes),
    /* K16  */  be_nested_str_weak(vendorid),
    /* K17  */  be_nested_str_weak(VENDOR_ID),
    /* K18  */  be_nested_str_weak(productid),
    /* K19  */  be_nested_str_weak(PRODUCT_ID),
    /* K20  */  be_nested_str_weak(root_iterations),
    /* K21  */  be_nested_str_weak(PBKDF_ITERATIONS),
    /* K22  */  be_nested_str_weak(next_ep),
    /* K23  */  be_const_int(1),
    /* K24  */  be_nested_str_weak(root_salt),
    /* K25  */  be_nested_str_weak(random),
    /* K26  */  be_nested_str_weak(ipv4only),
    /* K27  */  be_nested_str_weak(disable_bridge_mode),
    /* K28  */  be_nested_str_weak(load_param),
    /* K29  */  be_nested_str_weak(sessions),
    /* K30  */  be_nested_str_weak(Session_Store),
    /* K31  */  be_nested_str_weak(load_fabrics),
    /* K32  */  be_nested_str_weak(message_handler),
    /* K33  */  be_nested_str_weak(MessageHandler),
    /* K34  */  be_nested_str_weak(ui),
    /* K35  */  be_nested_str_weak(wifi),
    /* K36  */  be_nested_str_weak(up),
    /* K37  */  be_nested_str_weak(eth),
    /* K38  */  be_nested_str_weak(start),
    /* K39  */  be_nested_str_weak(add_rule),
    /* K40  */  be_nested_str_weak(Wifi_X23Connected),
    /* K41  */  be_nested_str_weak(matter_start),
    /* K42  */  be_nested_str_weak(Eth_X23Connected),
    /* K43  */  be_nested_str_weak(_init_basic_commissioning),
    /* K44  */  be_nested_str_weak(add_driver),
    /* K45  */  be_nested_str_weak(register_commands),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[112]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xB80A0200,  //  0001  GETNGBL	R2	K1
      0x8C080502,  //  0002  GETMET	R2	R2	K2
      0xB8120600,  //  0003  GETNGBL	R4	K3
      0x88100904,  //  0004  GETMBR	R4	R4	K4
      0x7C080400,  //  0005  CALL	R2	2
      0x740A0004,  //  0006  JMPT	R2	#000C
      0xB80A0600,  //  0007  GETNGBL	R2	K3
      0x8C080505,  //  0008  GETMET	R2	R2	K5
      0x5C100000,  //  0009  MOVE	R4	R0
      0x7C080400,  //  000A  CALL	R2	2
      0x80000400,  //  000B  RET	0
      0xB80A0600,  //  000C  GETNGBL	R2	K3
      0xB80E0600,  //  000D  GETNGBL	R3	K3
      0x8C0C0707,  //  000E  GETMET	R3	R3	K7
      0x7C0C0200,  //  000F  CALL	R3	1
      0x900A0C03,  //  0010  SETMBR	R2	K6	R3
      0x50080000,  //  0011  LDBOOL	R2	0	0
      0x90021002,  //  0012  SETMBR	R0	K8	R2
      0x9002130A,  //  0013  SETMBR	R0	K9	K10
      0x60080012,  //  0014  GETGBL	R2	G18
      0x7C080000,  //  0015  CALL	R2	0
      0x90021602,  //  0016  SETMBR	R0	K11	R2
      0x50080000,  //  0017  LDBOOL	R2	0	0
      0x90021802,  //  0018  SETMBR	R0	K12	R2
      0x60080013,  //  0019  GETGBL	R2	G19
      0x7C080000,  //  001A  CALL	R2	0
      0x90021A02,  //  001B  SETMBR	R0	K13	R2
      0x60080013,  //  001C  GETGBL	R2	G19
      0x7C080000,  //  001D  CALL	R2	0
      0x90021C02,  //  001E  SETMBR	R0	K14	R2
      0x8C08010F,  //  001F  GETMET	R2	R0	K15
      0x7C080200,  //  0020  CALL	R2	1
      0x88080111,  //  0021  GETMBR	R2	R0	K17
      0x90022002,  //  0022  SETMBR	R0	K16	R2
      0x88080113,  //  0023  GETMBR	R2	R0	K19
      0x90022402,  //  0024  SETMBR	R0	K18	R2
      0x88080115,  //  0025  GETMBR	R2	R0	K21
      0x90022802,  //  0026  SETMBR	R0	K20	R2
      0x90022D17,  //  0027  SETMBR	R0	K22	K23
      0x8C080319,  //  0028  GETMET	R2	R1	K25
      0x5412000F,  //  0029  LDINT	R4	16
      0x7C080400,  //  002A  CALL	R2	2
      0x90023002,  //  002B  SETMBR	R0	K24	R2
      0x50080000,  //  002C  LDBOOL	R2	0	0
      0x90023402,  //  002D  SETMBR	R0	K26	R2
      0x50080000,  //  002E  LDBOOL	R2	0	0
      0x90023602,  //  002F  SETMBR	R0	K27	R2
      0x8C08011C,  //  0030  GETMET	R2	R0	K28
      0x7C080200,  //  0031  CALL	R2	1
      0xB80A0600,  //  0032  GETNGBL	R2	K3
      0x8C08051E,  //  0033  GETMET	R2	R2	K30
      0x5C100000,  //  0034  MOVE	R4	R0
      0x7C080400,  //  0035  CALL	R2	2
      0x90023A02,  //  0036  SETMBR	R0	K29	R2
      0x8808011D,  //  0037  GETMBR	R2	R0	K29
      0x8C08051F,  //  0038  GETMET	R2	R2	K31
      0x7C080200,  //  0039  CALL	R2	1
      0xB80A0600,  //  003A  GETNGBL	R2	K3
      0x8C080521,  //  003B  GETMET	R2	R2	K33
      0x5C100000,  //  003C  MOVE	R4	R0
      0x7C080400,  //  003D  CALL	R2	2
      0x90024002,  //  003E  SETMBR	R0	K32	R2
      0xB80A0600,  //  003F  GETNGBL	R2	K3
      0x8C080505,  //  0040  GETMET	R2	R2	K5
      0x5C100000,  //  0041  MOVE	R4	R0
      0x7C080400,  //  0042  CALL	R2	2
      0x90024402,  //  0043  SETMBR	R0	K34	R2
      0xB80A0200,  //  0044  GETNGBL	R2	K1
      0x8C080523,  //  0045  GETMET	R2	R2	K35
      0x7C080200,  //  0046  CALL	R2	1
      0x94080524,  //  0047  GETIDX	R2	R2	K36
      0x740A0004,  //  0048  JMPT	R2	#004E
      0xB80A0200,  //  0049  GETNGBL	R2	K1
      0x8C080525,  //  004A  GETMET	R2	R2	K37
      0x7C080200,  //  004B  CALL	R2	1
      0x94080524,  //  004C  GETIDX	R2	R2	K36
      0x780A0001,  //  004D  JMPF	R2	#0050
      0x8C080126,  //  004E  GETMET	R2	R0	K38
      0x7C080200,  //  004F  CALL	R2	1
      0xB80A0200,  //  0050  GETNGBL	R2	K1
      0x8C080523,  //  0051  GETMET	R2	R2	K35
      0x7C080200,  //  0052  CALL	R2	1
      0x94080524,  //  0053  GETIDX	R2	R2	K36
      0x740A0005,  //  0054  JMPT	R2	#005B
      0xB80A0200,  //  0055  GETNGBL	R2	K1
      0x8C080527,  //  0056  GETMET	R2	R2	K39
      0x58100028,  //  0057  LDCONST	R4	K40
      0x84140000,  //  0058  CLOSURE	R5	P0
      0x58180029,  //  0059  LDCONST	R6	K41
      0x7C080800,  //  005A  CALL	R2	4
      0xB80A0200,  //  005B  GETNGBL	R2	K1
      0x8C080525,  //  005C  GETMET	R2	R2	K37
      0x7C080200,  //  005D  CALL	R2	1
      0x94080524,  //  005E  GETIDX	R2	R2	K36
      0x740A0005,  //  005F  JMPT	R2	#0066
      0xB80A0200,  //  0060  GETNGBL	R2	K1
      0x8C080527,  //  0061  GETMET	R2	R2	K39
      0x5810002A,  //  0062  LDCONST	R4	K42
      0x84140001,  //  0063  CLOSURE	R5	P1
      0x58180029,  //  0064  LDCONST	R6	K41
      0x7C080800,  //  0065  CALL	R2	4
      0x8C08012B,  //  0066  GETMET	R2	R0	K43
      0x7C080200,  //  0067  CALL	R2	1
      0xB80A0200,  //  0068  GETNGBL	R2	K1
      0x8C08052C,  //  0069  GETMET	R2	R2	K44
      0x5C100000,  //  006A  MOVE	R4	R0
      0x7C080400,  //  006B  CALL	R2	2
      0x8C08012D,  //  006C  GETMET	R2	R0	K45
      0x7C080200,  //  006D  CALL	R2	1
      0xA0000000,  //  006E  CLOSE	R0
      0x80000000,  //  006F  RET	0
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
** Solidified function: update_remotes_info
********************************************************************/
be_local_closure(Matter_Device_update_remotes_info,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(http_remotes),
    /* K1   */  be_nested_str_weak(keys),
    /* K2   */  be_nested_str_weak(get_info),
    /* K3   */  be_const_int(0),
    /* K4   */  be_nested_str_weak(stop_iteration),
    /* K5   */  be_nested_str_weak(plugins_config_remotes),
    }),
    be_str_weak(update_remotes_info),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x60040013,  //  0000  GETGBL	R1	G19
      0x7C040000,  //  0001  CALL	R1	0
      0x88080100,  //  0002  GETMBR	R2	R0	K0
      0x4C0C0000,  //  0003  LDNIL	R3
      0x20080403,  //  0004  NE	R2	R2	R3
      0x780A0018,  //  0005  JMPF	R2	#001F
      0x60080010,  //  0006  GETGBL	R2	G16
      0x880C0100,  //  0007  GETMBR	R3	R0	K0
      0x8C0C0701,  //  0008  GETMET	R3	R3	K1
      0x7C0C0200,  //  0009  CALL	R3	1
      0x7C080200,  //  000A  CALL	R2	1
      0xA802000F,  //  000B  EXBLK	0	#001C
      0x5C0C0400,  //  000C  MOVE	R3	R2
      0x7C0C0000,  //  000D  CALL	R3	0
      0x88100100,  //  000E  GETMBR	R4	R0	K0
      0x94100803,  //  000F  GETIDX	R4	R4	R3
      0x8C100902,  //  0010  GETMET	R4	R4	K2
      0x7C100200,  //  0011  CALL	R4	1
      0x4C140000,  //  0012  LDNIL	R5
      0x20140805,  //  0013  NE	R5	R4	R5
      0x78160005,  //  0014  JMPF	R5	#001B
      0x6014000C,  //  0015  GETGBL	R5	G12
      0x5C180800,  //  0016  MOVE	R6	R4
      0x7C140200,  //  0017  CALL	R5	1
      0x24140B03,  //  0018  GT	R5	R5	K3
      0x78160000,  //  0019  JMPF	R5	#001B
      0x98040604,  //  001A  SETIDX	R1	R3	R4
      0x7001FFEF,  //  001B  JMP		#000C
      0x58080004,  //  001C  LDCONST	R2	K4
      0xAC080200,  //  001D  CATCH	R2	1	0
      0xB0080000,  //  001E  RAISE	2	R0	R0
      0x90020A01,  //  001F  SETMBR	R0	K5	R1
      0x80040200,  //  0020  RET	1	R1
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
** Solidified function: start_root_basic_commissioning
********************************************************************/
be_local_closure(Matter_Device_start_root_basic_commissioning,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[18]) {     /* constants */
    /* K0   */  be_nested_str_weak(PASE_TIMEOUT),
    /* K1   */  be_nested_str_weak(compute_manual_pairing_code),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(log),
    /* K4   */  be_nested_str_weak(MTR_X3A_X20Manual_X20pairing_X20code_X3A_X20_X25s),
    /* K5   */  be_const_int(2),
    /* K6   */  be_nested_str_weak(compute_qrcode_content),
    /* K7   */  be_nested_str_weak(publish_result),
    /* K8   */  be_nested_str_weak(_X7B_X22Matter_X22_X3A_X7B_X22Commissioning_X22_X3A1_X2C_X22PairingCode_X22_X3A_X22_X25s_X22_X2C_X22QRCode_X22_X3A_X22_X25s_X22_X7D_X7D),
    /* K9   */  be_nested_str_weak(Matter),
    /* K10  */  be_nested_str_weak(_compute_pbkdf),
    /* K11  */  be_nested_str_weak(root_passcode),
    /* K12  */  be_nested_str_weak(root_iterations),
    /* K13  */  be_nested_str_weak(root_salt),
    /* K14  */  be_nested_str_weak(start_basic_commissioning),
    /* K15  */  be_nested_str_weak(root_discriminator),
    /* K16  */  be_nested_str_weak(root_w0),
    /* K17  */  be_nested_str_weak(root_L),
    }),
    be_str_weak(start_root_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x88040100,  //  0003  GETMBR	R1	R0	K0
      0x8C080101,  //  0004  GETMET	R2	R0	K1
      0x7C080200,  //  0005  CALL	R2	1
      0xB80E0400,  //  0006  GETNGBL	R3	K2
      0x8C0C0703,  //  0007  GETMET	R3	R3	K3
      0x60140018,  //  0008  GETGBL	R5	G24
      0x58180004,  //  0009  LDCONST	R6	K4
      0x5C1C0400,  //  000A  MOVE	R7	R2
      0x7C140400,  //  000B  CALL	R5	2
      0x58180005,  //  000C  LDCONST	R6	K5
      0x7C0C0600,  //  000D  CALL	R3	3
      0x8C0C0106,  //  000E  GETMET	R3	R0	K6
      0x7C0C0200,  //  000F  CALL	R3	1
      0xB8120400,  //  0010  GETNGBL	R4	K2
      0x8C100907,  //  0011  GETMET	R4	R4	K7
      0x60180018,  //  0012  GETGBL	R6	G24
      0x581C0008,  //  0013  LDCONST	R7	K8
      0x5C200400,  //  0014  MOVE	R8	R2
      0x5C240600,  //  0015  MOVE	R9	R3
      0x7C180600,  //  0016  CALL	R6	3
      0x581C0009,  //  0017  LDCONST	R7	K9
      0x7C100600,  //  0018  CALL	R4	3
      0x8C10010A,  //  0019  GETMET	R4	R0	K10
      0x8818010B,  //  001A  GETMBR	R6	R0	K11
      0x881C010C,  //  001B  GETMBR	R7	R0	K12
      0x8820010D,  //  001C  GETMBR	R8	R0	K13
      0x7C100800,  //  001D  CALL	R4	4
      0x8C10010E,  //  001E  GETMET	R4	R0	K14
      0x5C180200,  //  001F  MOVE	R6	R1
      0x881C010C,  //  0020  GETMBR	R7	R0	K12
      0x8820010F,  //  0021  GETMBR	R8	R0	K15
      0x8824010D,  //  0022  GETMBR	R9	R0	K13
      0x88280110,  //  0023  GETMBR	R10	R0	K16
      0x882C0111,  //  0024  GETMBR	R11	R0	K17
      0x4C300000,  //  0025  LDNIL	R12
      0x7C101000,  //  0026  CALL	R4	8
      0x80000000,  //  0027  RET	0
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
    ( &(const struct bproto*[ 3]) {
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
        /* K0   */  be_nested_str_weak(MtrUpdate),
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
        /* K0   */  be_nested_str_weak(MtrInfo),
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
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(add_cmd),
    /* K2   */  be_nested_str_weak(MtrJoin),
    /* K3   */  be_nested_str_weak(MtrUpdate),
    /* K4   */  be_nested_str_weak(MtrInfo),
    }),
    be_str_weak(register_commands),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xB8060000,  //  0000  GETNGBL	R1	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x580C0002,  //  0002  LDCONST	R3	K2
      0x84100000,  //  0003  CLOSURE	R4	P0
      0x7C040600,  //  0004  CALL	R1	3
      0xB8060000,  //  0005  GETNGBL	R1	K0
      0x8C040301,  //  0006  GETMET	R1	R1	K1
      0x580C0003,  //  0007  LDCONST	R3	K3
      0x84100001,  //  0008  CLOSURE	R4	P1
      0x7C040600,  //  0009  CALL	R1	3
      0xB8060000,  //  000A  GETNGBL	R1	K0
      0x8C040301,  //  000B  GETMET	R1	R1	K1
      0x580C0004,  //  000C  LDCONST	R3	K4
      0x84100002,  //  000D  CLOSURE	R4	P2
      0x7C040600,  //  000E  CALL	R1	3
      0xA0000000,  //  000F  CLOSE	R0
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: MtrInfo_one
********************************************************************/
be_local_closure(Matter_Device_MtrInfo_one,   /* name */
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
    /* K0   */  be_nested_str_weak(find_plugin_by_endpoint),
    /* K1   */  be_nested_str_weak(state_json),
    /* K2   */  be_nested_str_weak(_X7B_X22MtrInfo_X22_X3A_X25s_X7D),
    /* K3   */  be_nested_str_weak(tasmota),
    /* K4   */  be_nested_str_weak(publish_result),
    /* K5   */  be_nested_str_weak(),
    }),
    be_str_weak(MtrInfo_one),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0403,  //  0004  EQ	R3	R2	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x80000600,  //  0006  RET	0
      0x8C0C0501,  //  0007  GETMET	R3	R2	K1
      0x7C0C0200,  //  0008  CALL	R3	1
      0x780E0008,  //  0009  JMPF	R3	#0013
      0x60100018,  //  000A  GETGBL	R4	G24
      0x58140002,  //  000B  LDCONST	R5	K2
      0x5C180600,  //  000C  MOVE	R6	R3
      0x7C100400,  //  000D  CALL	R4	2
      0xB8160600,  //  000E  GETNGBL	R5	K3
      0x8C140B04,  //  000F  GETMET	R5	R5	K4
      0x5C1C0800,  //  0010  MOVE	R7	R4
      0x58200005,  //  0011  LDCONST	R8	K5
      0x7C140600,  //  0012  CALL	R5	3
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_plugin_by_friendly_name
********************************************************************/
be_local_closure(Matter_Device_find_plugin_by_friendly_name,   /* name */
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
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(plugins),
    /* K2   */  be_nested_str_weak(get_name),
    /* K3   */  be_const_int(1),
    }),
    be_str_weak(find_plugin_by_friendly_name),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x740A0004,  //  0002  JMPT	R2	#0008
      0x6008000C,  //  0003  GETGBL	R2	G12
      0x5C0C0200,  //  0004  MOVE	R3	R1
      0x7C080200,  //  0005  CALL	R2	1
      0x1C080500,  //  0006  EQ	R2	R2	K0
      0x780A0001,  //  0007  JMPF	R2	#000A
      0x4C080000,  //  0008  LDNIL	R2
      0x80040400,  //  0009  RET	1	R2
      0x58080000,  //  000A  LDCONST	R2	K0
      0x600C000C,  //  000B  GETGBL	R3	G12
      0x88100101,  //  000C  GETMBR	R4	R0	K1
      0x7C0C0200,  //  000D  CALL	R3	1
      0x140C0403,  //  000E  LT	R3	R2	R3
      0x780E0010,  //  000F  JMPF	R3	#0021
      0x880C0101,  //  0010  GETMBR	R3	R0	K1
      0x940C0602,  //  0011  GETIDX	R3	R3	R2
      0x8C100702,  //  0012  GETMET	R4	R3	K2
      0x7C100200,  //  0013  CALL	R4	1
      0x4C140000,  //  0014  LDNIL	R5
      0x20140805,  //  0015  NE	R5	R4	R5
      0x78160007,  //  0016  JMPF	R5	#001F
      0x6014000C,  //  0017  GETGBL	R5	G12
      0x5C180800,  //  0018  MOVE	R6	R4
      0x7C140200,  //  0019  CALL	R5	1
      0x24140B00,  //  001A  GT	R5	R5	K0
      0x78160002,  //  001B  JMPF	R5	#001F
      0x1C140801,  //  001C  EQ	R5	R4	R1
      0x78160000,  //  001D  JMPF	R5	#001F
      0x80040600,  //  001E  RET	1	R3
      0x00080503,  //  001F  ADD	R2	R2	K3
      0x7001FFE9,  //  0020  JMP		#000B
      0x4C0C0000,  //  0021  LDNIL	R3
      0x80040600,  //  0022  RET	1	R3
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
** Solidified function: load_param
********************************************************************/
be_local_closure(Matter_Device_load_param,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[35]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(FILENAME),
    /* K2   */  be_nested_str_weak(read),
    /* K3   */  be_nested_str_weak(close),
    /* K4   */  be_nested_str_weak(json),
    /* K5   */  be_nested_str_weak(load),
    /* K6   */  be_nested_str_weak(root_discriminator),
    /* K7   */  be_nested_str_weak(find),
    /* K8   */  be_nested_str_weak(distinguish),
    /* K9   */  be_nested_str_weak(root_passcode),
    /* K10  */  be_nested_str_weak(passcode),
    /* K11  */  be_nested_str_weak(ipv4only),
    /* K12  */  be_nested_str_weak(disable_bridge_mode),
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
    /* K23  */  be_nested_str_weak(plugins_config_remotes),
    /* K24  */  be_nested_str_weak(remotes),
    /* K25  */  be_nested_str_weak(MTR_X3A_X20load_remotes_X20_X3D_X20),
    /* K26  */  be_nested_str_weak(io_error),
    /* K27  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Aload_X20Exception_X3A),
    /* K28  */  be_nested_str_weak(_X7C),
    /* K29  */  be_const_int(2),
    /* K30  */  be_nested_str_weak(random),
    /* K31  */  be_nested_str_weak(get),
    /* K32  */  be_const_int(0),
    /* K33  */  be_nested_str_weak(generate_random_passcode),
    /* K34  */  be_nested_str_weak(save_param),
    }),
    be_str_weak(load_param),
    &be_const_str_solidified,
    ( &(const binstruction[127]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA802004D,  //  0001  EXBLK	0	#0050
      0x60080011,  //  0002  GETGBL	R2	G17
      0x880C0101,  //  0003  GETMBR	R3	R0	K1
      0x7C080200,  //  0004  CALL	R2	1
      0x8C0C0502,  //  0005  GETMET	R3	R2	K2
      0x7C0C0200,  //  0006  CALL	R3	1
      0x8C100503,  //  0007  GETMET	R4	R2	K3
      0x7C100200,  //  0008  CALL	R4	1
      0xA4120800,  //  0009  IMPORT	R4	K4
      0x8C140905,  //  000A  GETMET	R5	R4	K5
      0x5C1C0600,  //  000B  MOVE	R7	R3
      0x7C140400,  //  000C  CALL	R5	2
      0x8C180B07,  //  000D  GETMET	R6	R5	K7
      0x58200008,  //  000E  LDCONST	R8	K8
      0x88240106,  //  000F  GETMBR	R9	R0	K6
      0x7C180600,  //  0010  CALL	R6	3
      0x90020C06,  //  0011  SETMBR	R0	K6	R6
      0x8C180B07,  //  0012  GETMET	R6	R5	K7
      0x5820000A,  //  0013  LDCONST	R8	K10
      0x88240109,  //  0014  GETMBR	R9	R0	K9
      0x7C180600,  //  0015  CALL	R6	3
      0x90021206,  //  0016  SETMBR	R0	K9	R6
      0x60180017,  //  0017  GETGBL	R6	G23
      0x8C1C0B07,  //  0018  GETMET	R7	R5	K7
      0x5824000B,  //  0019  LDCONST	R9	K11
      0x50280000,  //  001A  LDBOOL	R10	0	0
      0x7C1C0600,  //  001B  CALL	R7	3
      0x7C180200,  //  001C  CALL	R6	1
      0x90021606,  //  001D  SETMBR	R0	K11	R6
      0x60180017,  //  001E  GETGBL	R6	G23
      0x8C1C0B07,  //  001F  GETMET	R7	R5	K7
      0x5824000C,  //  0020  LDCONST	R9	K12
      0x50280000,  //  0021  LDBOOL	R10	0	0
      0x7C1C0600,  //  0022  CALL	R7	3
      0x7C180200,  //  0023  CALL	R6	1
      0x90021806,  //  0024  SETMBR	R0	K12	R6
      0x8C180B07,  //  0025  GETMET	R6	R5	K7
      0x5820000E,  //  0026  LDCONST	R8	K14
      0x8824010D,  //  0027  GETMBR	R9	R0	K13
      0x7C180600,  //  0028  CALL	R6	3
      0x90021A06,  //  0029  SETMBR	R0	K13	R6
      0x8C180B07,  //  002A  GETMET	R6	R5	K7
      0x58200010,  //  002B  LDCONST	R8	K16
      0x7C180400,  //  002C  CALL	R6	2
      0x90021E06,  //  002D  SETMBR	R0	K15	R6
      0x8818010F,  //  002E  GETMBR	R6	R0	K15
      0x4C1C0000,  //  002F  LDNIL	R7
      0x20180C07,  //  0030  NE	R6	R6	R7
      0x781A000B,  //  0031  JMPF	R6	#003E
      0xB81A2200,  //  0032  GETNGBL	R6	K17
      0x8C180D12,  //  0033  GETMET	R6	R6	K18
      0x60200008,  //  0034  GETGBL	R8	G8
      0x8824010F,  //  0035  GETMBR	R9	R0	K15
      0x7C200200,  //  0036  CALL	R8	1
      0x00222608,  //  0037  ADD	R8	K19	R8
      0x58240014,  //  0038  LDCONST	R9	K20
      0x7C180600,  //  0039  CALL	R6	3
      0x8C180115,  //  003A  GETMET	R6	R0	K21
      0x7C180200,  //  003B  CALL	R6	1
      0x50180200,  //  003C  LDBOOL	R6	1	0
      0x90022C06,  //  003D  SETMBR	R0	K22	R6
      0x8C180B07,  //  003E  GETMET	R6	R5	K7
      0x58200018,  //  003F  LDCONST	R8	K24
      0x60240013,  //  0040  GETGBL	R9	G19
      0x7C240000,  //  0041  CALL	R9	0
      0x7C180600,  //  0042  CALL	R6	3
      0x90022E06,  //  0043  SETMBR	R0	K23	R6
      0x88180117,  //  0044  GETMBR	R6	R0	K23
      0x781A0007,  //  0045  JMPF	R6	#004E
      0xB81A2200,  //  0046  GETNGBL	R6	K17
      0x8C180D12,  //  0047  GETMET	R6	R6	K18
      0x60200008,  //  0048  GETGBL	R8	G8
      0x88240117,  //  0049  GETMBR	R9	R0	K23
      0x7C200200,  //  004A  CALL	R8	1
      0x00223208,  //  004B  ADD	R8	K25	R8
      0x58240014,  //  004C  LDCONST	R9	K20
      0x7C180600,  //  004D  CALL	R6	3
      0xA8040001,  //  004E  EXBLK	1	1
      0x70020012,  //  004F  JMP		#0063
      0xAC080002,  //  0050  CATCH	R2	0	2
      0x7002000F,  //  0051  JMP		#0062
      0x2010051A,  //  0052  NE	R4	R2	K26
      0x7812000C,  //  0053  JMPF	R4	#0061
      0xB8122200,  //  0054  GETNGBL	R4	K17
      0x8C100912,  //  0055  GETMET	R4	R4	K18
      0x60180008,  //  0056  GETGBL	R6	G8
      0x5C1C0400,  //  0057  MOVE	R7	R2
      0x7C180200,  //  0058  CALL	R6	1
      0x001A3606,  //  0059  ADD	R6	K27	R6
      0x00180D1C,  //  005A  ADD	R6	R6	K28
      0x601C0008,  //  005B  GETGBL	R7	G8
      0x5C200600,  //  005C  MOVE	R8	R3
      0x7C1C0200,  //  005D  CALL	R7	1
      0x00180C07,  //  005E  ADD	R6	R6	R7
      0x581C001D,  //  005F  LDCONST	R7	K29
      0x7C100600,  //  0060  CALL	R4	3
      0x70020000,  //  0061  JMP		#0063
      0xB0080000,  //  0062  RAISE	2	R0	R0
      0x50080000,  //  0063  LDBOOL	R2	0	0
      0x880C0106,  //  0064  GETMBR	R3	R0	K6
      0x4C100000,  //  0065  LDNIL	R4
      0x1C0C0604,  //  0066  EQ	R3	R3	R4
      0x780E000A,  //  0067  JMPF	R3	#0073
      0x8C0C031E,  //  0068  GETMET	R3	R1	K30
      0x5814001D,  //  0069  LDCONST	R5	K29
      0x7C0C0400,  //  006A  CALL	R3	2
      0x8C0C071F,  //  006B  GETMET	R3	R3	K31
      0x58140020,  //  006C  LDCONST	R5	K32
      0x5818001D,  //  006D  LDCONST	R6	K29
      0x7C0C0600,  //  006E  CALL	R3	3
      0x54120FFE,  //  006F  LDINT	R4	4095
      0x2C0C0604,  //  0070  AND	R3	R3	R4
      0x90020C03,  //  0071  SETMBR	R0	K6	R3
      0x50080200,  //  0072  LDBOOL	R2	1	0
      0x880C0109,  //  0073  GETMBR	R3	R0	K9
      0x4C100000,  //  0074  LDNIL	R4
      0x1C0C0604,  //  0075  EQ	R3	R3	R4
      0x780E0003,  //  0076  JMPF	R3	#007B
      0x8C0C0121,  //  0077  GETMET	R3	R0	K33
      0x7C0C0200,  //  0078  CALL	R3	1
      0x90021203,  //  0079  SETMBR	R0	K9	R3
      0x50080200,  //  007A  LDBOOL	R2	1	0
      0x780A0001,  //  007B  JMPF	R2	#007E
      0x8C0C0122,  //  007C  GETMET	R3	R0	K34
      0x7C0C0200,  //  007D  CALL	R3	1
      0x80000000,  //  007E  RET	0
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
** Solidified function: process_attribute_expansion
********************************************************************/
be_local_closure(Matter_Device_process_attribute_expansion,   /* name */
  be_nested_proto(
    24,                          /* nstack */
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
    ( &(const bvalue[16]) {     /* constants */
    /* K0   */  be_nested_str_weak(endpoint),
    /* K1   */  be_nested_str_weak(cluster),
    /* K2   */  be_nested_str_weak(attribute),
    /* K3   */  be_nested_str_weak(plugins),
    /* K4   */  be_nested_str_weak(get_endpoint),
    /* K5   */  be_nested_str_weak(contains),
    /* K6   */  be_nested_str_weak(get_cluster_list),
    /* K7   */  be_nested_str_weak(get_attribute_list),
    /* K8   */  be_nested_str_weak(push),
    /* K9   */  be_nested_str_weak(stop_iteration),
    /* K10  */  be_nested_str_weak(status),
    /* K11  */  be_nested_str_weak(matter),
    /* K12  */  be_nested_str_weak(UNSUPPORTED_ENDPOINT),
    /* K13  */  be_nested_str_weak(UNSUPPORTED_CLUSTER),
    /* K14  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K15  */  be_nested_str_weak(UNREPORTABLE_ATTRIBUTE),
    }),
    be_str_weak(process_attribute_expansion),
    &be_const_str_solidified,
    ( &(const binstruction[203]) {  /* code */
      0x840C0000,  //  0000  CLOSURE	R3	P0
      0x88100300,  //  0001  GETMBR	R4	R1	K0
      0x88140301,  //  0002  GETMBR	R5	R1	K1
      0x88180302,  //  0003  GETMBR	R6	R1	K2
      0x501C0000,  //  0004  LDBOOL	R7	0	0
      0x50200000,  //  0005  LDBOOL	R8	0	0
      0x50240000,  //  0006  LDBOOL	R9	0	0
      0x88280300,  //  0007  GETMBR	R10	R1	K0
      0x4C2C0000,  //  0008  LDNIL	R11
      0x2028140B,  //  0009  NE	R10	R10	R11
      0x782A0007,  //  000A  JMPF	R10	#0013
      0x88280301,  //  000B  GETMBR	R10	R1	K1
      0x4C2C0000,  //  000C  LDNIL	R11
      0x2028140B,  //  000D  NE	R10	R10	R11
      0x782A0003,  //  000E  JMPF	R10	#0013
      0x88280302,  //  000F  GETMBR	R10	R1	K2
      0x4C2C0000,  //  0010  LDNIL	R11
      0x2028140B,  //  0011  NE	R10	R10	R11
      0x742A0000,  //  0012  JMPT	R10	#0014
      0x50280001,  //  0013  LDBOOL	R10	0	1
      0x50280200,  //  0014  LDBOOL	R10	1	0
      0x602C0013,  //  0015  GETGBL	R11	G19
      0x7C2C0000,  //  0016  CALL	R11	0
      0x60300010,  //  0017  GETGBL	R12	G16
      0x88340103,  //  0018  GETMBR	R13	R0	K3
      0x7C300200,  //  0019  CALL	R12	1
      0xA8020053,  //  001A  EXBLK	0	#006F
      0x5C341800,  //  001B  MOVE	R13	R12
      0x7C340000,  //  001C  CALL	R13	0
      0x8C381B04,  //  001D  GETMET	R14	R13	K4
      0x7C380200,  //  001E  CALL	R14	1
      0x4C3C0000,  //  001F  LDNIL	R15
      0x203C080F,  //  0020  NE	R15	R4	R15
      0x783E0002,  //  0021  JMPF	R15	#0025
      0x203C1C04,  //  0022  NE	R15	R14	R4
      0x783E0000,  //  0023  JMPF	R15	#0025
      0x7001FFF5,  //  0024  JMP		#001B
      0x8C3C1705,  //  0025  GETMET	R15	R11	K5
      0x5C441C00,  //  0026  MOVE	R17	R14
      0x7C3C0400,  //  0027  CALL	R15	2
      0x743E0002,  //  0028  JMPT	R15	#002C
      0x603C0013,  //  0029  GETGBL	R15	G19
      0x7C3C0000,  //  002A  CALL	R15	0
      0x982C1C0F,  //  002B  SETIDX	R11	R14	R15
      0x501C0200,  //  002C  LDBOOL	R7	1	0
      0x8C3C1B06,  //  002D  GETMET	R15	R13	K6
      0x7C3C0200,  //  002E  CALL	R15	1
      0x60400010,  //  002F  GETGBL	R16	G16
      0x5C441E00,  //  0030  MOVE	R17	R15
      0x7C400200,  //  0031  CALL	R16	1
      0xA8020037,  //  0032  EXBLK	0	#006B
      0x5C442000,  //  0033  MOVE	R17	R16
      0x7C440000,  //  0034  CALL	R17	0
      0x4C480000,  //  0035  LDNIL	R18
      0x20480A12,  //  0036  NE	R18	R5	R18
      0x784A0002,  //  0037  JMPF	R18	#003B
      0x20482205,  //  0038  NE	R18	R17	R5
      0x784A0000,  //  0039  JMPF	R18	#003B
      0x7001FFF7,  //  003A  JMP		#0033
      0x9448160E,  //  003B  GETIDX	R18	R11	R14
      0x8C482505,  //  003C  GETMET	R18	R18	K5
      0x5C502200,  //  003D  MOVE	R20	R17
      0x7C480400,  //  003E  CALL	R18	2
      0x744A0003,  //  003F  JMPT	R18	#0044
      0x9448160E,  //  0040  GETIDX	R18	R11	R14
      0x604C0013,  //  0041  GETGBL	R19	G19
      0x7C4C0000,  //  0042  CALL	R19	0
      0x98482213,  //  0043  SETIDX	R18	R17	R19
      0x50200200,  //  0044  LDBOOL	R8	1	0
      0x8C481B07,  //  0045  GETMET	R18	R13	K7
      0x5C502200,  //  0046  MOVE	R20	R17
      0x7C480400,  //  0047  CALL	R18	2
      0x604C0010,  //  0048  GETGBL	R19	G16
      0x5C502400,  //  0049  MOVE	R20	R18
      0x7C4C0200,  //  004A  CALL	R19	1
      0xA802001A,  //  004B  EXBLK	0	#0067
      0x5C502600,  //  004C  MOVE	R20	R19
      0x7C500000,  //  004D  CALL	R20	0
      0x4C540000,  //  004E  LDNIL	R21
      0x20540C15,  //  004F  NE	R21	R6	R21
      0x78560002,  //  0050  JMPF	R21	#0054
      0x20542806,  //  0051  NE	R21	R20	R6
      0x78560000,  //  0052  JMPF	R21	#0054
      0x7001FFF7,  //  0053  JMP		#004C
      0x9454160E,  //  0054  GETIDX	R21	R11	R14
      0x94542A11,  //  0055  GETIDX	R21	R21	R17
      0x8C542B05,  //  0056  GETMET	R21	R21	K5
      0x5C5C2800,  //  0057  MOVE	R23	R20
      0x7C540400,  //  0058  CALL	R21	2
      0x74560004,  //  0059  JMPT	R21	#005F
      0x9454160E,  //  005A  GETIDX	R21	R11	R14
      0x94542A11,  //  005B  GETIDX	R21	R21	R17
      0x60580012,  //  005C  GETGBL	R22	G18
      0x7C580000,  //  005D  CALL	R22	0
      0x98542816,  //  005E  SETIDX	R21	R20	R22
      0x50240200,  //  005F  LDBOOL	R9	1	0
      0x9454160E,  //  0060  GETIDX	R21	R11	R14
      0x94542A11,  //  0061  GETIDX	R21	R21	R17
      0x94542A14,  //  0062  GETIDX	R21	R21	R20
      0x8C542B08,  //  0063  GETMET	R21	R21	K8
      0x5C5C1A00,  //  0064  MOVE	R23	R13
      0x7C540400,  //  0065  CALL	R21	2
      0x7001FFE4,  //  0066  JMP		#004C
      0x584C0009,  //  0067  LDCONST	R19	K9
      0xAC4C0200,  //  0068  CATCH	R19	1	0
      0xB0080000,  //  0069  RAISE	2	R0	R0
      0x7001FFC7,  //  006A  JMP		#0033
      0x58400009,  //  006B  LDCONST	R16	K9
      0xAC400200,  //  006C  CATCH	R16	1	0
      0xB0080000,  //  006D  RAISE	2	R0	R0
      0x7001FFAB,  //  006E  JMP		#001B
      0x58300009,  //  006F  LDCONST	R12	K9
      0xAC300200,  //  0070  CATCH	R12	1	0
      0xB0080000,  //  0071  RAISE	2	R0	R0
      0x60300010,  //  0072  GETGBL	R12	G16
      0x5C340600,  //  0073  MOVE	R13	R3
      0x5C381600,  //  0074  MOVE	R14	R11
      0x7C340200,  //  0075  CALL	R13	1
      0x7C300200,  //  0076  CALL	R12	1
      0xA8020033,  //  0077  EXBLK	0	#00AC
      0x5C341800,  //  0078  MOVE	R13	R12
      0x7C340000,  //  0079  CALL	R13	0
      0x60380010,  //  007A  GETGBL	R14	G16
      0x5C3C0600,  //  007B  MOVE	R15	R3
      0x9440160D,  //  007C  GETIDX	R16	R11	R13
      0x7C3C0200,  //  007D  CALL	R15	1
      0x7C380200,  //  007E  CALL	R14	1
      0xA8020027,  //  007F  EXBLK	0	#00A8
      0x5C3C1C00,  //  0080  MOVE	R15	R14
      0x7C3C0000,  //  0081  CALL	R15	0
      0x60400010,  //  0082  GETGBL	R16	G16
      0x5C440600,  //  0083  MOVE	R17	R3
      0x9448160D,  //  0084  GETIDX	R18	R11	R13
      0x9448240F,  //  0085  GETIDX	R18	R18	R15
      0x7C440200,  //  0086  CALL	R17	1
      0x7C400200,  //  0087  CALL	R16	1
      0xA802001A,  //  0088  EXBLK	0	#00A4
      0x5C442000,  //  0089  MOVE	R17	R16
      0x7C440000,  //  008A  CALL	R17	0
      0x60480010,  //  008B  GETGBL	R18	G16
      0x944C160D,  //  008C  GETIDX	R19	R11	R13
      0x944C260F,  //  008D  GETIDX	R19	R19	R15
      0x944C2611,  //  008E  GETIDX	R19	R19	R17
      0x7C480200,  //  008F  CALL	R18	1
      0xA802000E,  //  0090  EXBLK	0	#00A0
      0x5C4C2400,  //  0091  MOVE	R19	R18
      0x7C4C0000,  //  0092  CALL	R19	0
      0x9006000D,  //  0093  SETMBR	R1	K0	R13
      0x9006020F,  //  0094  SETMBR	R1	K1	R15
      0x90060411,  //  0095  SETMBR	R1	K2	R17
      0x5C500400,  //  0096  MOVE	R20	R2
      0x5C542600,  //  0097  MOVE	R21	R19
      0x5C580200,  //  0098  MOVE	R22	R1
      0x5C5C1400,  //  0099  MOVE	R23	R10
      0x7C500600,  //  009A  CALL	R20	3
      0x782A0002,  //  009B  JMPF	R10	#009F
      0x78520001,  //  009C  JMPF	R20	#009F
      0xA8040004,  //  009D  EXBLK	1	4
      0x80002A00,  //  009E  RET	0
      0x7001FFF0,  //  009F  JMP		#0091
      0x58480009,  //  00A0  LDCONST	R18	K9
      0xAC480200,  //  00A1  CATCH	R18	1	0
      0xB0080000,  //  00A2  RAISE	2	R0	R0
      0x7001FFE4,  //  00A3  JMP		#0089
      0x58400009,  //  00A4  LDCONST	R16	K9
      0xAC400200,  //  00A5  CATCH	R16	1	0
      0xB0080000,  //  00A6  RAISE	2	R0	R0
      0x7001FFD7,  //  00A7  JMP		#0080
      0x58380009,  //  00A8  LDCONST	R14	K9
      0xAC380200,  //  00A9  CATCH	R14	1	0
      0xB0080000,  //  00AA  RAISE	2	R0	R0
      0x7001FFCB,  //  00AB  JMP		#0078
      0x58300009,  //  00AC  LDCONST	R12	K9
      0xAC300200,  //  00AD  CATCH	R12	1	0
      0xB0080000,  //  00AE  RAISE	2	R0	R0
      0x782A0019,  //  00AF  JMPF	R10	#00CA
      0x5C300E00,  //  00B0  MOVE	R12	R7
      0x74320003,  //  00B1  JMPT	R12	#00B6
      0xB8321600,  //  00B2  GETNGBL	R12	K11
      0x8830190C,  //  00B3  GETMBR	R12	R12	K12
      0x9006140C,  //  00B4  SETMBR	R1	K10	R12
      0x7002000E,  //  00B5  JMP		#00C5
      0x5C301000,  //  00B6  MOVE	R12	R8
      0x74320003,  //  00B7  JMPT	R12	#00BC
      0xB8321600,  //  00B8  GETNGBL	R12	K11
      0x8830190D,  //  00B9  GETMBR	R12	R12	K13
      0x9006140C,  //  00BA  SETMBR	R1	K10	R12
      0x70020008,  //  00BB  JMP		#00C5
      0x5C301200,  //  00BC  MOVE	R12	R9
      0x74320003,  //  00BD  JMPT	R12	#00C2
      0xB8321600,  //  00BE  GETNGBL	R12	K11
      0x8830190E,  //  00BF  GETMBR	R12	R12	K14
      0x9006140C,  //  00C0  SETMBR	R1	K10	R12
      0x70020002,  //  00C1  JMP		#00C5
      0xB8321600,  //  00C2  GETNGBL	R12	K11
      0x8830190F,  //  00C3  GETMBR	R12	R12	K15
      0x9006140C,  //  00C4  SETMBR	R1	K10	R12
      0x5C300400,  //  00C5  MOVE	R12	R2
      0x4C340000,  //  00C6  LDNIL	R13
      0x5C380200,  //  00C7  MOVE	R14	R1
      0x503C0200,  //  00C8  LDBOOL	R15	1	0
      0x7C300600,  //  00C9  CALL	R12	3
      0x80000000,  //  00CA  RET	0
    })
  )
);
/*******************************************************************/


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
** Solidified function: start_operational_discovery
********************************************************************/
be_local_closure(Matter_Device_start_operational_discovery,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(mdns),
    /* K2   */  be_nested_str_weak(stop_basic_commissioning),
    /* K3   */  be_nested_str_weak(root_w0),
    /* K4   */  be_nested_str_weak(root_L),
    /* K5   */  be_nested_str_weak(mdns_announce_op_discovery),
    }),
    be_str_weak(start_operational_discovery),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x8C100102,  //  0002  GETMET	R4	R0	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x4C100000,  //  0004  LDNIL	R4
      0x90020604,  //  0005  SETMBR	R0	K3	R4
      0x4C100000,  //  0006  LDNIL	R4
      0x90020804,  //  0007  SETMBR	R0	K4	R4
      0x8C100105,  //  0008  GETMET	R4	R0	K5
      0x5C180200,  //  0009  MOVE	R6	R1
      0x7C100400,  //  000A  CALL	R4	2
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: MtrInfo
********************************************************************/
be_local_closure(Matter_Device_MtrInfo,   /* name */
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
    /* K0   */  be_nested_str_weak(),
    /* K1   */  be_nested_str_weak(plugins),
    /* K2   */  be_nested_str_weak(MtrInfo_one),
    /* K3   */  be_nested_str_weak(endpoint),
    /* K4   */  be_nested_str_weak(stop_iteration),
    /* K5   */  be_nested_str_weak(int),
    /* K6   */  be_nested_str_weak(find_plugin_by_friendly_name),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(resp_cmnd_done),
    }),
    be_str_weak(MtrInfo),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x1C140700,  //  0000  EQ	R5	R3	K0
      0x7815FFFF,  //  0001  JMPF	R5	#0002
      0x1C140700,  //  0002  EQ	R5	R3	K0
      0x7816000D,  //  0003  JMPF	R5	#0012
      0x60140010,  //  0004  GETGBL	R5	G16
      0x88180101,  //  0005  GETMBR	R6	R0	K1
      0x7C140200,  //  0006  CALL	R5	1
      0xA8020005,  //  0007  EXBLK	0	#000E
      0x5C180A00,  //  0008  MOVE	R6	R5
      0x7C180000,  //  0009  CALL	R6	0
      0x8C1C0102,  //  000A  GETMET	R7	R0	K2
      0x88240D03,  //  000B  GETMBR	R9	R6	K3
      0x7C1C0400,  //  000C  CALL	R7	2
      0x7001FFF9,  //  000D  JMP		#0008
      0x58140004,  //  000E  LDCONST	R5	K4
      0xAC140200,  //  000F  CATCH	R5	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x70020011,  //  0011  JMP		#0024
      0x60140004,  //  0012  GETGBL	R5	G4
      0x5C180800,  //  0013  MOVE	R6	R4
      0x7C140200,  //  0014  CALL	R5	1
      0x1C140B05,  //  0015  EQ	R5	R5	K5
      0x78160003,  //  0016  JMPF	R5	#001B
      0x8C140102,  //  0017  GETMET	R5	R0	K2
      0x5C1C0800,  //  0018  MOVE	R7	R4
      0x7C140400,  //  0019  CALL	R5	2
      0x70020008,  //  001A  JMP		#0024
      0x8C140106,  //  001B  GETMET	R5	R0	K6
      0x5C1C0600,  //  001C  MOVE	R7	R3
      0x7C140400,  //  001D  CALL	R5	2
      0x4C180000,  //  001E  LDNIL	R6
      0x20180A06,  //  001F  NE	R6	R5	R6
      0x781A0002,  //  0020  JMPF	R6	#0024
      0x8C180102,  //  0021  GETMET	R6	R0	K2
      0x88200B03,  //  0022  GETMBR	R8	R5	K3
      0x7C180400,  //  0023  CALL	R6	2
      0xB8160E00,  //  0024  GETNGBL	R5	K7
      0x8C140B08,  //  0025  GETMET	R5	R5	K8
      0x7C140200,  //  0026  CALL	R5	1
      0x80000000,  //  0027  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_announce_op_discovery
********************************************************************/
be_local_closure(Matter_Device_mdns_announce_op_discovery,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[27]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(get_device_id),
    /* K2   */  be_nested_str_weak(copy),
    /* K3   */  be_nested_str_weak(reverse),
    /* K4   */  be_nested_str_weak(get_fabric_compressed),
    /* K5   */  be_nested_str_weak(tohex),
    /* K6   */  be_nested_str_weak(_X2D),
    /* K7   */  be_nested_str_weak(tasmota),
    /* K8   */  be_nested_str_weak(log),
    /* K9   */  be_nested_str_weak(MTR_X3A_X20Operational_X20Discovery_X20node_X20_X3D_X20),
    /* K10  */  be_const_int(3),
    /* K11  */  be_nested_str_weak(eth),
    /* K12  */  be_nested_str_weak(find),
    /* K13  */  be_nested_str_weak(up),
    /* K14  */  be_nested_str_weak(MTR_X3A_X20adding_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
    /* K15  */  be_nested_str_weak(hostname_eth),
    /* K16  */  be_nested_str_weak(add_service),
    /* K17  */  be_nested_str_weak(_matter),
    /* K18  */  be_nested_str_weak(_tcp),
    /* K19  */  be_nested_str_weak(_I),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20adding_X20subtype_X3A_X20),
    /* K21  */  be_nested_str_weak(add_subtype),
    /* K22  */  be_nested_str_weak(wifi),
    /* K23  */  be_nested_str_weak(hostname_wifi),
    /* K24  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K25  */  be_nested_str_weak(_X7C),
    /* K26  */  be_const_int(2),
    }),
    be_str_weak(mdns_announce_op_discovery),
    &be_const_str_solidified,
    ( &(const binstruction[121]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA8020064,  //  0001  EXBLK	0	#0067
      0x8C0C0301,  //  0002  GETMET	R3	R1	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8C0C0702,  //  0004  GETMET	R3	R3	K2
      0x7C0C0200,  //  0005  CALL	R3	1
      0x8C0C0703,  //  0006  GETMET	R3	R3	K3
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C100304,  //  0008  GETMET	R4	R1	K4
      0x7C100200,  //  0009  CALL	R4	1
      0x8C140905,  //  000A  GETMET	R5	R4	K5
      0x7C140200,  //  000B  CALL	R5	1
      0x00140B06,  //  000C  ADD	R5	R5	K6
      0x8C180705,  //  000D  GETMET	R6	R3	K5
      0x7C180200,  //  000E  CALL	R6	1
      0x00140A06,  //  000F  ADD	R5	R5	R6
      0xB81A0E00,  //  0010  GETNGBL	R6	K7
      0x8C180D08,  //  0011  GETMET	R6	R6	K8
      0x00221205,  //  0012  ADD	R8	K9	R5
      0x5824000A,  //  0013  LDCONST	R9	K10
      0x7C180600,  //  0014  CALL	R6	3
      0xB81A0E00,  //  0015  GETNGBL	R6	K7
      0x8C180D0B,  //  0016  GETMET	R6	R6	K11
      0x7C180200,  //  0017  CALL	R6	1
      0x8C180D0C,  //  0018  GETMET	R6	R6	K12
      0x5820000D,  //  0019  LDCONST	R8	K13
      0x7C180400,  //  001A  CALL	R6	2
      0x781A0020,  //  001B  JMPF	R6	#003D
      0xB81A0E00,  //  001C  GETNGBL	R6	K7
      0x8C180D08,  //  001D  GETMET	R6	R6	K8
      0x60200018,  //  001E  GETGBL	R8	G24
      0x5824000E,  //  001F  LDCONST	R9	K14
      0x5828000B,  //  0020  LDCONST	R10	K11
      0x5C2C0A00,  //  0021  MOVE	R11	R5
      0x8830010F,  //  0022  GETMBR	R12	R0	K15
      0x7C200800,  //  0023  CALL	R8	4
      0x5824000A,  //  0024  LDCONST	R9	K10
      0x7C180600,  //  0025  CALL	R6	3
      0x8C180510,  //  0026  GETMET	R6	R2	K16
      0x58200011,  //  0027  LDCONST	R8	K17
      0x58240012,  //  0028  LDCONST	R9	K18
      0x542A15A3,  //  0029  LDINT	R10	5540
      0x4C2C0000,  //  002A  LDNIL	R11
      0x5C300A00,  //  002B  MOVE	R12	R5
      0x8834010F,  //  002C  GETMBR	R13	R0	K15
      0x7C180E00,  //  002D  CALL	R6	7
      0x8C180905,  //  002E  GETMET	R6	R4	K5
      0x7C180200,  //  002F  CALL	R6	1
      0x001A2606,  //  0030  ADD	R6	K19	R6
      0xB81E0E00,  //  0031  GETNGBL	R7	K7
      0x8C1C0F08,  //  0032  GETMET	R7	R7	K8
      0x00262806,  //  0033  ADD	R9	K20	R6
      0x5828000A,  //  0034  LDCONST	R10	K10
      0x7C1C0600,  //  0035  CALL	R7	3
      0x8C1C0515,  //  0036  GETMET	R7	R2	K21
      0x58240011,  //  0037  LDCONST	R9	K17
      0x58280012,  //  0038  LDCONST	R10	K18
      0x5C2C0A00,  //  0039  MOVE	R11	R5
      0x8830010F,  //  003A  GETMBR	R12	R0	K15
      0x5C340C00,  //  003B  MOVE	R13	R6
      0x7C1C0C00,  //  003C  CALL	R7	6
      0xB81A0E00,  //  003D  GETNGBL	R6	K7
      0x8C180D16,  //  003E  GETMET	R6	R6	K22
      0x7C180200,  //  003F  CALL	R6	1
      0x8C180D0C,  //  0040  GETMET	R6	R6	K12
      0x5820000D,  //  0041  LDCONST	R8	K13
      0x7C180400,  //  0042  CALL	R6	2
      0x781A0020,  //  0043  JMPF	R6	#0065
      0xB81A0E00,  //  0044  GETNGBL	R6	K7
      0x8C180D08,  //  0045  GETMET	R6	R6	K8
      0x60200018,  //  0046  GETGBL	R8	G24
      0x5824000E,  //  0047  LDCONST	R9	K14
      0x58280016,  //  0048  LDCONST	R10	K22
      0x5C2C0A00,  //  0049  MOVE	R11	R5
      0x88300117,  //  004A  GETMBR	R12	R0	K23
      0x7C200800,  //  004B  CALL	R8	4
      0x5824000A,  //  004C  LDCONST	R9	K10
      0x7C180600,  //  004D  CALL	R6	3
      0x8C180510,  //  004E  GETMET	R6	R2	K16
      0x58200011,  //  004F  LDCONST	R8	K17
      0x58240012,  //  0050  LDCONST	R9	K18
      0x542A15A3,  //  0051  LDINT	R10	5540
      0x4C2C0000,  //  0052  LDNIL	R11
      0x5C300A00,  //  0053  MOVE	R12	R5
      0x88340117,  //  0054  GETMBR	R13	R0	K23
      0x7C180E00,  //  0055  CALL	R6	7
      0x8C180905,  //  0056  GETMET	R6	R4	K5
      0x7C180200,  //  0057  CALL	R6	1
      0x001A2606,  //  0058  ADD	R6	K19	R6
      0xB81E0E00,  //  0059  GETNGBL	R7	K7
      0x8C1C0F08,  //  005A  GETMET	R7	R7	K8
      0x00262806,  //  005B  ADD	R9	K20	R6
      0x5828000A,  //  005C  LDCONST	R10	K10
      0x7C1C0600,  //  005D  CALL	R7	3
      0x8C1C0515,  //  005E  GETMET	R7	R2	K21
      0x58240011,  //  005F  LDCONST	R9	K17
      0x58280012,  //  0060  LDCONST	R10	K18
      0x5C2C0A00,  //  0061  MOVE	R11	R5
      0x88300117,  //  0062  GETMBR	R12	R0	K23
      0x5C340C00,  //  0063  MOVE	R13	R6
      0x7C1C0C00,  //  0064  CALL	R7	6
      0xA8040001,  //  0065  EXBLK	1	1
      0x70020010,  //  0066  JMP		#0078
      0xAC0C0002,  //  0067  CATCH	R3	0	2
      0x7002000D,  //  0068  JMP		#0077
      0xB8160E00,  //  0069  GETNGBL	R5	K7
      0x8C140B08,  //  006A  GETMET	R5	R5	K8
      0x601C0008,  //  006B  GETGBL	R7	G8
      0x5C200600,  //  006C  MOVE	R8	R3
      0x7C1C0200,  //  006D  CALL	R7	1
      0x001E3007,  //  006E  ADD	R7	K24	R7
      0x001C0F19,  //  006F  ADD	R7	R7	K25
      0x60200008,  //  0070  GETGBL	R8	G8
      0x5C240800,  //  0071  MOVE	R9	R4
      0x7C200200,  //  0072  CALL	R8	1
      0x001C0E08,  //  0073  ADD	R7	R7	R8
      0x5820001A,  //  0074  LDCONST	R8	K26
      0x7C140600,  //  0075  CALL	R5	3
      0x70020000,  //  0076  JMP		#0078
      0xB0080000,  //  0077  RAISE	2	R0	R0
      0x80000000,  //  0078  RET	0
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
** Solidified function: start_commissioning_complete
********************************************************************/
be_local_closure(Matter_Device_start_commissioning_complete,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[11]) {     /* constants */
    /* K0   */  be_nested_str_weak(get_fabric),
    /* K1   */  be_nested_str_weak(get_fabric_id),
    /* K2   */  be_nested_str_weak(copy),
    /* K3   */  be_nested_str_weak(reverse),
    /* K4   */  be_nested_str_weak(tohex),
    /* K5   */  be_nested_str_weak(get_admin_vendor_name),
    /* K6   */  be_nested_str_weak(tasmota),
    /* K7   */  be_nested_str_weak(log),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20_X2D_X2D_X2D_X20Commissioning_X20complete_X20for_X20Fabric_X20_X27_X25s_X27_X20_X28Vendor_X20_X25s_X29_X20_X2D_X2D_X2D),
    /* K9   */  be_const_int(2),
    /* K10  */  be_nested_str_weak(stop_basic_commissioning),
    }),
    be_str_weak(start_commissioning_complete),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x8C080300,  //  0000  GETMET	R2	R1	K0
      0x7C080200,  //  0001  CALL	R2	1
      0x8C0C0501,  //  0002  GETMET	R3	R2	K1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8C0C0702,  //  0004  GETMET	R3	R3	K2
      0x7C0C0200,  //  0005  CALL	R3	1
      0x8C0C0703,  //  0006  GETMET	R3	R3	K3
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C0C0704,  //  0008  GETMET	R3	R3	K4
      0x7C0C0200,  //  0009  CALL	R3	1
      0x8C100505,  //  000A  GETMET	R4	R2	K5
      0x7C100200,  //  000B  CALL	R4	1
      0xB8160C00,  //  000C  GETNGBL	R5	K6
      0x8C140B07,  //  000D  GETMET	R5	R5	K7
      0x601C0018,  //  000E  GETGBL	R7	G24
      0x58200008,  //  000F  LDCONST	R8	K8
      0x5C240600,  //  0010  MOVE	R9	R3
      0x5C280800,  //  0011  MOVE	R10	R4
      0x7C1C0600,  //  0012  CALL	R7	3
      0x58200009,  //  0013  LDCONST	R8	K9
      0x7C140600,  //  0014  CALL	R5	3
      0x8C14010A,  //  0015  GETMET	R5	R0	K10
      0x7C140200,  //  0016  CALL	R5	1
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: MtrUpdate
********************************************************************/
be_local_closure(Matter_Device_MtrUpdate,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[25]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(resp_cmnd_str),
    /* K2   */  be_nested_str_weak(Invalid_X20JSON),
    /* K3   */  be_nested_str_weak(find_key_i),
    /* K4   */  be_nested_str_weak(Ep),
    /* K5   */  be_nested_str_weak(Name),
    /* K6   */  be_const_int(0),
    /* K7   */  be_nested_str_weak(Invalid_X20_X27Ep_X27_X20attribute),
    /* K8   */  be_nested_str_weak(find_plugin_by_endpoint),
    /* K9   */  be_nested_str_weak(remove),
    /* K10  */  be_nested_str_weak(find_plugin_by_friendly_name),
    /* K11  */  be_nested_str_weak(Invalid_X20Device),
    /* K12  */  be_nested_str_weak(VIRTUAL),
    /* K13  */  be_nested_str_weak(Device_X20is_X20not_X20virtual),
    /* K14  */  be_nested_str_weak(consolidate_update_commands),
    /* K15  */  be_nested_str_weak(keys),
    /* K16  */  be_nested_str_weak(find_list_i),
    /* K17  */  be_nested_str_weak(Invalid_X20attribute_X20_X27_X25s_X27),
    /* K18  */  be_nested_str_weak(stop_iteration),
    /* K19  */  be_nested_str_weak(update_virtual),
    /* K20  */  be_nested_str_weak(state_json),
    /* K21  */  be_nested_str_weak(_X7B_X22_X25s_X22_X3A_X25s_X7D),
    /* K22  */  be_nested_str_weak(resp_cmnd),
    /* K23  */  be_nested_str_weak(resp_cmnd_done),
    /* K24  */  be_nested_str_weak(Missing_X20_X27Device_X27_X20attribute),
    }),
    be_str_weak(MtrUpdate),
    &be_const_str_solidified,
    ( &(const binstruction[126]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140805,  //  0001  EQ	R5	R4	R5
      0x78160004,  //  0002  JMPF	R5	#0008
      0xB8160000,  //  0003  GETNGBL	R5	K0
      0x8C140B01,  //  0004  GETMET	R5	R5	K1
      0x581C0002,  //  0005  LDCONST	R7	K2
      0x7C140400,  //  0006  CALL	R5	2
      0x80040A00,  //  0007  RET	1	R5
      0xB8160000,  //  0008  GETNGBL	R5	K0
      0x8C140B03,  //  0009  GETMET	R5	R5	K3
      0x5C1C0800,  //  000A  MOVE	R7	R4
      0x58200004,  //  000B  LDCONST	R8	K4
      0x7C140600,  //  000C  CALL	R5	3
      0xB81A0000,  //  000D  GETNGBL	R6	K0
      0x8C180D03,  //  000E  GETMET	R6	R6	K3
      0x5C200800,  //  000F  MOVE	R8	R4
      0x58240005,  //  0010  LDCONST	R9	K5
      0x7C180600,  //  0011  CALL	R6	3
      0x74160000,  //  0012  JMPT	R5	#0014
      0x781A0064,  //  0013  JMPF	R6	#0079
      0x4C1C0000,  //  0014  LDNIL	R7
      0x78160010,  //  0015  JMPF	R5	#0027
      0x60200009,  //  0016  GETGBL	R8	G9
      0x94240805,  //  0017  GETIDX	R9	R4	R5
      0x7C200200,  //  0018  CALL	R8	1
      0x18241106,  //  0019  LE	R9	R8	K6
      0x78260004,  //  001A  JMPF	R9	#0020
      0xB8260000,  //  001B  GETNGBL	R9	K0
      0x8C241301,  //  001C  GETMET	R9	R9	K1
      0x582C0007,  //  001D  LDCONST	R11	K7
      0x7C240400,  //  001E  CALL	R9	2
      0x80041200,  //  001F  RET	1	R9
      0x8C240108,  //  0020  GETMET	R9	R0	K8
      0x5C2C1000,  //  0021  MOVE	R11	R8
      0x7C240400,  //  0022  CALL	R9	2
      0x5C1C1200,  //  0023  MOVE	R7	R9
      0x8C240909,  //  0024  GETMET	R9	R4	K9
      0x5C2C0A00,  //  0025  MOVE	R11	R5
      0x7C240400,  //  0026  CALL	R9	2
      0x781A0009,  //  0027  JMPF	R6	#0032
      0x4C200000,  //  0028  LDNIL	R8
      0x1C200E08,  //  0029  EQ	R8	R7	R8
      0x78220003,  //  002A  JMPF	R8	#002F
      0x8C20010A,  //  002B  GETMET	R8	R0	K10
      0x94280806,  //  002C  GETIDX	R10	R4	R6
      0x7C200400,  //  002D  CALL	R8	2
      0x5C1C1000,  //  002E  MOVE	R7	R8
      0x8C200909,  //  002F  GETMET	R8	R4	K9
      0x5C280C00,  //  0030  MOVE	R10	R6
      0x7C200400,  //  0031  CALL	R8	2
      0x4C200000,  //  0032  LDNIL	R8
      0x1C200E08,  //  0033  EQ	R8	R7	R8
      0x78220004,  //  0034  JMPF	R8	#003A
      0xB8220000,  //  0035  GETNGBL	R8	K0
      0x8C201101,  //  0036  GETMET	R8	R8	K1
      0x5828000B,  //  0037  LDCONST	R10	K11
      0x7C200400,  //  0038  CALL	R8	2
      0x80041000,  //  0039  RET	1	R8
      0x88200F0C,  //  003A  GETMBR	R8	R7	K12
      0x74220004,  //  003B  JMPT	R8	#0041
      0xB8220000,  //  003C  GETNGBL	R8	K0
      0x8C201101,  //  003D  GETMET	R8	R8	K1
      0x5828000D,  //  003E  LDCONST	R10	K13
      0x7C200400,  //  003F  CALL	R8	2
      0x80041000,  //  0040  RET	1	R8
      0x8C200F0E,  //  0041  GETMET	R8	R7	K14
      0x7C200200,  //  0042  CALL	R8	1
      0x60240013,  //  0043  GETGBL	R9	G19
      0x7C240000,  //  0044  CALL	R9	0
      0x60280010,  //  0045  GETGBL	R10	G16
      0x8C2C090F,  //  0046  GETMET	R11	R4	K15
      0x7C2C0200,  //  0047  CALL	R11	1
      0x7C280200,  //  0048  CALL	R10	1
      0xA8020016,  //  0049  EXBLK	0	#0061
      0x5C2C1400,  //  004A  MOVE	R11	R10
      0x7C2C0000,  //  004B  CALL	R11	0
      0xB8320000,  //  004C  GETNGBL	R12	K0
      0x8C301910,  //  004D  GETMET	R12	R12	K16
      0x5C381000,  //  004E  MOVE	R14	R8
      0x5C3C1600,  //  004F  MOVE	R15	R11
      0x7C300600,  //  0050  CALL	R12	3
      0x4C340000,  //  0051  LDNIL	R13
      0x1C34180D,  //  0052  EQ	R13	R12	R13
      0x78360008,  //  0053  JMPF	R13	#005D
      0xB8360000,  //  0054  GETNGBL	R13	K0
      0x8C341B01,  //  0055  GETMET	R13	R13	K1
      0x603C0018,  //  0056  GETGBL	R15	G24
      0x58400011,  //  0057  LDCONST	R16	K17
      0x5C441600,  //  0058  MOVE	R17	R11
      0x7C3C0400,  //  0059  CALL	R15	2
      0x7C340400,  //  005A  CALL	R13	2
      0xA8040001,  //  005B  EXBLK	1	1
      0x80001A00,  //  005C  RET	0
      0x9434100C,  //  005D  GETIDX	R13	R8	R12
      0x9438080B,  //  005E  GETIDX	R14	R4	R11
      0x98241A0E,  //  005F  SETIDX	R9	R13	R14
      0x7001FFE8,  //  0060  JMP		#004A
      0x58280012,  //  0061  LDCONST	R10	K18
      0xAC280200,  //  0062  CATCH	R10	1	0
      0xB0080000,  //  0063  RAISE	2	R0	R0
      0x8C280F13,  //  0064  GETMET	R10	R7	K19
      0x5C301200,  //  0065  MOVE	R12	R9
      0x7C280400,  //  0066  CALL	R10	2
      0x8C280F14,  //  0067  GETMET	R10	R7	K20
      0x7C280200,  //  0068  CALL	R10	1
      0x782A000A,  //  0069  JMPF	R10	#0075
      0x602C0018,  //  006A  GETGBL	R11	G24
      0x58300015,  //  006B  LDCONST	R12	K21
      0x5C340200,  //  006C  MOVE	R13	R1
      0x5C381400,  //  006D  MOVE	R14	R10
      0x7C2C0600,  //  006E  CALL	R11	3
      0xB8320000,  //  006F  GETNGBL	R12	K0
      0x8C301916,  //  0070  GETMET	R12	R12	K22
      0x5C381600,  //  0071  MOVE	R14	R11
      0x7C300400,  //  0072  CALL	R12	2
      0x80041800,  //  0073  RET	1	R12
      0x70020003,  //  0074  JMP		#0079
      0xB82E0000,  //  0075  GETNGBL	R11	K0
      0x8C2C1717,  //  0076  GETMET	R11	R11	K23
      0x7C2C0200,  //  0077  CALL	R11	1
      0x80041600,  //  0078  RET	1	R11
      0xB81E0000,  //  0079  GETNGBL	R7	K0
      0x8C1C0F01,  //  007A  GETMET	R7	R7	K1
      0x58240018,  //  007B  LDCONST	R9	K24
      0x7C1C0400,  //  007C  CALL	R7	2
      0x80000000,  //  007D  RET	0
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
** Solidified function: conf_to_log
********************************************************************/
be_local_closure(Matter_Device_conf_to_log,   /* name */
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
    /* K1   */  be_nested_str_weak(),
    /* K2   */  be_nested_str_weak(k2l),
    /* K3   */  be_nested_str_weak(type),
    /* K4   */  be_nested_str_weak(_X20_X25s_X3A_X25s),
    /* K5   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(conf_to_log),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x58040000,  //  0000  LDCONST	R1	K0
      0x58080001,  //  0001  LDCONST	R2	K1
      0x600C0010,  //  0002  GETGBL	R3	G16
      0x8C100302,  //  0003  GETMET	R4	R1	K2
      0x5C180000,  //  0004  MOVE	R6	R0
      0x7C100400,  //  0005  CALL	R4	2
      0x7C0C0200,  //  0006  CALL	R3	1
      0xA802000B,  //  0007  EXBLK	0	#0014
      0x5C100600,  //  0008  MOVE	R4	R3
      0x7C100000,  //  0009  CALL	R4	0
      0x1C140903,  //  000A  EQ	R5	R4	K3
      0x78160000,  //  000B  JMPF	R5	#000D
      0x7001FFFA,  //  000C  JMP		#0008
      0x60140018,  //  000D  GETGBL	R5	G24
      0x58180004,  //  000E  LDCONST	R6	K4
      0x5C1C0800,  //  000F  MOVE	R7	R4
      0x94200004,  //  0010  GETIDX	R8	R0	R4
      0x7C140600,  //  0011  CALL	R5	3
      0x00080405,  //  0012  ADD	R2	R2	R5
      0x7001FFF3,  //  0013  JMP		#0008
      0x580C0005,  //  0014  LDCONST	R3	K5
      0xAC0C0200,  //  0015  CATCH	R3	1	0
      0xB0080000,  //  0016  RAISE	2	R0	R0
      0x80040400,  //  0017  RET	1	R2
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
** Solidified function: _instantiate_plugins_from_config
********************************************************************/
be_local_closure(Matter_Device__instantiate_plugins_from_config,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(k2l_num),
    /* K1   */  be_nested_str_weak(tasmota),
    /* K2   */  be_nested_str_weak(log),
    /* K3   */  be_nested_str_weak(MTR_X3A_X20Configuring_X20endpoints),
    /* K4   */  be_const_int(2),
    /* K5   */  be_nested_str_weak(plugins),
    /* K6   */  be_nested_str_weak(push),
    /* K7   */  be_nested_str_weak(matter),
    /* K8   */  be_nested_str_weak(Plugin_Root),
    /* K9   */  be_const_int(0),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20_X20_X20endpoint_X20_X3D_X20_X255i_X20type_X3A_X25s_X25s),
    /* K11  */  be_nested_str_weak(root),
    /* K12  */  be_nested_str_weak(),
    /* K13  */  be_nested_str_weak(Plugin_Aggregator),
    /* K14  */  be_nested_str_weak(find),
    /* K15  */  be_nested_str_weak(type),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20no_X20class_X20name_X2C_X20skipping),
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20only_X20one_X20root_X20node_X20allowed),
    /* K19  */  be_nested_str_weak(plugins_classes),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20unknown_X20class_X20name_X20_X27),
    /* K21  */  be_nested_str_weak(_X27_X20skipping),
    /* K22  */  be_nested_str_weak(conf_to_log),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K24  */  be_nested_str_weak(_X7C),
    /* K25  */  be_nested_str_weak(stop_iteration),
    /* K26  */  be_nested_str_weak(aggregator),
    /* K27  */  be_nested_str_weak(publish_result),
    /* K28  */  be_nested_str_weak(_X7B_X22Matter_X22_X3A_X7B_X22Initialized_X22_X3A1_X7D_X7D),
    /* K29  */  be_nested_str_weak(Matter),
    }),
    be_str_weak(_instantiate_plugins_from_config),
    &be_const_str_solidified,
    ( &(const binstruction[152]) {  /* code */
      0x8C080100,  //  0000  GETMET	R2	R0	K0
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0xB80E0200,  //  0003  GETNGBL	R3	K1
      0x8C0C0702,  //  0004  GETMET	R3	R3	K2
      0x58140003,  //  0005  LDCONST	R5	K3
      0x58180004,  //  0006  LDCONST	R6	K4
      0x7C0C0600,  //  0007  CALL	R3	3
      0x880C0105,  //  0008  GETMBR	R3	R0	K5
      0x8C0C0706,  //  0009  GETMET	R3	R3	K6
      0xB8160E00,  //  000A  GETNGBL	R5	K7
      0x8C140B08,  //  000B  GETMET	R5	R5	K8
      0x5C1C0000,  //  000C  MOVE	R7	R0
      0x58200009,  //  000D  LDCONST	R8	K9
      0x60240013,  //  000E  GETGBL	R9	G19
      0x7C240000,  //  000F  CALL	R9	0
      0x7C140800,  //  0010  CALL	R5	4
      0x7C0C0400,  //  0011  CALL	R3	2
      0xB80E0200,  //  0012  GETNGBL	R3	K1
      0x8C0C0702,  //  0013  GETMET	R3	R3	K2
      0x60140018,  //  0014  GETGBL	R5	G24
      0x5818000A,  //  0015  LDCONST	R6	K10
      0x581C0009,  //  0016  LDCONST	R7	K9
      0x5820000B,  //  0017  LDCONST	R8	K11
      0x5824000C,  //  0018  LDCONST	R9	K12
      0x7C140800,  //  0019  CALL	R5	4
      0x58180004,  //  001A  LDCONST	R6	K4
      0x7C0C0600,  //  001B  CALL	R3	3
      0x880C0105,  //  001C  GETMBR	R3	R0	K5
      0x8C0C0706,  //  001D  GETMET	R3	R3	K6
      0xB8160E00,  //  001E  GETNGBL	R5	K7
      0x8C140B0D,  //  001F  GETMET	R5	R5	K13
      0x5C1C0000,  //  0020  MOVE	R7	R0
      0x5422FEFF,  //  0021  LDINT	R8	65280
      0x60240013,  //  0022  GETGBL	R9	G19
      0x7C240000,  //  0023  CALL	R9	0
      0x7C140800,  //  0024  CALL	R5	4
      0x7C0C0400,  //  0025  CALL	R3	2
      0x600C0010,  //  0026  GETGBL	R3	G16
      0x5C100400,  //  0027  MOVE	R4	R2
      0x7C0C0200,  //  0028  CALL	R3	1
      0xA802005A,  //  0029  EXBLK	0	#0085
      0x5C100600,  //  002A  MOVE	R4	R3
      0x7C100000,  //  002B  CALL	R4	0
      0x1C140909,  //  002C  EQ	R5	R4	K9
      0x78160000,  //  002D  JMPF	R5	#002F
      0x7001FFFA,  //  002E  JMP		#002A
      0xA8020042,  //  002F  EXBLK	0	#0073
      0x60140008,  //  0030  GETGBL	R5	G8
      0x5C180800,  //  0031  MOVE	R6	R4
      0x7C140200,  //  0032  CALL	R5	1
      0x94140205,  //  0033  GETIDX	R5	R1	R5
      0x8C180B0E,  //  0034  GETMET	R6	R5	K14
      0x5820000F,  //  0035  LDCONST	R8	K15
      0x7C180400,  //  0036  CALL	R6	2
      0x4C1C0000,  //  0037  LDNIL	R7
      0x1C1C0C07,  //  0038  EQ	R7	R6	R7
      0x781E0006,  //  0039  JMPF	R7	#0041
      0xB81E0200,  //  003A  GETNGBL	R7	K1
      0x8C1C0F02,  //  003B  GETMET	R7	R7	K2
      0x58240010,  //  003C  LDCONST	R9	K16
      0x58280011,  //  003D  LDCONST	R10	K17
      0x7C1C0600,  //  003E  CALL	R7	3
      0xA8040001,  //  003F  EXBLK	1	1
      0x7001FFE8,  //  0040  JMP		#002A
      0x1C1C0D0B,  //  0041  EQ	R7	R6	K11
      0x781E0006,  //  0042  JMPF	R7	#004A
      0xB81E0200,  //  0043  GETNGBL	R7	K1
      0x8C1C0F02,  //  0044  GETMET	R7	R7	K2
      0x58240012,  //  0045  LDCONST	R9	K18
      0x58280011,  //  0046  LDCONST	R10	K17
      0x7C1C0600,  //  0047  CALL	R7	3
      0xA8040001,  //  0048  EXBLK	1	1
      0x7001FFDF,  //  0049  JMP		#002A
      0x881C0113,  //  004A  GETMBR	R7	R0	K19
      0x8C1C0F0E,  //  004B  GETMET	R7	R7	K14
      0x5C240C00,  //  004C  MOVE	R9	R6
      0x7C1C0400,  //  004D  CALL	R7	2
      0x4C200000,  //  004E  LDNIL	R8
      0x1C200E08,  //  004F  EQ	R8	R7	R8
      0x7822000A,  //  0050  JMPF	R8	#005C
      0xB8220200,  //  0051  GETNGBL	R8	K1
      0x8C201102,  //  0052  GETMET	R8	R8	K2
      0x60280008,  //  0053  GETGBL	R10	G8
      0x5C2C0C00,  //  0054  MOVE	R11	R6
      0x7C280200,  //  0055  CALL	R10	1
      0x002A280A,  //  0056  ADD	R10	K20	R10
      0x00281515,  //  0057  ADD	R10	R10	K21
      0x582C0004,  //  0058  LDCONST	R11	K4
      0x7C200600,  //  0059  CALL	R8	3
      0xA8040001,  //  005A  EXBLK	1	1
      0x7001FFCD,  //  005B  JMP		#002A
      0x5C200E00,  //  005C  MOVE	R8	R7
      0x5C240000,  //  005D  MOVE	R9	R0
      0x5C280800,  //  005E  MOVE	R10	R4
      0x5C2C0A00,  //  005F  MOVE	R11	R5
      0x7C200600,  //  0060  CALL	R8	3
      0x88240105,  //  0061  GETMBR	R9	R0	K5
      0x8C241306,  //  0062  GETMET	R9	R9	K6
      0x5C2C1000,  //  0063  MOVE	R11	R8
      0x7C240400,  //  0064  CALL	R9	2
      0xB8260200,  //  0065  GETNGBL	R9	K1
      0x8C241302,  //  0066  GETMET	R9	R9	K2
      0x602C0018,  //  0067  GETGBL	R11	G24
      0x5830000A,  //  0068  LDCONST	R12	K10
      0x5C340800,  //  0069  MOVE	R13	R4
      0x5C380C00,  //  006A  MOVE	R14	R6
      0x8C3C0116,  //  006B  GETMET	R15	R0	K22
      0x5C440A00,  //  006C  MOVE	R17	R5
      0x7C3C0400,  //  006D  CALL	R15	2
      0x7C2C0800,  //  006E  CALL	R11	4
      0x58300004,  //  006F  LDCONST	R12	K4
      0x7C240600,  //  0070  CALL	R9	3
      0xA8040001,  //  0071  EXBLK	1	1
      0x70020010,  //  0072  JMP		#0084
      0xAC140002,  //  0073  CATCH	R5	0	2
      0x7002000D,  //  0074  JMP		#0083
      0xB81E0200,  //  0075  GETNGBL	R7	K1
      0x8C1C0F02,  //  0076  GETMET	R7	R7	K2
      0x60240008,  //  0077  GETGBL	R9	G8
      0x5C280A00,  //  0078  MOVE	R10	R5
      0x7C240200,  //  0079  CALL	R9	1
      0x00262E09,  //  007A  ADD	R9	K23	R9
      0x00241318,  //  007B  ADD	R9	R9	K24
      0x60280008,  //  007C  GETGBL	R10	G8
      0x5C2C0C00,  //  007D  MOVE	R11	R6
      0x7C280200,  //  007E  CALL	R10	1
      0x0024120A,  //  007F  ADD	R9	R9	R10
      0x58280004,  //  0080  LDCONST	R10	K4
      0x7C1C0600,  //  0081  CALL	R7	3
      0x70020000,  //  0082  JMP		#0084
      0xB0080000,  //  0083  RAISE	2	R0	R0
      0x7001FFA4,  //  0084  JMP		#002A
      0x580C0019,  //  0085  LDCONST	R3	K25
      0xAC0C0200,  //  0086  CATCH	R3	1	0
      0xB0080000,  //  0087  RAISE	2	R0	R0
      0xB80E0200,  //  0088  GETNGBL	R3	K1
      0x8C0C0702,  //  0089  GETMET	R3	R3	K2
      0x60140018,  //  008A  GETGBL	R5	G24
      0x5818000A,  //  008B  LDCONST	R6	K10
      0x541EFEFF,  //  008C  LDINT	R7	65280
      0x5820001A,  //  008D  LDCONST	R8	K26
      0x5824000C,  //  008E  LDCONST	R9	K12
      0x7C140800,  //  008F  CALL	R5	4
      0x58180004,  //  0090  LDCONST	R6	K4
      0x7C0C0600,  //  0091  CALL	R3	3
      0xB80E0200,  //  0092  GETNGBL	R3	K1
      0x8C0C071B,  //  0093  GETMET	R3	R3	K27
      0x5814001C,  //  0094  LDCONST	R5	K28
      0x5818001D,  //  0095  LDCONST	R6	K29
      0x7C0C0600,  //  0096  CALL	R3	3
      0x80000000,  //  0097  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _compute_pbkdf
********************************************************************/
be_local_closure(Matter_Device__compute_pbkdf,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[10]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(add),
    /* K2   */  be_nested_str_weak(PBKDF2_HMAC_SHA256),
    /* K3   */  be_nested_str_weak(derive),
    /* K4   */  be_const_int(0),
    /* K5   */  be_nested_str_weak(root_w0),
    /* K6   */  be_nested_str_weak(EC_P256),
    /* K7   */  be_nested_str_weak(mod),
    /* K8   */  be_nested_str_weak(root_L),
    /* K9   */  be_nested_str_weak(public_key),
    }),
    be_str_weak(_compute_pbkdf),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0xA4120000,  //  0000  IMPORT	R4	K0
      0x60140015,  //  0001  GETGBL	R5	G21
      0x7C140000,  //  0002  CALL	R5	0
      0x8C140B01,  //  0003  GETMET	R5	R5	K1
      0x5C1C0200,  //  0004  MOVE	R7	R1
      0x54220003,  //  0005  LDINT	R8	4
      0x7C140600,  //  0006  CALL	R5	3
      0x8C180902,  //  0007  GETMET	R6	R4	K2
      0x7C180200,  //  0008  CALL	R6	1
      0x8C180D03,  //  0009  GETMET	R6	R6	K3
      0x5C200A00,  //  000A  MOVE	R8	R5
      0x5C240600,  //  000B  MOVE	R9	R3
      0x5C280400,  //  000C  MOVE	R10	R2
      0x542E004F,  //  000D  LDINT	R11	80
      0x7C180A00,  //  000E  CALL	R6	5
      0x541E0026,  //  000F  LDINT	R7	39
      0x401E0807,  //  0010  CONNECT	R7	K4	R7
      0x941C0C07,  //  0011  GETIDX	R7	R6	R7
      0x54220027,  //  0012  LDINT	R8	40
      0x5426004E,  //  0013  LDINT	R9	79
      0x40201009,  //  0014  CONNECT	R8	R8	R9
      0x94200C08,  //  0015  GETIDX	R8	R6	R8
      0x8C240906,  //  0016  GETMET	R9	R4	K6
      0x7C240200,  //  0017  CALL	R9	1
      0x8C241307,  //  0018  GETMET	R9	R9	K7
      0x5C2C0E00,  //  0019  MOVE	R11	R7
      0x7C240400,  //  001A  CALL	R9	2
      0x90020A09,  //  001B  SETMBR	R0	K5	R9
      0x8C240906,  //  001C  GETMET	R9	R4	K6
      0x7C240200,  //  001D  CALL	R9	1
      0x8C241307,  //  001E  GETMET	R9	R9	K7
      0x5C2C1000,  //  001F  MOVE	R11	R8
      0x7C240400,  //  0020  CALL	R9	2
      0x8C280906,  //  0021  GETMET	R10	R4	K6
      0x7C280200,  //  0022  CALL	R10	1
      0x8C281509,  //  0023  GETMET	R10	R10	K9
      0x5C301200,  //  0024  MOVE	R12	R9
      0x7C280400,  //  0025  CALL	R10	2
      0x9002100A,  //  0026  SETMBR	R0	K8	R10
      0x80000000,  //  0027  RET	0
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
** Solidified function: save_param
********************************************************************/
be_local_closure(Matter_Device_save_param,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[30]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(update_remotes_info),
    /* K2   */  be_nested_str_weak(_X7B_X22distinguish_X22_X3A_X25i_X2C_X22passcode_X22_X3A_X25i_X2C_X22ipv4only_X22_X3A_X25s_X2C_X22disable_bridge_mode_X22_X3A_X25s_X2C_X22nextep_X22_X3A_X25i),
    /* K3   */  be_nested_str_weak(root_discriminator),
    /* K4   */  be_nested_str_weak(root_passcode),
    /* K5   */  be_nested_str_weak(ipv4only),
    /* K6   */  be_nested_str_weak(true),
    /* K7   */  be_nested_str_weak(false),
    /* K8   */  be_nested_str_weak(disable_bridge_mode),
    /* K9   */  be_nested_str_weak(next_ep),
    /* K10  */  be_nested_str_weak(plugins_persist),
    /* K11  */  be_nested_str_weak(_X2C_X22config_X22_X3A),
    /* K12  */  be_nested_str_weak(dump),
    /* K13  */  be_nested_str_weak(plugins_config),
    /* K14  */  be_nested_str_weak(plugins_config_remotes),
    /* K15  */  be_const_int(0),
    /* K16  */  be_nested_str_weak(_X2C_X22remotes_X22_X3A),
    /* K17  */  be_nested_str_weak(_X7D),
    /* K18  */  be_nested_str_weak(FILENAME),
    /* K19  */  be_nested_str_weak(w),
    /* K20  */  be_nested_str_weak(write),
    /* K21  */  be_nested_str_weak(close),
    /* K22  */  be_nested_str_weak(tasmota),
    /* K23  */  be_nested_str_weak(log),
    /* K24  */  be_nested_str_weak(MTR_X3A_X20_X3DSaved_X20_X20_X20_X20_X20parameters_X25s),
    /* K25  */  be_nested_str_weak(_X20and_X20configuration),
    /* K26  */  be_nested_str_weak(),
    /* K27  */  be_const_int(2),
    /* K28  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Asave_X20Exception_X3A),
    /* K29  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(save_param),
    &be_const_str_solidified,
    ( &(const binstruction[82]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080101,  //  0001  GETMET	R2	R0	K1
      0x7C080200,  //  0002  CALL	R2	1
      0x60080018,  //  0003  GETGBL	R2	G24
      0x580C0002,  //  0004  LDCONST	R3	K2
      0x88100103,  //  0005  GETMBR	R4	R0	K3
      0x88140104,  //  0006  GETMBR	R5	R0	K4
      0x88180105,  //  0007  GETMBR	R6	R0	K5
      0x781A0001,  //  0008  JMPF	R6	#000B
      0x58180006,  //  0009  LDCONST	R6	K6
      0x70020000,  //  000A  JMP		#000C
      0x58180007,  //  000B  LDCONST	R6	K7
      0x881C0108,  //  000C  GETMBR	R7	R0	K8
      0x781E0001,  //  000D  JMPF	R7	#0010
      0x581C0006,  //  000E  LDCONST	R7	K6
      0x70020000,  //  000F  JMP		#0011
      0x581C0007,  //  0010  LDCONST	R7	K7
      0x88200109,  //  0011  GETMBR	R8	R0	K9
      0x7C080C00,  //  0012  CALL	R2	6
      0x880C010A,  //  0013  GETMBR	R3	R0	K10
      0x780E000E,  //  0014  JMPF	R3	#0024
      0x0008050B,  //  0015  ADD	R2	R2	K11
      0x8C0C030C,  //  0016  GETMET	R3	R1	K12
      0x8814010D,  //  0017  GETMBR	R5	R0	K13
      0x7C0C0400,  //  0018  CALL	R3	2
      0x00080403,  //  0019  ADD	R2	R2	R3
      0x600C000C,  //  001A  GETGBL	R3	G12
      0x8810010E,  //  001B  GETMBR	R4	R0	K14
      0x7C0C0200,  //  001C  CALL	R3	1
      0x240C070F,  //  001D  GT	R3	R3	K15
      0x780E0004,  //  001E  JMPF	R3	#0024
      0x00080510,  //  001F  ADD	R2	R2	K16
      0x8C0C030C,  //  0020  GETMET	R3	R1	K12
      0x8814010E,  //  0021  GETMBR	R5	R0	K14
      0x7C0C0400,  //  0022  CALL	R3	2
      0x00080403,  //  0023  ADD	R2	R2	R3
      0x00080511,  //  0024  ADD	R2	R2	K17
      0xA8020018,  //  0025  EXBLK	0	#003F
      0x600C0011,  //  0026  GETGBL	R3	G17
      0x88100112,  //  0027  GETMBR	R4	R0	K18
      0x58140013,  //  0028  LDCONST	R5	K19
      0x7C0C0400,  //  0029  CALL	R3	2
      0x8C100714,  //  002A  GETMET	R4	R3	K20
      0x5C180400,  //  002B  MOVE	R6	R2
      0x7C100400,  //  002C  CALL	R4	2
      0x8C100715,  //  002D  GETMET	R4	R3	K21
      0x7C100200,  //  002E  CALL	R4	1
      0xB8122C00,  //  002F  GETNGBL	R4	K22
      0x8C100917,  //  0030  GETMET	R4	R4	K23
      0x60180018,  //  0031  GETGBL	R6	G24
      0x581C0018,  //  0032  LDCONST	R7	K24
      0x8820010A,  //  0033  GETMBR	R8	R0	K10
      0x78220001,  //  0034  JMPF	R8	#0037
      0x58200019,  //  0035  LDCONST	R8	K25
      0x70020000,  //  0036  JMP		#0038
      0x5820001A,  //  0037  LDCONST	R8	K26
      0x7C180400,  //  0038  CALL	R6	2
      0x581C001B,  //  0039  LDCONST	R7	K27
      0x7C100600,  //  003A  CALL	R4	3
      0xA8040001,  //  003B  EXBLK	1	1
      0x80040400,  //  003C  RET	1	R2
      0xA8040001,  //  003D  EXBLK	1	1
      0x70020011,  //  003E  JMP		#0051
      0xAC0C0002,  //  003F  CATCH	R3	0	2
      0x7002000E,  //  0040  JMP		#0050
      0xB8162C00,  //  0041  GETNGBL	R5	K22
      0x8C140B17,  //  0042  GETMET	R5	R5	K23
      0x601C0008,  //  0043  GETGBL	R7	G8
      0x5C200600,  //  0044  MOVE	R8	R3
      0x7C1C0200,  //  0045  CALL	R7	1
      0x001E3807,  //  0046  ADD	R7	K28	R7
      0x001C0F1D,  //  0047  ADD	R7	R7	K29
      0x60200008,  //  0048  GETGBL	R8	G8
      0x5C240800,  //  0049  MOVE	R9	R4
      0x7C200200,  //  004A  CALL	R8	1
      0x001C0E08,  //  004B  ADD	R7	R7	R8
      0x5820001B,  //  004C  LDCONST	R8	K27
      0x7C140600,  //  004D  CALL	R5	3
      0x80040400,  //  004E  RET	1	R2
      0x70020000,  //  004F  JMP		#0051
      0xB0080000,  //  0050  RAISE	2	R0	R0
      0x80000000,  //  0051  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_announce_PASE
********************************************************************/
be_local_closure(Matter_Device_mdns_announce_PASE,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[41]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(crypto),
    /* K2   */  be_nested_str_weak(VP),
    /* K3   */  be_nested_str_weak(vendorid),
    /* K4   */  be_nested_str_weak(_X2B),
    /* K5   */  be_nested_str_weak(productid),
    /* K6   */  be_nested_str_weak(D),
    /* K7   */  be_nested_str_weak(commissioning_discriminator),
    /* K8   */  be_nested_str_weak(CM),
    /* K9   */  be_const_int(1),
    /* K10  */  be_nested_str_weak(T),
    /* K11  */  be_const_int(0),
    /* K12  */  be_nested_str_weak(SII),
    /* K13  */  be_nested_str_weak(SAI),
    /* K14  */  be_nested_str_weak(commissioning_instance_wifi),
    /* K15  */  be_nested_str_weak(random),
    /* K16  */  be_nested_str_weak(tohex),
    /* K17  */  be_nested_str_weak(commissioning_instance_eth),
    /* K18  */  be_nested_str_weak(hostname_eth),
    /* K19  */  be_nested_str_weak(add_service),
    /* K20  */  be_nested_str_weak(_matterc),
    /* K21  */  be_nested_str_weak(_udp),
    /* K22  */  be_nested_str_weak(mdns_pase_eth),
    /* K23  */  be_nested_str_weak(tasmota),
    /* K24  */  be_nested_str_weak(log),
    /* K25  */  be_nested_str_weak(MTR_X3A_X20announce_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
    /* K26  */  be_nested_str_weak(eth),
    /* K27  */  be_const_int(2),
    /* K28  */  be_nested_str_weak(_L),
    /* K29  */  be_nested_str_weak(MTR_X3A_X20adding_X20subtype_X3A_X20),
    /* K30  */  be_const_int(3),
    /* K31  */  be_nested_str_weak(add_subtype),
    /* K32  */  be_nested_str_weak(_S),
    /* K33  */  be_nested_str_weak(_V),
    /* K34  */  be_nested_str_weak(_CM1),
    /* K35  */  be_nested_str_weak(hostname_wifi),
    /* K36  */  be_nested_str_weak(mdns_pase_wifi),
    /* K37  */  be_nested_str_weak(MTR_X3A_X20starting_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
    /* K38  */  be_nested_str_weak(wifi),
    /* K39  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K40  */  be_nested_str_weak(_X7C),
    }),
    be_str_weak(mdns_announce_PASE),
    &be_const_str_solidified,
    ( &(const binstruction[236]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0x600C0013,  //  0002  GETGBL	R3	G19
      0x7C0C0000,  //  0003  CALL	R3	0
      0x60100008,  //  0004  GETGBL	R4	G8
      0x88140103,  //  0005  GETMBR	R5	R0	K3
      0x7C100200,  //  0006  CALL	R4	1
      0x00100904,  //  0007  ADD	R4	R4	K4
      0x60140008,  //  0008  GETGBL	R5	G8
      0x88180105,  //  0009  GETMBR	R6	R0	K5
      0x7C140200,  //  000A  CALL	R5	1
      0x00100805,  //  000B  ADD	R4	R4	R5
      0x980E0404,  //  000C  SETIDX	R3	K2	R4
      0x88100107,  //  000D  GETMBR	R4	R0	K7
      0x980E0C04,  //  000E  SETIDX	R3	K6	R4
      0x980E1109,  //  000F  SETIDX	R3	K8	K9
      0x980E150B,  //  0010  SETIDX	R3	K10	K11
      0x54121387,  //  0011  LDINT	R4	5000
      0x980E1804,  //  0012  SETIDX	R3	K12	R4
      0x5412012B,  //  0013  LDINT	R4	300
      0x980E1A04,  //  0014  SETIDX	R3	K13	R4
      0x8C10050F,  //  0015  GETMET	R4	R2	K15
      0x541A0007,  //  0016  LDINT	R6	8
      0x7C100400,  //  0017  CALL	R4	2
      0x8C100910,  //  0018  GETMET	R4	R4	K16
      0x7C100200,  //  0019  CALL	R4	1
      0x90021C04,  //  001A  SETMBR	R0	K14	R4
      0x8C10050F,  //  001B  GETMET	R4	R2	K15
      0x541A0007,  //  001C  LDINT	R6	8
      0x7C100400,  //  001D  CALL	R4	2
      0x8C100910,  //  001E  GETMET	R4	R4	K16
      0x7C100200,  //  001F  CALL	R4	1
      0x90022204,  //  0020  SETMBR	R0	K17	R4
      0xA80200B7,  //  0021  EXBLK	0	#00DA
      0x88100112,  //  0022  GETMBR	R4	R0	K18
      0x78120058,  //  0023  JMPF	R4	#007D
      0x8C100313,  //  0024  GETMET	R4	R1	K19
      0x58180014,  //  0025  LDCONST	R6	K20
      0x581C0015,  //  0026  LDCONST	R7	K21
      0x542215A3,  //  0027  LDINT	R8	5540
      0x5C240600,  //  0028  MOVE	R9	R3
      0x88280111,  //  0029  GETMBR	R10	R0	K17
      0x882C0112,  //  002A  GETMBR	R11	R0	K18
      0x7C100E00,  //  002B  CALL	R4	7
      0x50100200,  //  002C  LDBOOL	R4	1	0
      0x90022C04,  //  002D  SETMBR	R0	K22	R4
      0xB8122E00,  //  002E  GETNGBL	R4	K23
      0x8C100918,  //  002F  GETMET	R4	R4	K24
      0x60180018,  //  0030  GETGBL	R6	G24
      0x581C0019,  //  0031  LDCONST	R7	K25
      0x5820001A,  //  0032  LDCONST	R8	K26
      0x88240111,  //  0033  GETMBR	R9	R0	K17
      0x88280112,  //  0034  GETMBR	R10	R0	K18
      0x7C180800,  //  0035  CALL	R6	4
      0x581C001B,  //  0036  LDCONST	R7	K27
      0x7C100600,  //  0037  CALL	R4	3
      0x60100008,  //  0038  GETGBL	R4	G8
      0x88140107,  //  0039  GETMBR	R5	R0	K7
      0x541A0FFE,  //  003A  LDINT	R6	4095
      0x2C140A06,  //  003B  AND	R5	R5	R6
      0x7C100200,  //  003C  CALL	R4	1
      0x00123804,  //  003D  ADD	R4	K28	R4
      0xB8162E00,  //  003E  GETNGBL	R5	K23
      0x8C140B18,  //  003F  GETMET	R5	R5	K24
      0x001E3A04,  //  0040  ADD	R7	K29	R4
      0x5820001E,  //  0041  LDCONST	R8	K30
      0x7C140600,  //  0042  CALL	R5	3
      0x8C14031F,  //  0043  GETMET	R5	R1	K31
      0x581C0014,  //  0044  LDCONST	R7	K20
      0x58200015,  //  0045  LDCONST	R8	K21
      0x88240111,  //  0046  GETMBR	R9	R0	K17
      0x88280112,  //  0047  GETMBR	R10	R0	K18
      0x5C2C0800,  //  0048  MOVE	R11	R4
      0x7C140C00,  //  0049  CALL	R5	6
      0x60140008,  //  004A  GETGBL	R5	G8
      0x88180107,  //  004B  GETMBR	R6	R0	K7
      0x541E0EFF,  //  004C  LDINT	R7	3840
      0x2C180C07,  //  004D  AND	R6	R6	R7
      0x541E0007,  //  004E  LDINT	R7	8
      0x3C180C07,  //  004F  SHR	R6	R6	R7
      0x7C140200,  //  0050  CALL	R5	1
      0x00164005,  //  0051  ADD	R5	K32	R5
      0x5C100A00,  //  0052  MOVE	R4	R5
      0xB8162E00,  //  0053  GETNGBL	R5	K23
      0x8C140B18,  //  0054  GETMET	R5	R5	K24
      0x001E3A04,  //  0055  ADD	R7	K29	R4
      0x5820001E,  //  0056  LDCONST	R8	K30
      0x7C140600,  //  0057  CALL	R5	3
      0x8C14031F,  //  0058  GETMET	R5	R1	K31
      0x581C0014,  //  0059  LDCONST	R7	K20
      0x58200015,  //  005A  LDCONST	R8	K21
      0x88240111,  //  005B  GETMBR	R9	R0	K17
      0x88280112,  //  005C  GETMBR	R10	R0	K18
      0x5C2C0800,  //  005D  MOVE	R11	R4
      0x7C140C00,  //  005E  CALL	R5	6
      0x60140008,  //  005F  GETGBL	R5	G8
      0x88180103,  //  0060  GETMBR	R6	R0	K3
      0x7C140200,  //  0061  CALL	R5	1
      0x00164205,  //  0062  ADD	R5	K33	R5
      0x5C100A00,  //  0063  MOVE	R4	R5
      0xB8162E00,  //  0064  GETNGBL	R5	K23
      0x8C140B18,  //  0065  GETMET	R5	R5	K24
      0x001E3A04,  //  0066  ADD	R7	K29	R4
      0x5820001E,  //  0067  LDCONST	R8	K30
      0x7C140600,  //  0068  CALL	R5	3
      0x8C14031F,  //  0069  GETMET	R5	R1	K31
      0x581C0014,  //  006A  LDCONST	R7	K20
      0x58200015,  //  006B  LDCONST	R8	K21
      0x88240111,  //  006C  GETMBR	R9	R0	K17
      0x88280112,  //  006D  GETMBR	R10	R0	K18
      0x5C2C0800,  //  006E  MOVE	R11	R4
      0x7C140C00,  //  006F  CALL	R5	6
      0x58100022,  //  0070  LDCONST	R4	K34
      0xB8162E00,  //  0071  GETNGBL	R5	K23
      0x8C140B18,  //  0072  GETMET	R5	R5	K24
      0x001E3A04,  //  0073  ADD	R7	K29	R4
      0x5820001E,  //  0074  LDCONST	R8	K30
      0x7C140600,  //  0075  CALL	R5	3
      0x8C14031F,  //  0076  GETMET	R5	R1	K31
      0x581C0014,  //  0077  LDCONST	R7	K20
      0x58200015,  //  0078  LDCONST	R8	K21
      0x88240111,  //  0079  GETMBR	R9	R0	K17
      0x88280112,  //  007A  GETMBR	R10	R0	K18
      0x5C2C0800,  //  007B  MOVE	R11	R4
      0x7C140C00,  //  007C  CALL	R5	6
      0x88100123,  //  007D  GETMBR	R4	R0	K35
      0x78120058,  //  007E  JMPF	R4	#00D8
      0x8C100313,  //  007F  GETMET	R4	R1	K19
      0x58180014,  //  0080  LDCONST	R6	K20
      0x581C0015,  //  0081  LDCONST	R7	K21
      0x542215A3,  //  0082  LDINT	R8	5540
      0x5C240600,  //  0083  MOVE	R9	R3
      0x8828010E,  //  0084  GETMBR	R10	R0	K14
      0x882C0123,  //  0085  GETMBR	R11	R0	K35
      0x7C100E00,  //  0086  CALL	R4	7
      0x50100200,  //  0087  LDBOOL	R4	1	0
      0x90024804,  //  0088  SETMBR	R0	K36	R4
      0xB8122E00,  //  0089  GETNGBL	R4	K23
      0x8C100918,  //  008A  GETMET	R4	R4	K24
      0x60180018,  //  008B  GETGBL	R6	G24
      0x581C0025,  //  008C  LDCONST	R7	K37
      0x58200026,  //  008D  LDCONST	R8	K38
      0x8824010E,  //  008E  GETMBR	R9	R0	K14
      0x88280123,  //  008F  GETMBR	R10	R0	K35
      0x7C180800,  //  0090  CALL	R6	4
      0x581C001E,  //  0091  LDCONST	R7	K30
      0x7C100600,  //  0092  CALL	R4	3
      0x60100008,  //  0093  GETGBL	R4	G8
      0x88140107,  //  0094  GETMBR	R5	R0	K7
      0x541A0FFE,  //  0095  LDINT	R6	4095
      0x2C140A06,  //  0096  AND	R5	R5	R6
      0x7C100200,  //  0097  CALL	R4	1
      0x00123804,  //  0098  ADD	R4	K28	R4
      0xB8162E00,  //  0099  GETNGBL	R5	K23
      0x8C140B18,  //  009A  GETMET	R5	R5	K24
      0x001E3A04,  //  009B  ADD	R7	K29	R4
      0x5820001E,  //  009C  LDCONST	R8	K30
      0x7C140600,  //  009D  CALL	R5	3
      0x8C14031F,  //  009E  GETMET	R5	R1	K31
      0x581C0014,  //  009F  LDCONST	R7	K20
      0x58200015,  //  00A0  LDCONST	R8	K21
      0x8824010E,  //  00A1  GETMBR	R9	R0	K14
      0x88280123,  //  00A2  GETMBR	R10	R0	K35
      0x5C2C0800,  //  00A3  MOVE	R11	R4
      0x7C140C00,  //  00A4  CALL	R5	6
      0x60140008,  //  00A5  GETGBL	R5	G8
      0x88180107,  //  00A6  GETMBR	R6	R0	K7
      0x541E0EFF,  //  00A7  LDINT	R7	3840
      0x2C180C07,  //  00A8  AND	R6	R6	R7
      0x541E0007,  //  00A9  LDINT	R7	8
      0x3C180C07,  //  00AA  SHR	R6	R6	R7
      0x7C140200,  //  00AB  CALL	R5	1
      0x00164005,  //  00AC  ADD	R5	K32	R5
      0x5C100A00,  //  00AD  MOVE	R4	R5
      0xB8162E00,  //  00AE  GETNGBL	R5	K23
      0x8C140B18,  //  00AF  GETMET	R5	R5	K24
      0x001E3A04,  //  00B0  ADD	R7	K29	R4
      0x5820001E,  //  00B1  LDCONST	R8	K30
      0x7C140600,  //  00B2  CALL	R5	3
      0x8C14031F,  //  00B3  GETMET	R5	R1	K31
      0x581C0014,  //  00B4  LDCONST	R7	K20
      0x58200015,  //  00B5  LDCONST	R8	K21
      0x8824010E,  //  00B6  GETMBR	R9	R0	K14
      0x88280123,  //  00B7  GETMBR	R10	R0	K35
      0x5C2C0800,  //  00B8  MOVE	R11	R4
      0x7C140C00,  //  00B9  CALL	R5	6
      0x60140008,  //  00BA  GETGBL	R5	G8
      0x88180103,  //  00BB  GETMBR	R6	R0	K3
      0x7C140200,  //  00BC  CALL	R5	1
      0x00164205,  //  00BD  ADD	R5	K33	R5
      0x5C100A00,  //  00BE  MOVE	R4	R5
      0xB8162E00,  //  00BF  GETNGBL	R5	K23
      0x8C140B18,  //  00C0  GETMET	R5	R5	K24
      0x001E3A04,  //  00C1  ADD	R7	K29	R4
      0x5820001E,  //  00C2  LDCONST	R8	K30
      0x7C140600,  //  00C3  CALL	R5	3
      0x8C14031F,  //  00C4  GETMET	R5	R1	K31
      0x581C0014,  //  00C5  LDCONST	R7	K20
      0x58200015,  //  00C6  LDCONST	R8	K21
      0x8824010E,  //  00C7  GETMBR	R9	R0	K14
      0x88280123,  //  00C8  GETMBR	R10	R0	K35
      0x5C2C0800,  //  00C9  MOVE	R11	R4
      0x7C140C00,  //  00CA  CALL	R5	6
      0x58100022,  //  00CB  LDCONST	R4	K34
      0xB8162E00,  //  00CC  GETNGBL	R5	K23
      0x8C140B18,  //  00CD  GETMET	R5	R5	K24
      0x001E3A04,  //  00CE  ADD	R7	K29	R4
      0x5820001E,  //  00CF  LDCONST	R8	K30
      0x7C140600,  //  00D0  CALL	R5	3
      0x8C14031F,  //  00D1  GETMET	R5	R1	K31
      0x581C0014,  //  00D2  LDCONST	R7	K20
      0x58200015,  //  00D3  LDCONST	R8	K21
      0x8824010E,  //  00D4  GETMBR	R9	R0	K14
      0x88280123,  //  00D5  GETMBR	R10	R0	K35
      0x5C2C0800,  //  00D6  MOVE	R11	R4
      0x7C140C00,  //  00D7  CALL	R5	6
      0xA8040001,  //  00D8  EXBLK	1	1
      0x70020010,  //  00D9  JMP		#00EB
      0xAC100002,  //  00DA  CATCH	R4	0	2
      0x7002000D,  //  00DB  JMP		#00EA
      0xB81A2E00,  //  00DC  GETNGBL	R6	K23
      0x8C180D18,  //  00DD  GETMET	R6	R6	K24
      0x60200008,  //  00DE  GETGBL	R8	G8
      0x5C240800,  //  00DF  MOVE	R9	R4
      0x7C200200,  //  00E0  CALL	R8	1
      0x00224E08,  //  00E1  ADD	R8	K39	R8
      0x00201128,  //  00E2  ADD	R8	R8	K40
      0x60240008,  //  00E3  GETGBL	R9	G8
      0x5C280A00,  //  00E4  MOVE	R10	R5
      0x7C240200,  //  00E5  CALL	R9	1
      0x00201009,  //  00E6  ADD	R8	R8	R9
      0x5824001B,  //  00E7  LDCONST	R9	K27
      0x7C180600,  //  00E8  CALL	R6	3
      0x70020000,  //  00E9  JMP		#00EB
      0xB0080000,  //  00EA  RAISE	2	R0	R0
      0x80000000,  //  00EB  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Device
********************************************************************/
be_local_class(Matter_Device,
    37,
    NULL,
    be_nested_map(114,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(PRODUCT_ID, -1), be_const_int(32768) },
        { be_const_key_weak(autoconf_device_map, -1), be_const_closure(Matter_Device_autoconf_device_map_closure) },
        { be_const_key_weak(PASSCODE_INVALID, 23), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_weak(remove_fabric, -1), be_const_closure(Matter_Device_remove_fabric_closure) },
        { be_const_key_weak(is_root_commissioning_open, -1), be_const_closure(Matter_Device_is_root_commissioning_open_closure) },
        { be_const_key_weak(_init_basic_commissioning, 4), be_const_closure(Matter_Device__init_basic_commissioning_closure) },
        { be_const_key_weak(compute_manual_pairing_code, -1), be_const_closure(Matter_Device_compute_manual_pairing_code_closure) },
        { be_const_key_weak(ui, -1), be_const_var(10) },
        { be_const_key_weak(vendorid, 50), be_const_var(23) },
        { be_const_key_weak(root_L, 87), be_const_var(36) },
        { be_const_key_weak(sessions, -1), be_const_var(9) },
        { be_const_key_weak(stop, -1), be_const_closure(Matter_Device_stop_closure) },
        { be_const_key_weak(bridge_add_endpoint, -1), be_const_closure(Matter_Device_bridge_add_endpoint_closure) },
        { be_const_key_weak(mdns_remove_op_discovery, -1), be_const_closure(Matter_Device_mdns_remove_op_discovery_closure) },
        { be_const_key_weak(start_mdns_announce_hostnames, 36), be_const_closure(Matter_Device_start_mdns_announce_hostnames_closure) },
        { be_const_key_weak(disable_bridge_mode, 17), be_const_var(31) },
        { be_const_key_weak(get_plugin_class_displayname, 1), be_const_closure(Matter_Device_get_plugin_class_displayname_closure) },
        { be_const_key_weak(http_remotes, -1), be_const_var(27) },
        { be_const_key_weak(clean_remotes, -1), be_const_closure(Matter_Device_clean_remotes_closure) },
        { be_const_key_weak(get_plugin_remote_info, 49), be_const_closure(Matter_Device_get_plugin_remote_info_closure) },
        { be_const_key_weak(_mdns_announce_hostname, -1), be_const_closure(Matter_Device__mdns_announce_hostname_closure) },
        { be_const_key_weak(register_native_classes, 55), be_const_closure(Matter_Device_register_native_classes_closure) },
        { be_const_key_weak(_start_udp, 109), be_const_closure(Matter_Device__start_udp_closure) },
        { be_const_key_weak(get_plugin_class_arg, 33), be_const_closure(Matter_Device_get_plugin_class_arg_closure) },
        { be_const_key_weak(commissioning_admin_fabric, -1), be_const_var(18) },
        { be_const_key_weak(msg_send, -1), be_const_closure(Matter_Device_msg_send_closure) },
        { be_const_key_weak(commissioning_w0, 102), be_const_var(16) },
        { be_const_key_weak(mdns_pase_eth, -1), be_const_var(25) },
        { be_const_key_weak(process_attribute_read_solo, -1), be_const_closure(Matter_Device_process_attribute_read_solo_closure) },
        { be_const_key_weak(invoke_request, 108), be_const_closure(Matter_Device_invoke_request_closure) },
        { be_const_key_weak(adjust_next_ep, 104), be_const_closure(Matter_Device_adjust_next_ep_closure) },
        { be_const_key_weak(attribute_updated, -1), be_const_closure(Matter_Device_attribute_updated_closure) },
        { be_const_key_weak(mdns_remove_PASE, 107), be_const_closure(Matter_Device_mdns_remove_PASE_closure) },
        { be_const_key_weak(_compute_pbkdf, 90), be_const_closure(Matter_Device__compute_pbkdf_closure) },
        { be_const_key_weak(ipv4only, 71), be_const_var(30) },
        { be_const_key_weak(get_active_endpoints, -1), be_const_closure(Matter_Device_get_active_endpoints_closure) },
        { be_const_key_weak(k2l_num, -1), be_const_static_closure(Matter_Device_k2l_num_closure) },
        { be_const_key_weak(autoconf_sensors_list, -1), be_const_closure(Matter_Device_autoconf_sensors_list_closure) },
        { be_const_key_weak(start_basic_commissioning, -1), be_const_closure(Matter_Device_start_basic_commissioning_closure) },
        { be_const_key_weak(bridge_remove_endpoint, 14), be_const_closure(Matter_Device_bridge_remove_endpoint_closure) },
        { be_const_key_weak(msg_received, -1), be_const_closure(Matter_Device_msg_received_closure) },
        { be_const_key_weak(start, -1), be_const_closure(Matter_Device_start_closure) },
        { be_const_key_weak(stop_basic_commissioning, -1), be_const_closure(Matter_Device_stop_basic_commissioning_closure) },
        { be_const_key_weak(generate_random_passcode, -1), be_const_closure(Matter_Device_generate_random_passcode_closure) },
        { be_const_key_weak(started, 65), be_const_var(0) },
        { be_const_key_weak(signal_endpoints_changed, -1), be_const_closure(Matter_Device_signal_endpoints_changed_closure) },
        { be_const_key_weak(MtrJoin, 48), be_const_closure(Matter_Device_MtrJoin_closure) },
        { be_const_key_weak(productid, -1), be_const_var(24) },
        { be_const_key_weak(find_plugin_by_endpoint, -1), be_const_closure(Matter_Device_find_plugin_by_endpoint_closure) },
        { be_const_key_weak(PBKDF_ITERATIONS, 3), be_const_int(1000) },
        { be_const_key_weak(hostname_eth, 97), be_const_var(22) },
        { be_const_key_weak(init, 37), be_const_closure(Matter_Device_init_closure) },
        { be_const_key_weak(register_http_remote, 40), be_const_closure(Matter_Device_register_http_remote_closure) },
        { be_const_key_weak(commissioning_salt, 98), be_const_var(15) },
        { be_const_key_weak(mdns_pase_wifi, -1), be_const_var(26) },
        { be_const_key_weak(load_param, -1), be_const_closure(Matter_Device_load_param_closure) },
        { be_const_key_weak(hostname_wifi, 93), be_const_var(21) },
        { be_const_key_weak(compute_qrcode_content, 45), be_const_closure(Matter_Device_compute_qrcode_content_closure) },
        { be_const_key_weak(plugins_config, -1), be_const_var(4) },
        { be_const_key_weak(start_root_basic_commissioning, -1), be_const_closure(Matter_Device_start_root_basic_commissioning_closure) },
        { be_const_key_weak(save_before_restart, -1), be_const_closure(Matter_Device_save_before_restart_closure) },
        { be_const_key_weak(register_commands, 54), be_const_closure(Matter_Device_register_commands_closure) },
        { be_const_key_weak(root_salt, -1), be_const_var(34) },
        { be_const_key_weak(commissioning_iterations, 2), be_const_var(13) },
        { be_const_key_weak(find_plugin_by_friendly_name, -1), be_const_closure(Matter_Device_find_plugin_by_friendly_name_closure) },
        { be_const_key_weak(plugins_persist, -1), be_const_var(2) },
        { be_const_key_weak(udp_server, -1), be_const_var(6) },
        { be_const_key_weak(next_ep, -1), be_const_var(32) },
        { be_const_key_weak(start_operational_discovery_deferred, 72), be_const_closure(Matter_Device_start_operational_discovery_deferred_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(Matter_Device_every_second_closure) },
        { be_const_key_weak(UDP_PORT, -1), be_const_int(5540) },
        { be_const_key_weak(mdns_announce_op_discovery, 30), be_const_closure(Matter_Device_mdns_announce_op_discovery_closure) },
        { be_const_key_weak(message_handler, -1), be_const_var(8) },
        { be_const_key_weak(process_attribute_expansion, 43), be_const_closure(Matter_Device_process_attribute_expansion_closure) },
        { be_const_key_weak(is_commissioning_open, 62), be_const_closure(Matter_Device_is_commissioning_open_closure) },
        { be_const_key_weak(start_commissioning_complete_deferred, -1), be_const_closure(Matter_Device_start_commissioning_complete_deferred_closure) },
        { be_const_key_weak(plugins_classes, 44), be_const_var(3) },
        { be_const_key_weak(start_operational_discovery, 68), be_const_closure(Matter_Device_start_operational_discovery_closure) },
        { be_const_key_weak(mdns_remove_op_discovery_all_fabrics, 9), be_const_closure(Matter_Device_mdns_remove_op_discovery_all_fabrics_closure) },
        { be_const_key_weak(commissioning_discriminator, -1), be_const_var(14) },
        { be_const_key_weak(FILENAME, -1), be_nested_str_weak(_matter_device_X2Ejson) },
        { be_const_key_weak(root_iterations, 112), be_const_var(33) },
        { be_const_key_weak(commissioning_open, -1), be_const_var(12) },
        { be_const_key_weak(_trigger_read_sensors, 82), be_const_closure(Matter_Device__trigger_read_sensors_closure) },
        { be_const_key_weak(commissioning_instance_wifi, 81), be_const_var(19) },
        { be_const_key_weak(VENDOR_ID, -1), be_const_int(65521) },
        { be_const_key_weak(tick, -1), be_const_var(11) },
        { be_const_key_weak(MtrInfo, -1), be_const_closure(Matter_Device_MtrInfo_closure) },
        { be_const_key_weak(root_discriminator, 67), be_const_var(28) },
        { be_const_key_weak(start_commissioning_complete, 7), be_const_closure(Matter_Device_start_commissioning_complete_closure) },
        { be_const_key_weak(register_plugin_class, 95), be_const_closure(Matter_Device_register_plugin_class_closure) },
        { be_const_key_weak(MtrUpdate, -1), be_const_closure(Matter_Device_MtrUpdate_closure) },
        { be_const_key_weak(sort_distinct, -1), be_const_static_closure(Matter_Device_sort_distinct_closure) },
        { be_const_key_weak(commissioning_instance_eth, -1), be_const_var(20) },
        { be_const_key_weak(conf_to_log, -1), be_const_static_closure(Matter_Device_conf_to_log_closure) },
        { be_const_key_weak(MtrInfo_one, -1), be_const_closure(Matter_Device_MtrInfo_one_closure) },
        { be_const_key_weak(received_ack, -1), be_const_closure(Matter_Device_received_ack_closure) },
        { be_const_key_weak(k2l, -1), be_const_static_closure(Matter_Device_k2l_closure) },
        { be_const_key_weak(update_remotes_info, -1), be_const_closure(Matter_Device_update_remotes_info_closure) },
        { be_const_key_weak(plugins, -1), be_const_var(1) },
        { be_const_key_weak(profiler, -1), be_const_var(7) },
        { be_const_key_weak(_instantiate_plugins_from_config, -1), be_const_closure(Matter_Device__instantiate_plugins_from_config_closure) },
        { be_const_key_weak(every_50ms, 22), be_const_closure(Matter_Device_every_50ms_closure) },
        { be_const_key_weak(commissioning_L, -1), be_const_var(17) },
        { be_const_key_weak(PASE_TIMEOUT, -1), be_const_int(600) },
        { be_const_key_weak(mdns_announce_op_discovery_all_fabrics, -1), be_const_closure(Matter_Device_mdns_announce_op_discovery_all_fabrics_closure) },
        { be_const_key_weak(event_fabrics_saved, -1), be_const_closure(Matter_Device_event_fabrics_saved_closure) },
        { be_const_key_weak(every_250ms, -1), be_const_closure(Matter_Device_every_250ms_closure) },
        { be_const_key_weak(autoconf_device, -1), be_const_closure(Matter_Device_autoconf_device_closure) },
        { be_const_key_weak(root_passcode, -1), be_const_var(29) },
        { be_const_key_weak(save_param, -1), be_const_closure(Matter_Device_save_param_closure) },
        { be_const_key_weak(mdns_announce_PASE, -1), be_const_closure(Matter_Device_mdns_announce_PASE_closure) },
        { be_const_key_weak(plugins_config_remotes, -1), be_const_var(5) },
        { be_const_key_weak(root_w0, -1), be_const_var(35) },
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
