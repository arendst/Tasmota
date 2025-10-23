/* Solidification of Matter_z_Commissioning.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'Matter_Commissioning' ktab size: 127, total: 259 (saved 1056 bytes)
static const bvalue be_ktab_class_Matter_Commissioning[127] = {
  /* K0   */  be_nested_str_weak(device),
  /* K1   */  be_nested_str_weak(sessions),
  /* K2   */  be_nested_str_weak(active_fabrics),
  /* K3   */  be_nested_str_weak(get_device_id),
  /* K4   */  be_nested_str_weak(get_fabric_id),
  /* K5   */  be_nested_str_weak(mdns_remove_op_discovery),
  /* K6   */  be_nested_str_weak(stop_iteration),
  /* K7   */  be_nested_str_weak(is_root_commissioning_open),
  /* K8   */  be_nested_str_weak(tasmota),
  /* K9   */  be_nested_str_weak(publish_result),
  /* K10  */  be_nested_str_weak(_X7B_X22Matter_X22_X3A_X7B_X22Commissioning_X22_X3A0_X7D_X7D),
  /* K11  */  be_nested_str_weak(Matter),
  /* K12  */  be_nested_str_weak(commissioning_open),
  /* K13  */  be_nested_str_weak(mdns_remove_PASE),
  /* K14  */  be_nested_str_weak(commissioning_iterations),
  /* K15  */  be_nested_str_weak(commissioning_discriminator),
  /* K16  */  be_nested_str_weak(commissioning_salt),
  /* K17  */  be_nested_str_weak(commissioning_w0),
  /* K18  */  be_nested_str_weak(commissioning_L),
  /* K19  */  be_nested_str_weak(commissioning_admin_fabric),
  /* K20  */  be_nested_str_weak(count_active_fabrics),
  /* K21  */  be_const_int(0),
  /* K22  */  be_nested_str_weak(start_root_basic_commissioning),
  /* K23  */  be_nested_str_weak(resize),
  /* K24  */  be_nested_str_weak(setbits),
  /* K25  */  be_const_int(3),
  /* K26  */  be_nested_str_weak(VENDOR_ID),
  /* K27  */  be_nested_str_weak(PRODUCT_ID),
  /* K28  */  be_nested_str_weak(root_discriminator),
  /* K29  */  be_nested_str_weak(root_passcode),
  /* K30  */  be_const_int(134217727),
  /* K31  */  be_nested_str_weak(MT_X3A),
  /* K32  */  be_nested_str_weak(matter),
  /* K33  */  be_nested_str_weak(Base38),
  /* K34  */  be_nested_str_weak(encode),
  /* K35  */  be_nested_str_weak(mdns),
  /* K36  */  be_nested_str_weak(mdns_pase_eth),
  /* K37  */  be_nested_str_weak(log),
  /* K38  */  be_nested_str_weak(MTR_X3A_X20calling_X20mdns_X2Eremove_service_X28_X25s_X2C_X20_X25s_X2C_X20_X25s_X2C_X20_X25s_X29),
  /* K39  */  be_nested_str_weak(_matterc),
  /* K40  */  be_nested_str_weak(_udp),
  /* K41  */  be_nested_str_weak(commissioning_instance_eth),
  /* K42  */  be_nested_str_weak(hostname_eth),
  /* K43  */  be_nested_str_weak(MTR_X3A_X20remove_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27),
  /* K44  */  be_nested_str_weak(eth),
  /* K45  */  be_nested_str_weak(remove_service),
  /* K46  */  be_nested_str_weak(mdns_pase_wifi),
  /* K47  */  be_nested_str_weak(commissioning_instance_wifi),
  /* K48  */  be_nested_str_weak(hostname_wifi),
  /* K49  */  be_nested_str_weak(wifi),
  /* K50  */  be_nested_str_weak(MTR_X3A_X20Exception),
  /* K51  */  be_nested_str_weak(_X7C),
  /* K52  */  be_const_int(2),
  /* K53  */  be_nested_str_weak(_X251i_X2505i_X2504i),
  /* K54  */  be_nested_str_weak(Verhoeff),
  /* K55  */  be_nested_str_weak(checksum),
  /* K56  */  be_nested_str_weak(copy),
  /* K57  */  be_nested_str_weak(reverse),
  /* K58  */  be_nested_str_weak(get_fabric_compressed),
  /* K59  */  be_nested_str_weak(tohex),
  /* K60  */  be_nested_str_weak(_X2D),
  /* K61  */  be_nested_str_weak(find),
  /* K62  */  be_nested_str_weak(up),
  /* K63  */  be_nested_str_weak(_matter),
  /* K64  */  be_nested_str_weak(_tcp),
  /* K65  */  be_nested_str_weak(defer),
  /* K66  */  be_nested_str_weak(crypto),
  /* K67  */  be_nested_str_weak(stop_basic_commissioning),
  /* K68  */  be_nested_str_weak(mdns_announce_op_discovery),
  /* K69  */  be_nested_str_weak(string),
  /* K70  */  be_nested_str_weak(start),
  /* K71  */  be_nested_str_weak(replace),
  /* K72  */  be_nested_str_weak(mac),
  /* K73  */  be_nested_str_weak(_X3A),
  /* K74  */  be_nested_str_weak(),
  /* K75  */  be_nested_str_weak(ipv4only),
  /* K76  */  be_nested_str_weak(contains),
  /* K77  */  be_nested_str_weak(ip6local),
  /* K78  */  be_nested_str_weak(add_hostname),
  /* K79  */  be_nested_str_weak(ip),
  /* K80  */  be_nested_str_weak(ip6),
  /* K81  */  be_nested_str_weak(MTR_X3A_X20calling_X20mdns_X2Eadd_hostname_X28_X25s_X2C_X20_X25s_X29),
  /* K82  */  be_nested_str_weak(MTR_X3A_X20start_X20mDNS_X20on_X20_X25s_X20host_X20_X27_X25s_X2Elocal_X27),
  /* K83  */  be_nested_str_weak(mdns_announce_op_discovery_all_fabrics),
  /* K84  */  be_nested_str_weak(PASE_TIMEOUT),
  /* K85  */  be_nested_str_weak(compute_manual_pairing_code),
  /* K86  */  be_nested_str_weak(MTR_X3A_X20Manual_X20pairing_X20code_X3A_X20_X25s),
  /* K87  */  be_nested_str_weak(compute_qrcode_content),
  /* K88  */  be_nested_str_weak(_X7B_X22Matter_X22_X3A_X7B_X22Commissioning_X22_X3A1_X2C_X22PairingCode_X22_X3A_X22_X25s_X22_X2C_X22QRCode_X22_X3A_X22_X25s_X22_X7D_X7D),
  /* K89  */  be_nested_str_weak(random),
  /* K90  */  be_nested_str_weak(add),
  /* K91  */  be_nested_str_weak(PBKDF2_HMAC_SHA256),
  /* K92  */  be_nested_str_weak(derive),
  /* K93  */  be_nested_str_weak(PBKDF_ITERATIONS),
  /* K94  */  be_nested_str_weak(EC_P256),
  /* K95  */  be_nested_str_weak(mod),
  /* K96  */  be_nested_str_weak(public_key),
  /* K97  */  be_nested_str_weak(start_basic_commissioning),
  /* K98  */  be_nested_str_weak(get),
  /* K99  */  be_const_int(99999998),
  /* K100 */  be_nested_str_weak(PASSCODE_INVALID),
  /* K101 */  be_nested_str_weak(MTR_X3A_X20Operational_X20Discovery_X20node_X20_X3D_X20),
  /* K102 */  be_nested_str_weak(MTR_X3A_X20adding_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
  /* K103 */  be_nested_str_weak(add_service),
  /* K104 */  be_nested_str_weak(_I),
  /* K105 */  be_nested_str_weak(MTR_X3A_X20adding_X20subtype_X3A_X20),
  /* K106 */  be_nested_str_weak(add_subtype),
  /* K107 */  be_nested_str_weak(time_reached),
  /* K108 */  be_nested_str_weak(VP),
  /* K109 */  be_nested_str_weak(_X25s_X2B_X25s),
  /* K110 */  be_nested_str_weak(D),
  /* K111 */  be_nested_str_weak(CM),
  /* K112 */  be_const_int(1),
  /* K113 */  be_nested_str_weak(T),
  /* K114 */  be_nested_str_weak(SII),
  /* K115 */  be_nested_str_weak(SAI),
  /* K116 */  be_nested_str_weak(MTR_X3A_X20announce_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
  /* K117 */  be_nested_str_weak(_L),
  /* K118 */  be_nested_str_weak(_S),
  /* K119 */  be_nested_str_weak(_V),
  /* K120 */  be_nested_str_weak(_CM1),
  /* K121 */  be_nested_str_weak(MTR_X3A_X20starting_X20mDNS_X20on_X20_X25s_X20_X27_X25s_X27_X20ptr_X20to_X20_X60_X25s_X2Elocal_X60),
  /* K122 */  be_nested_str_weak(millis),
  /* K123 */  be_nested_str_weak(when_network_up),
  /* K124 */  be_nested_str_weak(get_fabric),
  /* K125 */  be_nested_str_weak(get_admin_vendor_name),
  /* K126 */  be_nested_str_weak(MTR_X3A_X20_X2D_X2D_X2D_X20Commissioning_X20complete_X20for_X20Fabric_X20_X27_X25s_X27_X20_X28Vendor_X20_X25s_X29_X20_X2D_X2D_X2D),
};


extern const bclass be_class_Matter_Commissioning;

/********************************************************************
** Solidified function: mdns_remove_op_discovery_all_fabrics
********************************************************************/
be_local_closure(class_Matter_Commissioning_mdns_remove_op_discovery_all_fabrics,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(mdns_remove_op_discovery_all_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x88080501,  //  0002  GETMBR	R2	R2	K1
      0x8C080502,  //  0003  GETMET	R2	R2	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x7C040200,  //  0005  CALL	R1	1
      0xA802000B,  //  0006  EXBLK	0	#0013
      0x5C080200,  //  0007  MOVE	R2	R1
      0x7C080000,  //  0008  CALL	R2	0
      0x8C0C0503,  //  0009  GETMET	R3	R2	K3
      0x7C0C0200,  //  000A  CALL	R3	1
      0x780E0005,  //  000B  JMPF	R3	#0012
      0x8C0C0504,  //  000C  GETMET	R3	R2	K4
      0x7C0C0200,  //  000D  CALL	R3	1
      0x780E0002,  //  000E  JMPF	R3	#0012
      0x8C0C0105,  //  000F  GETMET	R3	R0	K5
      0x5C140400,  //  0010  MOVE	R5	R2
      0x7C0C0400,  //  0011  CALL	R3	2
      0x7001FFF3,  //  0012  JMP		#0007
      0x58040006,  //  0013  LDCONST	R1	K6
      0xAC040200,  //  0014  CATCH	R1	1	0
      0xB0080000,  //  0015  RAISE	2	R0	R0
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop_basic_commissioning
********************************************************************/
be_local_closure(class_Matter_Commissioning_stop_basic_commissioning,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(stop_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0x4C040000,  //  0000  LDNIL	R1
      0x8C080107,  //  0001  GETMET	R2	R0	K7
      0x7C080200,  //  0002  CALL	R2	1
      0x780A0004,  //  0003  JMPF	R2	#0009
      0xB80A1000,  //  0004  GETNGBL	R2	K8
      0x8C080509,  //  0005  GETMET	R2	R2	K9
      0x5810000A,  //  0006  LDCONST	R4	K10
      0x5814000B,  //  0007  LDCONST	R5	K11
      0x7C080600,  //  0008  CALL	R2	3
      0x90021801,  //  0009  SETMBR	R0	K12	R1
      0x8C08010D,  //  000A  GETMET	R2	R0	K13
      0x7C080200,  //  000B  CALL	R2	1
      0x90021C01,  //  000C  SETMBR	R0	K14	R1
      0x90021E01,  //  000D  SETMBR	R0	K15	R1
      0x90022001,  //  000E  SETMBR	R0	K16	R1
      0x90022201,  //  000F  SETMBR	R0	K17	R1
      0x90022401,  //  0010  SETMBR	R0	K18	R1
      0x90022601,  //  0011  SETMBR	R0	K19	R1
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init_basic_commissioning
********************************************************************/
be_local_closure(class_Matter_Commissioning_init_basic_commissioning,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(init_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x88040301,  //  0001  GETMBR	R1	R1	K1
      0x8C040314,  //  0002  GETMET	R1	R1	K20
      0x7C040200,  //  0003  CALL	R1	1
      0x1C040315,  //  0004  EQ	R1	R1	K21
      0x78060001,  //  0005  JMPF	R1	#0008
      0x8C040116,  //  0006  GETMET	R1	R0	K22
      0x7C040200,  //  0007  CALL	R1	1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compute_qrcode_content
********************************************************************/
be_local_closure(class_Matter_Commissioning_compute_qrcode_content,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(compute_qrcode_content),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0x60040015,  //  0000  GETGBL	R1	G21
      0x7C040000,  //  0001  CALL	R1	0
      0x8C040317,  //  0002  GETMET	R1	R1	K23
      0x540E000A,  //  0003  LDINT	R3	11
      0x7C040400,  //  0004  CALL	R1	2
      0x8C080318,  //  0005  GETMET	R2	R1	K24
      0x58100019,  //  0006  LDCONST	R4	K25
      0x5416000F,  //  0007  LDINT	R5	16
      0x88180100,  //  0008  GETMBR	R6	R0	K0
      0x88180D1A,  //  0009  GETMBR	R6	R6	K26
      0x7C080800,  //  000A  CALL	R2	4
      0x8C080318,  //  000B  GETMET	R2	R1	K24
      0x54120012,  //  000C  LDINT	R4	19
      0x5416000F,  //  000D  LDINT	R5	16
      0x88180100,  //  000E  GETMBR	R6	R0	K0
      0x88180D1B,  //  000F  GETMBR	R6	R6	K27
      0x7C080800,  //  0010  CALL	R2	4
      0x8C080318,  //  0011  GETMET	R2	R1	K24
      0x54120024,  //  0012  LDINT	R4	37
      0x54160007,  //  0013  LDINT	R5	8
      0x541A0003,  //  0014  LDINT	R6	4
      0x7C080800,  //  0015  CALL	R2	4
      0x8C080318,  //  0016  GETMET	R2	R1	K24
      0x5412002C,  //  0017  LDINT	R4	45
      0x5416000B,  //  0018  LDINT	R5	12
      0x88180100,  //  0019  GETMBR	R6	R0	K0
      0x88180D1C,  //  001A  GETMBR	R6	R6	K28
      0x541E0FFE,  //  001B  LDINT	R7	4095
      0x2C180C07,  //  001C  AND	R6	R6	R7
      0x7C080800,  //  001D  CALL	R2	4
      0x8C080318,  //  001E  GETMET	R2	R1	K24
      0x54120038,  //  001F  LDINT	R4	57
      0x5416001A,  //  0020  LDINT	R5	27
      0x88180100,  //  0021  GETMBR	R6	R0	K0
      0x88180D1D,  //  0022  GETMBR	R6	R6	K29
      0x2C180D1E,  //  0023  AND	R6	R6	K30
      0x7C080800,  //  0024  CALL	R2	4
      0xB80A4000,  //  0025  GETNGBL	R2	K32
      0x88080521,  //  0026  GETMBR	R2	R2	K33
      0x8C080522,  //  0027  GETMET	R2	R2	K34
      0x5C100200,  //  0028  MOVE	R4	R1
      0x7C080400,  //  0029  CALL	R2	2
      0x000A3E02,  //  002A  ADD	R2	K31	R2
      0x80040400,  //  002B  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_remove_PASE
********************************************************************/
be_local_closure(class_Matter_Commissioning_mdns_remove_PASE,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(mdns_remove_PASE),
    &be_const_str_solidified,
    ( &(const binstruction[77]) {  /* code */
      0xA4064600,  //  0000  IMPORT	R1	K35
      0xA8020039,  //  0001  EXBLK	0	#003C
      0x88080124,  //  0002  GETMBR	R2	R0	K36
      0x780A0019,  //  0003  JMPF	R2	#001E
      0xB80A4A00,  //  0004  GETNGBL	R2	K37
      0x600C0018,  //  0005  GETGBL	R3	G24
      0x58100026,  //  0006  LDCONST	R4	K38
      0x58140027,  //  0007  LDCONST	R5	K39
      0x58180028,  //  0008  LDCONST	R6	K40
      0x881C0129,  //  0009  GETMBR	R7	R0	K41
      0x8820012A,  //  000A  GETMBR	R8	R0	K42
      0x7C0C0A00,  //  000B  CALL	R3	5
      0x58100019,  //  000C  LDCONST	R4	K25
      0x7C080400,  //  000D  CALL	R2	2
      0xB80A4A00,  //  000E  GETNGBL	R2	K37
      0x600C0018,  //  000F  GETGBL	R3	G24
      0x5810002B,  //  0010  LDCONST	R4	K43
      0x5814002C,  //  0011  LDCONST	R5	K44
      0x88180129,  //  0012  GETMBR	R6	R0	K41
      0x7C0C0600,  //  0013  CALL	R3	3
      0x58100019,  //  0014  LDCONST	R4	K25
      0x7C080400,  //  0015  CALL	R2	2
      0x50080000,  //  0016  LDBOOL	R2	0	0
      0x90024802,  //  0017  SETMBR	R0	K36	R2
      0x8C08032D,  //  0018  GETMET	R2	R1	K45
      0x58100027,  //  0019  LDCONST	R4	K39
      0x58140028,  //  001A  LDCONST	R5	K40
      0x88180129,  //  001B  GETMBR	R6	R0	K41
      0x881C012A,  //  001C  GETMBR	R7	R0	K42
      0x7C080A00,  //  001D  CALL	R2	5
      0x8808012E,  //  001E  GETMBR	R2	R0	K46
      0x780A0019,  //  001F  JMPF	R2	#003A
      0xB80A4A00,  //  0020  GETNGBL	R2	K37
      0x600C0018,  //  0021  GETGBL	R3	G24
      0x58100026,  //  0022  LDCONST	R4	K38
      0x58140027,  //  0023  LDCONST	R5	K39
      0x58180028,  //  0024  LDCONST	R6	K40
      0x881C012F,  //  0025  GETMBR	R7	R0	K47
      0x88200130,  //  0026  GETMBR	R8	R0	K48
      0x7C0C0A00,  //  0027  CALL	R3	5
      0x58100019,  //  0028  LDCONST	R4	K25
      0x7C080400,  //  0029  CALL	R2	2
      0xB80A4A00,  //  002A  GETNGBL	R2	K37
      0x600C0018,  //  002B  GETGBL	R3	G24
      0x5810002B,  //  002C  LDCONST	R4	K43
      0x58140031,  //  002D  LDCONST	R5	K49
      0x8818012F,  //  002E  GETMBR	R6	R0	K47
      0x7C0C0600,  //  002F  CALL	R3	3
      0x58100019,  //  0030  LDCONST	R4	K25
      0x7C080400,  //  0031  CALL	R2	2
      0x50080000,  //  0032  LDBOOL	R2	0	0
      0x90025C02,  //  0033  SETMBR	R0	K46	R2
      0x8C08032D,  //  0034  GETMET	R2	R1	K45
      0x58100027,  //  0035  LDCONST	R4	K39
      0x58140028,  //  0036  LDCONST	R5	K40
      0x8818012F,  //  0037  GETMBR	R6	R0	K47
      0x881C0130,  //  0038  GETMBR	R7	R0	K48
      0x7C080A00,  //  0039  CALL	R2	5
      0xA8040001,  //  003A  EXBLK	1	1
      0x7002000F,  //  003B  JMP		#004C
      0xAC080002,  //  003C  CATCH	R2	0	2
      0x7002000C,  //  003D  JMP		#004B
      0xB8124A00,  //  003E  GETNGBL	R4	K37
      0x60140008,  //  003F  GETGBL	R5	G8
      0x5C180400,  //  0040  MOVE	R6	R2
      0x7C140200,  //  0041  CALL	R5	1
      0x00166405,  //  0042  ADD	R5	K50	R5
      0x00140B33,  //  0043  ADD	R5	R5	K51
      0x60180008,  //  0044  GETGBL	R6	G8
      0x5C1C0600,  //  0045  MOVE	R7	R3
      0x7C180200,  //  0046  CALL	R6	1
      0x00140A06,  //  0047  ADD	R5	R5	R6
      0x58180034,  //  0048  LDCONST	R6	K52
      0x7C100400,  //  0049  CALL	R4	2
      0x70020000,  //  004A  JMP		#004C
      0xB0080000,  //  004B  RAISE	2	R0	R0
      0x80000000,  //  004C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: compute_manual_pairing_code
********************************************************************/
be_local_closure(class_Matter_Commissioning_compute_manual_pairing_code,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(compute_manual_pairing_code),
    &be_const_str_solidified,
    ( &(const binstruction[34]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8804031C,  //  0001  GETMBR	R1	R1	K28
      0x540A0FFE,  //  0002  LDINT	R2	4095
      0x2C040202,  //  0003  AND	R1	R1	R2
      0x540A0009,  //  0004  LDINT	R2	10
      0x3C040202,  //  0005  SHR	R1	R1	R2
      0x88080100,  //  0006  GETMBR	R2	R0	K0
      0x8808051C,  //  0007  GETMBR	R2	R2	K28
      0x540E02FF,  //  0008  LDINT	R3	768
      0x2C080403,  //  0009  AND	R2	R2	R3
      0x540E0005,  //  000A  LDINT	R3	6
      0x38080403,  //  000B  SHL	R2	R2	R3
      0x880C0100,  //  000C  GETMBR	R3	R0	K0
      0x880C071D,  //  000D  GETMBR	R3	R3	K29
      0x54123FFE,  //  000E  LDINT	R4	16383
      0x2C0C0604,  //  000F  AND	R3	R3	R4
      0x30080403,  //  0010  OR	R2	R2	R3
      0x880C0100,  //  0011  GETMBR	R3	R0	K0
      0x880C071D,  //  0012  GETMBR	R3	R3	K29
      0x5412000D,  //  0013  LDINT	R4	14
      0x3C0C0604,  //  0014  SHR	R3	R3	R4
      0x60100018,  //  0015  GETGBL	R4	G24
      0x58140035,  //  0016  LDCONST	R5	K53
      0x5C180200,  //  0017  MOVE	R6	R1
      0x5C1C0400,  //  0018  MOVE	R7	R2
      0x5C200600,  //  0019  MOVE	R8	R3
      0x7C100800,  //  001A  CALL	R4	4
      0xB8164000,  //  001B  GETNGBL	R5	K32
      0x88140B36,  //  001C  GETMBR	R5	R5	K54
      0x8C140B37,  //  001D  GETMET	R5	R5	K55
      0x5C1C0800,  //  001E  MOVE	R7	R4
      0x7C140400,  //  001F  CALL	R5	2
      0x00100805,  //  0020  ADD	R4	R4	R5
      0x80040800,  //  0021  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Commissioning_init,   /* name */
  be_nested_proto(
    2,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
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
** Solidified function: mdns_remove_op_discovery
********************************************************************/
be_local_closure(class_Matter_Commissioning_mdns_remove_op_discovery,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(mdns_remove_op_discovery),
    &be_const_str_solidified,
    ( &(const binstruction[77]) {  /* code */
      0xA40A4600,  //  0000  IMPORT	R2	K35
      0xA8020039,  //  0001  EXBLK	0	#003C
      0x8C0C0303,  //  0002  GETMET	R3	R1	K3
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8C0C0738,  //  0004  GETMET	R3	R3	K56
      0x7C0C0200,  //  0005  CALL	R3	1
      0x8C0C0739,  //  0006  GETMET	R3	R3	K57
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C10033A,  //  0008  GETMET	R4	R1	K58
      0x7C100200,  //  0009  CALL	R4	1
      0x8C14093B,  //  000A  GETMET	R5	R4	K59
      0x7C140200,  //  000B  CALL	R5	1
      0x00140B3C,  //  000C  ADD	R5	R5	K60
      0x8C18073B,  //  000D  GETMET	R6	R3	K59
      0x7C180200,  //  000E  CALL	R6	1
      0x00140A06,  //  000F  ADD	R5	R5	R6
      0xB81A1000,  //  0010  GETNGBL	R6	K8
      0x8C180D2C,  //  0011  GETMET	R6	R6	K44
      0x7C180200,  //  0012  CALL	R6	1
      0x8C180D3D,  //  0013  GETMET	R6	R6	K61
      0x5820003E,  //  0014  LDCONST	R8	K62
      0x7C180400,  //  0015  CALL	R6	2
      0x781A000D,  //  0016  JMPF	R6	#0025
      0xB81A4A00,  //  0017  GETNGBL	R6	K37
      0x601C0018,  //  0018  GETGBL	R7	G24
      0x5820002B,  //  0019  LDCONST	R8	K43
      0x5824002C,  //  001A  LDCONST	R9	K44
      0x5C280A00,  //  001B  MOVE	R10	R5
      0x7C1C0600,  //  001C  CALL	R7	3
      0x58200019,  //  001D  LDCONST	R8	K25
      0x7C180400,  //  001E  CALL	R6	2
      0x8C18052D,  //  001F  GETMET	R6	R2	K45
      0x5820003F,  //  0020  LDCONST	R8	K63
      0x58240040,  //  0021  LDCONST	R9	K64
      0x5C280A00,  //  0022  MOVE	R10	R5
      0x882C012A,  //  0023  GETMBR	R11	R0	K42
      0x7C180A00,  //  0024  CALL	R6	5
      0xB81A1000,  //  0025  GETNGBL	R6	K8
      0x8C180D31,  //  0026  GETMET	R6	R6	K49
      0x7C180200,  //  0027  CALL	R6	1
      0x8C180D3D,  //  0028  GETMET	R6	R6	K61
      0x5820003E,  //  0029  LDCONST	R8	K62
      0x7C180400,  //  002A  CALL	R6	2
      0x781A000D,  //  002B  JMPF	R6	#003A
      0xB81A4A00,  //  002C  GETNGBL	R6	K37
      0x601C0018,  //  002D  GETGBL	R7	G24
      0x5820002B,  //  002E  LDCONST	R8	K43
      0x58240031,  //  002F  LDCONST	R9	K49
      0x5C280A00,  //  0030  MOVE	R10	R5
      0x7C1C0600,  //  0031  CALL	R7	3
      0x58200019,  //  0032  LDCONST	R8	K25
      0x7C180400,  //  0033  CALL	R6	2
      0x8C18052D,  //  0034  GETMET	R6	R2	K45
      0x5820003F,  //  0035  LDCONST	R8	K63
      0x58240040,  //  0036  LDCONST	R9	K64
      0x5C280A00,  //  0037  MOVE	R10	R5
      0x882C0130,  //  0038  GETMBR	R11	R0	K48
      0x7C180A00,  //  0039  CALL	R6	5
      0xA8040001,  //  003A  EXBLK	1	1
      0x7002000F,  //  003B  JMP		#004C
      0xAC0C0002,  //  003C  CATCH	R3	0	2
      0x7002000C,  //  003D  JMP		#004B
      0xB8164A00,  //  003E  GETNGBL	R5	K37
      0x60180008,  //  003F  GETGBL	R6	G8
      0x5C1C0600,  //  0040  MOVE	R7	R3
      0x7C180200,  //  0041  CALL	R6	1
      0x001A6406,  //  0042  ADD	R6	K50	R6
      0x00180D33,  //  0043  ADD	R6	R6	K51
      0x601C0008,  //  0044  GETGBL	R7	G8
      0x5C200800,  //  0045  MOVE	R8	R4
      0x7C1C0200,  //  0046  CALL	R7	1
      0x00180C07,  //  0047  ADD	R6	R6	R7
      0x581C0034,  //  0048  LDCONST	R7	K52
      0x7C140400,  //  0049  CALL	R5	2
      0x70020000,  //  004A  JMP		#004C
      0xB0080000,  //  004B  RAISE	2	R0	R0
      0x80000000,  //  004C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_operational_discovery_deferred
********************************************************************/
be_local_closure(class_Matter_Commissioning_start_operational_discovery_deferred,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
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
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(start_operational_discovery_deferred),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xB80A1000,  //  0000  GETNGBL	R2	K8
      0x8C080541,  //  0001  GETMET	R2	R2	K65
      0x84100000,  //  0002  CLOSURE	R4	P0
      0x7C080400,  //  0003  CALL	R2	2
      0xA0000000,  //  0004  CLOSE	R0
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_operational_discovery
********************************************************************/
be_local_closure(class_Matter_Commissioning_start_operational_discovery,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(start_operational_discovery),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0xA40A8400,  //  0000  IMPORT	R2	K66
      0xA40E4600,  //  0001  IMPORT	R3	K35
      0x8C100143,  //  0002  GETMET	R4	R0	K67
      0x7C100200,  //  0003  CALL	R4	1
      0x8C100144,  //  0004  GETMET	R4	R0	K68
      0x5C180200,  //  0005  MOVE	R6	R1
      0x7C100400,  //  0006  CALL	R4	2
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _mdns_announce_hostname
********************************************************************/
be_local_closure(class_Matter_Commissioning__mdns_announce_hostname,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(_mdns_announce_hostname),
    &be_const_str_solidified,
    ( &(const binstruction[146]) {  /* code */
      0xA40A4600,  //  0000  IMPORT	R2	K35
      0xA40E8A00,  //  0001  IMPORT	R3	K69
      0x8C100546,  //  0002  GETMET	R4	R2	K70
      0x7C100200,  //  0003  CALL	R4	1
      0xA8020079,  //  0004  EXBLK	0	#007F
      0x78060034,  //  0005  JMPF	R1	#003B
      0xB8121000,  //  0006  GETNGBL	R4	K8
      0x8C10092C,  //  0007  GETMET	R4	R4	K44
      0x7C100200,  //  0008  CALL	R4	1
      0x8C140747,  //  0009  GETMET	R5	R3	K71
      0x8C1C093D,  //  000A  GETMET	R7	R4	K61
      0x58240048,  //  000B  LDCONST	R9	K72
      0x7C1C0400,  //  000C  CALL	R7	2
      0x58200049,  //  000D  LDCONST	R8	K73
      0x5824004A,  //  000E  LDCONST	R9	K74
      0x7C140800,  //  000F  CALL	R5	4
      0x90025405,  //  0010  SETMBR	R0	K42	R5
      0x88140100,  //  0011  GETMBR	R5	R0	K0
      0x88140B4B,  //  0012  GETMBR	R5	R5	K75
      0x78160003,  //  0013  JMPF	R5	#0018
      0x8C14094C,  //  0014  GETMET	R5	R4	K76
      0x581C004D,  //  0015  LDCONST	R7	K77
      0x7C140400,  //  0016  CALL	R5	2
      0x7416000F,  //  0017  JMPT	R5	#0028
      0x8C14054E,  //  0018  GETMET	R5	R2	K78
      0x881C012A,  //  0019  GETMBR	R7	R0	K42
      0x8C20093D,  //  001A  GETMET	R8	R4	K61
      0x5828004D,  //  001B  LDCONST	R10	K77
      0x582C004A,  //  001C  LDCONST	R11	K74
      0x7C200600,  //  001D  CALL	R8	3
      0x8C24093D,  //  001E  GETMET	R9	R4	K61
      0x582C004F,  //  001F  LDCONST	R11	K79
      0x5830004A,  //  0020  LDCONST	R12	K74
      0x7C240600,  //  0021  CALL	R9	3
      0x8C28093D,  //  0022  GETMET	R10	R4	K61
      0x58300050,  //  0023  LDCONST	R12	K80
      0x5834004A,  //  0024  LDCONST	R13	K74
      0x7C280600,  //  0025  CALL	R10	3
      0x7C140A00,  //  0026  CALL	R5	5
      0x70020011,  //  0027  JMP		#003A
      0xB8164A00,  //  0028  GETNGBL	R5	K37
      0x60180018,  //  0029  GETGBL	R6	G24
      0x581C0051,  //  002A  LDCONST	R7	K81
      0x8820012A,  //  002B  GETMBR	R8	R0	K42
      0x8C24093D,  //  002C  GETMET	R9	R4	K61
      0x582C004F,  //  002D  LDCONST	R11	K79
      0x5830004A,  //  002E  LDCONST	R12	K74
      0x7C240600,  //  002F  CALL	R9	3
      0x7C180600,  //  0030  CALL	R6	3
      0x581C0019,  //  0031  LDCONST	R7	K25
      0x7C140400,  //  0032  CALL	R5	2
      0x8C14054E,  //  0033  GETMET	R5	R2	K78
      0x881C012A,  //  0034  GETMBR	R7	R0	K42
      0x8C20093D,  //  0035  GETMET	R8	R4	K61
      0x5828004F,  //  0036  LDCONST	R10	K79
      0x582C004A,  //  0037  LDCONST	R11	K74
      0x7C200600,  //  0038  CALL	R8	3
      0x7C140600,  //  0039  CALL	R5	3
      0x70020033,  //  003A  JMP		#006F
      0xB8121000,  //  003B  GETNGBL	R4	K8
      0x8C100931,  //  003C  GETMET	R4	R4	K49
      0x7C100200,  //  003D  CALL	R4	1
      0x8C140747,  //  003E  GETMET	R5	R3	K71
      0x8C1C093D,  //  003F  GETMET	R7	R4	K61
      0x58240048,  //  0040  LDCONST	R9	K72
      0x7C1C0400,  //  0041  CALL	R7	2
      0x58200049,  //  0042  LDCONST	R8	K73
      0x5824004A,  //  0043  LDCONST	R9	K74
      0x7C140800,  //  0044  CALL	R5	4
      0x90026005,  //  0045  SETMBR	R0	K48	R5
      0x88140100,  //  0046  GETMBR	R5	R0	K0
      0x88140B4B,  //  0047  GETMBR	R5	R5	K75
      0x78160003,  //  0048  JMPF	R5	#004D
      0x8C14094C,  //  0049  GETMET	R5	R4	K76
      0x581C004D,  //  004A  LDCONST	R7	K77
      0x7C140400,  //  004B  CALL	R5	2
      0x7416000F,  //  004C  JMPT	R5	#005D
      0x8C14054E,  //  004D  GETMET	R5	R2	K78
      0x881C0130,  //  004E  GETMBR	R7	R0	K48
      0x8C20093D,  //  004F  GETMET	R8	R4	K61
      0x5828004D,  //  0050  LDCONST	R10	K77
      0x582C004A,  //  0051  LDCONST	R11	K74
      0x7C200600,  //  0052  CALL	R8	3
      0x8C24093D,  //  0053  GETMET	R9	R4	K61
      0x582C004F,  //  0054  LDCONST	R11	K79
      0x5830004A,  //  0055  LDCONST	R12	K74
      0x7C240600,  //  0056  CALL	R9	3
      0x8C28093D,  //  0057  GETMET	R10	R4	K61
      0x58300050,  //  0058  LDCONST	R12	K80
      0x5834004A,  //  0059  LDCONST	R13	K74
      0x7C280600,  //  005A  CALL	R10	3
      0x7C140A00,  //  005B  CALL	R5	5
      0x70020011,  //  005C  JMP		#006F
      0xB8164A00,  //  005D  GETNGBL	R5	K37
      0x60180018,  //  005E  GETGBL	R6	G24
      0x581C0051,  //  005F  LDCONST	R7	K81
      0x88200130,  //  0060  GETMBR	R8	R0	K48
      0x8C24093D,  //  0061  GETMET	R9	R4	K61
      0x582C004F,  //  0062  LDCONST	R11	K79
      0x5830004A,  //  0063  LDCONST	R12	K74
      0x7C240600,  //  0064  CALL	R9	3
      0x7C180600,  //  0065  CALL	R6	3
      0x581C0019,  //  0066  LDCONST	R7	K25
      0x7C140400,  //  0067  CALL	R5	2
      0x8C14054E,  //  0068  GETMET	R5	R2	K78
      0x881C0130,  //  0069  GETMBR	R7	R0	K48
      0x8C20093D,  //  006A  GETMET	R8	R4	K61
      0x5828004F,  //  006B  LDCONST	R10	K79
      0x582C004A,  //  006C  LDCONST	R11	K74
      0x7C200600,  //  006D  CALL	R8	3
      0x7C140600,  //  006E  CALL	R5	3
      0xB8124A00,  //  006F  GETNGBL	R4	K37
      0x60140018,  //  0070  GETGBL	R5	G24
      0x58180052,  //  0071  LDCONST	R6	K82
      0x78060001,  //  0072  JMPF	R1	#0075
      0x581C002C,  //  0073  LDCONST	R7	K44
      0x70020000,  //  0074  JMP		#0076
      0x581C0031,  //  0075  LDCONST	R7	K49
      0x78060001,  //  0076  JMPF	R1	#0079
      0x8820012A,  //  0077  GETMBR	R8	R0	K42
      0x70020000,  //  0078  JMP		#007A
      0x88200130,  //  0079  GETMBR	R8	R0	K48
      0x7C140600,  //  007A  CALL	R5	3
      0x58180019,  //  007B  LDCONST	R6	K25
      0x7C100400,  //  007C  CALL	R4	2
      0xA8040001,  //  007D  EXBLK	1	1
      0x7002000F,  //  007E  JMP		#008F
      0xAC100002,  //  007F  CATCH	R4	0	2
      0x7002000C,  //  0080  JMP		#008E
      0xB81A4A00,  //  0081  GETNGBL	R6	K37
      0x601C0008,  //  0082  GETGBL	R7	G8
      0x5C200800,  //  0083  MOVE	R8	R4
      0x7C1C0200,  //  0084  CALL	R7	1
      0x001E6407,  //  0085  ADD	R7	K50	R7
      0x001C0F33,  //  0086  ADD	R7	R7	K51
      0x60200008,  //  0087  GETGBL	R8	G8
      0x5C240A00,  //  0088  MOVE	R9	R5
      0x7C200200,  //  0089  CALL	R8	1
      0x001C0E08,  //  008A  ADD	R7	R7	R8
      0x58200034,  //  008B  LDCONST	R8	K52
      0x7C180400,  //  008C  CALL	R6	2
      0x70020000,  //  008D  JMP		#008F
      0xB0080000,  //  008E  RAISE	2	R0	R0
      0x8C100153,  //  008F  GETMET	R4	R0	K83
      0x7C100200,  //  0090  CALL	R4	1
      0x80000000,  //  0091  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_announce_op_discovery_all_fabrics
********************************************************************/
be_local_closure(class_Matter_Commissioning_mdns_announce_op_discovery_all_fabrics,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(mdns_announce_op_discovery_all_fabrics),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x88080100,  //  0001  GETMBR	R2	R0	K0
      0x88080501,  //  0002  GETMBR	R2	R2	K1
      0x8C080502,  //  0003  GETMET	R2	R2	K2
      0x7C080200,  //  0004  CALL	R2	1
      0x7C040200,  //  0005  CALL	R1	1
      0xA802000B,  //  0006  EXBLK	0	#0013
      0x5C080200,  //  0007  MOVE	R2	R1
      0x7C080000,  //  0008  CALL	R2	0
      0x8C0C0503,  //  0009  GETMET	R3	R2	K3
      0x7C0C0200,  //  000A  CALL	R3	1
      0x780E0005,  //  000B  JMPF	R3	#0012
      0x8C0C0504,  //  000C  GETMET	R3	R2	K4
      0x7C0C0200,  //  000D  CALL	R3	1
      0x780E0002,  //  000E  JMPF	R3	#0012
      0x8C0C0144,  //  000F  GETMET	R3	R0	K68
      0x5C140400,  //  0010  MOVE	R5	R2
      0x7C0C0400,  //  0011  CALL	R3	2
      0x7001FFF3,  //  0012  JMP		#0007
      0x58040006,  //  0013  LDCONST	R1	K6
      0xAC040200,  //  0014  CATCH	R1	1	0
      0xB0080000,  //  0015  RAISE	2	R0	R0
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_root_basic_commissioning
********************************************************************/
be_local_closure(class_Matter_Commissioning_start_root_basic_commissioning,   /* name */
  be_nested_proto(
    22,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(start_root_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[76]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0x88040154,  //  0003  GETMBR	R1	R0	K84
      0x8C080155,  //  0004  GETMET	R2	R0	K85
      0x7C080200,  //  0005  CALL	R2	1
      0xB80E4A00,  //  0006  GETNGBL	R3	K37
      0x60100018,  //  0007  GETGBL	R4	G24
      0x58140056,  //  0008  LDCONST	R5	K86
      0x5C180400,  //  0009  MOVE	R6	R2
      0x7C100400,  //  000A  CALL	R4	2
      0x58140034,  //  000B  LDCONST	R5	K52
      0x7C0C0400,  //  000C  CALL	R3	2
      0x8C0C0157,  //  000D  GETMET	R3	R0	K87
      0x7C0C0200,  //  000E  CALL	R3	1
      0xB8121000,  //  000F  GETNGBL	R4	K8
      0x8C100909,  //  0010  GETMET	R4	R4	K9
      0x60180018,  //  0011  GETGBL	R6	G24
      0x581C0058,  //  0012  LDCONST	R7	K88
      0x5C200400,  //  0013  MOVE	R8	R2
      0x5C240600,  //  0014  MOVE	R9	R3
      0x7C180600,  //  0015  CALL	R6	3
      0x581C000B,  //  0016  LDCONST	R7	K11
      0x7C100600,  //  0017  CALL	R4	3
      0xA4128400,  //  0018  IMPORT	R4	K66
      0x8C140959,  //  0019  GETMET	R5	R4	K89
      0x541E000F,  //  001A  LDINT	R7	16
      0x7C140400,  //  001B  CALL	R5	2
      0x60180015,  //  001C  GETGBL	R6	G21
      0x7C180000,  //  001D  CALL	R6	0
      0x8C180D5A,  //  001E  GETMET	R6	R6	K90
      0x88200100,  //  001F  GETMBR	R8	R0	K0
      0x8820111D,  //  0020  GETMBR	R8	R8	K29
      0x54260003,  //  0021  LDINT	R9	4
      0x7C180600,  //  0022  CALL	R6	3
      0x8C1C095B,  //  0023  GETMET	R7	R4	K91
      0x7C1C0200,  //  0024  CALL	R7	1
      0x8C1C0F5C,  //  0025  GETMET	R7	R7	K92
      0x5C240C00,  //  0026  MOVE	R9	R6
      0x5C280A00,  //  0027  MOVE	R10	R5
      0x882C015D,  //  0028  GETMBR	R11	R0	K93
      0x5432004F,  //  0029  LDINT	R12	80
      0x7C1C0A00,  //  002A  CALL	R7	5
      0x54220026,  //  002B  LDINT	R8	39
      0x40222A08,  //  002C  CONNECT	R8	K21	R8
      0x94200E08,  //  002D  GETIDX	R8	R7	R8
      0x54260027,  //  002E  LDINT	R9	40
      0x542A004E,  //  002F  LDINT	R10	79
      0x4024120A,  //  0030  CONNECT	R9	R9	R10
      0x94240E09,  //  0031  GETIDX	R9	R7	R9
      0x8C28095E,  //  0032  GETMET	R10	R4	K94
      0x7C280200,  //  0033  CALL	R10	1
      0x8C28155F,  //  0034  GETMET	R10	R10	K95
      0x5C301000,  //  0035  MOVE	R12	R8
      0x7C280400,  //  0036  CALL	R10	2
      0x8C2C095E,  //  0037  GETMET	R11	R4	K94
      0x7C2C0200,  //  0038  CALL	R11	1
      0x8C2C175F,  //  0039  GETMET	R11	R11	K95
      0x5C341200,  //  003A  MOVE	R13	R9
      0x7C2C0400,  //  003B  CALL	R11	2
      0x8C30095E,  //  003C  GETMET	R12	R4	K94
      0x7C300200,  //  003D  CALL	R12	1
      0x8C301960,  //  003E  GETMET	R12	R12	K96
      0x5C381600,  //  003F  MOVE	R14	R11
      0x7C300400,  //  0040  CALL	R12	2
      0x8C340161,  //  0041  GETMET	R13	R0	K97
      0x5C3C0200,  //  0042  MOVE	R15	R1
      0x8840015D,  //  0043  GETMBR	R16	R0	K93
      0x88440100,  //  0044  GETMBR	R17	R0	K0
      0x8844231C,  //  0045  GETMBR	R17	R17	K28
      0x5C480A00,  //  0046  MOVE	R18	R5
      0x5C4C1400,  //  0047  MOVE	R19	R10
      0x5C501800,  //  0048  MOVE	R20	R12
      0x4C540000,  //  0049  LDNIL	R21
      0x7C341000,  //  004A  CALL	R13	8
      0x80000000,  //  004B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: generate_random_passcode
********************************************************************/
be_local_closure(class_Matter_Commissioning_generate_random_passcode,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(generate_random_passcode),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0xA4068400,  //  0000  IMPORT	R1	K66
      0x4C080000,  //  0001  LDNIL	R2
      0x500C0200,  //  0002  LDBOOL	R3	1	0
      0x780E001D,  //  0003  JMPF	R3	#0022
      0x8C0C0359,  //  0004  GETMET	R3	R1	K89
      0x54160003,  //  0005  LDINT	R5	4
      0x7C0C0400,  //  0006  CALL	R3	2
      0x8C0C0762,  //  0007  GETMET	R3	R3	K98
      0x58140015,  //  0008  LDCONST	R5	K21
      0x541A0003,  //  0009  LDINT	R6	4
      0x7C0C0600,  //  000A  CALL	R3	3
      0x2C0C071E,  //  000B  AND	R3	R3	K30
      0x5C080600,  //  000C  MOVE	R2	R3
      0x240C0563,  //  000D  GT	R3	R2	K99
      0x780E0000,  //  000E  JMPF	R3	#0010
      0x7001FFF1,  //  000F  JMP		#0002
      0x600C0010,  //  0010  GETGBL	R3	G16
      0x88100164,  //  0011  GETMBR	R4	R0	K100
      0x7C0C0200,  //  0012  CALL	R3	1
      0xA8020005,  //  0013  EXBLK	0	#001A
      0x5C100600,  //  0014  MOVE	R4	R3
      0x7C100000,  //  0015  CALL	R4	0
      0x1C140404,  //  0016  EQ	R5	R2	R4
      0x78160000,  //  0017  JMPF	R5	#0019
      0x4C080000,  //  0018  LDNIL	R2
      0x7001FFF9,  //  0019  JMP		#0014
      0x580C0006,  //  001A  LDCONST	R3	K6
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
** Solidified function: mdns_announce_op_discovery
********************************************************************/
be_local_closure(class_Matter_Commissioning_mdns_announce_op_discovery,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(mdns_announce_op_discovery),
    &be_const_str_solidified,
    ( &(const binstruction[115]) {  /* code */
      0xA40A4600,  //  0000  IMPORT	R2	K35
      0xA802005F,  //  0001  EXBLK	0	#0062
      0x8C0C0303,  //  0002  GETMET	R3	R1	K3
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8C0C0738,  //  0004  GETMET	R3	R3	K56
      0x7C0C0200,  //  0005  CALL	R3	1
      0x8C0C0739,  //  0006  GETMET	R3	R3	K57
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C10033A,  //  0008  GETMET	R4	R1	K58
      0x7C100200,  //  0009  CALL	R4	1
      0x8C14093B,  //  000A  GETMET	R5	R4	K59
      0x7C140200,  //  000B  CALL	R5	1
      0x00140B3C,  //  000C  ADD	R5	R5	K60
      0x8C18073B,  //  000D  GETMET	R6	R3	K59
      0x7C180200,  //  000E  CALL	R6	1
      0x00140A06,  //  000F  ADD	R5	R5	R6
      0xB81A4A00,  //  0010  GETNGBL	R6	K37
      0x001ECA05,  //  0011  ADD	R7	K101	R5
      0x58200019,  //  0012  LDCONST	R8	K25
      0x7C180400,  //  0013  CALL	R6	2
      0xB81A1000,  //  0014  GETNGBL	R6	K8
      0x8C180D2C,  //  0015  GETMET	R6	R6	K44
      0x7C180200,  //  0016  CALL	R6	1
      0x8C180D3D,  //  0017  GETMET	R6	R6	K61
      0x5820003E,  //  0018  LDCONST	R8	K62
      0x7C180400,  //  0019  CALL	R6	2
      0x781A001E,  //  001A  JMPF	R6	#003A
      0xB81A4A00,  //  001B  GETNGBL	R6	K37
      0x601C0018,  //  001C  GETGBL	R7	G24
      0x58200066,  //  001D  LDCONST	R8	K102
      0x5824002C,  //  001E  LDCONST	R9	K44
      0x5C280A00,  //  001F  MOVE	R10	R5
      0x882C012A,  //  0020  GETMBR	R11	R0	K42
      0x7C1C0800,  //  0021  CALL	R7	4
      0x58200019,  //  0022  LDCONST	R8	K25
      0x7C180400,  //  0023  CALL	R6	2
      0x8C180567,  //  0024  GETMET	R6	R2	K103
      0x5820003F,  //  0025  LDCONST	R8	K63
      0x58240040,  //  0026  LDCONST	R9	K64
      0x542A15A3,  //  0027  LDINT	R10	5540
      0x4C2C0000,  //  0028  LDNIL	R11
      0x5C300A00,  //  0029  MOVE	R12	R5
      0x8834012A,  //  002A  GETMBR	R13	R0	K42
      0x7C180E00,  //  002B  CALL	R6	7
      0x8C18093B,  //  002C  GETMET	R6	R4	K59
      0x7C180200,  //  002D  CALL	R6	1
      0x001AD006,  //  002E  ADD	R6	K104	R6
      0xB81E4A00,  //  002F  GETNGBL	R7	K37
      0x0022D206,  //  0030  ADD	R8	K105	R6
      0x58240019,  //  0031  LDCONST	R9	K25
      0x7C1C0400,  //  0032  CALL	R7	2
      0x8C1C056A,  //  0033  GETMET	R7	R2	K106
      0x5824003F,  //  0034  LDCONST	R9	K63
      0x58280040,  //  0035  LDCONST	R10	K64
      0x5C2C0A00,  //  0036  MOVE	R11	R5
      0x8830012A,  //  0037  GETMBR	R12	R0	K42
      0x5C340C00,  //  0038  MOVE	R13	R6
      0x7C1C0C00,  //  0039  CALL	R7	6
      0xB81A1000,  //  003A  GETNGBL	R6	K8
      0x8C180D31,  //  003B  GETMET	R6	R6	K49
      0x7C180200,  //  003C  CALL	R6	1
      0x8C180D3D,  //  003D  GETMET	R6	R6	K61
      0x5820003E,  //  003E  LDCONST	R8	K62
      0x7C180400,  //  003F  CALL	R6	2
      0x781A001E,  //  0040  JMPF	R6	#0060
      0xB81A4A00,  //  0041  GETNGBL	R6	K37
      0x601C0018,  //  0042  GETGBL	R7	G24
      0x58200066,  //  0043  LDCONST	R8	K102
      0x58240031,  //  0044  LDCONST	R9	K49
      0x5C280A00,  //  0045  MOVE	R10	R5
      0x882C0130,  //  0046  GETMBR	R11	R0	K48
      0x7C1C0800,  //  0047  CALL	R7	4
      0x58200019,  //  0048  LDCONST	R8	K25
      0x7C180400,  //  0049  CALL	R6	2
      0x8C180567,  //  004A  GETMET	R6	R2	K103
      0x5820003F,  //  004B  LDCONST	R8	K63
      0x58240040,  //  004C  LDCONST	R9	K64
      0x542A15A3,  //  004D  LDINT	R10	5540
      0x4C2C0000,  //  004E  LDNIL	R11
      0x5C300A00,  //  004F  MOVE	R12	R5
      0x88340130,  //  0050  GETMBR	R13	R0	K48
      0x7C180E00,  //  0051  CALL	R6	7
      0x8C18093B,  //  0052  GETMET	R6	R4	K59
      0x7C180200,  //  0053  CALL	R6	1
      0x001AD006,  //  0054  ADD	R6	K104	R6
      0xB81E4A00,  //  0055  GETNGBL	R7	K37
      0x0022D206,  //  0056  ADD	R8	K105	R6
      0x58240019,  //  0057  LDCONST	R9	K25
      0x7C1C0400,  //  0058  CALL	R7	2
      0x8C1C056A,  //  0059  GETMET	R7	R2	K106
      0x5824003F,  //  005A  LDCONST	R9	K63
      0x58280040,  //  005B  LDCONST	R10	K64
      0x5C2C0A00,  //  005C  MOVE	R11	R5
      0x88300130,  //  005D  GETMBR	R12	R0	K48
      0x5C340C00,  //  005E  MOVE	R13	R6
      0x7C1C0C00,  //  005F  CALL	R7	6
      0xA8040001,  //  0060  EXBLK	1	1
      0x7002000F,  //  0061  JMP		#0072
      0xAC0C0002,  //  0062  CATCH	R3	0	2
      0x7002000C,  //  0063  JMP		#0071
      0xB8164A00,  //  0064  GETNGBL	R5	K37
      0x60180008,  //  0065  GETGBL	R6	G8
      0x5C1C0600,  //  0066  MOVE	R7	R3
      0x7C180200,  //  0067  CALL	R6	1
      0x001A6406,  //  0068  ADD	R6	K50	R6
      0x00180D33,  //  0069  ADD	R6	R6	K51
      0x601C0008,  //  006A  GETGBL	R7	G8
      0x5C200800,  //  006B  MOVE	R8	R4
      0x7C1C0200,  //  006C  CALL	R7	1
      0x00180C07,  //  006D  ADD	R6	R6	R7
      0x581C0034,  //  006E  LDCONST	R7	K52
      0x7C140400,  //  006F  CALL	R5	2
      0x70020000,  //  0070  JMP		#0072
      0xB0080000,  //  0071  RAISE	2	R0	R0
      0x80000000,  //  0072  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_commissioning_complete_deferred
********************************************************************/
be_local_closure(class_Matter_Commissioning_start_commissioning_complete_deferred,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
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
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(start_commissioning_complete_deferred),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xB80A1000,  //  0000  GETNGBL	R2	K8
      0x8C080541,  //  0001  GETMET	R2	R2	K65
      0x84100000,  //  0002  CLOSURE	R4	P0
      0x7C080400,  //  0003  CALL	R2	2
      0xA0000000,  //  0004  CLOSE	R0
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(class_Matter_Commissioning_every_second,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x8804010C,  //  0000  GETMBR	R1	R0	K12
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060006,  //  0003  JMPF	R1	#000B
      0xB8061000,  //  0004  GETNGBL	R1	K8
      0x8C04036B,  //  0005  GETMET	R1	R1	K107
      0x880C010C,  //  0006  GETMBR	R3	R0	K12
      0x7C040400,  //  0007  CALL	R1	2
      0x78060001,  //  0008  JMPF	R1	#000B
      0x4C040000,  //  0009  LDNIL	R1
      0x90021801,  //  000A  SETMBR	R0	K12	R1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: mdns_announce_PASE
********************************************************************/
be_local_closure(class_Matter_Commissioning_mdns_announce_PASE,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(mdns_announce_PASE),
    &be_const_str_solidified,
    ( &(const binstruction[226]) {  /* code */
      0xA4064600,  //  0000  IMPORT	R1	K35
      0xA40A8400,  //  0001  IMPORT	R2	K66
      0x600C0013,  //  0002  GETGBL	R3	G19
      0x7C0C0000,  //  0003  CALL	R3	0
      0x60100018,  //  0004  GETGBL	R4	G24
      0x5814006D,  //  0005  LDCONST	R5	K109
      0x88180100,  //  0006  GETMBR	R6	R0	K0
      0x88180D1A,  //  0007  GETMBR	R6	R6	K26
      0x881C0100,  //  0008  GETMBR	R7	R0	K0
      0x881C0F1B,  //  0009  GETMBR	R7	R7	K27
      0x7C100600,  //  000A  CALL	R4	3
      0x980ED804,  //  000B  SETIDX	R3	K108	R4
      0x8810010F,  //  000C  GETMBR	R4	R0	K15
      0x980EDC04,  //  000D  SETIDX	R3	K110	R4
      0x980EDF70,  //  000E  SETIDX	R3	K111	K112
      0x980EE315,  //  000F  SETIDX	R3	K113	K21
      0x54121387,  //  0010  LDINT	R4	5000
      0x980EE404,  //  0011  SETIDX	R3	K114	R4
      0x5412012B,  //  0012  LDINT	R4	300
      0x980EE604,  //  0013  SETIDX	R3	K115	R4
      0x8C100559,  //  0014  GETMET	R4	R2	K89
      0x541A0007,  //  0015  LDINT	R6	8
      0x7C100400,  //  0016  CALL	R4	2
      0x8C10093B,  //  0017  GETMET	R4	R4	K59
      0x7C100200,  //  0018  CALL	R4	1
      0x90025E04,  //  0019  SETMBR	R0	K47	R4
      0x8C100559,  //  001A  GETMET	R4	R2	K89
      0x541A0007,  //  001B  LDINT	R6	8
      0x7C100400,  //  001C  CALL	R4	2
      0x8C10093B,  //  001D  GETMET	R4	R4	K59
      0x7C100200,  //  001E  CALL	R4	1
      0x90025204,  //  001F  SETMBR	R0	K41	R4
      0xA80200AF,  //  0020  EXBLK	0	#00D1
      0x8810012A,  //  0021  GETMBR	R4	R0	K42
      0x78120054,  //  0022  JMPF	R4	#0078
      0x8C100367,  //  0023  GETMET	R4	R1	K103
      0x58180027,  //  0024  LDCONST	R6	K39
      0x581C0028,  //  0025  LDCONST	R7	K40
      0x542215A3,  //  0026  LDINT	R8	5540
      0x5C240600,  //  0027  MOVE	R9	R3
      0x88280129,  //  0028  GETMBR	R10	R0	K41
      0x882C012A,  //  0029  GETMBR	R11	R0	K42
      0x7C100E00,  //  002A  CALL	R4	7
      0x50100200,  //  002B  LDBOOL	R4	1	0
      0x90024804,  //  002C  SETMBR	R0	K36	R4
      0xB8124A00,  //  002D  GETNGBL	R4	K37
      0x60140018,  //  002E  GETGBL	R5	G24
      0x58180074,  //  002F  LDCONST	R6	K116
      0x581C002C,  //  0030  LDCONST	R7	K44
      0x88200129,  //  0031  GETMBR	R8	R0	K41
      0x8824012A,  //  0032  GETMBR	R9	R0	K42
      0x7C140800,  //  0033  CALL	R5	4
      0x58180034,  //  0034  LDCONST	R6	K52
      0x7C100400,  //  0035  CALL	R4	2
      0x60100008,  //  0036  GETGBL	R4	G8
      0x8814010F,  //  0037  GETMBR	R5	R0	K15
      0x541A0FFE,  //  0038  LDINT	R6	4095
      0x2C140A06,  //  0039  AND	R5	R5	R6
      0x7C100200,  //  003A  CALL	R4	1
      0x0012EA04,  //  003B  ADD	R4	K117	R4
      0xB8164A00,  //  003C  GETNGBL	R5	K37
      0x001AD204,  //  003D  ADD	R6	K105	R4
      0x581C0019,  //  003E  LDCONST	R7	K25
      0x7C140400,  //  003F  CALL	R5	2
      0x8C14036A,  //  0040  GETMET	R5	R1	K106
      0x581C0027,  //  0041  LDCONST	R7	K39
      0x58200028,  //  0042  LDCONST	R8	K40
      0x88240129,  //  0043  GETMBR	R9	R0	K41
      0x8828012A,  //  0044  GETMBR	R10	R0	K42
      0x5C2C0800,  //  0045  MOVE	R11	R4
      0x7C140C00,  //  0046  CALL	R5	6
      0x60140008,  //  0047  GETGBL	R5	G8
      0x8818010F,  //  0048  GETMBR	R6	R0	K15
      0x541E0EFF,  //  0049  LDINT	R7	3840
      0x2C180C07,  //  004A  AND	R6	R6	R7
      0x541E0007,  //  004B  LDINT	R7	8
      0x3C180C07,  //  004C  SHR	R6	R6	R7
      0x7C140200,  //  004D  CALL	R5	1
      0x0016EC05,  //  004E  ADD	R5	K118	R5
      0x5C100A00,  //  004F  MOVE	R4	R5
      0xB8164A00,  //  0050  GETNGBL	R5	K37
      0x001AD204,  //  0051  ADD	R6	K105	R4
      0x581C0019,  //  0052  LDCONST	R7	K25
      0x7C140400,  //  0053  CALL	R5	2
      0x8C14036A,  //  0054  GETMET	R5	R1	K106
      0x581C0027,  //  0055  LDCONST	R7	K39
      0x58200028,  //  0056  LDCONST	R8	K40
      0x88240129,  //  0057  GETMBR	R9	R0	K41
      0x8828012A,  //  0058  GETMBR	R10	R0	K42
      0x5C2C0800,  //  0059  MOVE	R11	R4
      0x7C140C00,  //  005A  CALL	R5	6
      0x60140008,  //  005B  GETGBL	R5	G8
      0x88180100,  //  005C  GETMBR	R6	R0	K0
      0x88180D1A,  //  005D  GETMBR	R6	R6	K26
      0x7C140200,  //  005E  CALL	R5	1
      0x0016EE05,  //  005F  ADD	R5	K119	R5
      0x5C100A00,  //  0060  MOVE	R4	R5
      0xB8164A00,  //  0061  GETNGBL	R5	K37
      0x001AD204,  //  0062  ADD	R6	K105	R4
      0x581C0019,  //  0063  LDCONST	R7	K25
      0x7C140400,  //  0064  CALL	R5	2
      0x8C14036A,  //  0065  GETMET	R5	R1	K106
      0x581C0027,  //  0066  LDCONST	R7	K39
      0x58200028,  //  0067  LDCONST	R8	K40
      0x88240129,  //  0068  GETMBR	R9	R0	K41
      0x8828012A,  //  0069  GETMBR	R10	R0	K42
      0x5C2C0800,  //  006A  MOVE	R11	R4
      0x7C140C00,  //  006B  CALL	R5	6
      0x58100078,  //  006C  LDCONST	R4	K120
      0xB8164A00,  //  006D  GETNGBL	R5	K37
      0x001AD204,  //  006E  ADD	R6	K105	R4
      0x581C0019,  //  006F  LDCONST	R7	K25
      0x7C140400,  //  0070  CALL	R5	2
      0x8C14036A,  //  0071  GETMET	R5	R1	K106
      0x581C0027,  //  0072  LDCONST	R7	K39
      0x58200028,  //  0073  LDCONST	R8	K40
      0x88240129,  //  0074  GETMBR	R9	R0	K41
      0x8828012A,  //  0075  GETMBR	R10	R0	K42
      0x5C2C0800,  //  0076  MOVE	R11	R4
      0x7C140C00,  //  0077  CALL	R5	6
      0x88100130,  //  0078  GETMBR	R4	R0	K48
      0x78120054,  //  0079  JMPF	R4	#00CF
      0x8C100367,  //  007A  GETMET	R4	R1	K103
      0x58180027,  //  007B  LDCONST	R6	K39
      0x581C0028,  //  007C  LDCONST	R7	K40
      0x542215A3,  //  007D  LDINT	R8	5540
      0x5C240600,  //  007E  MOVE	R9	R3
      0x8828012F,  //  007F  GETMBR	R10	R0	K47
      0x882C0130,  //  0080  GETMBR	R11	R0	K48
      0x7C100E00,  //  0081  CALL	R4	7
      0x50100200,  //  0082  LDBOOL	R4	1	0
      0x90025C04,  //  0083  SETMBR	R0	K46	R4
      0xB8124A00,  //  0084  GETNGBL	R4	K37
      0x60140018,  //  0085  GETGBL	R5	G24
      0x58180079,  //  0086  LDCONST	R6	K121
      0x581C0031,  //  0087  LDCONST	R7	K49
      0x8820012F,  //  0088  GETMBR	R8	R0	K47
      0x88240130,  //  0089  GETMBR	R9	R0	K48
      0x7C140800,  //  008A  CALL	R5	4
      0x58180019,  //  008B  LDCONST	R6	K25
      0x7C100400,  //  008C  CALL	R4	2
      0x60100008,  //  008D  GETGBL	R4	G8
      0x8814010F,  //  008E  GETMBR	R5	R0	K15
      0x541A0FFE,  //  008F  LDINT	R6	4095
      0x2C140A06,  //  0090  AND	R5	R5	R6
      0x7C100200,  //  0091  CALL	R4	1
      0x0012EA04,  //  0092  ADD	R4	K117	R4
      0xB8164A00,  //  0093  GETNGBL	R5	K37
      0x001AD204,  //  0094  ADD	R6	K105	R4
      0x581C0019,  //  0095  LDCONST	R7	K25
      0x7C140400,  //  0096  CALL	R5	2
      0x8C14036A,  //  0097  GETMET	R5	R1	K106
      0x581C0027,  //  0098  LDCONST	R7	K39
      0x58200028,  //  0099  LDCONST	R8	K40
      0x8824012F,  //  009A  GETMBR	R9	R0	K47
      0x88280130,  //  009B  GETMBR	R10	R0	K48
      0x5C2C0800,  //  009C  MOVE	R11	R4
      0x7C140C00,  //  009D  CALL	R5	6
      0x60140008,  //  009E  GETGBL	R5	G8
      0x8818010F,  //  009F  GETMBR	R6	R0	K15
      0x541E0EFF,  //  00A0  LDINT	R7	3840
      0x2C180C07,  //  00A1  AND	R6	R6	R7
      0x541E0007,  //  00A2  LDINT	R7	8
      0x3C180C07,  //  00A3  SHR	R6	R6	R7
      0x7C140200,  //  00A4  CALL	R5	1
      0x0016EC05,  //  00A5  ADD	R5	K118	R5
      0x5C100A00,  //  00A6  MOVE	R4	R5
      0xB8164A00,  //  00A7  GETNGBL	R5	K37
      0x001AD204,  //  00A8  ADD	R6	K105	R4
      0x581C0019,  //  00A9  LDCONST	R7	K25
      0x7C140400,  //  00AA  CALL	R5	2
      0x8C14036A,  //  00AB  GETMET	R5	R1	K106
      0x581C0027,  //  00AC  LDCONST	R7	K39
      0x58200028,  //  00AD  LDCONST	R8	K40
      0x8824012F,  //  00AE  GETMBR	R9	R0	K47
      0x88280130,  //  00AF  GETMBR	R10	R0	K48
      0x5C2C0800,  //  00B0  MOVE	R11	R4
      0x7C140C00,  //  00B1  CALL	R5	6
      0x60140008,  //  00B2  GETGBL	R5	G8
      0x88180100,  //  00B3  GETMBR	R6	R0	K0
      0x88180D1A,  //  00B4  GETMBR	R6	R6	K26
      0x7C140200,  //  00B5  CALL	R5	1
      0x0016EE05,  //  00B6  ADD	R5	K119	R5
      0x5C100A00,  //  00B7  MOVE	R4	R5
      0xB8164A00,  //  00B8  GETNGBL	R5	K37
      0x001AD204,  //  00B9  ADD	R6	K105	R4
      0x581C0019,  //  00BA  LDCONST	R7	K25
      0x7C140400,  //  00BB  CALL	R5	2
      0x8C14036A,  //  00BC  GETMET	R5	R1	K106
      0x581C0027,  //  00BD  LDCONST	R7	K39
      0x58200028,  //  00BE  LDCONST	R8	K40
      0x8824012F,  //  00BF  GETMBR	R9	R0	K47
      0x88280130,  //  00C0  GETMBR	R10	R0	K48
      0x5C2C0800,  //  00C1  MOVE	R11	R4
      0x7C140C00,  //  00C2  CALL	R5	6
      0x58100078,  //  00C3  LDCONST	R4	K120
      0xB8164A00,  //  00C4  GETNGBL	R5	K37
      0x001AD204,  //  00C5  ADD	R6	K105	R4
      0x581C0019,  //  00C6  LDCONST	R7	K25
      0x7C140400,  //  00C7  CALL	R5	2
      0x8C14036A,  //  00C8  GETMET	R5	R1	K106
      0x581C0027,  //  00C9  LDCONST	R7	K39
      0x58200028,  //  00CA  LDCONST	R8	K40
      0x8824012F,  //  00CB  GETMBR	R9	R0	K47
      0x88280130,  //  00CC  GETMBR	R10	R0	K48
      0x5C2C0800,  //  00CD  MOVE	R11	R4
      0x7C140C00,  //  00CE  CALL	R5	6
      0xA8040001,  //  00CF  EXBLK	1	1
      0x7002000F,  //  00D0  JMP		#00E1
      0xAC100002,  //  00D1  CATCH	R4	0	2
      0x7002000C,  //  00D2  JMP		#00E0
      0xB81A4A00,  //  00D3  GETNGBL	R6	K37
      0x601C0008,  //  00D4  GETGBL	R7	G8
      0x5C200800,  //  00D5  MOVE	R8	R4
      0x7C1C0200,  //  00D6  CALL	R7	1
      0x001E6407,  //  00D7  ADD	R7	K50	R7
      0x001C0F33,  //  00D8  ADD	R7	R7	K51
      0x60200008,  //  00D9  GETGBL	R8	G8
      0x5C240A00,  //  00DA  MOVE	R9	R5
      0x7C200200,  //  00DB  CALL	R8	1
      0x001C0E08,  //  00DC  ADD	R7	R7	R8
      0x58200034,  //  00DD  LDCONST	R8	K52
      0x7C180400,  //  00DE  CALL	R6	2
      0x70020000,  //  00DF  JMP		#00E1
      0xB0080000,  //  00E0  RAISE	2	R0	R0
      0x80000000,  //  00E1  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_basic_commissioning
********************************************************************/
be_local_closure(class_Matter_Commissioning_start_basic_commissioning,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    8,                          /* argc */
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
        /* K0   */  be_nested_str_weak(mdns_announce_PASE),
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
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(start_basic_commissioning),
    &be_const_str_solidified,
    ( &(const binstruction[19]) {  /* code */
      0xB8221000,  //  0000  GETNGBL	R8	K8
      0x8C20117A,  //  0001  GETMET	R8	R8	K122
      0x7C200200,  //  0002  CALL	R8	1
      0x542603E7,  //  0003  LDINT	R9	1000
      0x08240209,  //  0004  MUL	R9	R1	R9
      0x00201009,  //  0005  ADD	R8	R8	R9
      0x90021808,  //  0006  SETMBR	R0	K12	R8
      0x90021C02,  //  0007  SETMBR	R0	K14	R2
      0x90021E03,  //  0008  SETMBR	R0	K15	R3
      0x90022004,  //  0009  SETMBR	R0	K16	R4
      0x90022205,  //  000A  SETMBR	R0	K17	R5
      0x90022406,  //  000B  SETMBR	R0	K18	R6
      0x90022607,  //  000C  SETMBR	R0	K19	R7
      0xB8221000,  //  000D  GETNGBL	R8	K8
      0x8C20117B,  //  000E  GETMET	R8	R8	K123
      0x84280000,  //  000F  CLOSURE	R10	P0
      0x7C200400,  //  0010  CALL	R8	2
      0xA0000000,  //  0011  CLOSE	R0
      0x80000000,  //  0012  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_mdns_announce_hostnames
********************************************************************/
be_local_closure(class_Matter_Commissioning_start_mdns_announce_hostnames,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    1,                          /* has sup protos */
    ( &(const struct bproto*[ 1]) {
      be_nested_proto(
        3,                          /* nstack */
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
        /* K0   */  be_nested_str_weak(tasmota),
        /* K1   */  be_nested_str_weak(wifi),
        /* K2   */  be_nested_str_weak(up),
        /* K3   */  be_nested_str_weak(_mdns_announce_hostname),
        /* K4   */  be_nested_str_weak(eth),
        }),
        be_str_weak(_anonymous_),
        &be_const_str_solidified,
        ( &(const binstruction[19]) {  /* code */
          0xB8020000,  //  0000  GETNGBL	R0	K0
          0x8C000101,  //  0001  GETMET	R0	R0	K1
          0x58080002,  //  0002  LDCONST	R2	K2
          0x7C000400,  //  0003  CALL	R0	2
          0x78020003,  //  0004  JMPF	R0	#0009
          0x68000000,  //  0005  GETUPV	R0	U0
          0x8C000103,  //  0006  GETMET	R0	R0	K3
          0x50080000,  //  0007  LDBOOL	R2	0	0
          0x7C000400,  //  0008  CALL	R0	2
          0xB8020000,  //  0009  GETNGBL	R0	K0
          0x8C000104,  //  000A  GETMET	R0	R0	K4
          0x58080002,  //  000B  LDCONST	R2	K2
          0x7C000400,  //  000C  CALL	R0	2
          0x78020003,  //  000D  JMPF	R0	#0012
          0x68000000,  //  000E  GETUPV	R0	U0
          0x8C000103,  //  000F  GETMET	R0	R0	K3
          0x50080200,  //  0010  LDBOOL	R2	1	0
          0x7C000400,  //  0011  CALL	R0	2
          0x80000000,  //  0012  RET	0
        })
      ),
    }),
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(start_mdns_announce_hostnames),
    &be_const_str_solidified,
    ( &(const binstruction[ 6]) {  /* code */
      0xB8061000,  //  0000  GETNGBL	R1	K8
      0x8C04037B,  //  0001  GETMET	R1	R1	K123
      0x840C0000,  //  0002  CLOSURE	R3	P0
      0x7C040400,  //  0003  CALL	R1	2
      0xA0000000,  //  0004  CLOSE	R0
      0x80000000,  //  0005  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: is_root_commissioning_open
********************************************************************/
be_local_closure(class_Matter_Commissioning_is_root_commissioning_open,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(is_root_commissioning_open),
    &be_const_str_solidified,
    ( &(const binstruction[11]) {  /* code */
      0x8804010C,  //  0000  GETMBR	R1	R0	K12
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x78060003,  //  0003  JMPF	R1	#0008
      0x88040113,  //  0004  GETMBR	R1	R0	K19
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
** Solidified function: is_commissioning_open
********************************************************************/
be_local_closure(class_Matter_Commissioning_is_commissioning_open,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(is_commissioning_open),
    &be_const_str_solidified,
    ( &(const binstruction[ 4]) {  /* code */
      0x8804010C,  //  0000  GETMBR	R1	R0	K12
      0x4C080000,  //  0001  LDNIL	R2
      0x20040202,  //  0002  NE	R1	R1	R2
      0x80040200,  //  0003  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start_commissioning_complete
********************************************************************/
be_local_closure(class_Matter_Commissioning_start_commissioning_complete,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Commissioning,     /* shared constants */
    be_str_weak(start_commissioning_complete),
    &be_const_str_solidified,
    ( &(const binstruction[23]) {  /* code */
      0x8C08037C,  //  0000  GETMET	R2	R1	K124
      0x7C080200,  //  0001  CALL	R2	1
      0x8C0C0504,  //  0002  GETMET	R3	R2	K4
      0x7C0C0200,  //  0003  CALL	R3	1
      0x8C0C0738,  //  0004  GETMET	R3	R3	K56
      0x7C0C0200,  //  0005  CALL	R3	1
      0x8C0C0739,  //  0006  GETMET	R3	R3	K57
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C0C073B,  //  0008  GETMET	R3	R3	K59
      0x7C0C0200,  //  0009  CALL	R3	1
      0x8C10057D,  //  000A  GETMET	R4	R2	K125
      0x7C100200,  //  000B  CALL	R4	1
      0xB8164A00,  //  000C  GETNGBL	R5	K37
      0x60180018,  //  000D  GETGBL	R6	G24
      0x581C007E,  //  000E  LDCONST	R7	K126
      0x5C200600,  //  000F  MOVE	R8	R3
      0x5C240800,  //  0010  MOVE	R9	R4
      0x7C180600,  //  0011  CALL	R6	3
      0x581C0034,  //  0012  LDCONST	R7	K52
      0x7C140400,  //  0013  CALL	R5	2
      0x8C140143,  //  0014  GETMET	R5	R0	K67
      0x7C140200,  //  0015  CALL	R5	1
      0x80000000,  //  0016  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Commissioning
********************************************************************/
be_local_class(Matter_Commissioning,
    14,
    NULL,
    be_nested_map(40,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(commissioning_admin_fabric, -1), be_const_var(7) },
        { be_const_key_weak(commissioning_w0, -1), be_const_var(5) },
        { be_const_key_weak(commissioning_instance_eth, 1), be_const_var(9) },
        { be_const_key_weak(start_commissioning_complete, -1), be_const_closure(class_Matter_Commissioning_start_commissioning_complete_closure) },
        { be_const_key_weak(stop_basic_commissioning, -1), be_const_closure(class_Matter_Commissioning_stop_basic_commissioning_closure) },
        { be_const_key_weak(mdns_pase_wifi, -1), be_const_var(13) },
        { be_const_key_weak(init_basic_commissioning, -1), be_const_closure(class_Matter_Commissioning_init_basic_commissioning_closure) },
        { be_const_key_weak(compute_qrcode_content, -1), be_const_closure(class_Matter_Commissioning_compute_qrcode_content_closure) },
        { be_const_key_weak(compute_manual_pairing_code, 38), be_const_closure(class_Matter_Commissioning_compute_manual_pairing_code_closure) },
        { be_const_key_weak(commissioning_open, -1), be_const_var(1) },
        { be_const_key_weak(mdns_remove_op_discovery_all_fabrics, 8), be_const_closure(class_Matter_Commissioning_mdns_remove_op_discovery_all_fabrics_closure) },
        { be_const_key_weak(device, -1), be_const_var(0) },
        { be_const_key_weak(start_operational_discovery, -1), be_const_closure(class_Matter_Commissioning_start_operational_discovery_closure) },
        { be_const_key_weak(PASSCODE_INVALID, -1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
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
        { be_const_key_weak(commissioning_discriminator, 30), be_const_var(3) },
        { be_const_key_weak(mdns_remove_op_discovery, 3), be_const_closure(class_Matter_Commissioning_mdns_remove_op_discovery_closure) },
        { be_const_key_weak(mdns_announce_op_discovery_all_fabrics, -1), be_const_closure(class_Matter_Commissioning_mdns_announce_op_discovery_all_fabrics_closure) },
        { be_const_key_weak(start_operational_discovery_deferred, 12), be_const_closure(class_Matter_Commissioning_start_operational_discovery_deferred_closure) },
        { be_const_key_weak(_mdns_announce_hostname, 28), be_const_closure(class_Matter_Commissioning__mdns_announce_hostname_closure) },
        { be_const_key_weak(commissioning_L, 16), be_const_var(6) },
        { be_const_key_weak(start_commissioning_complete_deferred, -1), be_const_closure(class_Matter_Commissioning_start_commissioning_complete_deferred_closure) },
        { be_const_key_weak(generate_random_passcode, -1), be_const_closure(class_Matter_Commissioning_generate_random_passcode_closure) },
        { be_const_key_weak(mdns_announce_op_discovery, -1), be_const_closure(class_Matter_Commissioning_mdns_announce_op_discovery_closure) },
        { be_const_key_weak(commissioning_instance_wifi, -1), be_const_var(8) },
        { be_const_key_weak(hostname_eth, -1), be_const_var(11) },
        { be_const_key_weak(start_root_basic_commissioning, 32), be_const_closure(class_Matter_Commissioning_start_root_basic_commissioning_closure) },
        { be_const_key_weak(PASE_TIMEOUT, 23), be_const_int(600) },
        { be_const_key_weak(mdns_announce_PASE, -1), be_const_closure(class_Matter_Commissioning_mdns_announce_PASE_closure) },
        { be_const_key_weak(start_basic_commissioning, -1), be_const_closure(class_Matter_Commissioning_start_basic_commissioning_closure) },
        { be_const_key_weak(start_mdns_announce_hostnames, -1), be_const_closure(class_Matter_Commissioning_start_mdns_announce_hostnames_closure) },
        { be_const_key_weak(mdns_pase_eth, 20), be_const_var(12) },
        { be_const_key_weak(is_root_commissioning_open, -1), be_const_closure(class_Matter_Commissioning_is_root_commissioning_open_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(class_Matter_Commissioning_every_second_closure) },
        { be_const_key_weak(commissioning_salt, -1), be_const_var(4) },
        { be_const_key_weak(is_commissioning_open, -1), be_const_closure(class_Matter_Commissioning_is_commissioning_open_closure) },
        { be_const_key_weak(init, 14), be_const_closure(class_Matter_Commissioning_init_closure) },
        { be_const_key_weak(hostname_wifi, -1), be_const_var(10) },
        { be_const_key_weak(commissioning_iterations, -1), be_const_var(2) },
        { be_const_key_weak(mdns_remove_PASE, -1), be_const_closure(class_Matter_Commissioning_mdns_remove_PASE_closure) },
        { be_const_key_weak(PBKDF_ITERATIONS, -1), be_const_int(1000) },
    })),
    be_str_weak(Matter_Commissioning)
);
/********************************************************************/
/* End of solidification */
