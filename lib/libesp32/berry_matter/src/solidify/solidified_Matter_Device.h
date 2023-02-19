/* Solidification of Matter_Device.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"

extern const bclass be_class_Matter_Device;

/********************************************************************
** Solidified function: mdns_announce_op_discovery_all_sessions
********************************************************************/
be_local_closure(Matter_Device_mdns_announce_op_discovery_all_sessions,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 5]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(get_deviceid),
    /* K2   */  be_nested_str_weak(get_fabric),
    /* K3   */  be_nested_str_weak(mdns_announce_op_discovery),
    /* K4   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(mdns_announce_op_discovery_all_sessions),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x88080500,  //  0002  GETMBR	R2	R2	K0
      0x7C040200,  //  0003  CALL	R1	1
      0xA802000B,  //  0004  EXBLK	0	#0011
      0x5C080200,  //  0005  MOVE	R2	R1
      0x7C080000,  //  0006  CALL	R2	0
      0x8C0C0501,  //  0007  GETMET	R3	R2	K1
      0x7C0C0200,  //  0008  CALL	R3	1
      0x780E0005,  //  0009  JMPF	R3	#0010
      0x8C0C0502,  //  000A  GETMET	R3	R2	K2
      0x7C0C0200,  //  000B  CALL	R3	1
      0x780E0002,  //  000C  JMPF	R3	#0010
      0x8C0C0103,  //  000D  GETMET	R3	R0	K3
      0x5C140400,  //  000E  MOVE	R5	R2
      0x7C0C0400,  //  000F  CALL	R3	2
      0x7001FFF3,  //  0010  JMP		#0005
      0x58040004,  //  0011  LDCONST	R1	K4
      0xAC040200,  //  0012  CATCH	R1	1	0
      0xB0080000,  //  0013  RAISE	2	R0	R0
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_operational_dicovery
********************************************************************/
be_local_closure(Matter_Device_start_operational_dicovery,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[13]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(mdns),
    /* K2   */  be_nested_str_weak(string),
    /* K3   */  be_nested_str_weak(salt),
    /* K4   */  be_nested_str_weak(w0),
    /* K5   */  be_nested_str_weak(w1),
    /* K6   */  be_nested_str_weak(L),
    /* K7   */  be_nested_str_weak(set_no_expiration),
    /* K8   */  be_nested_str_weak(set_persist),
    /* K9   */  be_nested_str_weak(close),
    /* K10  */  be_nested_str_weak(sessions),
    /* K11  */  be_nested_str_weak(save),
    /* K12  */  be_nested_str_weak(mdns_announce_op_discovery),
    }),
    be_str_weak(start_operational_dicovery),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0xA4120400,  //  0002  IMPORT	R4	K2
      0x4C140000,  //  0003  LDNIL	R5
      0x90020605,  //  0004  SETMBR	R0	K3	R5
      0x4C140000,  //  0005  LDNIL	R5
      0x90020805,  //  0006  SETMBR	R0	K4	R5
      0x4C140000,  //  0007  LDNIL	R5
      0x90020A05,  //  0008  SETMBR	R0	K5	R5
      0x4C140000,  //  0009  LDNIL	R5
      0x90020C05,  //  000A  SETMBR	R0	K6	R5
      0x8C140307,  //  000B  GETMET	R5	R1	K7
      0x7C140200,  //  000C  CALL	R5	1
      0x8C140308,  //  000D  GETMET	R5	R1	K8
      0x501C0200,  //  000E  LDBOOL	R7	1	0
      0x7C140400,  //  000F  CALL	R5	2
      0x8C140309,  //  0010  GETMET	R5	R1	K9
      0x7C140200,  //  0011  CALL	R5	1
      0x8814010A,  //  0012  GETMBR	R5	R0	K10
      0x8C140B0B,  //  0013  GETMET	R5	R5	K11
      0x7C140200,  //  0014  CALL	R5	1
      0x8C14010C,  //  0015  GETMET	R5	R0	K12
      0x5C1C0200,  //  0016  MOVE	R7	R1
      0x7C140400,  //  0017  CALL	R5	2
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
        ( &(const bvalue[ 6]) {     /* constants */
        /* K0   */  be_nested_str_weak(start_udp),
        /* K1   */  be_nested_str_weak(UDP_PORT),
        /* K2   */  be_nested_str_weak(tasmota),
        /* K3   */  be_nested_str_weak(remove_rule),
        /* K4   */  be_nested_str_weak(Wifi_X23Connected),
        /* K5   */  be_nested_str_weak(matter_device_udp),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[11]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x68080000,  //  0002  GETUPV	R2	U0
          0x88080501,  //  0003  GETMBR	R2	R2	K1
          0x7C000400,  //  0004  CALL	R0	2
          0xB8020400,  //  0005  GETNGBL	R0	K2
          0x8C000103,  //  0006  GETMET	R0	R0	K3
          0x58080004,  //  0007  LDCONST	R2	K4
          0x580C0005,  //  0008  LDCONST	R3	K5
          0x7C000600,  //  0009  CALL	R0	3
          0x80000000,  //  000A  RET	0
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
        ( &(const bvalue[ 6]) {     /* constants */
        /* K0   */  be_nested_str_weak(start_udp),
        /* K1   */  be_nested_str_weak(UDP_PORT),
        /* K2   */  be_nested_str_weak(tasmota),
        /* K3   */  be_nested_str_weak(remove_rule),
        /* K4   */  be_nested_str_weak(Eth_X23Connected),
        /* K5   */  be_nested_str_weak(matter_device_udp),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[11]) {  /* code */
          0x68000000,  //  0000  GETUPV	R0	U0
          0x8C000100,  //  0001  GETMET	R0	R0	K0
          0x68080000,  //  0002  GETUPV	R2	U0
          0x88080501,  //  0003  GETMBR	R2	R2	K1
          0x7C000400,  //  0004  CALL	R0	2
          0xB8020400,  //  0005  GETNGBL	R0	K2
          0x8C000103,  //  0006  GETMET	R0	R0	K3
          0x58080004,  //  0007  LDCONST	R2	K4
          0x580C0005,  //  0008  LDCONST	R3	K5
          0x7C000600,  //  0009  CALL	R0	3
          0x80000000,  //  000A  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    ( &(const bvalue[39]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(tasmota),
    /* K3   */  be_nested_str_weak(get_option),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(MATTER_OPTION),
    /* K6   */  be_nested_str_weak(UI),
    /* K7   */  be_nested_str_weak(plugins),
    /* K8   */  be_nested_str_weak(vendorid),
    /* K9   */  be_nested_str_weak(VENDOR_ID),
    /* K10  */  be_nested_str_weak(productid),
    /* K11  */  be_nested_str_weak(PRODUCT_ID),
    /* K12  */  be_nested_str_weak(iterations),
    /* K13  */  be_nested_str_weak(PBKDF_ITERATIONS),
    /* K14  */  be_nested_str_weak(load_param),
    /* K15  */  be_nested_str_weak(commissioning_instance_wifi),
    /* K16  */  be_nested_str_weak(random),
    /* K17  */  be_nested_str_weak(tohex),
    /* K18  */  be_nested_str_weak(commissioning_instance_eth),
    /* K19  */  be_nested_str_weak(sessions),
    /* K20  */  be_nested_str_weak(Session_Store),
    /* K21  */  be_nested_str_weak(load),
    /* K22  */  be_nested_str_weak(message_handler),
    /* K23  */  be_nested_str_weak(MessageHandler),
    /* K24  */  be_nested_str_weak(ui),
    /* K25  */  be_nested_str_weak(push),
    /* K26  */  be_nested_str_weak(Plugin_Root),
    /* K27  */  be_nested_str_weak(Plugin_OnOff),
    /* K28  */  be_nested_str_weak(start_mdns_announce_hostnames),
    /* K29  */  be_nested_str_weak(wifi),
    /* K30  */  be_nested_str_weak(up),
    /* K31  */  be_nested_str_weak(start_udp),
    /* K32  */  be_nested_str_weak(UDP_PORT),
    /* K33  */  be_nested_str_weak(add_rule),
    /* K34  */  be_nested_str_weak(Wifi_X23Connected),
    /* K35  */  be_nested_str_weak(eth),
    /* K36  */  be_nested_str_weak(Eth_X23Connected),
    /* K37  */  be_nested_str_weak(start_basic_commissioning),
    /* K38  */  be_nested_str_weak(add_driver),
    }),
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[107]) {  /* code */
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
      0x600C0012,  //  000D  GETGBL	R3	G18
      0x7C0C0000,  //  000E  CALL	R3	0
      0x90020E03,  //  000F  SETMBR	R0	K7	R3
      0x880C0109,  //  0010  GETMBR	R3	R0	K9
      0x90021003,  //  0011  SETMBR	R0	K8	R3
      0x880C010B,  //  0012  GETMBR	R3	R0	K11
      0x90021403,  //  0013  SETMBR	R0	K10	R3
      0x880C010D,  //  0014  GETMBR	R3	R0	K13
      0x90021803,  //  0015  SETMBR	R0	K12	R3
      0x8C0C010E,  //  0016  GETMET	R3	R0	K14
      0x7C0C0200,  //  0017  CALL	R3	1
      0x8C0C0310,  //  0018  GETMET	R3	R1	K16
      0x54160007,  //  0019  LDINT	R5	8
      0x7C0C0400,  //  001A  CALL	R3	2
      0x8C0C0711,  //  001B  GETMET	R3	R3	K17
      0x7C0C0200,  //  001C  CALL	R3	1
      0x90021E03,  //  001D  SETMBR	R0	K15	R3
      0x8C0C0310,  //  001E  GETMET	R3	R1	K16
      0x54160007,  //  001F  LDINT	R5	8
      0x7C0C0400,  //  0020  CALL	R3	2
      0x8C0C0711,  //  0021  GETMET	R3	R3	K17
      0x7C0C0200,  //  0022  CALL	R3	1
      0x90022403,  //  0023  SETMBR	R0	K18	R3
      0xB80E0800,  //  0024  GETNGBL	R3	K4
      0x8C0C0714,  //  0025  GETMET	R3	R3	K20
      0x7C0C0200,  //  0026  CALL	R3	1
      0x90022603,  //  0027  SETMBR	R0	K19	R3
      0x880C0113,  //  0028  GETMBR	R3	R0	K19
      0x8C0C0715,  //  0029  GETMET	R3	R3	K21
      0x7C0C0200,  //  002A  CALL	R3	1
      0xB80E0800,  //  002B  GETNGBL	R3	K4
      0x8C0C0717,  //  002C  GETMET	R3	R3	K23
      0x5C140000,  //  002D  MOVE	R5	R0
      0x7C0C0400,  //  002E  CALL	R3	2
      0x90022C03,  //  002F  SETMBR	R0	K22	R3
      0xB80E0800,  //  0030  GETNGBL	R3	K4
      0x8C0C0706,  //  0031  GETMET	R3	R3	K6
      0x5C140000,  //  0032  MOVE	R5	R0
      0x7C0C0400,  //  0033  CALL	R3	2
      0x90023003,  //  0034  SETMBR	R0	K24	R3
      0x880C0107,  //  0035  GETMBR	R3	R0	K7
      0x8C0C0719,  //  0036  GETMET	R3	R3	K25
      0xB8160800,  //  0037  GETNGBL	R5	K4
      0x8C140B1A,  //  0038  GETMET	R5	R5	K26
      0x5C1C0000,  //  0039  MOVE	R7	R0
      0x7C140400,  //  003A  CALL	R5	2
      0x7C0C0400,  //  003B  CALL	R3	2
      0x880C0107,  //  003C  GETMBR	R3	R0	K7
      0x8C0C0719,  //  003D  GETMET	R3	R3	K25
      0xB8160800,  //  003E  GETNGBL	R5	K4
      0x8C140B1B,  //  003F  GETMET	R5	R5	K27
      0x5C1C0000,  //  0040  MOVE	R7	R0
      0x7C140400,  //  0041  CALL	R5	2
      0x7C0C0400,  //  0042  CALL	R3	2
      0x8C0C011C,  //  0043  GETMET	R3	R0	K28
      0x7C0C0200,  //  0044  CALL	R3	1
      0xB80E0400,  //  0045  GETNGBL	R3	K2
      0x8C0C071D,  //  0046  GETMET	R3	R3	K29
      0x7C0C0200,  //  0047  CALL	R3	1
      0x940C071E,  //  0048  GETIDX	R3	R3	K30
      0x780E0003,  //  0049  JMPF	R3	#004E
      0x8C0C011F,  //  004A  GETMET	R3	R0	K31
      0x88140120,  //  004B  GETMBR	R5	R0	K32
      0x7C0C0400,  //  004C  CALL	R3	2
      0x70020005,  //  004D  JMP		#0054
      0xB80E0400,  //  004E  GETNGBL	R3	K2
      0x8C0C0721,  //  004F  GETMET	R3	R3	K33
      0x58140022,  //  0050  LDCONST	R5	K34
      0x84180000,  //  0051  CLOSURE	R6	P0
      0x5C1C0000,  //  0052  MOVE	R7	R0
      0x7C0C0800,  //  0053  CALL	R3	4
      0xB80E0400,  //  0054  GETNGBL	R3	K2
      0x8C0C0723,  //  0055  GETMET	R3	R3	K35
      0x7C0C0200,  //  0056  CALL	R3	1
      0x940C071E,  //  0057  GETIDX	R3	R3	K30
      0x780E0003,  //  0058  JMPF	R3	#005D
      0x8C0C011F,  //  0059  GETMET	R3	R0	K31
      0x88140120,  //  005A  GETMBR	R5	R0	K32
      0x7C0C0400,  //  005B  CALL	R3	2
      0x70020005,  //  005C  JMP		#0063
      0xB80E0400,  //  005D  GETNGBL	R3	K2
      0x8C0C0721,  //  005E  GETMET	R3	R3	K33
      0x58140024,  //  005F  LDCONST	R5	K36
      0x84180001,  //  0060  CLOSURE	R6	P1
      0x5C1C0000,  //  0061  MOVE	R7	R0
      0x7C0C0800,  //  0062  CALL	R3	4
      0x8C0C0125,  //  0063  GETMET	R3	R0	K37
      0x7C0C0200,  //  0064  CALL	R3	1
      0xB80E0400,  //  0065  GETNGBL	R3	K2
      0x8C0C0726,  //  0066  GETMET	R3	R3	K38
      0x5C140000,  //  0067  MOVE	R5	R0
      0x7C0C0400,  //  0068  CALL	R3	2
      0xA0000000,  //  0069  CLOSE	R0
      0x80000000,  //  006A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compute_pbkdf
********************************************************************/
be_local_closure(Matter_Device_compute_pbkdf,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[24]) {     /* constants */
    /* K0   */  be_nested_str_weak(crypto),
    /* K1   */  be_nested_str_weak(salt),
    /* K2   */  be_nested_str_weak(random),
    /* K3   */  be_nested_str_weak(add),
    /* K4   */  be_nested_str_weak(PBKDF2_HMAC_SHA256),
    /* K5   */  be_nested_str_weak(derive),
    /* K6   */  be_nested_str_weak(iterations),
    /* K7   */  be_const_int(0),
    /* K8   */  be_nested_str_weak(w0),
    /* K9   */  be_nested_str_weak(EC_P256),
    /* K10  */  be_nested_str_weak(mod),
    /* K11  */  be_nested_str_weak(w1),
    /* K12  */  be_nested_str_weak(L),
    /* K13  */  be_nested_str_weak(public_key),
    /* K14  */  be_nested_str_weak(tasmota),
    /* K15  */  be_nested_str_weak(log),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A_X2A),
    /* K17  */  be_const_int(3),
    /* K18  */  be_nested_str_weak(MTR_X3A_X20salt_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K19  */  be_nested_str_weak(tohex),
    /* K20  */  be_nested_str_weak(MTR_X3A_X20passcode_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K21  */  be_nested_str_weak(MTR_X3A_X20w0_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K22  */  be_nested_str_weak(MTR_X3A_X20w1_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20L_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X20_X3D_X20),
    }),
    be_str_weak(compute_pbkdf),
    &be_const_str_solidified,
    ( &(const binstruction[94]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0x8C0C0502,  //  0001  GETMET	R3	R2	K2
      0x5416000F,  //  0002  LDINT	R5	16
      0x7C0C0400,  //  0003  CALL	R3	2
      0x90020203,  //  0004  SETMBR	R0	K1	R3
      0x600C0015,  //  0005  GETGBL	R3	G21
      0x7C0C0000,  //  0006  CALL	R3	0
      0x8C0C0703,  //  0007  GETMET	R3	R3	K3
      0x5C140200,  //  0008  MOVE	R5	R1
      0x541A0003,  //  0009  LDINT	R6	4
      0x7C0C0600,  //  000A  CALL	R3	3
      0x8C100504,  //  000B  GETMET	R4	R2	K4
      0x7C100200,  //  000C  CALL	R4	1
      0x8C100905,  //  000D  GETMET	R4	R4	K5
      0x5C180600,  //  000E  MOVE	R6	R3
      0x881C0101,  //  000F  GETMBR	R7	R0	K1
      0x88200106,  //  0010  GETMBR	R8	R0	K6
      0x5426004F,  //  0011  LDINT	R9	80
      0x7C100A00,  //  0012  CALL	R4	5
      0x54160026,  //  0013  LDINT	R5	39
      0x40160E05,  //  0014  CONNECT	R5	K7	R5
      0x94140805,  //  0015  GETIDX	R5	R4	R5
      0x541A0027,  //  0016  LDINT	R6	40
      0x541E004E,  //  0017  LDINT	R7	79
      0x40180C07,  //  0018  CONNECT	R6	R6	R7
      0x94180806,  //  0019  GETIDX	R6	R4	R6
      0x8C1C0509,  //  001A  GETMET	R7	R2	K9
      0x7C1C0200,  //  001B  CALL	R7	1
      0x8C1C0F0A,  //  001C  GETMET	R7	R7	K10
      0x5C240A00,  //  001D  MOVE	R9	R5
      0x7C1C0400,  //  001E  CALL	R7	2
      0x90021007,  //  001F  SETMBR	R0	K8	R7
      0x8C1C0509,  //  0020  GETMET	R7	R2	K9
      0x7C1C0200,  //  0021  CALL	R7	1
      0x8C1C0F0A,  //  0022  GETMET	R7	R7	K10
      0x5C240C00,  //  0023  MOVE	R9	R6
      0x7C1C0400,  //  0024  CALL	R7	2
      0x90021607,  //  0025  SETMBR	R0	K11	R7
      0x8C1C0509,  //  0026  GETMET	R7	R2	K9
      0x7C1C0200,  //  0027  CALL	R7	1
      0x8C1C0F0D,  //  0028  GETMET	R7	R7	K13
      0x8824010B,  //  0029  GETMBR	R9	R0	K11
      0x7C1C0400,  //  002A  CALL	R7	2
      0x90021807,  //  002B  SETMBR	R0	K12	R7
      0xB81E1C00,  //  002C  GETNGBL	R7	K14
      0x8C1C0F0F,  //  002D  GETMET	R7	R7	K15
      0x58240010,  //  002E  LDCONST	R9	K16
      0x58280011,  //  002F  LDCONST	R10	K17
      0x7C1C0600,  //  0030  CALL	R7	3
      0xB81E1C00,  //  0031  GETNGBL	R7	K14
      0x8C1C0F0F,  //  0032  GETMET	R7	R7	K15
      0x88240101,  //  0033  GETMBR	R9	R0	K1
      0x8C241313,  //  0034  GETMET	R9	R9	K19
      0x7C240200,  //  0035  CALL	R9	1
      0x00262409,  //  0036  ADD	R9	K18	R9
      0x58280011,  //  0037  LDCONST	R10	K17
      0x7C1C0600,  //  0038  CALL	R7	3
      0xB81E1C00,  //  0039  GETNGBL	R7	K14
      0x8C1C0F0F,  //  003A  GETMET	R7	R7	K15
      0x8C240713,  //  003B  GETMET	R9	R3	K19
      0x7C240200,  //  003C  CALL	R9	1
      0x00262809,  //  003D  ADD	R9	K20	R9
      0x58280011,  //  003E  LDCONST	R10	K17
      0x7C1C0600,  //  003F  CALL	R7	3
      0xB81E1C00,  //  0040  GETNGBL	R7	K14
      0x8C1C0F0F,  //  0041  GETMET	R7	R7	K15
      0x88240108,  //  0042  GETMBR	R9	R0	K8
      0x8C241313,  //  0043  GETMET	R9	R9	K19
      0x7C240200,  //  0044  CALL	R9	1
      0x00262A09,  //  0045  ADD	R9	K21	R9
      0x58280011,  //  0046  LDCONST	R10	K17
      0x7C1C0600,  //  0047  CALL	R7	3
      0xB81E1C00,  //  0048  GETNGBL	R7	K14
      0x8C1C0F0F,  //  0049  GETMET	R7	R7	K15
      0x8824010B,  //  004A  GETMBR	R9	R0	K11
      0x8C241313,  //  004B  GETMET	R9	R9	K19
      0x7C240200,  //  004C  CALL	R9	1
      0x00262C09,  //  004D  ADD	R9	K22	R9
      0x58280011,  //  004E  LDCONST	R10	K17
      0x7C1C0600,  //  004F  CALL	R7	3
      0xB81E1C00,  //  0050  GETNGBL	R7	K14
      0x8C1C0F0F,  //  0051  GETMET	R7	R7	K15
      0x8824010C,  //  0052  GETMBR	R9	R0	K12
      0x8C241313,  //  0053  GETMET	R9	R9	K19
      0x7C240200,  //  0054  CALL	R9	1
      0x00262E09,  //  0055  ADD	R9	K23	R9
      0x58280011,  //  0056  LDCONST	R10	K17
      0x7C1C0600,  //  0057  CALL	R7	3
      0xB81E1C00,  //  0058  GETNGBL	R7	K14
      0x8C1C0F0F,  //  0059  GETMET	R7	R7	K15
      0x58240010,  //  005A  LDCONST	R9	K16
      0x58280011,  //  005B  LDCONST	R10	K17
      0x7C1C0600,  //  005C  CALL	R7	3
      0x80000000,  //  005D  RET	0
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
    ( &(const bvalue[15]) {     /* constants */
    /* K0   */  be_nested_str_weak(json),
    /* K1   */  be_nested_str_weak(dump),
    /* K2   */  be_nested_str_weak(distinguish),
    /* K3   */  be_nested_str_weak(discriminator),
    /* K4   */  be_nested_str_weak(passcode),
    /* K5   */  be_nested_str_weak(string),
    /* K6   */  be_nested_str_weak(FILENAME),
    /* K7   */  be_nested_str_weak(w),
    /* K8   */  be_nested_str_weak(write),
    /* K9   */  be_nested_str_weak(close),
    /* K10  */  be_nested_str_weak(tasmota),
    /* K11  */  be_nested_str_weak(log),
    /* K12  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Asave_X20Exception_X3A),
    /* K13  */  be_nested_str_weak(_X7C),
    /* K14  */  be_const_int(2),
    }),
    be_str_weak(save_param),
    &be_const_str_solidified,
    ( &(const binstruction[43]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x8C080301,  //  0001  GETMET	R2	R1	K1
      0x60100013,  //  0002  GETGBL	R4	G19
      0x7C100000,  //  0003  CALL	R4	0
      0x88140103,  //  0004  GETMBR	R5	R0	K3
      0x98120405,  //  0005  SETIDX	R4	K2	R5
      0x88140104,  //  0006  GETMBR	R5	R0	K4
      0x98120805,  //  0007  SETIDX	R4	K4	R5
      0x7C080400,  //  0008  CALL	R2	2
      0xA802000D,  //  0009  EXBLK	0	#0018
      0xA40E0A00,  //  000A  IMPORT	R3	K5
      0x60100011,  //  000B  GETGBL	R4	G17
      0x88140106,  //  000C  GETMBR	R5	R0	K6
      0x58180007,  //  000D  LDCONST	R6	K7
      0x7C100400,  //  000E  CALL	R4	2
      0x8C140908,  //  000F  GETMET	R5	R4	K8
      0x5C1C0400,  //  0010  MOVE	R7	R2
      0x7C140400,  //  0011  CALL	R5	2
      0x8C140909,  //  0012  GETMET	R5	R4	K9
      0x7C140200,  //  0013  CALL	R5	1
      0xA8040001,  //  0014  EXBLK	1	1
      0x80040400,  //  0015  RET	1	R2
      0xA8040001,  //  0016  EXBLK	1	1
      0x70020011,  //  0017  JMP		#002A
      0xAC0C0002,  //  0018  CATCH	R3	0	2
      0x7002000E,  //  0019  JMP		#0029
      0xB8161400,  //  001A  GETNGBL	R5	K10
      0x8C140B0B,  //  001B  GETMET	R5	R5	K11
      0x601C0008,  //  001C  GETGBL	R7	G8
      0x5C200600,  //  001D  MOVE	R8	R3
      0x7C1C0200,  //  001E  CALL	R7	1
      0x001E1807,  //  001F  ADD	R7	K12	R7
      0x001C0F0D,  //  0020  ADD	R7	R7	K13
      0x60200008,  //  0021  GETGBL	R8	G8
      0x5C240800,  //  0022  MOVE	R9	R4
      0x7C200200,  //  0023  CALL	R8	1
      0x001C0E08,  //  0024  ADD	R7	R7	R8
      0x5820000E,  //  0025  LDCONST	R8	K14
      0x7C140600,  //  0026  CALL	R5	3
      0x80040400,  //  0027  RET	1	R2
      0x70020000,  //  0028  JMP		#002A
      0xB0080000,  //  0029  RAISE	2	R0	R0
      0x80000000,  //  002A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_operational_dicovery_deferred
********************************************************************/
be_local_closure(Matter_Device_start_operational_dicovery_deferred,   /* name */
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
        /* K0   */  be_nested_str_weak(start_operational_dicovery),
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
    be_str_weak(start_operational_dicovery_deferred),
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
** Solidified function: get_active_endpoints
********************************************************************/
be_local_closure(Matter_Device_get_active_endpoints,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 6]) {     /* constants */
    /* K0   */  be_nested_str_weak(plugins),
    /* K1   */  be_nested_str_weak(get_endpoints),
    /* K2   */  be_const_int(0),
    /* K3   */  be_nested_str_weak(find),
    /* K4   */  be_nested_str_weak(push),
    /* K5   */  be_nested_str_weak(stop_iteration),
    }),
    be_str_weak(get_active_endpoints),
    &be_const_str_solidified,
    ( &(const binstruction[38]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x600C0010,  //  0002  GETGBL	R3	G16
      0x88100100,  //  0003  GETMBR	R4	R0	K0
      0x7C0C0200,  //  0004  CALL	R3	1
      0xA802001B,  //  0005  EXBLK	0	#0022
      0x5C100600,  //  0006  MOVE	R4	R3
      0x7C100000,  //  0007  CALL	R4	0
      0x8C140901,  //  0008  GETMET	R5	R4	K1
      0x7C140200,  //  0009  CALL	R5	1
      0x60180010,  //  000A  GETGBL	R6	G16
      0x5C1C0A00,  //  000B  MOVE	R7	R5
      0x7C180200,  //  000C  CALL	R6	1
      0xA802000F,  //  000D  EXBLK	0	#001E
      0x5C1C0C00,  //  000E  MOVE	R7	R6
      0x7C1C0000,  //  000F  CALL	R7	0
      0x78060002,  //  0010  JMPF	R1	#0014
      0x1C200F02,  //  0011  EQ	R8	R7	K2
      0x78220000,  //  0012  JMPF	R8	#0014
      0x7001FFF9,  //  0013  JMP		#000E
      0x8C200503,  //  0014  GETMET	R8	R2	K3
      0x5C280E00,  //  0015  MOVE	R10	R7
      0x7C200400,  //  0016  CALL	R8	2
      0x4C240000,  //  0017  LDNIL	R9
      0x1C201009,  //  0018  EQ	R8	R8	R9
      0x78220002,  //  0019  JMPF	R8	#001D
      0x8C200504,  //  001A  GETMET	R8	R2	K4
      0x5C280E00,  //  001B  MOVE	R10	R7
      0x7C200400,  //  001C  CALL	R8	2
      0x7001FFEF,  //  001D  JMP		#000E
      0x58180005,  //  001E  LDCONST	R6	K5
      0xAC180200,  //  001F  CATCH	R6	1	0
      0xB0080000,  //  0020  RAISE	2	R0	R0
      0x7001FFE3,  //  0021  JMP		#0006
      0x580C0005,  //  0022  LDCONST	R3	K5
      0xAC0C0200,  //  0023  CATCH	R3	1	0
      0xB0080000,  //  0024  RAISE	2	R0	R0
      0x80040400,  //  0025  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: msg_send
********************************************************************/
be_local_closure(Matter_Device_msg_send,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    5,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(udp_server),
    /* K1   */  be_nested_str_weak(send_response),
    }),
    be_str_weak(msg_send),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88140100,  //  0000  GETMBR	R5	R0	K0
      0x8C140B01,  //  0001  GETMET	R5	R5	K1
      0x5C1C0200,  //  0002  MOVE	R7	R1
      0x5C200400,  //  0003  MOVE	R8	R2
      0x5C240600,  //  0004  MOVE	R9	R3
      0x5C280800,  //  0005  MOVE	R10	R4
      0x7C140A00,  //  0006  CALL	R5	5
      0x80040A00,  //  0007  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: finish_commissioning
********************************************************************/
be_local_closure(Matter_Device_finish_commissioning,   /* name */
  be_nested_proto(
    1,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    0,                          /* has constants */
    NULL,                       /* no const */
    be_str_weak(finish_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(Matter_Device_every_second,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 3]) {     /* constants */
    /* K0   */  be_nested_str_weak(sessions),
    /* K1   */  be_nested_str_weak(every_second),
    /* K2   */  be_nested_str_weak(message_handler),
    }),
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x7C040200,  //  0002  CALL	R1	1
      0x88040102,  //  0003  GETMBR	R1	R0	K2
      0x8C040301,  //  0004  GETMET	R1	R1	K1
      0x7C040200,  //  0005  CALL	R1	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(Matter_Device_stop,   /* name */
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
    /* K0   */  be_nested_str_weak(udp_server),
    /* K1   */  be_nested_str_weak(stop),
    }),
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x78060002,  //  0001  JMPF	R1	#0005
      0x88040100,  //  0002  GETMBR	R1	R0	K0
      0x8C040301,  //  0003  GETMET	R1	R1	K1
      0x7C040200,  //  0004  CALL	R1	1
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_basic_commissioning
********************************************************************/
be_local_closure(Matter_Device_start_basic_commissioning,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 2]) {     /* constants */
    /* K0   */  be_nested_str_weak(compute_pbkdf),
    /* K1   */  be_nested_str_weak(passcode),
    }),
    be_str_weak(start_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8C040100,  //  0000  GETMET	R1	R0	K0
      0x880C0101,  //  0001  GETMBR	R3	R0	K1
      0x7C040400,  //  0002  CALL	R1	2
      0x80000000,  //  0003  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_param
********************************************************************/
be_local_closure(Matter_Device_load_param,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[22]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(crypto),
    /* K2   */  be_nested_str_weak(FILENAME),
    /* K3   */  be_nested_str_weak(read),
    /* K4   */  be_nested_str_weak(close),
    /* K5   */  be_nested_str_weak(json),
    /* K6   */  be_nested_str_weak(load),
    /* K7   */  be_nested_str_weak(discriminator),
    /* K8   */  be_nested_str_weak(find),
    /* K9   */  be_nested_str_weak(distinguish),
    /* K10  */  be_nested_str_weak(passcode),
    /* K11  */  be_nested_str_weak(io_error),
    /* K12  */  be_nested_str_weak(tasmota),
    /* K13  */  be_nested_str_weak(log),
    /* K14  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Aload_X20Exception_X3A),
    /* K15  */  be_nested_str_weak(_X7C),
    /* K16  */  be_const_int(2),
    /* K17  */  be_nested_str_weak(random),
    /* K18  */  be_nested_str_weak(get),
    /* K19  */  be_const_int(0),
    /* K20  */  be_nested_str_weak(PASSCODE_DEFAULT),
    /* K21  */  be_nested_str_weak(save_param),
    }),
    be_str_weak(load_param),
    &be_const_str_solidified,
    ( &(const binstruction[70]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0xA40A0200,  //  0001  IMPORT	R2	K1
      0xA8020014,  //  0002  EXBLK	0	#0018
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
      0x7C1C0400,  //  0010  CALL	R7	2
      0x90020E07,  //  0011  SETMBR	R0	K7	R7
      0x8C1C0D08,  //  0012  GETMET	R7	R6	K8
      0x5824000A,  //  0013  LDCONST	R9	K10
      0x7C1C0400,  //  0014  CALL	R7	2
      0x90021407,  //  0015  SETMBR	R0	K10	R7
      0xA8040001,  //  0016  EXBLK	1	1
      0x70020012,  //  0017  JMP		#002B
      0xAC0C0002,  //  0018  CATCH	R3	0	2
      0x7002000F,  //  0019  JMP		#002A
      0x2014070B,  //  001A  NE	R5	R3	K11
      0x7816000C,  //  001B  JMPF	R5	#0029
      0xB8161800,  //  001C  GETNGBL	R5	K12
      0x8C140B0D,  //  001D  GETMET	R5	R5	K13
      0x601C0008,  //  001E  GETGBL	R7	G8
      0x5C200600,  //  001F  MOVE	R8	R3
      0x7C1C0200,  //  0020  CALL	R7	1
      0x001E1C07,  //  0021  ADD	R7	K14	R7
      0x001C0F0F,  //  0022  ADD	R7	R7	K15
      0x60200008,  //  0023  GETGBL	R8	G8
      0x5C240800,  //  0024  MOVE	R9	R4
      0x7C200200,  //  0025  CALL	R8	1
      0x001C0E08,  //  0026  ADD	R7	R7	R8
      0x58200010,  //  0027  LDCONST	R8	K16
      0x7C140600,  //  0028  CALL	R5	3
      0x70020000,  //  0029  JMP		#002B
      0xB0080000,  //  002A  RAISE	2	R0	R0
      0x500C0000,  //  002B  LDBOOL	R3	0	0
      0x88100107,  //  002C  GETMBR	R4	R0	K7
      0x4C140000,  //  002D  LDNIL	R5
      0x1C100805,  //  002E  EQ	R4	R4	R5
      0x7812000A,  //  002F  JMPF	R4	#003B
      0x8C100511,  //  0030  GETMET	R4	R2	K17
      0x58180010,  //  0031  LDCONST	R6	K16
      0x7C100400,  //  0032  CALL	R4	2
      0x8C100912,  //  0033  GETMET	R4	R4	K18
      0x58180013,  //  0034  LDCONST	R6	K19
      0x581C0010,  //  0035  LDCONST	R7	K16
      0x7C100600,  //  0036  CALL	R4	3
      0x54160FFE,  //  0037  LDINT	R5	4095
      0x2C100805,  //  0038  AND	R4	R4	R5
      0x90020E04,  //  0039  SETMBR	R0	K7	R4
      0x500C0200,  //  003A  LDBOOL	R3	1	0
      0x8810010A,  //  003B  GETMBR	R4	R0	K10
      0x4C140000,  //  003C  LDNIL	R5
      0x1C100805,  //  003D  EQ	R4	R4	R5
      0x78120002,  //  003E  JMPF	R4	#0042
      0x88100114,  //  003F  GETMBR	R4	R0	K20
      0x90021404,  //  0040  SETMBR	R0	K10	R4
      0x500C0200,  //  0041  LDBOOL	R3	1	0
      0x780E0001,  //  0042  JMPF	R3	#0045
      0x8C100115,  //  0043  GETMET	R4	R0	K21
      0x7C100200,  //  0044  CALL	R4	1
      0x80000000,  //  0045  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_attribute_expansion
********************************************************************/
be_local_closure(Matter_Device_process_attribute_expansion,   /* name */
  be_nested_proto(
    32,                          /* nstack */
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
    ( &(const bvalue[27]) {     /* constants */
    /* K0   */  be_nested_str_weak(string),
    /* K1   */  be_nested_str_weak(endpoint),
    /* K2   */  be_nested_str_weak(cluster),
    /* K3   */  be_nested_str_weak(attribute),
    /* K4   */  be_nested_str_weak(tasmota),
    /* K5   */  be_nested_str_weak(log),
    /* K6   */  be_nested_str_weak(format),
    /* K7   */  be_nested_str_weak(MTR_X3A_X20process_attribute_expansion_X20_X25s),
    /* K8   */  be_nested_str_weak(MTR_X3A_X20endpoint_X3D_X25s_X20cluster_X3D_X25s_X20attribute_X3D_X25s),
    /* K9   */  be_nested_str_weak(plugins),
    /* K10  */  be_nested_str_weak(get_endpoints),
    /* K11  */  be_nested_str_weak(MTR_X3A_X20pi_X3D_X25s_X20ep_list_X3D_X25s),
    /* K12  */  be_nested_str_weak(contains),
    /* K13  */  be_nested_str_weak(get_cluster_list),
    /* K14  */  be_nested_str_weak(MTR_X3A_X20pi_X3D_X25s_X20ep_X3D_X25s_X20cl_list_X3D_X25s),
    /* K15  */  be_nested_str_weak(get_attribute_list),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20pi_X3D_X25s_X20ep_X3D_X25s_X20cl_X3D_X25s_X20at_list_X3D_X25s),
    /* K17  */  be_nested_str_weak(push),
    /* K18  */  be_nested_str_weak(stop_iteration),
    /* K19  */  be_nested_str_weak(MTR_X3A_X20expansion_X20_X5B_X2502X_X5D_X2504X_X2F_X2504X),
    /* K20  */  be_const_int(3),
    /* K21  */  be_nested_str_weak(status),
    /* K22  */  be_nested_str_weak(matter),
    /* K23  */  be_nested_str_weak(UNSUPPORTED_ENDPOINT),
    /* K24  */  be_nested_str_weak(UNSUPPORTED_CLUSTER),
    /* K25  */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
    /* K26  */  be_nested_str_weak(UNREPORTABLE_ATTRIBUTE),
    }),
    be_str_weak(process_attribute_expansion),
    &be_const_str_solidified,
    ( &(const binstruction[294]) {  /* code */
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
      0xB8320800,  //  0016  GETNGBL	R12	K4
      0x8C301905,  //  0017  GETMET	R12	R12	K5
      0x8C380906,  //  0018  GETMET	R14	R4	K6
      0x58400007,  //  0019  LDCONST	R16	K7
      0x60440008,  //  001A  GETGBL	R17	G8
      0x5C480200,  //  001B  MOVE	R18	R1
      0x7C440200,  //  001C  CALL	R17	1
      0x7C380600,  //  001D  CALL	R14	3
      0x543E0003,  //  001E  LDINT	R15	4
      0x7C300600,  //  001F  CALL	R12	3
      0x60300013,  //  0020  GETGBL	R12	G19
      0x7C300000,  //  0021  CALL	R12	0
      0xB8360800,  //  0022  GETNGBL	R13	K4
      0x8C341B05,  //  0023  GETMET	R13	R13	K5
      0x8C3C0906,  //  0024  GETMET	R15	R4	K6
      0x58440008,  //  0025  LDCONST	R17	K8
      0x5C480A00,  //  0026  MOVE	R18	R5
      0x5C4C0E00,  //  0027  MOVE	R19	R7
      0x5C501200,  //  0028  MOVE	R20	R9
      0x7C3C0A00,  //  0029  CALL	R15	5
      0x54420003,  //  002A  LDINT	R16	4
      0x7C340600,  //  002B  CALL	R13	3
      0x60340010,  //  002C  GETGBL	R13	G16
      0x88380109,  //  002D  GETMBR	R14	R0	K9
      0x7C340200,  //  002E  CALL	R13	1
      0xA802008F,  //  002F  EXBLK	0	#00C0
      0x5C381A00,  //  0030  MOVE	R14	R13
      0x7C380000,  //  0031  CALL	R14	0
      0x8C3C1D0A,  //  0032  GETMET	R15	R14	K10
      0x7C3C0200,  //  0033  CALL	R15	1
      0xB8420800,  //  0034  GETNGBL	R16	K4
      0x8C402105,  //  0035  GETMET	R16	R16	K5
      0x8C480906,  //  0036  GETMET	R18	R4	K6
      0x5850000B,  //  0037  LDCONST	R20	K11
      0x60540008,  //  0038  GETGBL	R21	G8
      0x5C581C00,  //  0039  MOVE	R22	R14
      0x7C540200,  //  003A  CALL	R21	1
      0x60580008,  //  003B  GETGBL	R22	G8
      0x5C5C1E00,  //  003C  MOVE	R23	R15
      0x7C580200,  //  003D  CALL	R22	1
      0x7C480800,  //  003E  CALL	R18	4
      0x544E0003,  //  003F  LDINT	R19	4
      0x7C400600,  //  0040  CALL	R16	3
      0x60400010,  //  0041  GETGBL	R16	G16
      0x5C441E00,  //  0042  MOVE	R17	R15
      0x7C400200,  //  0043  CALL	R16	1
      0xA8020076,  //  0044  EXBLK	0	#00BC
      0x5C442000,  //  0045  MOVE	R17	R16
      0x7C440000,  //  0046  CALL	R17	0
      0x4C480000,  //  0047  LDNIL	R18
      0x20480A12,  //  0048  NE	R18	R5	R18
      0x784A0002,  //  0049  JMPF	R18	#004D
      0x20482205,  //  004A  NE	R18	R17	R5
      0x784A0000,  //  004B  JMPF	R18	#004D
      0x7001FFF7,  //  004C  JMP		#0045
      0x8C48190C,  //  004D  GETMET	R18	R12	K12
      0x5C502200,  //  004E  MOVE	R20	R17
      0x7C480400,  //  004F  CALL	R18	2
      0x744A0002,  //  0050  JMPT	R18	#0054
      0x60480013,  //  0051  GETGBL	R18	G19
      0x7C480000,  //  0052  CALL	R18	0
      0x98302212,  //  0053  SETIDX	R12	R17	R18
      0x50180200,  //  0054  LDBOOL	R6	1	0
      0x8C481D0D,  //  0055  GETMET	R18	R14	K13
      0x5C502200,  //  0056  MOVE	R20	R17
      0x7C480400,  //  0057  CALL	R18	2
      0xB84E0800,  //  0058  GETNGBL	R19	K4
      0x8C4C2705,  //  0059  GETMET	R19	R19	K5
      0x8C540906,  //  005A  GETMET	R21	R4	K6
      0x585C000E,  //  005B  LDCONST	R23	K14
      0x60600008,  //  005C  GETGBL	R24	G8
      0x5C641C00,  //  005D  MOVE	R25	R14
      0x7C600200,  //  005E  CALL	R24	1
      0x60640008,  //  005F  GETGBL	R25	G8
      0x5C682200,  //  0060  MOVE	R26	R17
      0x7C640200,  //  0061  CALL	R25	1
      0x60680008,  //  0062  GETGBL	R26	G8
      0x5C6C2400,  //  0063  MOVE	R27	R18
      0x7C680200,  //  0064  CALL	R26	1
      0x7C540A00,  //  0065  CALL	R21	5
      0x545A0003,  //  0066  LDINT	R22	4
      0x7C4C0600,  //  0067  CALL	R19	3
      0x604C0010,  //  0068  GETGBL	R19	G16
      0x5C502400,  //  0069  MOVE	R20	R18
      0x7C4C0200,  //  006A  CALL	R19	1
      0xA802004B,  //  006B  EXBLK	0	#00B8
      0x5C502600,  //  006C  MOVE	R20	R19
      0x7C500000,  //  006D  CALL	R20	0
      0x4C540000,  //  006E  LDNIL	R21
      0x20540E15,  //  006F  NE	R21	R7	R21
      0x78560002,  //  0070  JMPF	R21	#0074
      0x20542807,  //  0071  NE	R21	R20	R7
      0x78560000,  //  0072  JMPF	R21	#0074
      0x7001FFF7,  //  0073  JMP		#006C
      0x94541811,  //  0074  GETIDX	R21	R12	R17
      0x8C542B0C,  //  0075  GETMET	R21	R21	K12
      0x5C5C2800,  //  0076  MOVE	R23	R20
      0x7C540400,  //  0077  CALL	R21	2
      0x74560003,  //  0078  JMPT	R21	#007D
      0x94541811,  //  0079  GETIDX	R21	R12	R17
      0x60580013,  //  007A  GETGBL	R22	G19
      0x7C580000,  //  007B  CALL	R22	0
      0x98542816,  //  007C  SETIDX	R21	R20	R22
      0x50200200,  //  007D  LDBOOL	R8	1	0
      0x8C541D0F,  //  007E  GETMET	R21	R14	K15
      0x5C5C2200,  //  007F  MOVE	R23	R17
      0x5C602800,  //  0080  MOVE	R24	R20
      0x7C540600,  //  0081  CALL	R21	3
      0xB85A0800,  //  0082  GETNGBL	R22	K4
      0x8C582D05,  //  0083  GETMET	R22	R22	K5
      0x8C600906,  //  0084  GETMET	R24	R4	K6
      0x58680010,  //  0085  LDCONST	R26	K16
      0x606C0008,  //  0086  GETGBL	R27	G8
      0x5C701C00,  //  0087  MOVE	R28	R14
      0x7C6C0200,  //  0088  CALL	R27	1
      0x60700008,  //  0089  GETGBL	R28	G8
      0x5C742200,  //  008A  MOVE	R29	R17
      0x7C700200,  //  008B  CALL	R28	1
      0x60740008,  //  008C  GETGBL	R29	G8
      0x5C782800,  //  008D  MOVE	R30	R20
      0x7C740200,  //  008E  CALL	R29	1
      0x60780008,  //  008F  GETGBL	R30	G8
      0x5C7C2A00,  //  0090  MOVE	R31	R21
      0x7C780200,  //  0091  CALL	R30	1
      0x7C600C00,  //  0092  CALL	R24	6
      0x54660003,  //  0093  LDINT	R25	4
      0x7C580600,  //  0094  CALL	R22	3
      0x60580010,  //  0095  GETGBL	R22	G16
      0x5C5C2A00,  //  0096  MOVE	R23	R21
      0x7C580200,  //  0097  CALL	R22	1
      0xA802001A,  //  0098  EXBLK	0	#00B4
      0x5C5C2C00,  //  0099  MOVE	R23	R22
      0x7C5C0000,  //  009A  CALL	R23	0
      0x4C600000,  //  009B  LDNIL	R24
      0x20601218,  //  009C  NE	R24	R9	R24
      0x78620002,  //  009D  JMPF	R24	#00A1
      0x20602E09,  //  009E  NE	R24	R23	R9
      0x78620000,  //  009F  JMPF	R24	#00A1
      0x7001FFF7,  //  00A0  JMP		#0099
      0x94601811,  //  00A1  GETIDX	R24	R12	R17
      0x94603014,  //  00A2  GETIDX	R24	R24	R20
      0x8C60310C,  //  00A3  GETMET	R24	R24	K12
      0x5C682E00,  //  00A4  MOVE	R26	R23
      0x7C600400,  //  00A5  CALL	R24	2
      0x74620004,  //  00A6  JMPT	R24	#00AC
      0x94601811,  //  00A7  GETIDX	R24	R12	R17
      0x94603014,  //  00A8  GETIDX	R24	R24	R20
      0x60640012,  //  00A9  GETGBL	R25	G18
      0x7C640000,  //  00AA  CALL	R25	0
      0x98602E19,  //  00AB  SETIDX	R24	R23	R25
      0x50280200,  //  00AC  LDBOOL	R10	1	0
      0x94601811,  //  00AD  GETIDX	R24	R12	R17
      0x94603014,  //  00AE  GETIDX	R24	R24	R20
      0x94603017,  //  00AF  GETIDX	R24	R24	R23
      0x8C603111,  //  00B0  GETMET	R24	R24	K17
      0x5C681C00,  //  00B1  MOVE	R26	R14
      0x7C600400,  //  00B2  CALL	R24	2
      0x7001FFE4,  //  00B3  JMP		#0099
      0x58580012,  //  00B4  LDCONST	R22	K18
      0xAC580200,  //  00B5  CATCH	R22	1	0
      0xB0080000,  //  00B6  RAISE	2	R0	R0
      0x7001FFB3,  //  00B7  JMP		#006C
      0x584C0012,  //  00B8  LDCONST	R19	K18
      0xAC4C0200,  //  00B9  CATCH	R19	1	0
      0xB0080000,  //  00BA  RAISE	2	R0	R0
      0x7001FF88,  //  00BB  JMP		#0045
      0x58400012,  //  00BC  LDCONST	R16	K18
      0xAC400200,  //  00BD  CATCH	R16	1	0
      0xB0080000,  //  00BE  RAISE	2	R0	R0
      0x7001FF6F,  //  00BF  JMP		#0030
      0x58340012,  //  00C0  LDCONST	R13	K18
      0xAC340200,  //  00C1  CATCH	R13	1	0
      0xB0080000,  //  00C2  RAISE	2	R0	R0
      0x60340010,  //  00C3  GETGBL	R13	G16
      0x5C380600,  //  00C4  MOVE	R14	R3
      0x5C3C1800,  //  00C5  MOVE	R15	R12
      0x7C380200,  //  00C6  CALL	R14	1
      0x7C340200,  //  00C7  CALL	R13	1
      0xA802003D,  //  00C8  EXBLK	0	#0107
      0x5C381A00,  //  00C9  MOVE	R14	R13
      0x7C380000,  //  00CA  CALL	R14	0
      0x603C0010,  //  00CB  GETGBL	R15	G16
      0x5C400600,  //  00CC  MOVE	R16	R3
      0x9444180E,  //  00CD  GETIDX	R17	R12	R14
      0x7C400200,  //  00CE  CALL	R16	1
      0x7C3C0200,  //  00CF  CALL	R15	1
      0xA8020031,  //  00D0  EXBLK	0	#0103
      0x5C401E00,  //  00D1  MOVE	R16	R15
      0x7C400000,  //  00D2  CALL	R16	0
      0x60440010,  //  00D3  GETGBL	R17	G16
      0x5C480600,  //  00D4  MOVE	R18	R3
      0x944C180E,  //  00D5  GETIDX	R19	R12	R14
      0x944C2610,  //  00D6  GETIDX	R19	R19	R16
      0x7C480200,  //  00D7  CALL	R18	1
      0x7C440200,  //  00D8  CALL	R17	1
      0xA8020024,  //  00D9  EXBLK	0	#00FF
      0x5C482200,  //  00DA  MOVE	R18	R17
      0x7C480000,  //  00DB  CALL	R18	0
      0x604C0010,  //  00DC  GETGBL	R19	G16
      0x9450180E,  //  00DD  GETIDX	R20	R12	R14
      0x94502810,  //  00DE  GETIDX	R20	R20	R16
      0x94502812,  //  00DF  GETIDX	R20	R20	R18
      0x7C4C0200,  //  00E0  CALL	R19	1
      0xA8020018,  //  00E1  EXBLK	0	#00FB
      0x5C502600,  //  00E2  MOVE	R20	R19
      0x7C500000,  //  00E3  CALL	R20	0
      0xB8560800,  //  00E4  GETNGBL	R21	K4
      0x8C542B05,  //  00E5  GETMET	R21	R21	K5
      0x8C5C0906,  //  00E6  GETMET	R23	R4	K6
      0x58640013,  //  00E7  LDCONST	R25	K19
      0x5C681C00,  //  00E8  MOVE	R26	R14
      0x5C6C2000,  //  00E9  MOVE	R27	R16
      0x5C702400,  //  00EA  MOVE	R28	R18
      0x7C5C0A00,  //  00EB  CALL	R23	5
      0x58600014,  //  00EC  LDCONST	R24	K20
      0x7C540600,  //  00ED  CALL	R21	3
      0x9006020E,  //  00EE  SETMBR	R1	K1	R14
      0x90060410,  //  00EF  SETMBR	R1	K2	R16
      0x90060612,  //  00F0  SETMBR	R1	K3	R18
      0x5C540400,  //  00F1  MOVE	R21	R2
      0x5C582800,  //  00F2  MOVE	R22	R20
      0x5C5C0200,  //  00F3  MOVE	R23	R1
      0x5C601600,  //  00F4  MOVE	R24	R11
      0x7C540600,  //  00F5  CALL	R21	3
      0x782E0002,  //  00F6  JMPF	R11	#00FA
      0x78560001,  //  00F7  JMPF	R21	#00FA
      0xA8040004,  //  00F8  EXBLK	1	4
      0x80002C00,  //  00F9  RET	0
      0x7001FFE6,  //  00FA  JMP		#00E2
      0x584C0012,  //  00FB  LDCONST	R19	K18
      0xAC4C0200,  //  00FC  CATCH	R19	1	0
      0xB0080000,  //  00FD  RAISE	2	R0	R0
      0x7001FFDA,  //  00FE  JMP		#00DA
      0x58440012,  //  00FF  LDCONST	R17	K18
      0xAC440200,  //  0100  CATCH	R17	1	0
      0xB0080000,  //  0101  RAISE	2	R0	R0
      0x7001FFCD,  //  0102  JMP		#00D1
      0x583C0012,  //  0103  LDCONST	R15	K18
      0xAC3C0200,  //  0104  CATCH	R15	1	0
      0xB0080000,  //  0105  RAISE	2	R0	R0
      0x7001FFC1,  //  0106  JMP		#00C9
      0x58340012,  //  0107  LDCONST	R13	K18
      0xAC340200,  //  0108  CATCH	R13	1	0
      0xB0080000,  //  0109  RAISE	2	R0	R0
      0x782E0019,  //  010A  JMPF	R11	#0125
      0x5C340C00,  //  010B  MOVE	R13	R6
      0x74360003,  //  010C  JMPT	R13	#0111
      0xB8362C00,  //  010D  GETNGBL	R13	K22
      0x88341B17,  //  010E  GETMBR	R13	R13	K23
      0x90062A0D,  //  010F  SETMBR	R1	K21	R13
      0x7002000E,  //  0110  JMP		#0120
      0x5C341000,  //  0111  MOVE	R13	R8
      0x74360003,  //  0112  JMPT	R13	#0117
      0xB8362C00,  //  0113  GETNGBL	R13	K22
      0x88341B18,  //  0114  GETMBR	R13	R13	K24
      0x90062A0D,  //  0115  SETMBR	R1	K21	R13
      0x70020008,  //  0116  JMP		#0120
      0x5C341400,  //  0117  MOVE	R13	R10
      0x74360003,  //  0118  JMPT	R13	#011D
      0xB8362C00,  //  0119  GETNGBL	R13	K22
      0x88341B19,  //  011A  GETMBR	R13	R13	K25
      0x90062A0D,  //  011B  SETMBR	R1	K21	R13
      0x70020002,  //  011C  JMP		#0120
      0xB8362C00,  //  011D  GETNGBL	R13	K22
      0x88341B1A,  //  011E  GETMBR	R13	R13	K26
      0x90062A0D,  //  011F  SETMBR	R1	K21	R13
      0x5C340400,  //  0120  MOVE	R13	R2
      0x4C380000,  //  0121  LDNIL	R14
      0x5C3C0200,  //  0122  MOVE	R15	R1
      0x50400200,  //  0123  LDBOOL	R16	1	0
      0x7C340600,  //  0124  CALL	R13	3
      0x80000000,  //  0125  RET	0
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
    /* K5   */  be_nested_str_weak(discriminator),
    /* K6   */  be_nested_str_weak(passcode),
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
        /* K0   */  be_nested_str_weak(_start_mdns_announce),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Wifi_X23Connected),
        /* K4   */  be_nested_str_weak(matter_device_mdns),
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
        /* K0   */  be_nested_str_weak(_start_mdns_announce),
        /* K1   */  be_nested_str_weak(tasmota),
        /* K2   */  be_nested_str_weak(remove_rule),
        /* K3   */  be_nested_str_weak(Eth_X23Connected),
        /* K4   */  be_nested_str_weak(matter_device_mdns),
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
    ( &(const bvalue[ 8]) {     /* constants */
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(wifi),
    /* K2   */  be_nested_str_weak(up),
    /* K3   */  be_nested_str_weak(_start_mdns_announce),
    /* K4   */  be_nested_str_weak(add_rule),
    /* K5   */  be_nested_str_weak(Wifi_X23Connected),
    /* K6   */  be_nested_str_weak(eth),
    /* K7   */  be_nested_str_weak(Eth_X23Connected),
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
      0x5C140000,  //  000D  MOVE	R5	R0
      0x7C040800,  //  000E  CALL	R1	4
      0xB8060000,  //  000F  GETNGBL	R1	K0
      0x8C040306,  //  0010  GETMET	R1	R1	K6
      0x7C040200,  //  0011  CALL	R1	1
      0x94040302,  //  0012  GETIDX	R1	R1	K2
      0x78060003,  //  0013  JMPF	R1	#0018
      0x8C040103,  //  0014  GETMET	R1	R0	K3
      0x500C0200,  //  0015  LDBOOL	R3	1	0
      0x7C040400,  //  0016  CALL	R1	2
      0x70020005,  //  0017  JMP		#001E
      0xB8060000,  //  0018  GETNGBL	R1	K0
      0x8C040304,  //  0019  GETMET	R1	R1	K4
      0x580C0007,  //  001A  LDCONST	R3	K7
      0x84100001,  //  001B  CLOSURE	R4	P1
      0x5C140000,  //  001C  MOVE	R5	R0
      0x7C040800,  //  001D  CALL	R1	4
      0xA0000000,  //  001E  CLOSE	R0
      0x80000000,  //  001F  RET	0
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
    /* K1   */  be_nested_str_weak(discriminator),
    /* K2   */  be_nested_str_weak(passcode),
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
** Solidified function: start_commissioning_complete
********************************************************************/
be_local_closure(Matter_Device_start_commissioning_complete,   /* name */
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
    /* K0   */  be_nested_str_weak(tasmota),
    /* K1   */  be_nested_str_weak(log),
    /* K2   */  be_nested_str_weak(MTR_X3A_X20_X2A_X2A_X2A_X20Commissioning_X20complete_X20_X2A_X2A_X2A),
    /* K3   */  be_const_int(2),
    }),
    be_str_weak(start_commissioning_complete),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xB80A0000,  //  0000  GETNGBL	R2	K0
      0x8C080501,  //  0001  GETMET	R2	R2	K1
      0x58100002,  //  0002  LDCONST	R4	K2
      0x58140003,  //  0003  LDCONST	R5	K3
      0x7C080600,  //  0004  CALL	R2	3
      0x80000000,  //  0005  RET	0
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
** Solidified function: invoke_request
********************************************************************/
be_local_closure(Matter_Device_invoke_request,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[ 7]) {     /* constants */
    /* K0   */  be_const_int(0),
    /* K1   */  be_nested_str_weak(plugins),
    /* K2   */  be_nested_str_weak(invoke_request),
    /* K3   */  be_nested_str_weak(status),
    /* K4   */  be_nested_str_weak(matter),
    /* K5   */  be_nested_str_weak(UNSUPPORTED_COMMAND),
    /* K6   */  be_const_int(1),
    }),
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x58100000,  //  0000  LDCONST	R4	K0
      0x6014000C,  //  0001  GETGBL	R5	G12
      0x88180101,  //  0002  GETMBR	R6	R0	K1
      0x7C140200,  //  0003  CALL	R5	1
      0x14140805,  //  0004  LT	R5	R4	R5
      0x78160011,  //  0005  JMPF	R5	#0018
      0x88140101,  //  0006  GETMBR	R5	R0	K1
      0x94140A04,  //  0007  GETIDX	R5	R5	R4
      0x8C180B02,  //  0008  GETMET	R6	R5	K2
      0x5C200200,  //  0009  MOVE	R8	R1
      0x5C240400,  //  000A  MOVE	R9	R2
      0x5C280600,  //  000B  MOVE	R10	R3
      0x7C180800,  //  000C  CALL	R6	4
      0x4C1C0000,  //  000D  LDNIL	R7
      0x201C0C07,  //  000E  NE	R7	R6	R7
      0x741E0004,  //  000F  JMPT	R7	#0015
      0x881C0703,  //  0010  GETMBR	R7	R3	K3
      0xB8220800,  //  0011  GETNGBL	R8	K4
      0x88201105,  //  0012  GETMBR	R8	R8	K5
      0x201C0E08,  //  0013  NE	R7	R7	R8
      0x781E0000,  //  0014  JMPF	R7	#0016
      0x80040C00,  //  0015  RET	1	R6
      0x00100906,  //  0016  ADD	R4	R4	K6
      0x7001FFE8,  //  0017  JMP		#0001
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
** Solidified function: _start_mdns_announce
********************************************************************/
be_local_closure(Matter_Device__start_mdns_announce,   /* name */
  be_nested_proto(
    15,                          /* nstack */
    2,                          /* argc */
    2,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    ( &(const bvalue[48]) {     /* constants */
    /* K0   */  be_nested_str_weak(mdns),
    /* K1   */  be_nested_str_weak(string),
    /* K2   */  be_nested_str_weak(start),
    /* K3   */  be_nested_str_weak(VP),
    /* K4   */  be_nested_str_weak(vendorid),
    /* K5   */  be_nested_str_weak(_X2B),
    /* K6   */  be_nested_str_weak(productid),
    /* K7   */  be_nested_str_weak(D),
    /* K8   */  be_nested_str_weak(discriminator),
    /* K9   */  be_nested_str_weak(CM),
    /* K10  */  be_const_int(1),
    /* K11  */  be_nested_str_weak(T),
    /* K12  */  be_const_int(0),
    /* K13  */  be_nested_str_weak(SII),
    /* K14  */  be_nested_str_weak(SAI),
    /* K15  */  be_nested_str_weak(tasmota),
    /* K16  */  be_nested_str_weak(eth),
    /* K17  */  be_nested_str_weak(hostname_eth),
    /* K18  */  be_nested_str_weak(replace),
    /* K19  */  be_nested_str_weak(find),
    /* K20  */  be_nested_str_weak(mac),
    /* K21  */  be_nested_str_weak(_X3A),
    /* K22  */  be_nested_str_weak(),
    /* K23  */  be_nested_str_weak(add_hostname),
    /* K24  */  be_nested_str_weak(ip6local),
    /* K25  */  be_nested_str_weak(ip),
    /* K26  */  be_nested_str_weak(ip6),
    /* K27  */  be_nested_str_weak(add_service),
    /* K28  */  be_nested_str_weak(_matterc),
    /* K29  */  be_nested_str_weak(_udp),
    /* K30  */  be_nested_str_weak(commissioning_instance_eth),
    /* K31  */  be_nested_str_weak(log),
    /* K32  */  be_nested_str_weak(format),
    /* K33  */  be_nested_str_weak(MTR_X3A_X20starting_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
    /* K34  */  be_nested_str_weak(wifi),
    /* K35  */  be_nested_str_weak(commissioning_instance_wifi),
    /* K36  */  be_nested_str_weak(hostname_wifi),
    /* K37  */  be_const_int(2),
    /* K38  */  be_nested_str_weak(_L),
    /* K39  */  be_nested_str_weak(MTR_X3A_X20adding_X20subtype_X3A_X20),
    /* K40  */  be_const_int(3),
    /* K41  */  be_nested_str_weak(add_subtype),
    /* K42  */  be_nested_str_weak(_S),
    /* K43  */  be_nested_str_weak(_V),
    /* K44  */  be_nested_str_weak(_CM1),
    /* K45  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K46  */  be_nested_str_weak(_X7C),
    /* K47  */  be_nested_str_weak(mdns_announce_op_discovery_all_sessions),
    }),
    be_str_weak(_start_mdns_announce),
    &be_const_str_solidified,
    ( &(const binstruction[292]) {  /* code */
      0xA40A0000,  //  0000  IMPORT	R2	K0
      0xA40E0200,  //  0001  IMPORT	R3	K1
      0x8C100502,  //  0002  GETMET	R4	R2	K2
      0x7C100200,  //  0003  CALL	R4	1
      0x60100013,  //  0004  GETGBL	R4	G19
      0x7C100000,  //  0005  CALL	R4	0
      0x60140008,  //  0006  GETGBL	R5	G8
      0x88180104,  //  0007  GETMBR	R6	R0	K4
      0x7C140200,  //  0008  CALL	R5	1
      0x00140B05,  //  0009  ADD	R5	R5	K5
      0x60180008,  //  000A  GETGBL	R6	G8
      0x881C0106,  //  000B  GETMBR	R7	R0	K6
      0x7C180200,  //  000C  CALL	R6	1
      0x00140A06,  //  000D  ADD	R5	R5	R6
      0x98120605,  //  000E  SETIDX	R4	K3	R5
      0x88140108,  //  000F  GETMBR	R5	R0	K8
      0x98120E05,  //  0010  SETIDX	R4	K7	R5
      0x9812130A,  //  0011  SETIDX	R4	K9	K10
      0x9812170C,  //  0012  SETIDX	R4	K11	K12
      0x54161387,  //  0013  LDINT	R5	5000
      0x98121A05,  //  0014  SETIDX	R4	K13	R5
      0x5416012B,  //  0015  LDINT	R5	300
      0x98121C05,  //  0016  SETIDX	R4	K14	R5
      0xA80200F7,  //  0017  EXBLK	0	#0110
      0x7806007A,  //  0018  JMPF	R1	#0094
      0xB8161E00,  //  0019  GETNGBL	R5	K15
      0x8C140B10,  //  001A  GETMET	R5	R5	K16
      0x7C140200,  //  001B  CALL	R5	1
      0x8C180712,  //  001C  GETMET	R6	R3	K18
      0x8C200B13,  //  001D  GETMET	R8	R5	K19
      0x58280014,  //  001E  LDCONST	R10	K20
      0x7C200400,  //  001F  CALL	R8	2
      0x58240015,  //  0020  LDCONST	R9	K21
      0x58280016,  //  0021  LDCONST	R10	K22
      0x7C180800,  //  0022  CALL	R6	4
      0x90022206,  //  0023  SETMBR	R0	K17	R6
      0x8C180517,  //  0024  GETMET	R6	R2	K23
      0x88200111,  //  0025  GETMBR	R8	R0	K17
      0x8C240B13,  //  0026  GETMET	R9	R5	K19
      0x582C0018,  //  0027  LDCONST	R11	K24
      0x58300016,  //  0028  LDCONST	R12	K22
      0x7C240600,  //  0029  CALL	R9	3
      0x8C280B13,  //  002A  GETMET	R10	R5	K19
      0x58300019,  //  002B  LDCONST	R12	K25
      0x58340016,  //  002C  LDCONST	R13	K22
      0x7C280600,  //  002D  CALL	R10	3
      0x8C2C0B13,  //  002E  GETMET	R11	R5	K19
      0x5834001A,  //  002F  LDCONST	R13	K26
      0x58380016,  //  0030  LDCONST	R14	K22
      0x7C2C0600,  //  0031  CALL	R11	3
      0x7C180A00,  //  0032  CALL	R6	5
      0x8C18051B,  //  0033  GETMET	R6	R2	K27
      0x5820001C,  //  0034  LDCONST	R8	K28
      0x5824001D,  //  0035  LDCONST	R9	K29
      0x542A15A3,  //  0036  LDINT	R10	5540
      0x5C2C0800,  //  0037  MOVE	R11	R4
      0x8830011E,  //  0038  GETMBR	R12	R0	K30
      0x88340111,  //  0039  GETMBR	R13	R0	K17
      0x7C180E00,  //  003A  CALL	R6	7
      0xB81A1E00,  //  003B  GETNGBL	R6	K15
      0x8C180D1F,  //  003C  GETMET	R6	R6	K31
      0x8C200720,  //  003D  GETMET	R8	R3	K32
      0x58280021,  //  003E  LDCONST	R10	K33
      0x78060001,  //  003F  JMPF	R1	#0042
      0x582C0010,  //  0040  LDCONST	R11	K16
      0x70020000,  //  0041  JMP		#0043
      0x582C0022,  //  0042  LDCONST	R11	K34
      0x78060001,  //  0043  JMPF	R1	#0046
      0x8830011E,  //  0044  GETMBR	R12	R0	K30
      0x70020000,  //  0045  JMP		#0047
      0x88300123,  //  0046  GETMBR	R12	R0	K35
      0x78060001,  //  0047  JMPF	R1	#004A
      0x88340111,  //  0048  GETMBR	R13	R0	K17
      0x70020000,  //  0049  JMP		#004B
      0x88340124,  //  004A  GETMBR	R13	R0	K36
      0x7C200A00,  //  004B  CALL	R8	5
      0x58240025,  //  004C  LDCONST	R9	K37
      0x7C180600,  //  004D  CALL	R6	3
      0x60180008,  //  004E  GETGBL	R6	G8
      0x881C0108,  //  004F  GETMBR	R7	R0	K8
      0x54220FFE,  //  0050  LDINT	R8	4095
      0x2C1C0E08,  //  0051  AND	R7	R7	R8
      0x7C180200,  //  0052  CALL	R6	1
      0x001A4C06,  //  0053  ADD	R6	K38	R6
      0xB81E1E00,  //  0054  GETNGBL	R7	K15
      0x8C1C0F1F,  //  0055  GETMET	R7	R7	K31
      0x00264E06,  //  0056  ADD	R9	K39	R6
      0x58280028,  //  0057  LDCONST	R10	K40
      0x7C1C0600,  //  0058  CALL	R7	3
      0x8C1C0529,  //  0059  GETMET	R7	R2	K41
      0x5824001C,  //  005A  LDCONST	R9	K28
      0x5828001D,  //  005B  LDCONST	R10	K29
      0x882C011E,  //  005C  GETMBR	R11	R0	K30
      0x88300111,  //  005D  GETMBR	R12	R0	K17
      0x5C340C00,  //  005E  MOVE	R13	R6
      0x7C1C0C00,  //  005F  CALL	R7	6
      0x601C0008,  //  0060  GETGBL	R7	G8
      0x88200108,  //  0061  GETMBR	R8	R0	K8
      0x54260EFF,  //  0062  LDINT	R9	3840
      0x2C201009,  //  0063  AND	R8	R8	R9
      0x54260007,  //  0064  LDINT	R9	8
      0x3C201009,  //  0065  SHR	R8	R8	R9
      0x7C1C0200,  //  0066  CALL	R7	1
      0x001E5407,  //  0067  ADD	R7	K42	R7
      0x5C180E00,  //  0068  MOVE	R6	R7
      0xB81E1E00,  //  0069  GETNGBL	R7	K15
      0x8C1C0F1F,  //  006A  GETMET	R7	R7	K31
      0x00264E06,  //  006B  ADD	R9	K39	R6
      0x58280028,  //  006C  LDCONST	R10	K40
      0x7C1C0600,  //  006D  CALL	R7	3
      0x8C1C0529,  //  006E  GETMET	R7	R2	K41
      0x5824001C,  //  006F  LDCONST	R9	K28
      0x5828001D,  //  0070  LDCONST	R10	K29
      0x882C011E,  //  0071  GETMBR	R11	R0	K30
      0x88300111,  //  0072  GETMBR	R12	R0	K17
      0x5C340C00,  //  0073  MOVE	R13	R6
      0x7C1C0C00,  //  0074  CALL	R7	6
      0x601C0008,  //  0075  GETGBL	R7	G8
      0x88200104,  //  0076  GETMBR	R8	R0	K4
      0x7C1C0200,  //  0077  CALL	R7	1
      0x001E5607,  //  0078  ADD	R7	K43	R7
      0x5C180E00,  //  0079  MOVE	R6	R7
      0xB81E1E00,  //  007A  GETNGBL	R7	K15
      0x8C1C0F1F,  //  007B  GETMET	R7	R7	K31
      0x00264E06,  //  007C  ADD	R9	K39	R6
      0x58280028,  //  007D  LDCONST	R10	K40
      0x7C1C0600,  //  007E  CALL	R7	3
      0x8C1C0529,  //  007F  GETMET	R7	R2	K41
      0x5824001C,  //  0080  LDCONST	R9	K28
      0x5828001D,  //  0081  LDCONST	R10	K29
      0x882C011E,  //  0082  GETMBR	R11	R0	K30
      0x88300111,  //  0083  GETMBR	R12	R0	K17
      0x5C340C00,  //  0084  MOVE	R13	R6
      0x7C1C0C00,  //  0085  CALL	R7	6
      0x5818002C,  //  0086  LDCONST	R6	K44
      0xB81E1E00,  //  0087  GETNGBL	R7	K15
      0x8C1C0F1F,  //  0088  GETMET	R7	R7	K31
      0x00264E06,  //  0089  ADD	R9	K39	R6
      0x58280028,  //  008A  LDCONST	R10	K40
      0x7C1C0600,  //  008B  CALL	R7	3
      0x8C1C0529,  //  008C  GETMET	R7	R2	K41
      0x5824001C,  //  008D  LDCONST	R9	K28
      0x5828001D,  //  008E  LDCONST	R10	K29
      0x882C011E,  //  008F  GETMBR	R11	R0	K30
      0x88300111,  //  0090  GETMBR	R12	R0	K17
      0x5C340C00,  //  0091  MOVE	R13	R6
      0x7C1C0C00,  //  0092  CALL	R7	6
      0x70020079,  //  0093  JMP		#010E
      0xB8161E00,  //  0094  GETNGBL	R5	K15
      0x8C140B22,  //  0095  GETMET	R5	R5	K34
      0x7C140200,  //  0096  CALL	R5	1
      0x8C180712,  //  0097  GETMET	R6	R3	K18
      0x8C200B13,  //  0098  GETMET	R8	R5	K19
      0x58280014,  //  0099  LDCONST	R10	K20
      0x7C200400,  //  009A  CALL	R8	2
      0x58240015,  //  009B  LDCONST	R9	K21
      0x58280016,  //  009C  LDCONST	R10	K22
      0x7C180800,  //  009D  CALL	R6	4
      0x90024806,  //  009E  SETMBR	R0	K36	R6
      0x8C180517,  //  009F  GETMET	R6	R2	K23
      0x88200124,  //  00A0  GETMBR	R8	R0	K36
      0x8C240B13,  //  00A1  GETMET	R9	R5	K19
      0x582C0018,  //  00A2  LDCONST	R11	K24
      0x58300016,  //  00A3  LDCONST	R12	K22
      0x7C240600,  //  00A4  CALL	R9	3
      0x8C280B13,  //  00A5  GETMET	R10	R5	K19
      0x58300019,  //  00A6  LDCONST	R12	K25
      0x58340016,  //  00A7  LDCONST	R13	K22
      0x7C280600,  //  00A8  CALL	R10	3
      0x8C2C0B13,  //  00A9  GETMET	R11	R5	K19
      0x5834001A,  //  00AA  LDCONST	R13	K26
      0x58380016,  //  00AB  LDCONST	R14	K22
      0x7C2C0600,  //  00AC  CALL	R11	3
      0x7C180A00,  //  00AD  CALL	R6	5
      0x8C18051B,  //  00AE  GETMET	R6	R2	K27
      0x5820001C,  //  00AF  LDCONST	R8	K28
      0x5824001D,  //  00B0  LDCONST	R9	K29
      0x542A15A3,  //  00B1  LDINT	R10	5540
      0x5C2C0800,  //  00B2  MOVE	R11	R4
      0x88300123,  //  00B3  GETMBR	R12	R0	K35
      0x88340124,  //  00B4  GETMBR	R13	R0	K36
      0x7C180E00,  //  00B5  CALL	R6	7
      0xB81A1E00,  //  00B6  GETNGBL	R6	K15
      0x8C180D1F,  //  00B7  GETMET	R6	R6	K31
      0x8C200720,  //  00B8  GETMET	R8	R3	K32
      0x58280021,  //  00B9  LDCONST	R10	K33
      0x78060001,  //  00BA  JMPF	R1	#00BD
      0x582C0010,  //  00BB  LDCONST	R11	K16
      0x70020000,  //  00BC  JMP		#00BE
      0x582C0022,  //  00BD  LDCONST	R11	K34
      0x78060001,  //  00BE  JMPF	R1	#00C1
      0x8830011E,  //  00BF  GETMBR	R12	R0	K30
      0x70020000,  //  00C0  JMP		#00C2
      0x88300123,  //  00C1  GETMBR	R12	R0	K35
      0x78060001,  //  00C2  JMPF	R1	#00C5
      0x88340111,  //  00C3  GETMBR	R13	R0	K17
      0x70020000,  //  00C4  JMP		#00C6
      0x88340124,  //  00C5  GETMBR	R13	R0	K36
      0x7C200A00,  //  00C6  CALL	R8	5
      0x58240025,  //  00C7  LDCONST	R9	K37
      0x7C180600,  //  00C8  CALL	R6	3
      0x60180008,  //  00C9  GETGBL	R6	G8
      0x881C0108,  //  00CA  GETMBR	R7	R0	K8
      0x54220FFE,  //  00CB  LDINT	R8	4095
      0x2C1C0E08,  //  00CC  AND	R7	R7	R8
      0x7C180200,  //  00CD  CALL	R6	1
      0x001A4C06,  //  00CE  ADD	R6	K38	R6
      0xB81E1E00,  //  00CF  GETNGBL	R7	K15
      0x8C1C0F1F,  //  00D0  GETMET	R7	R7	K31
      0x00264E06,  //  00D1  ADD	R9	K39	R6
      0x58280028,  //  00D2  LDCONST	R10	K40
      0x7C1C0600,  //  00D3  CALL	R7	3
      0x8C1C0529,  //  00D4  GETMET	R7	R2	K41
      0x5824001C,  //  00D5  LDCONST	R9	K28
      0x5828001D,  //  00D6  LDCONST	R10	K29
      0x882C0123,  //  00D7  GETMBR	R11	R0	K35
      0x88300124,  //  00D8  GETMBR	R12	R0	K36
      0x5C340C00,  //  00D9  MOVE	R13	R6
      0x7C1C0C00,  //  00DA  CALL	R7	6
      0x601C0008,  //  00DB  GETGBL	R7	G8
      0x88200108,  //  00DC  GETMBR	R8	R0	K8
      0x54260EFF,  //  00DD  LDINT	R9	3840
      0x2C201009,  //  00DE  AND	R8	R8	R9
      0x54260007,  //  00DF  LDINT	R9	8
      0x3C201009,  //  00E0  SHR	R8	R8	R9
      0x7C1C0200,  //  00E1  CALL	R7	1
      0x001E5407,  //  00E2  ADD	R7	K42	R7
      0x5C180E00,  //  00E3  MOVE	R6	R7
      0xB81E1E00,  //  00E4  GETNGBL	R7	K15
      0x8C1C0F1F,  //  00E5  GETMET	R7	R7	K31
      0x00264E06,  //  00E6  ADD	R9	K39	R6
      0x58280028,  //  00E7  LDCONST	R10	K40
      0x7C1C0600,  //  00E8  CALL	R7	3
      0x8C1C0529,  //  00E9  GETMET	R7	R2	K41
      0x5824001C,  //  00EA  LDCONST	R9	K28
      0x5828001D,  //  00EB  LDCONST	R10	K29
      0x882C0123,  //  00EC  GETMBR	R11	R0	K35
      0x88300124,  //  00ED  GETMBR	R12	R0	K36
      0x5C340C00,  //  00EE  MOVE	R13	R6
      0x7C1C0C00,  //  00EF  CALL	R7	6
      0x601C0008,  //  00F0  GETGBL	R7	G8
      0x88200104,  //  00F1  GETMBR	R8	R0	K4
      0x7C1C0200,  //  00F2  CALL	R7	1
      0x001E5607,  //  00F3  ADD	R7	K43	R7
      0x5C180E00,  //  00F4  MOVE	R6	R7
      0xB81E1E00,  //  00F5  GETNGBL	R7	K15
      0x8C1C0F1F,  //  00F6  GETMET	R7	R7	K31
      0x00264E06,  //  00F7  ADD	R9	K39	R6
      0x58280028,  //  00F8  LDCONST	R10	K40
      0x7C1C0600,  //  00F9  CALL	R7	3
      0x8C1C0529,  //  00FA  GETMET	R7	R2	K41
      0x5824001C,  //  00FB  LDCONST	R9	K28
      0x5828001D,  //  00FC  LDCONST	R10	K29
      0x882C0123,  //  00FD  GETMBR	R11	R0	K35
      0x88300124,  //  00FE  GETMBR	R12	R0	K36
      0x5C340C00,  //  00FF  MOVE	R13	R6
      0x7C1C0C00,  //  0100  CALL	R7	6
      0x5818002C,  //  0101  LDCONST	R6	K44
      0xB81E1E00,  //  0102  GETNGBL	R7	K15
      0x8C1C0F1F,  //  0103  GETMET	R7	R7	K31
      0x00264E06,  //  0104  ADD	R9	K39	R6
      0x58280028,  //  0105  LDCONST	R10	K40
      0x7C1C0600,  //  0106  CALL	R7	3
      0x8C1C0529,  //  0107  GETMET	R7	R2	K41
      0x5824001C,  //  0108  LDCONST	R9	K28
      0x5828001D,  //  0109  LDCONST	R10	K29
      0x882C0123,  //  010A  GETMBR	R11	R0	K35
      0x88300124,  //  010B  GETMBR	R12	R0	K36
      0x5C340C00,  //  010C  MOVE	R13	R6
      0x7C1C0C00,  //  010D  CALL	R7	6
      0xA8040001,  //  010E  EXBLK	1	1
      0x70020010,  //  010F  JMP		#0121
      0xAC140002,  //  0110  CATCH	R5	0	2
      0x7002000D,  //  0111  JMP		#0120
      0xB81E1E00,  //  0112  GETNGBL	R7	K15
      0x8C1C0F1F,  //  0113  GETMET	R7	R7	K31
      0x60240008,  //  0114  GETGBL	R9	G8
      0x5C280A00,  //  0115  MOVE	R10	R5
      0x7C240200,  //  0116  CALL	R9	1
      0x00265A09,  //  0117  ADD	R9	K45	R9
      0x0024132E,  //  0118  ADD	R9	R9	K46
      0x60280008,  //  0119  GETGBL	R10	G8
      0x5C2C0C00,  //  011A  MOVE	R11	R6
      0x7C280200,  //  011B  CALL	R10	1
      0x0024120A,  //  011C  ADD	R9	R9	R10
      0x58280025,  //  011D  LDCONST	R10	K37
      0x7C1C0600,  //  011E  CALL	R7	3
      0x70020000,  //  011F  JMP		#0121
      0xB0080000,  //  0120  RAISE	2	R0	R0
      0x8C14012F,  //  0121  GETMET	R5	R0	K47
      0x7C140200,  //  0122  CALL	R5	1
      0x80000000,  //  0123  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: packet_ack
********************************************************************/
be_local_closure(Matter_Device_packet_ack,   /* name */
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
    /* K1   */  be_nested_str_weak(packet_ack),
    }),
    be_str_weak(packet_ack),
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
    /* K2   */  be_nested_str_weak(get_deviceid),
    /* K3   */  be_nested_str_weak(copy),
    /* K4   */  be_nested_str_weak(reverse),
    /* K5   */  be_nested_str_weak(get_fabric_compressed),
    /* K6   */  be_nested_str_weak(tohex),
    /* K7   */  be_nested_str_weak(_X2D),
    /* K8   */  be_nested_str_weak(tasmota),
    /* K9   */  be_nested_str_weak(log),
    /* K10  */  be_nested_str_weak(MTR_X3A_X20Operational_X20Discovery_X20node_X20_X3D_X20),
    /* K11  */  be_const_int(2),
    /* K12  */  be_nested_str_weak(eth),
    /* K13  */  be_nested_str_weak(find),
    /* K14  */  be_nested_str_weak(up),
    /* K15  */  be_nested_str_weak(format),
    /* K16  */  be_nested_str_weak(MTR_X3A_X20adding_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
    /* K17  */  be_nested_str_weak(hostname_eth),
    /* K18  */  be_const_int(3),
    /* K19  */  be_nested_str_weak(add_service),
    /* K20  */  be_nested_str_weak(_matter),
    /* K21  */  be_nested_str_weak(_tcp),
    /* K22  */  be_nested_str_weak(_I),
    /* K23  */  be_nested_str_weak(MTR_X3A_X20adding_X20subtype_X3A_X20),
    /* K24  */  be_nested_str_weak(add_subtype),
    /* K25  */  be_nested_str_weak(wifi),
    /* K26  */  be_nested_str_weak(hostname_wifi),
    /* K27  */  be_nested_str_weak(MTR_X3A_X20Exception),
    /* K28  */  be_nested_str_weak(_X7C),
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
      0x58280012,  //  0025  LDCONST	R10	K18
      0x7C1C0600,  //  0026  CALL	R7	3
      0x8C1C0513,  //  0027  GETMET	R7	R2	K19
      0x58240014,  //  0028  LDCONST	R9	K20
      0x58280015,  //  0029  LDCONST	R10	K21
      0x542E15A3,  //  002A  LDINT	R11	5540
      0x4C300000,  //  002B  LDNIL	R12
      0x5C340C00,  //  002C  MOVE	R13	R6
      0x88380111,  //  002D  GETMBR	R14	R0	K17
      0x7C1C0E00,  //  002E  CALL	R7	7
      0x8C1C0B06,  //  002F  GETMET	R7	R5	K6
      0x7C1C0200,  //  0030  CALL	R7	1
      0x001E2C07,  //  0031  ADD	R7	K22	R7
      0xB8221000,  //  0032  GETNGBL	R8	K8
      0x8C201109,  //  0033  GETMET	R8	R8	K9
      0x002A2E07,  //  0034  ADD	R10	K23	R7
      0x582C0012,  //  0035  LDCONST	R11	K18
      0x7C200600,  //  0036  CALL	R8	3
      0x8C200518,  //  0037  GETMET	R8	R2	K24
      0x58280014,  //  0038  LDCONST	R10	K20
      0x582C0015,  //  0039  LDCONST	R11	K21
      0x5C300C00,  //  003A  MOVE	R12	R6
      0x88340111,  //  003B  GETMBR	R13	R0	K17
      0x5C380E00,  //  003C  MOVE	R14	R7
      0x7C200C00,  //  003D  CALL	R8	6
      0xB81E1000,  //  003E  GETNGBL	R7	K8
      0x8C1C0F19,  //  003F  GETMET	R7	R7	K25
      0x7C1C0200,  //  0040  CALL	R7	1
      0x8C1C0F0D,  //  0041  GETMET	R7	R7	K13
      0x5824000E,  //  0042  LDCONST	R9	K14
      0x7C1C0400,  //  0043  CALL	R7	2
      0x781E0020,  //  0044  JMPF	R7	#0066
      0xB81E1000,  //  0045  GETNGBL	R7	K8
      0x8C1C0F09,  //  0046  GETMET	R7	R7	K9
      0x8C24070F,  //  0047  GETMET	R9	R3	K15
      0x582C0010,  //  0048  LDCONST	R11	K16
      0x58300019,  //  0049  LDCONST	R12	K25
      0x5C340C00,  //  004A  MOVE	R13	R6
      0x8838011A,  //  004B  GETMBR	R14	R0	K26
      0x7C240A00,  //  004C  CALL	R9	5
      0x58280012,  //  004D  LDCONST	R10	K18
      0x7C1C0600,  //  004E  CALL	R7	3
      0x8C1C0513,  //  004F  GETMET	R7	R2	K19
      0x58240014,  //  0050  LDCONST	R9	K20
      0x58280015,  //  0051  LDCONST	R10	K21
      0x542E15A3,  //  0052  LDINT	R11	5540
      0x4C300000,  //  0053  LDNIL	R12
      0x5C340C00,  //  0054  MOVE	R13	R6
      0x8838011A,  //  0055  GETMBR	R14	R0	K26
      0x7C1C0E00,  //  0056  CALL	R7	7
      0x8C1C0B06,  //  0057  GETMET	R7	R5	K6
      0x7C1C0200,  //  0058  CALL	R7	1
      0x001E2C07,  //  0059  ADD	R7	K22	R7
      0xB8221000,  //  005A  GETNGBL	R8	K8
      0x8C201109,  //  005B  GETMET	R8	R8	K9
      0x002A2E07,  //  005C  ADD	R10	K23	R7
      0x582C0012,  //  005D  LDCONST	R11	K18
      0x7C200600,  //  005E  CALL	R8	3
      0x8C200518,  //  005F  GETMET	R8	R2	K24
      0x58280014,  //  0060  LDCONST	R10	K20
      0x582C0015,  //  0061  LDCONST	R11	K21
      0x5C300C00,  //  0062  MOVE	R12	R6
      0x8834011A,  //  0063  GETMBR	R13	R0	K26
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
      0x00223608,  //  006F  ADD	R8	K27	R8
      0x0020111C,  //  0070  ADD	R8	R8	K28
      0x60240008,  //  0071  GETGBL	R9	G8
      0x5C280A00,  //  0072  MOVE	R10	R5
      0x7C240200,  //  0073  CALL	R9	1
      0x00201009,  //  0074  ADD	R8	R8	R9
      0x5824000B,  //  0075  LDCONST	R9	K11
      0x7C180600,  //  0076  CALL	R6	3
      0x70020000,  //  0077  JMP		#0079
      0xB0080000,  //  0078  RAISE	2	R0	R0
      0x80000000,  //  0079  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_udp
********************************************************************/
be_local_closure(Matter_Device_start_udp,   /* name */
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
    /* K3   */  be_nested_str_weak(MTR_X3A_X20starting_X20UDP_X20server_X20on_X20port_X3A_X20),
    /* K4   */  be_const_int(2),
    /* K5   */  be_nested_str_weak(matter),
    /* K6   */  be_nested_str_weak(UDPServer),
    /* K7   */  be_nested_str_weak(),
    /* K8   */  be_nested_str_weak(start),
    }),
    be_str_weak(start_udp),
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
** Solidified class: Matter_Device
********************************************************************/
be_local_class(Matter_Device,
    18,
    NULL,
    be_nested_map(50,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(mdns_announce_op_discovery_all_sessions, -1), be_const_closure(Matter_Device_mdns_announce_op_discovery_all_sessions_closure) },
        { be_const_key_weak(salt, 31), be_const_var(14) },
        { be_const_key_weak(start_operational_dicovery, -1), be_const_closure(Matter_Device_start_operational_dicovery_closure) },
        { be_const_key_weak(w1, -1), be_const_var(16) },
        { be_const_key_weak(L, -1), be_const_var(17) },
        { be_const_key_weak(init, 42), be_const_closure(Matter_Device_init_closure) },
        { be_const_key_weak(compute_pbkdf, 12), be_const_closure(Matter_Device_compute_pbkdf_closure) },
        { be_const_key_weak(ui, 4), be_const_var(4) },
        { be_const_key_weak(UDP_PORT, -1), be_const_int(5540) },
        { be_const_key_weak(PASSCODE_DEFAULT, -1), be_const_int(20202021) },
        { be_const_key_weak(start_operational_dicovery_deferred, 43), be_const_closure(Matter_Device_start_operational_dicovery_deferred_closure) },
        { be_const_key_weak(productid, -1), be_const_var(10) },
        { be_const_key_weak(hostname_wifi, -1), be_const_var(7) },
        { be_const_key_weak(msg_send, -1), be_const_closure(Matter_Device_msg_send_closure) },
        { be_const_key_weak(finish_commissioning, -1), be_const_closure(Matter_Device_finish_commissioning_closure) },
        { be_const_key_weak(every_second, 27), be_const_closure(Matter_Device_every_second_closure) },
        { be_const_key_weak(commissioning_instance_wifi, -1), be_const_var(5) },
        { be_const_key_weak(stop, 29), be_const_closure(Matter_Device_stop_closure) },
        { be_const_key_weak(start_basic_commissioning, -1), be_const_closure(Matter_Device_start_basic_commissioning_closure) },
        { be_const_key_weak(load_param, -1), be_const_closure(Matter_Device_load_param_closure) },
        { be_const_key_weak(udp_server, -1), be_const_var(1) },
        { be_const_key_weak(plugins, -1), be_const_var(0) },
        { be_const_key_weak(FILENAME, 26), be_nested_str_weak(_matter_device_X2Ejson) },
        { be_const_key_weak(process_attribute_expansion, 30), be_const_closure(Matter_Device_process_attribute_expansion_closure) },
        { be_const_key_weak(PRODUCT_ID, -1), be_const_int(32768) },
        { be_const_key_weak(iterations, -1), be_const_var(13) },
        { be_const_key_weak(mdns_announce_op_discovery, 32), be_const_closure(Matter_Device_mdns_announce_op_discovery_closure) },
        { be_const_key_weak(passcode, -1), be_const_var(12) },
        { be_const_key_weak(sessions, -1), be_const_var(3) },
        { be_const_key_weak(compute_qrcode_content, -1), be_const_closure(Matter_Device_compute_qrcode_content_closure) },
        { be_const_key_weak(packet_ack, -1), be_const_closure(Matter_Device_packet_ack_closure) },
        { be_const_key_weak(_start_mdns_announce, 36), be_const_closure(Matter_Device__start_mdns_announce_closure) },
        { be_const_key_weak(vendorid, 45), be_const_var(9) },
        { be_const_key_weak(discriminator, -1), be_const_var(11) },
        { be_const_key_weak(w0, -1), be_const_var(15) },
        { be_const_key_weak(start_commissioning_complete, -1), be_const_closure(Matter_Device_start_commissioning_complete_closure) },
        { be_const_key_weak(VENDOR_ID, -1), be_const_int(65521) },
        { be_const_key_weak(get_active_endpoints, 41), be_const_closure(Matter_Device_get_active_endpoints_closure) },
        { be_const_key_weak(msg_received, -1), be_const_closure(Matter_Device_msg_received_closure) },
        { be_const_key_weak(start_mdns_announce_hostnames, 40), be_const_closure(Matter_Device_start_mdns_announce_hostnames_closure) },
        { be_const_key_weak(message_handler, -1), be_const_var(2) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(Matter_Device_invoke_request_closure) },
        { be_const_key_weak(start_commissioning_complete_deferred, -1), be_const_closure(Matter_Device_start_commissioning_complete_deferred_closure) },
        { be_const_key_weak(compute_manual_pairing_code, -1), be_const_closure(Matter_Device_compute_manual_pairing_code_closure) },
        { be_const_key_weak(hostname_eth, -1), be_const_var(8) },
        { be_const_key_weak(commissioning_instance_eth, 46), be_const_var(6) },
        { be_const_key_weak(sort_distinct, -1), be_const_static_closure(Matter_Device_sort_distinct_closure) },
        { be_const_key_weak(start_udp, -1), be_const_closure(Matter_Device_start_udp_closure) },
        { be_const_key_weak(save_param, 8), be_const_closure(Matter_Device_save_param_closure) },
        { be_const_key_weak(PBKDF_ITERATIONS, -1), be_const_int(1000) },
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
