/* Solidification of Matter_Plugin_1_Device.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
// compact class 'GetOptionReader' ktab size: 17, total: 24 (saved 56 bytes)
static const bvalue be_ktab_class_GetOptionReader[17] = {
  /* K0   */  be_nested_str_weak(flag),
  /* K1   */  be_nested_str_weak(getbits),
  /* K2   */  be_const_int(1),
  /* K3   */  be_nested_str_weak(flag2),
  /* K4   */  be_nested_str_weak(get),
  /* K5   */  be_nested_str_weak(flag3),
  /* K6   */  be_nested_str_weak(flag4),
  /* K7   */  be_nested_str_weak(flag5),
  /* K8   */  be_nested_str_weak(flag6),
  /* K9   */  be_nested_str_weak(value_error),
  /* K10  */  be_nested_str_weak(invalid_X20json),
  /* K11  */  be_nested_str_weak(SetOption),
  /* K12  */  be_nested_str_weak(fromhex),
  /* K13  */  be_const_int(0),
  /* K14  */  be_nested_str_weak(reverse),
  /* K15  */  be_const_int(2),
  /* K16  */  be_const_int(3),
};


extern const bclass be_class_GetOptionReader;

/********************************************************************
** Solidified function: getoption
********************************************************************/
be_local_closure(class_GetOptionReader_getoption,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_GetOptionReader,     /* shared constants */
    be_str_weak(getoption),
    &be_const_str_solidified,
    ( &(const binstruction[65]) {  /* code */
      0x540A001F,  //  0000  LDINT	R2	32
      0x14080202,  //  0001  LT	R2	R1	R2
      0x780A0006,  //  0002  JMPF	R2	#000A
      0x88080100,  //  0003  GETMBR	R2	R0	K0
      0x8C080501,  //  0004  GETMET	R2	R2	K1
      0x5C100200,  //  0005  MOVE	R4	R1
      0x58140002,  //  0006  LDCONST	R5	K2
      0x7C080600,  //  0007  CALL	R2	3
      0x80040400,  //  0008  RET	1	R2
      0x70020035,  //  0009  JMP		#0040
      0x540A0031,  //  000A  LDINT	R2	50
      0x14080202,  //  000B  LT	R2	R1	R2
      0x780A0007,  //  000C  JMPF	R2	#0015
      0x88080103,  //  000D  GETMBR	R2	R0	K3
      0x8C080504,  //  000E  GETMET	R2	R2	K4
      0x5412001F,  //  000F  LDINT	R4	32
      0x04100204,  //  0010  SUB	R4	R1	R4
      0x58140002,  //  0011  LDCONST	R5	K2
      0x7C080600,  //  0012  CALL	R2	3
      0x80040400,  //  0013  RET	1	R2
      0x7002002A,  //  0014  JMP		#0040
      0x540A0051,  //  0015  LDINT	R2	82
      0x14080202,  //  0016  LT	R2	R1	R2
      0x780A0007,  //  0017  JMPF	R2	#0020
      0x88080105,  //  0018  GETMBR	R2	R0	K5
      0x8C080501,  //  0019  GETMET	R2	R2	K1
      0x54120031,  //  001A  LDINT	R4	50
      0x04100204,  //  001B  SUB	R4	R1	R4
      0x58140002,  //  001C  LDCONST	R5	K2
      0x7C080600,  //  001D  CALL	R2	3
      0x80040400,  //  001E  RET	1	R2
      0x7002001F,  //  001F  JMP		#0040
      0x540A0071,  //  0020  LDINT	R2	114
      0x14080202,  //  0021  LT	R2	R1	R2
      0x780A0007,  //  0022  JMPF	R2	#002B
      0x88080106,  //  0023  GETMBR	R2	R0	K6
      0x8C080501,  //  0024  GETMET	R2	R2	K1
      0x54120051,  //  0025  LDINT	R4	82
      0x04100204,  //  0026  SUB	R4	R1	R4
      0x58140002,  //  0027  LDCONST	R5	K2
      0x7C080600,  //  0028  CALL	R2	3
      0x80040400,  //  0029  RET	1	R2
      0x70020014,  //  002A  JMP		#0040
      0x540A0091,  //  002B  LDINT	R2	146
      0x14080202,  //  002C  LT	R2	R1	R2
      0x780A0007,  //  002D  JMPF	R2	#0036
      0x88080107,  //  002E  GETMBR	R2	R0	K7
      0x8C080501,  //  002F  GETMET	R2	R2	K1
      0x54120071,  //  0030  LDINT	R4	114
      0x04100204,  //  0031  SUB	R4	R1	R4
      0x58140002,  //  0032  LDCONST	R5	K2
      0x7C080600,  //  0033  CALL	R2	3
      0x80040400,  //  0034  RET	1	R2
      0x70020009,  //  0035  JMP		#0040
      0x540A00B1,  //  0036  LDINT	R2	178
      0x14080202,  //  0037  LT	R2	R1	R2
      0x780A0006,  //  0038  JMPF	R2	#0040
      0x88080108,  //  0039  GETMBR	R2	R0	K8
      0x8C080501,  //  003A  GETMET	R2	R2	K1
      0x54120091,  //  003B  LDINT	R4	146
      0x04100204,  //  003C  SUB	R4	R1	R4
      0x58140002,  //  003D  LDCONST	R5	K2
      0x7C080600,  //  003E  CALL	R2	3
      0x80040400,  //  003F  RET	1	R2
      0x80000000,  //  0040  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_GetOptionReader_init,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_GetOptionReader,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[54]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x780A0000,  //  0002  JMPF	R2	#0004
      0xB006130A,  //  0003  RAISE	1	K9	K10
      0x9408030B,  //  0004  GETIDX	R2	R1	K11
      0x600C0015,  //  0005  GETGBL	R3	G21
      0x7C0C0000,  //  0006  CALL	R3	0
      0x8C0C070C,  //  0007  GETMET	R3	R3	K12
      0x9414050D,  //  0008  GETIDX	R5	R2	K13
      0x7C0C0400,  //  0009  CALL	R3	2
      0x8C0C070E,  //  000A  GETMET	R3	R3	K14
      0x7C0C0200,  //  000B  CALL	R3	1
      0x90020003,  //  000C  SETMBR	R0	K0	R3
      0x600C0015,  //  000D  GETGBL	R3	G21
      0x7C0C0000,  //  000E  CALL	R3	0
      0x8C0C070C,  //  000F  GETMET	R3	R3	K12
      0x94140502,  //  0010  GETIDX	R5	R2	K2
      0x7C0C0400,  //  0011  CALL	R3	2
      0x90020603,  //  0012  SETMBR	R0	K3	R3
      0x600C0015,  //  0013  GETGBL	R3	G21
      0x7C0C0000,  //  0014  CALL	R3	0
      0x8C0C070C,  //  0015  GETMET	R3	R3	K12
      0x9414050F,  //  0016  GETIDX	R5	R2	K15
      0x7C0C0400,  //  0017  CALL	R3	2
      0x8C0C070E,  //  0018  GETMET	R3	R3	K14
      0x7C0C0200,  //  0019  CALL	R3	1
      0x90020A03,  //  001A  SETMBR	R0	K5	R3
      0x600C0015,  //  001B  GETGBL	R3	G21
      0x7C0C0000,  //  001C  CALL	R3	0
      0x8C0C070C,  //  001D  GETMET	R3	R3	K12
      0x94140510,  //  001E  GETIDX	R5	R2	K16
      0x7C0C0400,  //  001F  CALL	R3	2
      0x8C0C070E,  //  0020  GETMET	R3	R3	K14
      0x7C0C0200,  //  0021  CALL	R3	1
      0x90020C03,  //  0022  SETMBR	R0	K6	R3
      0x600C0015,  //  0023  GETGBL	R3	G21
      0x7C0C0000,  //  0024  CALL	R3	0
      0x8C0C070C,  //  0025  GETMET	R3	R3	K12
      0x54160003,  //  0026  LDINT	R5	4
      0x94140405,  //  0027  GETIDX	R5	R2	R5
      0x7C0C0400,  //  0028  CALL	R3	2
      0x8C0C070E,  //  0029  GETMET	R3	R3	K14
      0x7C0C0200,  //  002A  CALL	R3	1
      0x90020E03,  //  002B  SETMBR	R0	K7	R3
      0x600C0015,  //  002C  GETGBL	R3	G21
      0x7C0C0000,  //  002D  CALL	R3	0
      0x8C0C070C,  //  002E  GETMET	R3	R3	K12
      0x54160004,  //  002F  LDINT	R5	5
      0x94140405,  //  0030  GETIDX	R5	R2	R5
      0x7C0C0400,  //  0031  CALL	R3	2
      0x8C0C070E,  //  0032  GETMET	R3	R3	K14
      0x7C0C0200,  //  0033  CALL	R3	1
      0x90021003,  //  0034  SETMBR	R0	K8	R3
      0x80000000,  //  0035  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: GetOptionReader
********************************************************************/
be_local_class(GetOptionReader,
    6,
    NULL,
    be_nested_map(8,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(flag2, -1), be_const_var(1) },
        { be_const_key_weak(flag4, -1), be_const_var(3) },
        { be_const_key_weak(getoption, -1), be_const_closure(class_GetOptionReader_getoption_closure) },
        { be_const_key_weak(init, 5), be_const_closure(class_GetOptionReader_init_closure) },
        { be_const_key_weak(flag3, -1), be_const_var(2) },
        { be_const_key_weak(flag6, -1), be_const_var(5) },
        { be_const_key_weak(flag5, -1), be_const_var(4) },
        { be_const_key_weak(flag, 0), be_const_var(0) },
    })),
    be_str_weak(GetOptionReader)
);
// compact class 'Matter_Plugin_Device' ktab size: 122, total: 158 (saved 288 bytes)
static const bvalue be_ktab_class_Matter_Plugin_Device[122] = {
  /* K0   */  be_nested_str_weak(http_remote),
  /* K1   */  be_nested_str_weak(add_schedule),
  /* K2   */  be_nested_str_weak(UPDATE_CMD),
  /* K3   */  be_nested_str_weak(UPDATE_TIME),
  /* K4   */  be_nested_str_weak(webserver),
  /* K5   */  be_nested_str_weak(web_values_prefix),
  /* K6   */  be_nested_str_weak(content_send),
  /* K7   */  be_nested_str_weak(_X26lt_X3B_X2D_X2D_X20_X28),
  /* K8   */  be_nested_str_weak(DISPLAY_NAME),
  /* K9   */  be_nested_str_weak(_X29_X20_X2D_X2D_X26gt_X3B),
  /* K10  */  be_nested_str_weak(BRIDGE),
  /* K11  */  be_nested_str_weak(scheduler),
  /* K12  */  be_nested_str_weak(every_250ms),
  /* K13  */  be_nested_str_weak(_X3Cb_X3EOn_X3C_X2Fb_X3E),
  /* K14  */  be_nested_str_weak(Off),
  /* K15  */  be_nested_str_weak(),
  /* K16  */  be_nested_str_weak(matter),
  /* K17  */  be_nested_str_weak(TLV),
  /* K18  */  be_nested_str_weak(cluster),
  /* K19  */  be_nested_str_weak(command),
  /* K20  */  be_const_int(3),
  /* K21  */  be_const_int(0),
  /* K22  */  be_const_int(1),
  /* K23  */  be_nested_str_weak(Matter_TLV_struct),
  /* K24  */  be_nested_str_weak(add_TLV),
  /* K25  */  be_nested_str_weak(U2),
  /* K26  */  be_nested_str_weak(invoke_request),
  /* K27  */  be_nested_str_weak(tick),
  /* K28  */  be_nested_str_weak(device),
  /* K29  */  be_nested_str_weak(call_remote_sync),
  /* K30  */  be_nested_str_weak(parse_http_response),
  /* K31  */  be_nested_str_weak(json),
  /* K32  */  be_const_int(2),
  /* K33  */  be_nested_str_weak(_X20),
  /* K34  */  be_nested_str_weak(call_sync),
  /* K35  */  be_nested_str_weak(SYNC_TIMEOUT),
  /* K36  */  be_nested_str_weak(device_is_alive),
  /* K37  */  be_nested_str_weak(load),
  /* K38  */  be_nested_str_weak(log),
  /* K39  */  be_nested_str_weak(MTR_X3A_X20HTTP_X20GET_X20retrying),
  /* K40  */  be_nested_str_weak(parse_status_response_and_call_method),
  /* K41  */  be_nested_str_weak(parse_status),
  /* K42  */  be_nested_str_weak(get_name),
  /* K43  */  be_nested_str_weak(PREFIX),
  /* K44  */  be_nested_str_weak(html_escape),
  /* K45  */  be_nested_str_weak(attribute),
  /* K46  */  be_nested_str_weak(set),
  /* K47  */  be_nested_str_weak(U1),
  /* K48  */  be_nested_str_weak(Matter_TLV_array),
  /* K49  */  be_nested_str_weak(TYPES),
  /* K50  */  be_nested_str_weak(keys),
  /* K51  */  be_nested_str_weak(add_struct),
  /* K52  */  be_nested_str_weak(stop_iteration),
  /* K53  */  be_nested_str_weak(NON_BRIDGE_VENDOR),
  /* K54  */  be_nested_str_weak(find),
  /* K55  */  be_nested_str_weak(get_admin_vendor),
  /* K56  */  be_nested_str_weak(disable_bridge_mode),
  /* K57  */  be_nested_str_weak(string),
  /* K58  */  be_nested_str_weak(get_info),
  /* K59  */  be_nested_str_weak(name),
  /* K60  */  be_nested_str_weak(set_or_nil),
  /* K61  */  be_nested_str_weak(UTF1),
  /* K62  */  be_nested_str_weak(tasmota),
  /* K63  */  be_nested_str_weak(cmd),
  /* K64  */  be_nested_str_weak(DeviceName),
  /* K65  */  be_nested_str_weak(version),
  /* K66  */  be_nested_str_weak(_X28),
  /* K67  */  be_nested_str_weak(NULL),
  /* K68  */  be_nested_str_weak(Status_X202),
  /* K69  */  be_nested_str_weak(StatusFWR),
  /* K70  */  be_nested_str_weak(Version),
  /* K71  */  be_nested_str_weak(mac),
  /* K72  */  be_nested_str_weak(wifi),
  /* K73  */  be_nested_str_weak(BOOL),
  /* K74  */  be_nested_str_weak(reachable),
  /* K75  */  be_nested_str_weak(read_attribute),
  /* K76  */  be_nested_str_weak(init),
  /* K77  */  be_nested_str_weak(ARG_HTTP),
  /* K78  */  be_nested_str_weak(register_http_remote),
  /* K79  */  be_nested_str_weak(PROBE_TIMEOUT),
  /* K80  */  be_nested_str_weak(register_cmd_cb),
  /* K81  */  be_nested_str_weak(introspect),
  /* K82  */  be_nested_str_weak(get),
  /* K83  */  be_nested_str_weak(JSON_NAME),
  /* K84  */  be_nested_str_weak(contains),
  /* K85  */  be_nested_str_weak(shadow_value),
  /* K86  */  be_nested_str_weak(dump),
  /* K87  */  be_nested_str_weak(null),
  /* K88  */  be_nested_str_weak(_X2C_X22_X25s_X22_X3A_X25s),
  /* K89  */  be_nested_str_weak(shadow_onoff),
  /* K90  */  be_nested_str_weak(Power),
  /* K91  */  be_nested_str_weak(shadow_bri),
  /* K92  */  be_nested_str_weak(Bri),
  /* K93  */  be_nested_str_weak(shadow_ct),
  /* K94  */  be_nested_str_weak(CT),
  /* K95  */  be_nested_str_weak(shadow_hue),
  /* K96  */  be_nested_str_weak(Hue),
  /* K97  */  be_nested_str_weak(shadow_sat),
  /* K98  */  be_nested_str_weak(Sat),
  /* K99  */  be_nested_str_weak(shadow_shutter_pos),
  /* K100 */  be_nested_str_weak(ShutterPos),
  /* K101 */  be_nested_str_weak(shadow_shutter_target),
  /* K102 */  be_nested_str_weak(ShutterTarget),
  /* K103 */  be_nested_str_weak(shadow_shutter_tilt),
  /* K104 */  be_nested_str_weak(ShutterTilt),
  /* K105 */  be_nested_str_weak(shadow_contact),
  /* K106 */  be_nested_str_weak(Contact),
  /* K107 */  be_nested_str_weak(shadow_occupancy),
  /* K108 */  be_nested_str_weak(Occupancy),
  /* K109 */  be_nested_str_weak(shadow_air_quality),
  /* K110 */  be_nested_str_weak(AirQuality),
  /* K111 */  be_nested_str_weak(shadow_co2),
  /* K112 */  be_nested_str_weak(CO2),
  /* K113 */  be_nested_str_weak(shadow_pm1),
  /* K114 */  be_nested_str_weak(PM1),
  /* K115 */  be_nested_str_weak(shadow_pm2_5),
  /* K116 */  be_nested_str_weak(PM2_X2E5),
  /* K117 */  be_nested_str_weak(shadow_pm10),
  /* K118 */  be_nested_str_weak(PM10),
  /* K119 */  be_nested_str_weak(shadow_tvoc),
  /* K120 */  be_nested_str_weak(TVOC),
  /* K121 */  be_nested_str_weak(attribute_updated),
};


extern const bclass be_class_Matter_Plugin_Device;

/********************************************************************
** Solidified function: parse_status
********************************************************************/
be_local_closure(class_Matter_Plugin_Device_parse_status,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Device,     /* shared constants */
    be_str_weak(parse_status),
    &be_const_str_solidified,
    ( &(const binstruction[ 1]) {  /* code */
      0x80000000,  //  0000  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_cmd_cb
********************************************************************/
be_local_closure(class_Matter_Plugin_Device_register_cmd_cb,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
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
        /* K0   */  be_nested_str_weak(parse_http_response),
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
    &be_ktab_class_Matter_Plugin_Device,     /* shared constants */
    be_str_weak(register_cmd_cb),
    &be_const_str_solidified,
    ( &(const binstruction[ 8]) {  /* code */
      0x88040100,  //  0000  GETMBR	R1	R0	K0
      0x8C040301,  //  0001  GETMET	R1	R1	K1
      0x880C0102,  //  0002  GETMBR	R3	R0	K2
      0x88100103,  //  0003  GETMBR	R4	R0	K3
      0x84140000,  //  0004  CLOSURE	R5	P0
      0x7C040800,  //  0005  CALL	R1	4
      0xA0000000,  //  0006  CLOSE	R0
      0x80000000,  //  0007  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values
********************************************************************/
be_local_closure(class_Matter_Plugin_Device_web_values,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Device,     /* shared constants */
    be_str_weak(web_values),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0xA4060800,  //  0000  IMPORT	R1	K4
      0x8C080105,  //  0001  GETMET	R2	R0	K5
      0x7C080200,  //  0002  CALL	R2	1
      0x8C080306,  //  0003  GETMET	R2	R1	K6
      0x88100108,  //  0004  GETMBR	R4	R0	K8
      0x00120E04,  //  0005  ADD	R4	K7	R4
      0x00100909,  //  0006  ADD	R4	R4	K9
      0x7C080400,  //  0007  CALL	R2	2
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_250ms
********************************************************************/
be_local_closure(class_Matter_Plugin_Device_every_250ms,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Device,     /* shared constants */
    be_str_weak(every_250ms),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x8804010A,  //  0000  GETMBR	R1	R0	K10
      0x78060003,  //  0001  JMPF	R1	#0006
      0x88040100,  //  0002  GETMBR	R1	R0	K0
      0x8C04030B,  //  0003  GETMET	R1	R1	K11
      0x7C040200,  //  0004  CALL	R1	1
      0x70020004,  //  0005  JMP		#000B
      0x60040003,  //  0006  GETGBL	R1	G3
      0x5C080000,  //  0007  MOVE	R2	R0
      0x7C040200,  //  0008  CALL	R1	1
      0x8C04030C,  //  0009  GETMET	R1	R1	K12
      0x7C040200,  //  000A  CALL	R1	1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_value_onoff
********************************************************************/
be_local_closure(class_Matter_Plugin_Device_web_value_onoff,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Device,     /* shared constants */
    be_str_weak(web_value_onoff),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0004,  //  0002  JMPF	R2	#0008
      0x78060001,  //  0003  JMPF	R1	#0006
      0x5808000D,  //  0004  LDCONST	R2	K13
      0x70020000,  //  0005  JMP		#0007
      0x5808000E,  //  0006  LDCONST	R2	K14
      0x70020000,  //  0007  JMP		#0009
      0x5808000F,  //  0008  LDCONST	R2	K15
      0x80040400,  //  0009  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(class_Matter_Plugin_Device_invoke_request,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Device,     /* shared constants */
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[51]) {  /* code */
      0xB8122000,  //  0000  GETNGBL	R4	K16
      0x88100911,  //  0001  GETMBR	R4	R4	K17
      0x88140712,  //  0002  GETMBR	R5	R3	K18
      0x88180713,  //  0003  GETMBR	R6	R3	K19
      0x1C1C0B14,  //  0004  EQ	R7	R5	K20
      0x781E0016,  //  0005  JMPF	R7	#001D
      0x1C1C0D15,  //  0006  EQ	R7	R6	K21
      0x781E0002,  //  0007  JMPF	R7	#000B
      0x501C0200,  //  0008  LDBOOL	R7	1	0
      0x80040E00,  //  0009  RET	1	R7
      0x70020010,  //  000A  JMP		#001C
      0x1C1C0D16,  //  000B  EQ	R7	R6	K22
      0x781E0009,  //  000C  JMPF	R7	#0017
      0x8C1C0917,  //  000D  GETMET	R7	R4	K23
      0x7C1C0200,  //  000E  CALL	R7	1
      0x8C200F18,  //  000F  GETMET	R8	R7	K24
      0x58280015,  //  0010  LDCONST	R10	K21
      0x882C0919,  //  0011  GETMBR	R11	R4	K25
      0x58300015,  //  0012  LDCONST	R12	K21
      0x7C200800,  //  0013  CALL	R8	4
      0x900E2715,  //  0014  SETMBR	R3	K19	K21
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
      0x8C1C0F1A,  //  002C  GETMET	R7	R7	K26
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
** Solidified function: update_shadow
********************************************************************/
be_local_closure(class_Matter_Plugin_Device_update_shadow,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Device,     /* shared constants */
    be_str_weak(update_shadow),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x8804010A,  //  0000  GETMBR	R1	R0	K10
      0x7806000D,  //  0001  JMPF	R1	#0010
      0x8804011B,  //  0002  GETMBR	R1	R0	K27
      0x8808011C,  //  0003  GETMBR	R2	R0	K28
      0x8808051B,  //  0004  GETMBR	R2	R2	K27
      0x20040202,  //  0005  NE	R1	R1	R2
      0x78060008,  //  0006  JMPF	R1	#0010
      0x8C04011D,  //  0007  GETMET	R1	R0	K29
      0x880C0102,  //  0008  GETMBR	R3	R0	K2
      0x7C040400,  //  0009  CALL	R1	2
      0x78060004,  //  000A  JMPF	R1	#0010
      0x8C08011E,  //  000B  GETMET	R2	R0	K30
      0x58100016,  //  000C  LDCONST	R4	K22
      0x5C140200,  //  000D  MOVE	R5	R1
      0x88180102,  //  000E  GETMBR	R6	R0	K2
      0x7C080800,  //  000F  CALL	R2	4
      0x8804011C,  //  0010  GETMBR	R1	R0	K28
      0x8804031B,  //  0011  GETMBR	R1	R1	K27
      0x90023601,  //  0012  SETMBR	R0	K27	R1
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: call_remote_sync
********************************************************************/
be_local_closure(class_Matter_Plugin_Device_call_remote_sync,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Device,     /* shared constants */
    be_str_weak(call_remote_sync),
    &be_const_str_solidified,
    ( &(const binstruction[44]) {  /* code */
      0x880C010A,  //  0000  GETMBR	R3	R0	K10
      0x780E0028,  //  0001  JMPF	R3	#002B
      0xA40E3E00,  //  0002  IMPORT	R3	K31
      0x58100020,  //  0003  LDCONST	R4	K32
      0x4C140000,  //  0004  LDNIL	R5
      0x20140405,  //  0005  NE	R5	R2	R5
      0x78160005,  //  0006  JMPF	R5	#000D
      0x00140321,  //  0007  ADD	R5	R1	K33
      0x60180008,  //  0008  GETGBL	R6	G8
      0x5C1C0400,  //  0009  MOVE	R7	R2
      0x7C180200,  //  000A  CALL	R6	1
      0x00140A06,  //  000B  ADD	R5	R5	R6
      0x5C040A00,  //  000C  MOVE	R1	R5
      0x24140915,  //  000D  GT	R5	R4	K21
      0x78160015,  //  000E  JMPF	R5	#0025
      0x88140100,  //  000F  GETMBR	R5	R0	K0
      0x8C140B22,  //  0010  GETMET	R5	R5	K34
      0x5C1C0200,  //  0011  MOVE	R7	R1
      0x88200123,  //  0012  GETMBR	R8	R0	K35
      0x7C140600,  //  0013  CALL	R5	3
      0x4C180000,  //  0014  LDNIL	R6
      0x20180A06,  //  0015  NE	R6	R5	R6
      0x781A0007,  //  0016  JMPF	R6	#001F
      0x88180100,  //  0017  GETMBR	R6	R0	K0
      0x8C180D24,  //  0018  GETMET	R6	R6	K36
      0x50200200,  //  0019  LDBOOL	R8	1	0
      0x7C180400,  //  001A  CALL	R6	2
      0x8C180725,  //  001B  GETMET	R6	R3	K37
      0x5C200A00,  //  001C  MOVE	R8	R5
      0x7C180400,  //  001D  CALL	R6	2
      0x80040C00,  //  001E  RET	1	R6
      0x04100916,  //  001F  SUB	R4	R4	K22
      0xB81A4C00,  //  0020  GETNGBL	R6	K38
      0x581C0027,  //  0021  LDCONST	R7	K39
      0x58200014,  //  0022  LDCONST	R8	K20
      0x7C180400,  //  0023  CALL	R6	2
      0x7001FFE7,  //  0024  JMP		#000D
      0x88140100,  //  0025  GETMBR	R5	R0	K0
      0x8C140B24,  //  0026  GETMET	R5	R5	K36
      0x501C0000,  //  0027  LDBOOL	R7	0	0
      0x7C140400,  //  0028  CALL	R5	2
      0x4C140000,  //  0029  LDNIL	R5
      0x80040A00,  //  002A  RET	1	R5
      0x80000000,  //  002B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: parse_http_response
********************************************************************/
be_local_closure(class_Matter_Plugin_Device_parse_http_response,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Device,     /* shared constants */
    be_str_weak(parse_http_response),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8810010A,  //  0000  GETMBR	R4	R0	K10
      0x7812000A,  //  0001  JMPF	R4	#000D
      0x8810011C,  //  0002  GETMBR	R4	R0	K28
      0x8810091B,  //  0003  GETMBR	R4	R4	K27
      0x90023604,  //  0004  SETMBR	R0	K27	R4
      0x88100100,  //  0005  GETMBR	R4	R0	K0
      0x8C100928,  //  0006  GETMET	R4	R4	K40
      0x5C180200,  //  0007  MOVE	R6	R1
      0x5C1C0400,  //  0008  MOVE	R7	R2
      0x5C200600,  //  0009  MOVE	R8	R3
      0x5C240000,  //  000A  MOVE	R9	R0
      0x88280129,  //  000B  GETMBR	R10	R0	K41
      0x7C100C00,  //  000C  CALL	R4	6
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: web_values_prefix
********************************************************************/
be_local_closure(class_Matter_Plugin_Device_web_values_prefix,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Device,     /* shared constants */
    be_str_weak(web_values_prefix),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0xA4060800,  //  0000  IMPORT	R1	K4
      0x8C08012A,  //  0001  GETMET	R2	R0	K42
      0x7C080200,  //  0002  CALL	R2	1
      0x8C0C0306,  //  0003  GETMET	R3	R1	K6
      0x60140018,  //  0004  GETGBL	R5	G24
      0x8818012B,  //  0005  GETMBR	R6	R0	K43
      0x780A0003,  //  0006  JMPF	R2	#000B
      0x8C1C032C,  //  0007  GETMET	R7	R1	K44
      0x5C240400,  //  0008  MOVE	R9	R2
      0x7C1C0400,  //  0009  CALL	R7	2
      0x70020000,  //  000A  JMP		#000C
      0x581C000F,  //  000B  LDCONST	R7	K15
      0x7C140400,  //  000C  CALL	R5	2
      0x7C0C0400,  //  000D  CALL	R3	2
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_attribute
********************************************************************/
be_local_closure(class_Matter_Plugin_Device_read_attribute,   /* name */
  be_nested_proto(
    17,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Device,     /* shared constants */
    be_str_weak(read_attribute),
    &be_const_str_solidified,
    ( &(const binstruction[244]) {  /* code */
      0xB8122000,  //  0000  GETNGBL	R4	K16
      0x88100911,  //  0001  GETMBR	R4	R4	K17
      0x88140512,  //  0002  GETMBR	R5	R2	K18
      0x8818052D,  //  0003  GETMBR	R6	R2	K45
      0x1C1C0B14,  //  0004  EQ	R7	R5	K20
      0x781E000F,  //  0005  JMPF	R7	#0016
      0x1C1C0D15,  //  0006  EQ	R7	R6	K21
      0x781E0005,  //  0007  JMPF	R7	#000E
      0x8C1C072E,  //  0008  GETMET	R7	R3	K46
      0x88240919,  //  0009  GETMBR	R9	R4	K25
      0x58280015,  //  000A  LDCONST	R10	K21
      0x7C1C0600,  //  000B  CALL	R7	3
      0x80040E00,  //  000C  RET	1	R7
      0x70020006,  //  000D  JMP		#0015
      0x1C1C0D16,  //  000E  EQ	R7	R6	K22
      0x781E0004,  //  000F  JMPF	R7	#0015
      0x8C1C072E,  //  0010  GETMET	R7	R3	K46
      0x8824092F,  //  0011  GETMBR	R9	R4	K47
      0x58280015,  //  0012  LDCONST	R10	K21
      0x7C1C0600,  //  0013  CALL	R7	3
      0x80040E00,  //  0014  RET	1	R7
      0x700200D4,  //  0015  JMP		#00EB
      0x541E0003,  //  0016  LDINT	R7	4
      0x1C1C0A07,  //  0017  EQ	R7	R5	R7
      0x781E0004,  //  0018  JMPF	R7	#001E
      0x1C1C0D15,  //  0019  EQ	R7	R6	K21
      0x781E0001,  //  001A  JMPF	R7	#001D
      0x4C1C0000,  //  001B  LDNIL	R7
      0x80040E00,  //  001C  RET	1	R7
      0x700200CC,  //  001D  JMP		#00EB
      0x541E0004,  //  001E  LDINT	R7	5
      0x1C1C0A07,  //  001F  EQ	R7	R5	R7
      0x781E0000,  //  0020  JMPF	R7	#0022
      0x700200C8,  //  0021  JMP		#00EB
      0x541E001C,  //  0022  LDINT	R7	29
      0x1C1C0A07,  //  0023  EQ	R7	R5	R7
      0x781E0034,  //  0024  JMPF	R7	#005A
      0x1C1C0D15,  //  0025  EQ	R7	R6	K21
      0x781E0031,  //  0026  JMPF	R7	#0059
      0x8C1C0930,  //  0027  GETMET	R7	R4	K48
      0x7C1C0200,  //  0028  CALL	R7	1
      0x88200131,  //  0029  GETMBR	R8	R0	K49
      0x60240010,  //  002A  GETGBL	R9	G16
      0x8C281132,  //  002B  GETMET	R10	R8	K50
      0x7C280200,  //  002C  CALL	R10	1
      0x7C240200,  //  002D  CALL	R9	1
      0xA802000E,  //  002E  EXBLK	0	#003E
      0x5C281200,  //  002F  MOVE	R10	R9
      0x7C280000,  //  0030  CALL	R10	0
      0x8C2C0F33,  //  0031  GETMET	R11	R7	K51
      0x7C2C0200,  //  0032  CALL	R11	1
      0x8C301718,  //  0033  GETMET	R12	R11	K24
      0x58380015,  //  0034  LDCONST	R14	K21
      0x883C0919,  //  0035  GETMBR	R15	R4	K25
      0x5C401400,  //  0036  MOVE	R16	R10
      0x7C300800,  //  0037  CALL	R12	4
      0x8C301718,  //  0038  GETMET	R12	R11	K24
      0x58380016,  //  0039  LDCONST	R14	K22
      0x883C0919,  //  003A  GETMBR	R15	R4	K25
      0x9440100A,  //  003B  GETIDX	R16	R8	R10
      0x7C300800,  //  003C  CALL	R12	4
      0x7001FFF0,  //  003D  JMP		#002F
      0x58240034,  //  003E  LDCONST	R9	K52
      0xAC240200,  //  003F  CATCH	R9	1	0
      0xB0080000,  //  0040  RAISE	2	R0	R0
      0x88240135,  //  0041  GETMBR	R9	R0	K53
      0x8C241336,  //  0042  GETMET	R9	R9	K54
      0x8C2C0337,  //  0043  GETMET	R11	R1	K55
      0x7C2C0200,  //  0044  CALL	R11	1
      0x7C240400,  //  0045  CALL	R9	2
      0x4C280000,  //  0046  LDNIL	R10
      0x1C24120A,  //  0047  EQ	R9	R9	R10
      0x7826000E,  //  0048  JMPF	R9	#0058
      0x8824011C,  //  0049  GETMBR	R9	R0	K28
      0x88241338,  //  004A  GETMBR	R9	R9	K56
      0x7426000B,  //  004B  JMPT	R9	#0058
      0x8C240F33,  //  004C  GETMET	R9	R7	K51
      0x7C240200,  //  004D  CALL	R9	1
      0x8C281318,  //  004E  GETMET	R10	R9	K24
      0x58300015,  //  004F  LDCONST	R12	K21
      0x88340919,  //  0050  GETMBR	R13	R4	K25
      0x543A0012,  //  0051  LDINT	R14	19
      0x7C280800,  //  0052  CALL	R10	4
      0x8C281318,  //  0053  GETMET	R10	R9	K24
      0x58300016,  //  0054  LDCONST	R12	K22
      0x88340919,  //  0055  GETMBR	R13	R4	K25
      0x58380016,  //  0056  LDCONST	R14	K22
      0x7C280800,  //  0057  CALL	R10	4
      0x80040E00,  //  0058  RET	1	R7
      0x70020090,  //  0059  JMP		#00EB
      0x541E0038,  //  005A  LDINT	R7	57
      0x1C1C0A07,  //  005B  EQ	R7	R5	R7
      0x781E008D,  //  005C  JMPF	R7	#00EB
      0xA41E7200,  //  005D  IMPORT	R7	K57
      0x1C200D14,  //  005E  EQ	R8	R6	K20
      0x78220018,  //  005F  JMPF	R8	#0079
      0x8820010A,  //  0060  GETMBR	R8	R0	K10
      0x7822000B,  //  0061  JMPF	R8	#006E
      0x88200100,  //  0062  GETMBR	R8	R0	K0
      0x8C20113A,  //  0063  GETMET	R8	R8	K58
      0x7C200200,  //  0064  CALL	R8	1
      0x8C201136,  //  0065  GETMET	R8	R8	K54
      0x5828003B,  //  0066  LDCONST	R10	K59
      0x7C200400,  //  0067  CALL	R8	2
      0x8C24073C,  //  0068  GETMET	R9	R3	K60
      0x882C093D,  //  0069  GETMBR	R11	R4	K61
      0x5C301000,  //  006A  MOVE	R12	R8
      0x7C240600,  //  006B  CALL	R9	3
      0x80041200,  //  006C  RET	1	R9
      0x70020009,  //  006D  JMP		#0078
      0x8C20072E,  //  006E  GETMET	R8	R3	K46
      0x8828093D,  //  006F  GETMBR	R10	R4	K61
      0xB82E7C00,  //  0070  GETNGBL	R11	K62
      0x8C2C173F,  //  0071  GETMET	R11	R11	K63
      0x58340040,  //  0072  LDCONST	R13	K64
      0x50380200,  //  0073  LDBOOL	R14	1	0
      0x7C2C0600,  //  0074  CALL	R11	3
      0x942C1740,  //  0075  GETIDX	R11	R11	K64
      0x7C200600,  //  0076  CALL	R8	3
      0x80041000,  //  0077  RET	1	R8
      0x70020071,  //  0078  JMP		#00EB
      0x54220004,  //  0079  LDINT	R8	5
      0x1C200C08,  //  007A  EQ	R8	R6	R8
      0x78220006,  //  007B  JMPF	R8	#0083
      0x8C20072E,  //  007C  GETMET	R8	R3	K46
      0x8828093D,  //  007D  GETMBR	R10	R4	K61
      0x8C2C012A,  //  007E  GETMET	R11	R0	K42
      0x7C2C0200,  //  007F  CALL	R11	1
      0x7C200600,  //  0080  CALL	R8	3
      0x80041000,  //  0081  RET	1	R8
      0x70020067,  //  0082  JMP		#00EB
      0x54220009,  //  0083  LDINT	R8	10
      0x1C200C08,  //  0084  EQ	R8	R6	R8
      0x78220033,  //  0085  JMPF	R8	#00BA
      0x8820010A,  //  0086  GETMBR	R8	R0	K10
      0x7822001B,  //  0087  JMPF	R8	#00A4
      0x88200100,  //  0088  GETMBR	R8	R0	K0
      0x8C20113A,  //  0089  GETMET	R8	R8	K58
      0x7C200200,  //  008A  CALL	R8	1
      0x8C201136,  //  008B  GETMET	R8	R8	K54
      0x58280041,  //  008C  LDCONST	R10	K65
      0x7C200400,  //  008D  CALL	R8	2
      0x7822000E,  //  008E  JMPF	R8	#009E
      0x8C240F36,  //  008F  GETMET	R9	R7	K54
      0x5C2C1000,  //  0090  MOVE	R11	R8
      0x58300042,  //  0091  LDCONST	R12	K66
      0x7C240600,  //  0092  CALL	R9	3
      0x24281315,  //  0093  GT	R10	R9	K21
      0x782A0002,  //  0094  JMPF	R10	#0098
      0x04281316,  //  0095  SUB	R10	R9	K22
      0x402A2A0A,  //  0096  CONNECT	R10	K21	R10
      0x9420100A,  //  0097  GETIDX	R8	R8	R10
      0x8C28072E,  //  0098  GETMET	R10	R3	K46
      0x8830093D,  //  0099  GETMBR	R12	R4	K61
      0x5C341000,  //  009A  MOVE	R13	R8
      0x7C280600,  //  009B  CALL	R10	3
      0x80041400,  //  009C  RET	1	R10
      0x70020004,  //  009D  JMP		#00A3
      0x8C24072E,  //  009E  GETMET	R9	R3	K46
      0x882C0943,  //  009F  GETMBR	R11	R4	K67
      0x4C300000,  //  00A0  LDNIL	R12
      0x7C240600,  //  00A1  CALL	R9	3
      0x80041200,  //  00A2  RET	1	R9
      0x70020014,  //  00A3  JMP		#00B9
      0xB8227C00,  //  00A4  GETNGBL	R8	K62
      0x8C20113F,  //  00A5  GETMET	R8	R8	K63
      0x58280044,  //  00A6  LDCONST	R10	K68
      0x502C0200,  //  00A7  LDBOOL	R11	1	0
      0x7C200600,  //  00A8  CALL	R8	3
      0x94201145,  //  00A9  GETIDX	R8	R8	K69
      0x94201146,  //  00AA  GETIDX	R8	R8	K70
      0x8C240F36,  //  00AB  GETMET	R9	R7	K54
      0x5C2C1000,  //  00AC  MOVE	R11	R8
      0x58300042,  //  00AD  LDCONST	R12	K66
      0x7C240600,  //  00AE  CALL	R9	3
      0x24281315,  //  00AF  GT	R10	R9	K21
      0x782A0002,  //  00B0  JMPF	R10	#00B4
      0x04281316,  //  00B1  SUB	R10	R9	K22
      0x402A2A0A,  //  00B2  CONNECT	R10	K21	R10
      0x9420100A,  //  00B3  GETIDX	R8	R8	R10
      0x8C28072E,  //  00B4  GETMET	R10	R3	K46
      0x8830093D,  //  00B5  GETMBR	R12	R4	K61
      0x5C341000,  //  00B6  MOVE	R13	R8
      0x7C280600,  //  00B7  CALL	R10	3
      0x80041400,  //  00B8  RET	1	R10
      0x70020030,  //  00B9  JMP		#00EB
      0x5422000E,  //  00BA  LDINT	R8	15
      0x1C200C08,  //  00BB  EQ	R8	R6	R8
      0x74220002,  //  00BC  JMPT	R8	#00C0
      0x54220011,  //  00BD  LDINT	R8	18
      0x1C200C08,  //  00BE  EQ	R8	R6	R8
      0x78220019,  //  00BF  JMPF	R8	#00DA
      0x8820010A,  //  00C0  GETMBR	R8	R0	K10
      0x7822000B,  //  00C1  JMPF	R8	#00CE
      0x88200100,  //  00C2  GETMBR	R8	R0	K0
      0x8C20113A,  //  00C3  GETMET	R8	R8	K58
      0x7C200200,  //  00C4  CALL	R8	1
      0x8C201136,  //  00C5  GETMET	R8	R8	K54
      0x58280047,  //  00C6  LDCONST	R10	K71
      0x7C200400,  //  00C7  CALL	R8	2
      0x8C24073C,  //  00C8  GETMET	R9	R3	K60
      0x882C093D,  //  00C9  GETMBR	R11	R4	K61
      0x5C301000,  //  00CA  MOVE	R12	R8
      0x7C240600,  //  00CB  CALL	R9	3
      0x80041200,  //  00CC  RET	1	R9
      0x7002000A,  //  00CD  JMP		#00D9
      0x8C20072E,  //  00CE  GETMET	R8	R3	K46
      0x8828093D,  //  00CF  GETMBR	R10	R4	K61
      0xB82E7C00,  //  00D0  GETNGBL	R11	K62
      0x8C2C1748,  //  00D1  GETMET	R11	R11	K72
      0x7C2C0200,  //  00D2  CALL	R11	1
      0x8C2C1736,  //  00D3  GETMET	R11	R11	K54
      0x58340047,  //  00D4  LDCONST	R13	K71
      0x5838000F,  //  00D5  LDCONST	R14	K15
      0x7C2C0600,  //  00D6  CALL	R11	3
      0x7C200600,  //  00D7  CALL	R8	3
      0x80041000,  //  00D8  RET	1	R8
      0x70020010,  //  00D9  JMP		#00EB
      0x54220010,  //  00DA  LDINT	R8	17
      0x1C200C08,  //  00DB  EQ	R8	R6	R8
      0x7822000D,  //  00DC  JMPF	R8	#00EB
      0x8820010A,  //  00DD  GETMBR	R8	R0	K10
      0x78220006,  //  00DE  JMPF	R8	#00E6
      0x8C20072E,  //  00DF  GETMET	R8	R3	K46
      0x88280949,  //  00E0  GETMBR	R10	R4	K73
      0x882C0100,  //  00E1  GETMBR	R11	R0	K0
      0x882C174A,  //  00E2  GETMBR	R11	R11	K74
      0x7C200600,  //  00E3  CALL	R8	3
      0x80041000,  //  00E4  RET	1	R8
      0x70020004,  //  00E5  JMP		#00EB
      0x8C20072E,  //  00E6  GETMET	R8	R3	K46
      0x88280949,  //  00E7  GETMBR	R10	R4	K73
      0x582C0016,  //  00E8  LDCONST	R11	K22
      0x7C200600,  //  00E9  CALL	R8	3
      0x80041000,  //  00EA  RET	1	R8
      0x601C0003,  //  00EB  GETGBL	R7	G3
      0x5C200000,  //  00EC  MOVE	R8	R0
      0x7C1C0200,  //  00ED  CALL	R7	1
      0x8C1C0F4B,  //  00EE  GETMET	R7	R7	K75
      0x5C240200,  //  00EF  MOVE	R9	R1
      0x5C280400,  //  00F0  MOVE	R10	R2
      0x5C2C0600,  //  00F1  MOVE	R11	R3
      0x7C1C0800,  //  00F2  CALL	R7	4
      0x80040E00,  //  00F3  RET	1	R7
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Plugin_Device_init,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Device,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[22]) {  /* code */
      0x60100003,  //  0000  GETGBL	R4	G3
      0x5C140000,  //  0001  MOVE	R5	R0
      0x7C100200,  //  0002  CALL	R4	1
      0x8C10094C,  //  0003  GETMET	R4	R4	K76
      0x5C180200,  //  0004  MOVE	R6	R1
      0x5C1C0400,  //  0005  MOVE	R7	R2
      0x5C200600,  //  0006  MOVE	R8	R3
      0x7C100800,  //  0007  CALL	R4	4
      0x8810010A,  //  0008  GETMBR	R4	R0	K10
      0x7812000A,  //  0009  JMPF	R4	#0015
      0x8C100736,  //  000A  GETMET	R4	R3	K54
      0x8818014D,  //  000B  GETMBR	R6	R0	K77
      0x7C100400,  //  000C  CALL	R4	2
      0x8814011C,  //  000D  GETMBR	R5	R0	K28
      0x8C140B4E,  //  000E  GETMET	R5	R5	K78
      0x5C1C0800,  //  000F  MOVE	R7	R4
      0x8820014F,  //  0010  GETMBR	R8	R0	K79
      0x7C140600,  //  0011  CALL	R5	3
      0x90020005,  //  0012  SETMBR	R0	K0	R5
      0x8C140150,  //  0013  GETMET	R5	R0	K80
      0x7C140200,  //  0014  CALL	R5	1
      0x80000000,  //  0015  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: append_state_json
********************************************************************/
be_local_closure(class_Matter_Plugin_Device_append_state_json,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
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
    &be_ktab_class_Matter_Plugin_Device,     /* shared constants */
    be_str_weak(append_state_json),
    &be_const_str_solidified,
    ( &(const binstruction[95]) {  /* code */
      0xA406A200,  //  0000  IMPORT	R1	K81
      0xA40A3E00,  //  0001  IMPORT	R2	K31
      0x580C000F,  //  0002  LDCONST	R3	K15
      0x84100000,  //  0003  CLOSURE	R4	P0
      0x8C140352,  //  0004  GETMET	R5	R1	K82
      0x5C1C0000,  //  0005  MOVE	R7	R0
      0x58200053,  //  0006  LDCONST	R8	K83
      0x7C140600,  //  0007  CALL	R5	3
      0x78160013,  //  0008  JMPF	R5	#001D
      0x8C180354,  //  0009  GETMET	R6	R1	K84
      0x5C200000,  //  000A  MOVE	R8	R0
      0x58240055,  //  000B  LDCONST	R9	K85
      0x7C180600,  //  000C  CALL	R6	3
      0x781A000E,  //  000D  JMPF	R6	#001D
      0x88180155,  //  000E  GETMBR	R6	R0	K85
      0x4C1C0000,  //  000F  LDNIL	R7
      0x20180C07,  //  0010  NE	R6	R6	R7
      0x781A0003,  //  0011  JMPF	R6	#0016
      0x8C180556,  //  0012  GETMET	R6	R2	K86
      0x88200155,  //  0013  GETMBR	R8	R0	K85
      0x7C180400,  //  0014  CALL	R6	2
      0x70020000,  //  0015  JMP		#0017
      0x58180057,  //  0016  LDCONST	R6	K87
      0x601C0018,  //  0017  GETGBL	R7	G24
      0x58200058,  //  0018  LDCONST	R8	K88
      0x5C240A00,  //  0019  MOVE	R9	R5
      0x5C280C00,  //  001A  MOVE	R10	R6
      0x7C1C0600,  //  001B  CALL	R7	3
      0x000C0607,  //  001C  ADD	R3	R3	R7
      0x5C180800,  //  001D  MOVE	R6	R4
      0x581C0059,  //  001E  LDCONST	R7	K89
      0x5820005A,  //  001F  LDCONST	R8	K90
      0x7C180400,  //  0020  CALL	R6	2
      0x5C180800,  //  0021  MOVE	R6	R4
      0x581C005B,  //  0022  LDCONST	R7	K91
      0x5820005C,  //  0023  LDCONST	R8	K92
      0x7C180400,  //  0024  CALL	R6	2
      0x5C180800,  //  0025  MOVE	R6	R4
      0x581C005D,  //  0026  LDCONST	R7	K93
      0x5820005E,  //  0027  LDCONST	R8	K94
      0x7C180400,  //  0028  CALL	R6	2
      0x5C180800,  //  0029  MOVE	R6	R4
      0x581C005F,  //  002A  LDCONST	R7	K95
      0x58200060,  //  002B  LDCONST	R8	K96
      0x7C180400,  //  002C  CALL	R6	2
      0x5C180800,  //  002D  MOVE	R6	R4
      0x581C0061,  //  002E  LDCONST	R7	K97
      0x58200062,  //  002F  LDCONST	R8	K98
      0x7C180400,  //  0030  CALL	R6	2
      0x5C180800,  //  0031  MOVE	R6	R4
      0x581C0063,  //  0032  LDCONST	R7	K99
      0x58200064,  //  0033  LDCONST	R8	K100
      0x7C180400,  //  0034  CALL	R6	2
      0x5C180800,  //  0035  MOVE	R6	R4
      0x581C0065,  //  0036  LDCONST	R7	K101
      0x58200066,  //  0037  LDCONST	R8	K102
      0x7C180400,  //  0038  CALL	R6	2
      0x5C180800,  //  0039  MOVE	R6	R4
      0x581C0067,  //  003A  LDCONST	R7	K103
      0x58200068,  //  003B  LDCONST	R8	K104
      0x7C180400,  //  003C  CALL	R6	2
      0x5C180800,  //  003D  MOVE	R6	R4
      0x581C0069,  //  003E  LDCONST	R7	K105
      0x5820006A,  //  003F  LDCONST	R8	K106
      0x7C180400,  //  0040  CALL	R6	2
      0x5C180800,  //  0041  MOVE	R6	R4
      0x581C006B,  //  0042  LDCONST	R7	K107
      0x5820006C,  //  0043  LDCONST	R8	K108
      0x7C180400,  //  0044  CALL	R6	2
      0x5C180800,  //  0045  MOVE	R6	R4
      0x581C006D,  //  0046  LDCONST	R7	K109
      0x5820006E,  //  0047  LDCONST	R8	K110
      0x7C180400,  //  0048  CALL	R6	2
      0x5C180800,  //  0049  MOVE	R6	R4
      0x581C006F,  //  004A  LDCONST	R7	K111
      0x58200070,  //  004B  LDCONST	R8	K112
      0x7C180400,  //  004C  CALL	R6	2
      0x5C180800,  //  004D  MOVE	R6	R4
      0x581C0071,  //  004E  LDCONST	R7	K113
      0x58200072,  //  004F  LDCONST	R8	K114
      0x7C180400,  //  0050  CALL	R6	2
      0x5C180800,  //  0051  MOVE	R6	R4
      0x581C0073,  //  0052  LDCONST	R7	K115
      0x58200074,  //  0053  LDCONST	R8	K116
      0x7C180400,  //  0054  CALL	R6	2
      0x5C180800,  //  0055  MOVE	R6	R4
      0x581C0075,  //  0056  LDCONST	R7	K117
      0x58200076,  //  0057  LDCONST	R8	K118
      0x7C180400,  //  0058  CALL	R6	2
      0x5C180800,  //  0059  MOVE	R6	R4
      0x581C0077,  //  005A  LDCONST	R7	K119
      0x58200078,  //  005B  LDCONST	R8	K120
      0x7C180400,  //  005C  CALL	R6	2
      0xA0000000,  //  005D  CLOSE	R0
      0x80040600,  //  005E  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _parse_sensor_entry
********************************************************************/
be_local_closure(class_Matter_Plugin_Device__parse_sensor_entry,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    7,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Plugin_Device,     /* shared constants */
    be_str_weak(_parse_sensor_entry),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x8C1C0336,  //  0000  GETMET	R7	R1	K54
      0x5C240400,  //  0001  MOVE	R9	R2
      0x7C1C0400,  //  0002  CALL	R7	2
      0x4C200000,  //  0003  LDNIL	R8
      0x20200E08,  //  0004  NE	R8	R7	R8
      0x7822000A,  //  0005  JMPF	R8	#0011
      0x5C200800,  //  0006  MOVE	R8	R4
      0x5C240E00,  //  0007  MOVE	R9	R7
      0x7C200200,  //  0008  CALL	R8	1
      0x5C1C1000,  //  0009  MOVE	R7	R8
      0x20200E03,  //  000A  NE	R8	R7	R3
      0x78220003,  //  000B  JMPF	R8	#0010
      0x8C200179,  //  000C  GETMET	R8	R0	K121
      0x5C280A00,  //  000D  MOVE	R10	R5
      0x5C2C0C00,  //  000E  MOVE	R11	R6
      0x7C200600,  //  000F  CALL	R8	3
      0x80040E00,  //  0010  RET	1	R7
      0x80040600,  //  0011  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Plugin_Device
********************************************************************/
extern const bclass be_class_Matter_Plugin;
be_local_class(Matter_Plugin_Device,
    1,
    &be_class_Matter_Plugin,
    be_nested_map(24,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(ARG_HTTP, -1), be_nested_str_weak(url) },
        { be_const_key_weak(_parse_sensor_entry, -1), be_const_closure(class_Matter_Plugin_Device__parse_sensor_entry_closure) },
        { be_const_key_weak(TYPES, 14), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(1,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(19, -1), be_const_int(1) },
    }))    ) } )) },
        { be_const_key_weak(parse_status, 12), be_const_closure(class_Matter_Plugin_Device_parse_status_closure) },
        { be_const_key_weak(http_remote, -1), be_const_var(0) },
        { be_const_key_weak(register_cmd_cb, 6), be_const_closure(class_Matter_Plugin_Device_register_cmd_cb_closure) },
        { be_const_key_weak(GetOptionReader, 16), be_const_class(be_class_GetOptionReader) },
        { be_const_key_weak(append_state_json, 19), be_const_closure(class_Matter_Plugin_Device_append_state_json_closure) },
        { be_const_key_weak(web_values, 22), be_const_closure(class_Matter_Plugin_Device_web_values_closure) },
        { be_const_key_weak(PREFIX, -1), be_nested_str_weak(_X7C_X20_X3Ci_X3E_X25s_X3C_X2Fi_X3E_X20) },
        { be_const_key_weak(CLUSTERS, 7), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(5,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_int(5, -1), be_const_bytes_instance(000000010002000300040005FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(29, -1), be_const_bytes_instance(0000000100020003FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(57, -1), be_const_bytes_instance(00030005000A000F00110012FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(3, -1), be_const_bytes_instance(00000001FFF8FFF9FFFAFFFBFFFCFFFD) },
        { be_const_key_int(4, 1), be_const_bytes_instance(0000FFF8FFF9FFFAFFFBFFFCFFFD) },
    }))    ) } )) },
        { be_const_key_weak(invoke_request, -1), be_const_closure(class_Matter_Plugin_Device_invoke_request_closure) },
        { be_const_key_weak(init, -1), be_const_closure(class_Matter_Plugin_Device_init_closure) },
        { be_const_key_weak(PROBE_TIMEOUT, 20), be_const_int(1700) },
        { be_const_key_weak(SYNC_TIMEOUT, -1), be_const_int(500) },
        { be_const_key_weak(UPDATE_CMD, -1), be_nested_str_weak(Status_X2011) },
        { be_const_key_weak(parse_http_response, -1), be_const_closure(class_Matter_Plugin_Device_parse_http_response_closure) },
        { be_const_key_weak(web_values_prefix, -1), be_const_closure(class_Matter_Plugin_Device_web_values_prefix_closure) },
        { be_const_key_weak(read_attribute, -1), be_const_closure(class_Matter_Plugin_Device_read_attribute_closure) },
        { be_const_key_weak(call_remote_sync, 21), be_const_closure(class_Matter_Plugin_Device_call_remote_sync_closure) },
        { be_const_key_weak(update_shadow, -1), be_const_closure(class_Matter_Plugin_Device_update_shadow_closure) },
        { be_const_key_weak(web_value_onoff, -1), be_const_closure(class_Matter_Plugin_Device_web_value_onoff_closure) },
        { be_const_key_weak(every_250ms, -1), be_const_closure(class_Matter_Plugin_Device_every_250ms_closure) },
        { be_const_key_weak(NON_BRIDGE_VENDOR, 1), be_const_simple_instance(be_nested_simple_instance(&be_class_list, {
        be_const_list( *     be_nested_list(2,
    ( (struct bvalue*) &(const bvalue[]) {
        be_const_int(4631),
        be_const_int(4993),
    }))    ) } )) },
    })),
    be_str_weak(Matter_Plugin_Device)
);
/********************************************************************/
/* End of solidification */
