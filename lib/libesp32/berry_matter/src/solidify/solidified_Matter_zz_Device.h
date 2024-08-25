/* Solidification of Matter_zz_Device.h */
/********************************************************************\
* Generated code, don't edit                                         *
\********************************************************************/
#include "be_constobj.h"
extern const bclass be_class_Matter_Device;
// compact class 'Matter_Device' ktab size: 210, total: 454 (saved 1952 bytes)
static const bvalue be_ktab_class_Matter_Device[210] = {
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
  /* K13  */  be_nested_str_weak(plugins_config_remotes),
  /* K14  */  be_nested_str_weak(next_ep),
  /* K15  */  be_nested_str_weak(EP),
  /* K16  */  be_nested_str_weak(ipv4only),
  /* K17  */  be_nested_str_weak(disable_bridge_mode),
  /* K18  */  be_nested_str_weak(commissioning),
  /* K19  */  be_nested_str_weak(Commissioning),
  /* K20  */  be_nested_str_weak(load_param),
  /* K21  */  be_nested_str_weak(sessions),
  /* K22  */  be_nested_str_weak(Session_Store),
  /* K23  */  be_nested_str_weak(load_fabrics),
  /* K24  */  be_nested_str_weak(message_handler),
  /* K25  */  be_nested_str_weak(MessageHandler),
  /* K26  */  be_nested_str_weak(events),
  /* K27  */  be_nested_str_weak(EventHandler),
  /* K28  */  be_nested_str_weak(ui),
  /* K29  */  be_nested_str_weak(init_basic_commissioning),
  /* K30  */  be_nested_str_weak(add_driver),
  /* K31  */  be_nested_str_weak(register_commands),
  /* K32  */  be_nested_str_weak(button_handler),
  /* K33  */  be_const_int(1),
  /* K34  */  be_const_int(2),
  /* K35  */  be_nested_str_weak(get_endpoint),
  /* K36  */  be_nested_str_weak(get_name),
  /* K37  */  be_nested_str_weak(log),
  /* K38  */  be_nested_str_weak(MTR_X3A_X20removing_X20fabric_X20),
  /* K39  */  be_nested_str_weak(get_fabric_id),
  /* K40  */  be_nested_str_weak(copy),
  /* K41  */  be_nested_str_weak(reverse),
  /* K42  */  be_nested_str_weak(tohex),
  /* K43  */  be_nested_str_weak(im),
  /* K44  */  be_nested_str_weak(subs_shop),
  /* K45  */  be_nested_str_weak(remove_by_fabric),
  /* K46  */  be_nested_str_weak(mdns_remove_op_discovery),
  /* K47  */  be_nested_str_weak(remove_fabric),
  /* K48  */  be_nested_str_weak(save_fabrics),
  /* K49  */  be_nested_str_weak(msg_received),
  /* K50  */  be_nested_str_weak(find),
  /* K51  */  be_nested_str_weak(add_cmd),
  /* K52  */  be_nested_str_weak(MtrJoin),
  /* K53  */  be_nested_str_weak(MtrUpdate),
  /* K54  */  be_nested_str_weak(MtrInfo),
  /* K55  */  be_const_class(be_class_Matter_Device),
  /* K56  */  be_nested_str_weak(keys),
  /* K57  */  be_nested_str_weak(push),
  /* K58  */  be_nested_str_weak(stop_iteration),
  /* K59  */  be_nested_str_weak(start_root_basic_commissioning),
  /* K60  */  be_nested_str_weak(stop_basic_commissioning),
  /* K61  */  be_nested_str_weak(resp_cmnd_done),
  /* K62  */  be_nested_str_weak(Path),
  /* K63  */  be_nested_str_weak(endpoint),
  /* K64  */  be_nested_str_weak(cluster),
  /* K65  */  be_nested_str_weak(attribute),
  /* K66  */  be_nested_str_weak(attribute_updated_ctx),
  /* K67  */  be_nested_str_weak(json),
  /* K68  */  be_nested_str_weak(update_remotes_info),
  /* K69  */  be_nested_str_weak(_X7B_X22distinguish_X22_X3A_X25i_X2C_X22passcode_X22_X3A_X25i_X2C_X22ipv4only_X22_X3A_X25s_X2C_X22disable_bridge_mode_X22_X3A_X25s_X2C_X22nextep_X22_X3A_X25i),
  /* K70  */  be_nested_str_weak(root_discriminator),
  /* K71  */  be_nested_str_weak(root_passcode),
  /* K72  */  be_nested_str_weak(true),
  /* K73  */  be_nested_str_weak(false),
  /* K74  */  be_nested_str_weak(debug),
  /* K75  */  be_nested_str_weak(_X2C_X22debug_X22_X3Atrue),
  /* K76  */  be_nested_str_weak(_X2C_X0A_X22config_X22_X3A),
  /* K77  */  be_nested_str_weak(dump),
  /* K78  */  be_nested_str_weak(plugins_config),
  /* K79  */  be_nested_str_weak(_X2C_X0A_X22remotes_X22_X3A),
  /* K80  */  be_nested_str_weak(_X7D),
  /* K81  */  be_nested_str_weak(FILENAME),
  /* K82  */  be_nested_str_weak(w),
  /* K83  */  be_nested_str_weak(write),
  /* K84  */  be_nested_str_weak(close),
  /* K85  */  be_nested_str_weak(MTR_X3A_X20_X3DSaved_X20_X20_X20_X20_X20parameters_X25s),
  /* K86  */  be_nested_str_weak(_X20and_X20configuration),
  /* K87  */  be_nested_str_weak(),
  /* K88  */  be_nested_str_weak(MTR_X3A_X20Session_Store_X3A_X3Asave_X20Exception_X3A),
  /* K89  */  be_nested_str_weak(_X7C),
  /* K90  */  be_nested_str_weak(read),
  /* K91  */  be_nested_str_weak(load),
  /* K92  */  be_nested_str_weak(distinguish),
  /* K93  */  be_nested_str_weak(passcode),
  /* K94  */  be_nested_str_weak(nextep),
  /* K95  */  be_nested_str_weak(config),
  /* K96  */  be_nested_str_weak(MTR_X3A_X20Load_config_X20_X3D_X20_X25s),
  /* K97  */  be_const_int(3),
  /* K98  */  be_nested_str_weak(adjust_next_ep),
  /* K99  */  be_nested_str_weak(check_config_ep),
  /* K100 */  be_nested_str_weak(remotes),
  /* K101 */  be_nested_str_weak(MTR_X3A_X20load_remotes_X20_X3D_X20),
  /* K102 */  be_nested_str_weak(io_error),
  /* K103 */  be_nested_str_weak(MTR_X3A_X20load_param_X20Exception_X3A),
  /* K104 */  be_nested_str_weak(random),
  /* K105 */  be_nested_str_weak(get),
  /* K106 */  be_nested_str_weak(generate_random_passcode),
  /* K107 */  be_nested_str_weak(save_param),
  /* K108 */  be_nested_str_weak(http_remotes),
  /* K109 */  be_nested_str_weak(contains),
  /* K110 */  be_nested_str_weak(get_timeout),
  /* K111 */  be_nested_str_weak(set_timeout),
  /* K112 */  be_nested_str_weak(HTTP_remote),
  /* K113 */  be_nested_str_weak(set_info),
  /* K114 */  be_nested_str_weak(remove),
  /* K115 */  be_nested_str_weak(udp_server),
  /* K116 */  be_nested_str_weak(received_ack),
  /* K117 */  be_nested_str_weak(every_second),
  /* K118 */  be_nested_str_weak(find_plugin_by_endpoint),
  /* K119 */  be_nested_str_weak(status),
  /* K120 */  be_nested_str_weak(UNSUPPORTED_ENDPOINT),
  /* K121 */  be_nested_str_weak(contains_cluster),
  /* K122 */  be_nested_str_weak(UNSUPPORTED_CLUSTER),
  /* K123 */  be_nested_str_weak(contains_attribute),
  /* K124 */  be_nested_str_weak(UNSUPPORTED_ATTRIBUTE),
  /* K125 */  be_nested_str_weak(introspect),
  /* K126 */  be_nested_str_weak(http_remote),
  /* K127 */  be_nested_str_weak(MTR_X3A_X20remove_X20unused_X20remote_X3A_X20),
  /* K128 */  be_nested_str_weak(addr),
  /* K129 */  be_nested_str_weak(autoconf_device),
  /* K130 */  be_nested_str_weak(_start_udp),
  /* K131 */  be_nested_str_weak(UDP_PORT),
  /* K132 */  be_nested_str_weak(start_mdns_announce_hostnames),
  /* K133 */  be_nested_str_weak(send_UDP),
  /* K134 */  be_nested_str_weak(probe_sensor_time),
  /* K135 */  be_nested_str_weak(probe_sensor_timestamp),
  /* K136 */  be_nested_str_weak(jitter),
  /* K137 */  be_nested_str_weak(remove_driver),
  /* K138 */  be_nested_str_weak(stop),
  /* K139 */  be_nested_str_weak(MTR_X3A_X20Starting_X20UDP_X20server_X20on_X20port_X3A_X20),
  /* K140 */  be_nested_str_weak(UDPServer),
  /* K141 */  be_nested_str_weak(start),
  /* K142 */  be_nested_str_weak(MtrInfo_one),
  /* K143 */  be_nested_str_weak(int),
  /* K144 */  be_nested_str_weak(find_plugin_by_friendly_name),
  /* K145 */  be_nested_str_weak(state_json),
  /* K146 */  be_nested_str_weak(_X7B_X22MtrInfo_X22_X3A_X25s_X7D),
  /* K147 */  be_nested_str_weak(publish_result),
  /* K148 */  be_nested_str_weak(wifi),
  /* K149 */  be_nested_str_weak(up),
  /* K150 */  be_nested_str_weak(eth),
  /* K151 */  be_nested_str_weak(read_sensors),
  /* K152 */  be_nested_str_weak(loglevel),
  /* K153 */  be_nested_str_weak(MTR_X3A_X20read_sensors_X3A_X20),
  /* K154 */  be_nested_str_weak(parse_sensors),
  /* K155 */  be_nested_str_weak(MTR_X3A_X20unable_X20to_X20parse_X20read_sensors_X3A_X20),
  /* K156 */  be_nested_str_weak(plugins_classes),
  /* K157 */  be_nested_str_weak(ARG),
  /* K158 */  be_nested_str_weak(MTR_X3A_X20unknown_X20class_X20name_X20_X27),
  /* K159 */  be_nested_str_weak(_X27_X20skipping),
  /* K160 */  be_nested_str_weak(type),
  /* K161 */  be_nested_str_weak(MTR_X3A_X20adding_X20endpoint_X20_X3D_X20_X25i_X20type_X3A_X25s_X25s),
  /* K162 */  be_nested_str_weak(conf_to_log),
  /* K163 */  be_nested_str_weak(signal_endpoints_changed),
  /* K164 */  be_nested_str_weak(attribute_updated),
  /* K165 */  be_nested_str_weak(AGGREGATOR_ENDPOINT),
  /* K166 */  be_nested_str_weak(check_network),
  /* K167 */  be_nested_str_weak(every_50ms),
  /* K168 */  be_nested_str_weak(k2l),
  /* K169 */  be_nested_str_weak(_X20_X25s_X3A_X25s),
  /* K170 */  be_nested_str_weak(resp_cmnd_str),
  /* K171 */  be_nested_str_weak(Invalid_X20JSON),
  /* K172 */  be_nested_str_weak(find_key_i),
  /* K173 */  be_nested_str_weak(Ep),
  /* K174 */  be_nested_str_weak(Name),
  /* K175 */  be_nested_str_weak(Invalid_X20_X27Ep_X27_X20attribute),
  /* K176 */  be_nested_str_weak(Invalid_X20Device),
  /* K177 */  be_nested_str_weak(VIRTUAL),
  /* K178 */  be_nested_str_weak(Device_X20is_X20not_X20virtual),
  /* K179 */  be_nested_str_weak(consolidate_update_commands),
  /* K180 */  be_nested_str_weak(find_list_i),
  /* K181 */  be_nested_str_weak(Invalid_X20attribute_X20_X27_X25s_X27),
  /* K182 */  be_nested_str_weak(update_virtual),
  /* K183 */  be_nested_str_weak(_X7B_X22_X25s_X22_X3A_X25s_X7D),
  /* K184 */  be_nested_str_weak(resp_cmnd),
  /* K185 */  be_nested_str_weak(Missing_X20_X27Device_X27_X20attribute),
  /* K186 */  be_nested_str_weak(count_active_fabrics),
  /* K187 */  be_nested_str_weak(PathGenerator),
  /* K188 */  be_nested_str_weak(is_direct),
  /* K189 */  be_nested_str_weak(next_attribute),
  /* K190 */  be_nested_str_weak(get_pi),
  /* K191 */  be_nested_str_weak(invoke_request),
  /* K192 */  be_nested_str_weak(get_info),
  /* K193 */  be_nested_str_weak(DISPLAY_NAME),
  /* K194 */  be_nested_str_weak(MTR_X3A_X20Cannot_X20remove_X20an_X20enpoint_X20not_X20configured_X3A_X20),
  /* K195 */  be_nested_str_weak(MTR_X3A_X20deleting_X20endpoint_X20_X3D_X20_X25i),
  /* K196 */  be_nested_str_weak(clean_remotes),
  /* K197 */  be_nested_str_weak(MTR_X3A_X20invalid_X20entry_X20with_X20ep_X20_X270_X27),
  /* K198 */  be_nested_str_weak(MTR_X3A_X20endpoint_X20_X25s_X20collides_X20wit_X20aggregator_X2C_X20relocating_X20to_X20_X25s),
  /* K199 */  be_nested_str_weak(time_reached),
  /* K200 */  be_nested_str_weak(_trigger_read_sensors),
  /* K201 */  be_nested_str_weak(millis),
  /* K202 */  be_nested_str_weak(mdns_remove_op_discovery_all_fabrics),
  /* K203 */  be_nested_str_weak(read_sensors_scheduler),
  /* K204 */  be_nested_str_weak(every_250ms),
  /* K205 */  be_nested_str_weak(autoconf),
  /* K206 */  be_nested_str_weak(Autoconf),
  /* K207 */  be_nested_str_weak(autoconf_device_map),
  /* K208 */  be_nested_str_weak(MTR_X3A_X20autoconfig_X20_X3D_X20),
  /* K209 */  be_nested_str_weak(instantiate_plugins_from_config),
};


extern const bclass be_class_Matter_Device;

/********************************************************************
** Solidified function: init
********************************************************************/
be_local_closure(class_Matter_Device_init,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(init),
    &be_const_str_solidified,
    ( &(const binstruction[74]) {  /* code */
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
      0x8808010F,  //  001C  GETMBR	R2	R0	K15
      0x90021C02,  //  001D  SETMBR	R0	K14	R2
      0x50080000,  //  001E  LDBOOL	R2	0	0
      0x90022002,  //  001F  SETMBR	R0	K16	R2
      0x50080000,  //  0020  LDBOOL	R2	0	0
      0x90022202,  //  0021  SETMBR	R0	K17	R2
      0xB80A0600,  //  0022  GETNGBL	R2	K3
      0x8C080513,  //  0023  GETMET	R2	R2	K19
      0x5C100000,  //  0024  MOVE	R4	R0
      0x7C080400,  //  0025  CALL	R2	2
      0x90022402,  //  0026  SETMBR	R0	K18	R2
      0x8C080114,  //  0027  GETMET	R2	R0	K20
      0x7C080200,  //  0028  CALL	R2	1
      0xB80A0600,  //  0029  GETNGBL	R2	K3
      0x8C080516,  //  002A  GETMET	R2	R2	K22
      0x5C100000,  //  002B  MOVE	R4	R0
      0x7C080400,  //  002C  CALL	R2	2
      0x90022A02,  //  002D  SETMBR	R0	K21	R2
      0x88080115,  //  002E  GETMBR	R2	R0	K21
      0x8C080517,  //  002F  GETMET	R2	R2	K23
      0x7C080200,  //  0030  CALL	R2	1
      0xB80A0600,  //  0031  GETNGBL	R2	K3
      0x8C080519,  //  0032  GETMET	R2	R2	K25
      0x5C100000,  //  0033  MOVE	R4	R0
      0x7C080400,  //  0034  CALL	R2	2
      0x90023002,  //  0035  SETMBR	R0	K24	R2
      0xB80A0600,  //  0036  GETNGBL	R2	K3
      0x8C08051B,  //  0037  GETMET	R2	R2	K27
      0x5C100000,  //  0038  MOVE	R4	R0
      0x7C080400,  //  0039  CALL	R2	2
      0x90023402,  //  003A  SETMBR	R0	K26	R2
      0xB80A0600,  //  003B  GETNGBL	R2	K3
      0x8C080505,  //  003C  GETMET	R2	R2	K5
      0x5C100000,  //  003D  MOVE	R4	R0
      0x7C080400,  //  003E  CALL	R2	2
      0x90023802,  //  003F  SETMBR	R0	K28	R2
      0x88080112,  //  0040  GETMBR	R2	R0	K18
      0x8C08051D,  //  0041  GETMET	R2	R2	K29
      0x7C080200,  //  0042  CALL	R2	1
      0xB80A0200,  //  0043  GETNGBL	R2	K1
      0x8C08051E,  //  0044  GETMET	R2	R2	K30
      0x5C100000,  //  0045  MOVE	R4	R0
      0x7C080400,  //  0046  CALL	R2	2
      0x8C08011F,  //  0047  GETMET	R2	R0	K31
      0x7C080200,  //  0048  CALL	R2	1
      0x80000000,  //  0049  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: button_multi_pressed
********************************************************************/
be_local_closure(class_Matter_Device_button_multi_pressed,   /* name */
  be_nested_proto(
    11,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(button_multi_pressed),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x540E0007,  //  0000  LDINT	R3	8
      0x3C0C0403,  //  0001  SHR	R3	R2	R3
      0x541200FE,  //  0002  LDINT	R4	255
      0x2C0C0604,  //  0003  AND	R3	R3	R4
      0x541200FE,  //  0004  LDINT	R4	255
      0x2C100404,  //  0005  AND	R4	R2	R4
      0x8C140120,  //  0006  GETMET	R5	R0	K32
      0x001C0921,  //  0007  ADD	R7	R4	K33
      0x58200022,  //  0008  LDCONST	R8	K34
      0x5824000A,  //  0009  LDCONST	R9	K10
      0x5C280600,  //  000A  MOVE	R10	R3
      0x7C140A00,  //  000B  CALL	R5	5
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_plugin_by_endpoint
********************************************************************/
be_local_closure(class_Matter_Device_find_plugin_by_endpoint,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(find_plugin_by_endpoint),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0x5808000A,  //  0000  LDCONST	R2	K10
      0x600C000C,  //  0001  GETGBL	R3	G12
      0x8810010B,  //  0002  GETMBR	R4	R0	K11
      0x7C0C0200,  //  0003  CALL	R3	1
      0x140C0403,  //  0004  LT	R3	R2	R3
      0x780E0008,  //  0005  JMPF	R3	#000F
      0x880C010B,  //  0006  GETMBR	R3	R0	K11
      0x940C0602,  //  0007  GETIDX	R3	R3	R2
      0x8C100723,  //  0008  GETMET	R4	R3	K35
      0x7C100200,  //  0009  CALL	R4	1
      0x1C100801,  //  000A  EQ	R4	R4	R1
      0x78120000,  //  000B  JMPF	R4	#000D
      0x80040600,  //  000C  RET	1	R3
      0x00080521,  //  000D  ADD	R2	R2	K33
      0x7001FFF1,  //  000E  JMP		#0001
      0x4C0C0000,  //  000F  LDNIL	R3
      0x80040600,  //  0010  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: find_plugin_by_friendly_name
********************************************************************/
be_local_closure(class_Matter_Device_find_plugin_by_friendly_name,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(find_plugin_by_friendly_name),
    &be_const_str_solidified,
    ( &(const binstruction[35]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x1C080202,  //  0001  EQ	R2	R1	R2
      0x740A0004,  //  0002  JMPT	R2	#0008
      0x6008000C,  //  0003  GETGBL	R2	G12
      0x5C0C0200,  //  0004  MOVE	R3	R1
      0x7C080200,  //  0005  CALL	R2	1
      0x1C08050A,  //  0006  EQ	R2	R2	K10
      0x780A0001,  //  0007  JMPF	R2	#000A
      0x4C080000,  //  0008  LDNIL	R2
      0x80040400,  //  0009  RET	1	R2
      0x5808000A,  //  000A  LDCONST	R2	K10
      0x600C000C,  //  000B  GETGBL	R3	G12
      0x8810010B,  //  000C  GETMBR	R4	R0	K11
      0x7C0C0200,  //  000D  CALL	R3	1
      0x140C0403,  //  000E  LT	R3	R2	R3
      0x780E0010,  //  000F  JMPF	R3	#0021
      0x880C010B,  //  0010  GETMBR	R3	R0	K11
      0x940C0602,  //  0011  GETIDX	R3	R3	R2
      0x8C100724,  //  0012  GETMET	R4	R3	K36
      0x7C100200,  //  0013  CALL	R4	1
      0x4C140000,  //  0014  LDNIL	R5
      0x20140805,  //  0015  NE	R5	R4	R5
      0x78160007,  //  0016  JMPF	R5	#001F
      0x6014000C,  //  0017  GETGBL	R5	G12
      0x5C180800,  //  0018  MOVE	R6	R4
      0x7C140200,  //  0019  CALL	R5	1
      0x24140B0A,  //  001A  GT	R5	R5	K10
      0x78160002,  //  001B  JMPF	R5	#001F
      0x1C140801,  //  001C  EQ	R5	R4	R1
      0x78160000,  //  001D  JMPF	R5	#001F
      0x80040600,  //  001E  RET	1	R3
      0x00080521,  //  001F  ADD	R2	R2	K33
      0x7001FFE9,  //  0020  JMP		#000B
      0x4C0C0000,  //  0021  LDNIL	R3
      0x80040600,  //  0022  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: remove_fabric
********************************************************************/
be_local_closure(class_Matter_Device_remove_fabric,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(remove_fabric),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x4C080000,  //  0000  LDNIL	R2
      0x20080202,  //  0001  NE	R2	R1	R2
      0x780A0019,  //  0002  JMPF	R2	#001D
      0xB80A4A00,  //  0003  GETNGBL	R2	K37
      0x8C0C0327,  //  0004  GETMET	R3	R1	K39
      0x7C0C0200,  //  0005  CALL	R3	1
      0x8C0C0728,  //  0006  GETMET	R3	R3	K40
      0x7C0C0200,  //  0007  CALL	R3	1
      0x8C0C0729,  //  0008  GETMET	R3	R3	K41
      0x7C0C0200,  //  0009  CALL	R3	1
      0x8C0C072A,  //  000A  GETMET	R3	R3	K42
      0x7C0C0200,  //  000B  CALL	R3	1
      0x000E4C03,  //  000C  ADD	R3	K38	R3
      0x58100022,  //  000D  LDCONST	R4	K34
      0x7C080400,  //  000E  CALL	R2	2
      0x88080118,  //  000F  GETMBR	R2	R0	K24
      0x8808052B,  //  0010  GETMBR	R2	R2	K43
      0x8808052C,  //  0011  GETMBR	R2	R2	K44
      0x8C08052D,  //  0012  GETMET	R2	R2	K45
      0x5C100200,  //  0013  MOVE	R4	R1
      0x7C080400,  //  0014  CALL	R2	2
      0x88080112,  //  0015  GETMBR	R2	R0	K18
      0x8C08052E,  //  0016  GETMET	R2	R2	K46
      0x5C100200,  //  0017  MOVE	R4	R1
      0x7C080400,  //  0018  CALL	R2	2
      0x88080115,  //  0019  GETMBR	R2	R0	K21
      0x8C08052F,  //  001A  GETMET	R2	R2	K47
      0x5C100200,  //  001B  MOVE	R4	R1
      0x7C080400,  //  001C  CALL	R2	2
      0x88080115,  //  001D  GETMBR	R2	R0	K21
      0x8C080530,  //  001E  GETMET	R2	R2	K48
      0x7C080200,  //  001F  CALL	R2	1
      0x80000000,  //  0020  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: msg_received
********************************************************************/
be_local_closure(class_Matter_Device_msg_received,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(msg_received),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88100118,  //  0000  GETMBR	R4	R0	K24
      0x8C100931,  //  0001  GETMET	R4	R4	K49
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
** Solidified function: get_plugin_remote_info
********************************************************************/
be_local_closure(class_Matter_Device_get_plugin_remote_info,   /* name */
  be_nested_proto(
    6,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(get_plugin_remote_info),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x8808010D,  //  0000  GETMBR	R2	R0	K13
      0x8C080532,  //  0001  GETMET	R2	R2	K50
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
** Solidified function: button_pressed
********************************************************************/
be_local_closure(class_Matter_Device_button_pressed,   /* name */
  be_nested_proto(
    13,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(button_pressed),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x540E000F,  //  0000  LDINT	R3	16
      0x3C0C0403,  //  0001  SHR	R3	R2	R3
      0x541200FE,  //  0002  LDINT	R4	255
      0x2C0C0604,  //  0003  AND	R3	R3	R4
      0x54120007,  //  0004  LDINT	R4	8
      0x3C100404,  //  0005  SHR	R4	R2	R4
      0x541600FE,  //  0006  LDINT	R5	255
      0x2C100805,  //  0007  AND	R4	R4	R5
      0x541600FE,  //  0008  LDINT	R5	255
      0x2C140405,  //  0009  AND	R5	R2	R5
      0x541A0017,  //  000A  LDINT	R6	24
      0x3C180406,  //  000B  SHR	R6	R2	R6
      0x541E00FE,  //  000C  LDINT	R7	255
      0x2C180C07,  //  000D  AND	R6	R6	R7
      0x8C1C0120,  //  000E  GETMET	R7	R0	K32
      0x00240B21,  //  000F  ADD	R9	R5	K33
      0x20280604,  //  0010  NE	R10	R3	R4
      0x782A0001,  //  0011  JMPF	R10	#0014
      0x58280021,  //  0012  LDCONST	R10	K33
      0x70020000,  //  0013  JMP		#0015
      0x5828000A,  //  0014  LDCONST	R10	K10
      0x780E0001,  //  0015  JMPF	R3	#0018
      0x582C000A,  //  0016  LDCONST	R11	K10
      0x70020000,  //  0017  JMP		#0019
      0x582C0021,  //  0018  LDCONST	R11	K33
      0x5C300C00,  //  0019  MOVE	R12	R6
      0x7C1C0A00,  //  001A  CALL	R7	5
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_commands
********************************************************************/
be_local_closure(class_Matter_Device_register_commands,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
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
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(register_commands),
    &be_const_str_solidified,
    ( &(const binstruction[17]) {  /* code */
      0xB8060200,  //  0000  GETNGBL	R1	K1
      0x8C040333,  //  0001  GETMET	R1	R1	K51
      0x580C0034,  //  0002  LDCONST	R3	K52
      0x84100000,  //  0003  CLOSURE	R4	P0
      0x7C040600,  //  0004  CALL	R1	3
      0xB8060200,  //  0005  GETNGBL	R1	K1
      0x8C040333,  //  0006  GETMET	R1	R1	K51
      0x580C0035,  //  0007  LDCONST	R3	K53
      0x84100001,  //  0008  CLOSURE	R4	P1
      0x7C040600,  //  0009  CALL	R1	3
      0xB8060200,  //  000A  GETNGBL	R1	K1
      0x8C040333,  //  000B  GETMET	R1	R1	K51
      0x580C0036,  //  000C  LDCONST	R3	K54
      0x84100002,  //  000D  CLOSURE	R4	P2
      0x7C040600,  //  000E  CALL	R1	3
      0xA0000000,  //  000F  CLOSE	R0
      0x80000000,  //  0010  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: k2l
********************************************************************/
be_local_closure(class_Matter_Device_k2l,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(k2l),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0x58040037,  //  0000  LDCONST	R1	K55
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0003,  //  0004  EQ	R3	R0	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x80040400,  //  0006  RET	1	R2
      0x600C0010,  //  0007  GETGBL	R3	G16
      0x8C100138,  //  0008  GETMET	R4	R0	K56
      0x7C100200,  //  0009  CALL	R4	1
      0x7C0C0200,  //  000A  CALL	R3	1
      0xA8020005,  //  000B  EXBLK	0	#0012
      0x5C100600,  //  000C  MOVE	R4	R3
      0x7C100000,  //  000D  CALL	R4	0
      0x8C140539,  //  000E  GETMET	R5	R2	K57
      0x5C1C0800,  //  000F  MOVE	R7	R4
      0x7C140400,  //  0010  CALL	R5	2
      0x7001FFF9,  //  0011  JMP		#000C
      0x580C003A,  //  0012  LDCONST	R3	K58
      0xAC0C0200,  //  0013  CATCH	R3	1	0
      0xB0080000,  //  0014  RAISE	2	R0	R0
      0x600C0010,  //  0015  GETGBL	R3	G16
      0x6010000C,  //  0016  GETGBL	R4	G12
      0x5C140400,  //  0017  MOVE	R5	R2
      0x7C100200,  //  0018  CALL	R4	1
      0x04100921,  //  0019  SUB	R4	R4	K33
      0x40124204,  //  001A  CONNECT	R4	K33	R4
      0x7C0C0200,  //  001B  CALL	R3	1
      0xA8020010,  //  001C  EXBLK	0	#002E
      0x5C100600,  //  001D  MOVE	R4	R3
      0x7C100000,  //  001E  CALL	R4	0
      0x94140404,  //  001F  GETIDX	R5	R2	R4
      0x5C180800,  //  0020  MOVE	R6	R4
      0x241C0D0A,  //  0021  GT	R7	R6	K10
      0x781E0008,  //  0022  JMPF	R7	#002C
      0x041C0D21,  //  0023  SUB	R7	R6	K33
      0x941C0407,  //  0024  GETIDX	R7	R2	R7
      0x241C0E05,  //  0025  GT	R7	R7	R5
      0x781E0004,  //  0026  JMPF	R7	#002C
      0x041C0D21,  //  0027  SUB	R7	R6	K33
      0x941C0407,  //  0028  GETIDX	R7	R2	R7
      0x98080C07,  //  0029  SETIDX	R2	R6	R7
      0x04180D21,  //  002A  SUB	R6	R6	K33
      0x7001FFF4,  //  002B  JMP		#0021
      0x98080C05,  //  002C  SETIDX	R2	R6	R5
      0x7001FFEE,  //  002D  JMP		#001D
      0x580C003A,  //  002E  LDCONST	R3	K58
      0xAC0C0200,  //  002F  CATCH	R3	1	0
      0xB0080000,  //  0030  RAISE	2	R0	R0
      0x80040400,  //  0031  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: MtrJoin
********************************************************************/
be_local_closure(class_Matter_Device_MtrJoin,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(MtrJoin),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x60140009,  //  0000  GETGBL	R5	G9
      0x5C180600,  //  0001  MOVE	R6	R3
      0x7C140200,  //  0002  CALL	R5	1
      0x78160003,  //  0003  JMPF	R5	#0008
      0x88180112,  //  0004  GETMBR	R6	R0	K18
      0x8C180D3B,  //  0005  GETMET	R6	R6	K59
      0x7C180200,  //  0006  CALL	R6	1
      0x70020002,  //  0007  JMP		#000B
      0x88180112,  //  0008  GETMBR	R6	R0	K18
      0x8C180D3C,  //  0009  GETMET	R6	R6	K60
      0x7C180200,  //  000A  CALL	R6	1
      0xB81A0200,  //  000B  GETNGBL	R6	K1
      0x8C180D3D,  //  000C  GETMET	R6	R6	K61
      0x7C180200,  //  000D  CALL	R6	1
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: attribute_updated
********************************************************************/
be_local_closure(class_Matter_Device_attribute_updated,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(attribute_updated),
    &be_const_str_solidified,
    ( &(const binstruction[18]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140805,  //  0001  EQ	R5	R4	R5
      0x78160000,  //  0002  JMPF	R5	#0004
      0x50100000,  //  0003  LDBOOL	R4	0	0
      0xB8160600,  //  0004  GETNGBL	R5	K3
      0x8C140B3E,  //  0005  GETMET	R5	R5	K62
      0x7C140200,  //  0006  CALL	R5	1
      0x90167E01,  //  0007  SETMBR	R5	K63	R1
      0x90168002,  //  0008  SETMBR	R5	K64	R2
      0x90168203,  //  0009  SETMBR	R5	K65	R3
      0x88180118,  //  000A  GETMBR	R6	R0	K24
      0x88180D2B,  //  000B  GETMBR	R6	R6	K43
      0x88180D2C,  //  000C  GETMBR	R6	R6	K44
      0x8C180D42,  //  000D  GETMET	R6	R6	K66
      0x5C200A00,  //  000E  MOVE	R8	R5
      0x5C240800,  //  000F  MOVE	R9	R4
      0x7C180600,  //  0010  CALL	R6	3
      0x80000000,  //  0011  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save_param
********************************************************************/
be_local_closure(class_Matter_Device_save_param,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(save_param),
    &be_const_str_solidified,
    ( &(const binstruction[83]) {  /* code */
      0xA4068600,  //  0000  IMPORT	R1	K67
      0x8C080144,  //  0001  GETMET	R2	R0	K68
      0x7C080200,  //  0002  CALL	R2	1
      0x60080018,  //  0003  GETGBL	R2	G24
      0x580C0045,  //  0004  LDCONST	R3	K69
      0x88100146,  //  0005  GETMBR	R4	R0	K70
      0x88140147,  //  0006  GETMBR	R5	R0	K71
      0x88180110,  //  0007  GETMBR	R6	R0	K16
      0x781A0001,  //  0008  JMPF	R6	#000B
      0x58180048,  //  0009  LDCONST	R6	K72
      0x70020000,  //  000A  JMP		#000C
      0x58180049,  //  000B  LDCONST	R6	K73
      0x881C0111,  //  000C  GETMBR	R7	R0	K17
      0x781E0001,  //  000D  JMPF	R7	#0010
      0x581C0048,  //  000E  LDCONST	R7	K72
      0x70020000,  //  000F  JMP		#0011
      0x581C0049,  //  0010  LDCONST	R7	K73
      0x8820010E,  //  0011  GETMBR	R8	R0	K14
      0x7C080C00,  //  0012  CALL	R2	6
      0x880C014A,  //  0013  GETMBR	R3	R0	K74
      0x780E0000,  //  0014  JMPF	R3	#0016
      0x0008054B,  //  0015  ADD	R2	R2	K75
      0x880C010C,  //  0016  GETMBR	R3	R0	K12
      0x780E000E,  //  0017  JMPF	R3	#0027
      0x0008054C,  //  0018  ADD	R2	R2	K76
      0x8C0C034D,  //  0019  GETMET	R3	R1	K77
      0x8814014E,  //  001A  GETMBR	R5	R0	K78
      0x7C0C0400,  //  001B  CALL	R3	2
      0x00080403,  //  001C  ADD	R2	R2	R3
      0x600C000C,  //  001D  GETGBL	R3	G12
      0x8810010D,  //  001E  GETMBR	R4	R0	K13
      0x7C0C0200,  //  001F  CALL	R3	1
      0x240C070A,  //  0020  GT	R3	R3	K10
      0x780E0004,  //  0021  JMPF	R3	#0027
      0x0008054F,  //  0022  ADD	R2	R2	K79
      0x8C0C034D,  //  0023  GETMET	R3	R1	K77
      0x8814010D,  //  0024  GETMBR	R5	R0	K13
      0x7C0C0400,  //  0025  CALL	R3	2
      0x00080403,  //  0026  ADD	R2	R2	R3
      0x00080550,  //  0027  ADD	R2	R2	K80
      0xA8020017,  //  0028  EXBLK	0	#0041
      0x600C0011,  //  0029  GETGBL	R3	G17
      0x88100151,  //  002A  GETMBR	R4	R0	K81
      0x58140052,  //  002B  LDCONST	R5	K82
      0x7C0C0400,  //  002C  CALL	R3	2
      0x8C100753,  //  002D  GETMET	R4	R3	K83
      0x5C180400,  //  002E  MOVE	R6	R2
      0x7C100400,  //  002F  CALL	R4	2
      0x8C100754,  //  0030  GETMET	R4	R3	K84
      0x7C100200,  //  0031  CALL	R4	1
      0xB8124A00,  //  0032  GETNGBL	R4	K37
      0x60140018,  //  0033  GETGBL	R5	G24
      0x58180055,  //  0034  LDCONST	R6	K85
      0x881C010C,  //  0035  GETMBR	R7	R0	K12
      0x781E0001,  //  0036  JMPF	R7	#0039
      0x581C0056,  //  0037  LDCONST	R7	K86
      0x70020000,  //  0038  JMP		#003A
      0x581C0057,  //  0039  LDCONST	R7	K87
      0x7C140400,  //  003A  CALL	R5	2
      0x58180022,  //  003B  LDCONST	R6	K34
      0x7C100400,  //  003C  CALL	R4	2
      0xA8040001,  //  003D  EXBLK	1	1
      0x80040400,  //  003E  RET	1	R2
      0xA8040001,  //  003F  EXBLK	1	1
      0x70020010,  //  0040  JMP		#0052
      0xAC0C0002,  //  0041  CATCH	R3	0	2
      0x7002000D,  //  0042  JMP		#0051
      0xB8164A00,  //  0043  GETNGBL	R5	K37
      0x60180008,  //  0044  GETGBL	R6	G8
      0x5C1C0600,  //  0045  MOVE	R7	R3
      0x7C180200,  //  0046  CALL	R6	1
      0x001AB006,  //  0047  ADD	R6	K88	R6
      0x00180D59,  //  0048  ADD	R6	R6	K89
      0x601C0008,  //  0049  GETGBL	R7	G8
      0x5C200800,  //  004A  MOVE	R8	R4
      0x7C1C0200,  //  004B  CALL	R7	1
      0x00180C07,  //  004C  ADD	R6	R6	R7
      0x581C0022,  //  004D  LDCONST	R7	K34
      0x7C140400,  //  004E  CALL	R5	2
      0x80040400,  //  004F  RET	1	R2
      0x70020000,  //  0050  JMP		#0052
      0xB0080000,  //  0051  RAISE	2	R0	R0
      0x80000000,  //  0052  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: load_param
********************************************************************/
be_local_closure(class_Matter_Device_load_param,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(load_param),
    &be_const_str_solidified,
    ( &(const binstruction[136]) {  /* code */
      0xA4060000,  //  0000  IMPORT	R1	K0
      0x50080000,  //  0001  LDBOOL	R2	0	0
      0xA8020056,  //  0002  EXBLK	0	#005A
      0x600C0011,  //  0003  GETGBL	R3	G17
      0x88100151,  //  0004  GETMBR	R4	R0	K81
      0x7C0C0200,  //  0005  CALL	R3	1
      0x8C10075A,  //  0006  GETMET	R4	R3	K90
      0x7C100200,  //  0007  CALL	R4	1
      0x8C140754,  //  0008  GETMET	R5	R3	K84
      0x7C140200,  //  0009  CALL	R5	1
      0xA4168600,  //  000A  IMPORT	R5	K67
      0x8C180B5B,  //  000B  GETMET	R6	R5	K91
      0x5C200800,  //  000C  MOVE	R8	R4
      0x7C180400,  //  000D  CALL	R6	2
      0x8C1C0D32,  //  000E  GETMET	R7	R6	K50
      0x5824005C,  //  000F  LDCONST	R9	K92
      0x88280146,  //  0010  GETMBR	R10	R0	K70
      0x7C1C0600,  //  0011  CALL	R7	3
      0x90028C07,  //  0012  SETMBR	R0	K70	R7
      0x8C1C0D32,  //  0013  GETMET	R7	R6	K50
      0x5824005D,  //  0014  LDCONST	R9	K93
      0x88280147,  //  0015  GETMBR	R10	R0	K71
      0x7C1C0600,  //  0016  CALL	R7	3
      0x90028E07,  //  0017  SETMBR	R0	K71	R7
      0x601C0017,  //  0018  GETGBL	R7	G23
      0x8C200D32,  //  0019  GETMET	R8	R6	K50
      0x58280010,  //  001A  LDCONST	R10	K16
      0x502C0000,  //  001B  LDBOOL	R11	0	0
      0x7C200600,  //  001C  CALL	R8	3
      0x7C1C0200,  //  001D  CALL	R7	1
      0x90022007,  //  001E  SETMBR	R0	K16	R7
      0x601C0017,  //  001F  GETGBL	R7	G23
      0x8C200D32,  //  0020  GETMET	R8	R6	K50
      0x58280011,  //  0021  LDCONST	R10	K17
      0x502C0000,  //  0022  LDBOOL	R11	0	0
      0x7C200600,  //  0023  CALL	R8	3
      0x7C1C0200,  //  0024  CALL	R7	1
      0x90022207,  //  0025  SETMBR	R0	K17	R7
      0x8C1C0D32,  //  0026  GETMET	R7	R6	K50
      0x5824005E,  //  0027  LDCONST	R9	K94
      0x8828010E,  //  0028  GETMBR	R10	R0	K14
      0x7C1C0600,  //  0029  CALL	R7	3
      0x90021C07,  //  002A  SETMBR	R0	K14	R7
      0x8C1C0D32,  //  002B  GETMET	R7	R6	K50
      0x5824005F,  //  002C  LDCONST	R9	K95
      0x60280013,  //  002D  GETGBL	R10	G19
      0x7C280000,  //  002E  CALL	R10	0
      0x7C1C0600,  //  002F  CALL	R7	3
      0x90029C07,  //  0030  SETMBR	R0	K78	R7
      0x601C0017,  //  0031  GETGBL	R7	G23
      0x8C200D32,  //  0032  GETMET	R8	R6	K50
      0x5828004A,  //  0033  LDCONST	R10	K74
      0x7C200400,  //  0034  CALL	R8	2
      0x7C1C0200,  //  0035  CALL	R7	1
      0x90029407,  //  0036  SETMBR	R0	K74	R7
      0x881C014E,  //  0037  GETMBR	R7	R0	K78
      0x4C200000,  //  0038  LDNIL	R8
      0x201C0E08,  //  0039  NE	R7	R7	R8
      0x781E000D,  //  003A  JMPF	R7	#0049
      0xB81E4A00,  //  003B  GETNGBL	R7	K37
      0x60200018,  //  003C  GETGBL	R8	G24
      0x58240060,  //  003D  LDCONST	R9	K96
      0x8828014E,  //  003E  GETMBR	R10	R0	K78
      0x7C200400,  //  003F  CALL	R8	2
      0x58240061,  //  0040  LDCONST	R9	K97
      0x7C1C0400,  //  0041  CALL	R7	2
      0x8C1C0162,  //  0042  GETMET	R7	R0	K98
      0x7C1C0200,  //  0043  CALL	R7	1
      0x8C1C0163,  //  0044  GETMET	R7	R0	K99
      0x7C1C0200,  //  0045  CALL	R7	1
      0x5C080E00,  //  0046  MOVE	R2	R7
      0x501C0200,  //  0047  LDBOOL	R7	1	0
      0x90021807,  //  0048  SETMBR	R0	K12	R7
      0x8C1C0D32,  //  0049  GETMET	R7	R6	K50
      0x58240064,  //  004A  LDCONST	R9	K100
      0x60280013,  //  004B  GETGBL	R10	G19
      0x7C280000,  //  004C  CALL	R10	0
      0x7C1C0600,  //  004D  CALL	R7	3
      0x90021A07,  //  004E  SETMBR	R0	K13	R7
      0x881C010D,  //  004F  GETMBR	R7	R0	K13
      0x781E0006,  //  0050  JMPF	R7	#0058
      0xB81E4A00,  //  0051  GETNGBL	R7	K37
      0x60200008,  //  0052  GETGBL	R8	G8
      0x8824010D,  //  0053  GETMBR	R9	R0	K13
      0x7C200200,  //  0054  CALL	R8	1
      0x0022CA08,  //  0055  ADD	R8	K101	R8
      0x58240061,  //  0056  LDCONST	R9	K97
      0x7C1C0400,  //  0057  CALL	R7	2
      0xA8040001,  //  0058  EXBLK	1	1
      0x70020011,  //  0059  JMP		#006C
      0xAC0C0002,  //  005A  CATCH	R3	0	2
      0x7002000E,  //  005B  JMP		#006B
      0x20140766,  //  005C  NE	R5	R3	K102
      0x7816000B,  //  005D  JMPF	R5	#006A
      0xB8164A00,  //  005E  GETNGBL	R5	K37
      0x60180008,  //  005F  GETGBL	R6	G8
      0x5C1C0600,  //  0060  MOVE	R7	R3
      0x7C180200,  //  0061  CALL	R6	1
      0x001ACE06,  //  0062  ADD	R6	K103	R6
      0x00180D59,  //  0063  ADD	R6	R6	K89
      0x601C0008,  //  0064  GETGBL	R7	G8
      0x5C200800,  //  0065  MOVE	R8	R4
      0x7C1C0200,  //  0066  CALL	R7	1
      0x00180C07,  //  0067  ADD	R6	R6	R7
      0x581C0022,  //  0068  LDCONST	R7	K34
      0x7C140400,  //  0069  CALL	R5	2
      0x70020000,  //  006A  JMP		#006C
      0xB0080000,  //  006B  RAISE	2	R0	R0
      0x880C0146,  //  006C  GETMBR	R3	R0	K70
      0x4C100000,  //  006D  LDNIL	R4
      0x1C0C0604,  //  006E  EQ	R3	R3	R4
      0x780E000A,  //  006F  JMPF	R3	#007B
      0x8C0C0368,  //  0070  GETMET	R3	R1	K104
      0x58140022,  //  0071  LDCONST	R5	K34
      0x7C0C0400,  //  0072  CALL	R3	2
      0x8C0C0769,  //  0073  GETMET	R3	R3	K105
      0x5814000A,  //  0074  LDCONST	R5	K10
      0x58180022,  //  0075  LDCONST	R6	K34
      0x7C0C0600,  //  0076  CALL	R3	3
      0x54120FFE,  //  0077  LDINT	R4	4095
      0x2C0C0604,  //  0078  AND	R3	R3	R4
      0x90028C03,  //  0079  SETMBR	R0	K70	R3
      0x50080200,  //  007A  LDBOOL	R2	1	0
      0x880C0147,  //  007B  GETMBR	R3	R0	K71
      0x4C100000,  //  007C  LDNIL	R4
      0x1C0C0604,  //  007D  EQ	R3	R3	R4
      0x780E0004,  //  007E  JMPF	R3	#0084
      0x880C0112,  //  007F  GETMBR	R3	R0	K18
      0x8C0C076A,  //  0080  GETMET	R3	R3	K106
      0x7C0C0200,  //  0081  CALL	R3	1
      0x90028E03,  //  0082  SETMBR	R0	K71	R3
      0x50080200,  //  0083  LDBOOL	R2	1	0
      0x780A0001,  //  0084  JMPF	R2	#0087
      0x8C0C016B,  //  0085  GETMET	R3	R0	K107
      0x7C0C0200,  //  0086  CALL	R3	1
      0x80000000,  //  0087  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: register_http_remote
********************************************************************/
be_local_closure(class_Matter_Device_register_http_remote,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(register_http_remote),
    &be_const_str_solidified,
    ( &(const binstruction[42]) {  /* code */
      0x880C016C,  //  0000  GETMBR	R3	R0	K108
      0x4C100000,  //  0001  LDNIL	R4
      0x1C0C0604,  //  0002  EQ	R3	R3	R4
      0x780E0002,  //  0003  JMPF	R3	#0007
      0x600C0013,  //  0004  GETGBL	R3	G19
      0x7C0C0000,  //  0005  CALL	R3	0
      0x9002D803,  //  0006  SETMBR	R0	K108	R3
      0x4C0C0000,  //  0007  LDNIL	R3
      0x8810016C,  //  0008  GETMBR	R4	R0	K108
      0x8C10096D,  //  0009  GETMET	R4	R4	K109
      0x5C180200,  //  000A  MOVE	R6	R1
      0x7C100400,  //  000B  CALL	R4	2
      0x78120009,  //  000C  JMPF	R4	#0017
      0x8810016C,  //  000D  GETMBR	R4	R0	K108
      0x940C0801,  //  000E  GETIDX	R3	R4	R1
      0x8C14076E,  //  000F  GETMET	R5	R3	K110
      0x7C140200,  //  0010  CALL	R5	1
      0x14140405,  //  0011  LT	R5	R2	R5
      0x78160002,  //  0012  JMPF	R5	#0016
      0x8C14076F,  //  0013  GETMET	R5	R3	K111
      0x5C1C0400,  //  0014  MOVE	R7	R2
      0x7C140400,  //  0015  CALL	R5	2
      0x70020011,  //  0016  JMP		#0029
      0xB8120600,  //  0017  GETNGBL	R4	K3
      0x8C100970,  //  0018  GETMET	R4	R4	K112
      0x5C180000,  //  0019  MOVE	R6	R0
      0x5C1C0200,  //  001A  MOVE	R7	R1
      0x5C200400,  //  001B  MOVE	R8	R2
      0x7C100800,  //  001C  CALL	R4	4
      0x5C0C0800,  //  001D  MOVE	R3	R4
      0x8810010D,  //  001E  GETMBR	R4	R0	K13
      0x8C10096D,  //  001F  GETMET	R4	R4	K109
      0x5C180200,  //  0020  MOVE	R6	R1
      0x7C100400,  //  0021  CALL	R4	2
      0x78120003,  //  0022  JMPF	R4	#0027
      0x8C100771,  //  0023  GETMET	R4	R3	K113
      0x8818010D,  //  0024  GETMBR	R6	R0	K13
      0x94180C01,  //  0025  GETIDX	R6	R6	R1
      0x7C100400,  //  0026  CALL	R4	2
      0x8810016C,  //  0027  GETMBR	R4	R0	K108
      0x98100203,  //  0028  SETIDX	R4	R1	R3
      0x80040600,  //  0029  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: sort_distinct
********************************************************************/
be_local_closure(class_Matter_Device_sort_distinct,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(sort_distinct),
    &be_const_str_solidified,
    ( &(const binstruction[53]) {  /* code */
      0x58040037,  //  0000  LDCONST	R1	K55
      0x60080010,  //  0001  GETGBL	R2	G16
      0x600C000C,  //  0002  GETGBL	R3	G12
      0x5C100000,  //  0003  MOVE	R4	R0
      0x7C0C0200,  //  0004  CALL	R3	1
      0x040C0721,  //  0005  SUB	R3	R3	K33
      0x400E4203,  //  0006  CONNECT	R3	K33	R3
      0x7C080200,  //  0007  CALL	R2	1
      0xA8020010,  //  0008  EXBLK	0	#001A
      0x5C0C0400,  //  0009  MOVE	R3	R2
      0x7C0C0000,  //  000A  CALL	R3	0
      0x94100003,  //  000B  GETIDX	R4	R0	R3
      0x5C140600,  //  000C  MOVE	R5	R3
      0x24180B0A,  //  000D  GT	R6	R5	K10
      0x781A0008,  //  000E  JMPF	R6	#0018
      0x04180B21,  //  000F  SUB	R6	R5	K33
      0x94180006,  //  0010  GETIDX	R6	R0	R6
      0x24180C04,  //  0011  GT	R6	R6	R4
      0x781A0004,  //  0012  JMPF	R6	#0018
      0x04180B21,  //  0013  SUB	R6	R5	K33
      0x94180006,  //  0014  GETIDX	R6	R0	R6
      0x98000A06,  //  0015  SETIDX	R0	R5	R6
      0x04140B21,  //  0016  SUB	R5	R5	K33
      0x7001FFF4,  //  0017  JMP		#000D
      0x98000A04,  //  0018  SETIDX	R0	R5	R4
      0x7001FFEE,  //  0019  JMP		#0009
      0x5808003A,  //  001A  LDCONST	R2	K58
      0xAC080200,  //  001B  CATCH	R2	1	0
      0xB0080000,  //  001C  RAISE	2	R0	R0
      0x58080021,  //  001D  LDCONST	R2	K33
      0x600C000C,  //  001E  GETGBL	R3	G12
      0x5C100000,  //  001F  MOVE	R4	R0
      0x7C0C0200,  //  0020  CALL	R3	1
      0x180C0721,  //  0021  LE	R3	R3	K33
      0x780E0000,  //  0022  JMPF	R3	#0024
      0x80040000,  //  0023  RET	1	R0
      0x940C010A,  //  0024  GETIDX	R3	R0	K10
      0x6010000C,  //  0025  GETGBL	R4	G12
      0x5C140000,  //  0026  MOVE	R5	R0
      0x7C100200,  //  0027  CALL	R4	1
      0x14100404,  //  0028  LT	R4	R2	R4
      0x78120009,  //  0029  JMPF	R4	#0034
      0x94100002,  //  002A  GETIDX	R4	R0	R2
      0x1C100803,  //  002B  EQ	R4	R4	R3
      0x78120003,  //  002C  JMPF	R4	#0031
      0x8C100172,  //  002D  GETMET	R4	R0	K114
      0x5C180400,  //  002E  MOVE	R6	R2
      0x7C100400,  //  002F  CALL	R4	2
      0x70020001,  //  0030  JMP		#0033
      0x940C0002,  //  0031  GETIDX	R3	R0	R2
      0x00080521,  //  0032  ADD	R2	R2	K33
      0x7001FFF0,  //  0033  JMP		#0025
      0x80040000,  //  0034  RET	1	R0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: received_ack
********************************************************************/
be_local_closure(class_Matter_Device_received_ack,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(received_ack),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080173,  //  0000  GETMBR	R2	R0	K115
      0x8C080574,  //  0001  GETMET	R2	R2	K116
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_second
********************************************************************/
be_local_closure(class_Matter_Device_every_second,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(every_second),
    &be_const_str_solidified,
    ( &(const binstruction[13]) {  /* code */
      0x88040115,  //  0000  GETMBR	R1	R0	K21
      0x8C040375,  //  0001  GETMET	R1	R1	K117
      0x7C040200,  //  0002  CALL	R1	1
      0x88040118,  //  0003  GETMBR	R1	R0	K24
      0x8C040375,  //  0004  GETMET	R1	R1	K117
      0x7C040200,  //  0005  CALL	R1	1
      0x8804011A,  //  0006  GETMBR	R1	R0	K26
      0x8C040375,  //  0007  GETMET	R1	R1	K117
      0x7C040200,  //  0008  CALL	R1	1
      0x88040112,  //  0009  GETMBR	R1	R0	K18
      0x8C040375,  //  000A  GETMET	R1	R1	K117
      0x7C040200,  //  000B  CALL	R1	1
      0x80000000,  //  000C  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_active_endpoints
********************************************************************/
be_local_closure(class_Matter_Device_get_active_endpoints,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(get_active_endpoints),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x60080012,  //  0000  GETGBL	R2	G18
      0x7C080000,  //  0001  CALL	R2	0
      0x600C0010,  //  0002  GETGBL	R3	G16
      0x8810010B,  //  0003  GETMBR	R4	R0	K11
      0x7C0C0200,  //  0004  CALL	R3	1
      0xA8020011,  //  0005  EXBLK	0	#0018
      0x5C100600,  //  0006  MOVE	R4	R3
      0x7C100000,  //  0007  CALL	R4	0
      0x8C140923,  //  0008  GETMET	R5	R4	K35
      0x7C140200,  //  0009  CALL	R5	1
      0x78060002,  //  000A  JMPF	R1	#000E
      0x1C180B0A,  //  000B  EQ	R6	R5	K10
      0x781A0000,  //  000C  JMPF	R6	#000E
      0x7001FFF7,  //  000D  JMP		#0006
      0x8C180532,  //  000E  GETMET	R6	R2	K50
      0x5C200A00,  //  000F  MOVE	R8	R5
      0x7C180400,  //  0010  CALL	R6	2
      0x4C1C0000,  //  0011  LDNIL	R7
      0x1C180C07,  //  0012  EQ	R6	R6	R7
      0x781A0002,  //  0013  JMPF	R6	#0017
      0x8C180539,  //  0014  GETMET	R6	R2	K57
      0x5C200A00,  //  0015  MOVE	R8	R5
      0x7C180400,  //  0016  CALL	R6	2
      0x7001FFED,  //  0017  JMP		#0006
      0x580C003A,  //  0018  LDCONST	R3	K58
      0xAC0C0200,  //  0019  CATCH	R3	1	0
      0xB0080000,  //  001A  RAISE	2	R0	R0
      0x80040400,  //  001B  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: resolve_attribute_read_solo
********************************************************************/
be_local_closure(class_Matter_Device_resolve_attribute_read_solo,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(resolve_attribute_read_solo),
    &be_const_str_solidified,
    ( &(const binstruction[47]) {  /* code */
      0x8808033F,  //  0000  GETMBR	R2	R1	K63
      0x880C0340,  //  0001  GETMBR	R3	R1	K64
      0x88100341,  //  0002  GETMBR	R4	R1	K65
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
      0x8C140176,  //  000E  GETMET	R5	R0	K118
      0x5C1C0400,  //  000F  MOVE	R7	R2
      0x7C140400,  //  0010  CALL	R5	2
      0x4C180000,  //  0011  LDNIL	R6
      0x1C180A06,  //  0012  EQ	R6	R5	R6
      0x781A0005,  //  0013  JMPF	R6	#001A
      0xB81A0600,  //  0014  GETNGBL	R6	K3
      0x88180D78,  //  0015  GETMBR	R6	R6	K120
      0x9006EE06,  //  0016  SETMBR	R1	K119	R6
      0x4C180000,  //  0017  LDNIL	R6
      0x80040C00,  //  0018  RET	1	R6
      0x70020013,  //  0019  JMP		#002E
      0x8C180B79,  //  001A  GETMET	R6	R5	K121
      0x5C200600,  //  001B  MOVE	R8	R3
      0x7C180400,  //  001C  CALL	R6	2
      0x741A0005,  //  001D  JMPT	R6	#0024
      0xB81A0600,  //  001E  GETNGBL	R6	K3
      0x88180D7A,  //  001F  GETMBR	R6	R6	K122
      0x9006EE06,  //  0020  SETMBR	R1	K119	R6
      0x4C180000,  //  0021  LDNIL	R6
      0x80040C00,  //  0022  RET	1	R6
      0x70020009,  //  0023  JMP		#002E
      0x8C180B7B,  //  0024  GETMET	R6	R5	K123
      0x5C200600,  //  0025  MOVE	R8	R3
      0x5C240800,  //  0026  MOVE	R9	R4
      0x7C180600,  //  0027  CALL	R6	3
      0x741A0004,  //  0028  JMPT	R6	#002E
      0xB81A0600,  //  0029  GETNGBL	R6	K3
      0x88180D7C,  //  002A  GETMBR	R6	R6	K124
      0x9006EE06,  //  002B  SETMBR	R1	K119	R6
      0x4C180000,  //  002C  LDNIL	R6
      0x80040C00,  //  002D  RET	1	R6
      0x80040A00,  //  002E  RET	1	R5
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: clean_remotes
********************************************************************/
be_local_closure(class_Matter_Device_clean_remotes,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(clean_remotes),
    &be_const_str_solidified,
    ( &(const binstruction[80]) {  /* code */
      0xA406FA00,  //  0000  IMPORT	R1	K125
      0x8808016C,  //  0001  GETMBR	R2	R0	K108
      0x780A004B,  //  0002  JMPF	R2	#004F
      0x60080013,  //  0003  GETGBL	R2	G19
      0x7C080000,  //  0004  CALL	R2	0
      0x600C0010,  //  0005  GETGBL	R3	G16
      0x8810016C,  //  0006  GETMBR	R4	R0	K108
      0x7C0C0200,  //  0007  CALL	R3	1
      0xA8020003,  //  0008  EXBLK	0	#000D
      0x5C100600,  //  0009  MOVE	R4	R3
      0x7C100000,  //  000A  CALL	R4	0
      0x9808090A,  //  000B  SETIDX	R2	R4	K10
      0x7001FFFB,  //  000C  JMP		#0009
      0x580C003A,  //  000D  LDCONST	R3	K58
      0xAC0C0200,  //  000E  CATCH	R3	1	0
      0xB0080000,  //  000F  RAISE	2	R0	R0
      0x600C0010,  //  0010  GETGBL	R3	G16
      0x8810010B,  //  0011  GETMBR	R4	R0	K11
      0x7C0C0200,  //  0012  CALL	R3	1
      0xA802000F,  //  0013  EXBLK	0	#0024
      0x5C100600,  //  0014  MOVE	R4	R3
      0x7C100000,  //  0015  CALL	R4	0
      0x8C140369,  //  0016  GETMET	R5	R1	K105
      0x5C1C0800,  //  0017  MOVE	R7	R4
      0x5820007E,  //  0018  LDCONST	R8	K126
      0x7C140600,  //  0019  CALL	R5	3
      0x4C180000,  //  001A  LDNIL	R6
      0x20180A06,  //  001B  NE	R6	R5	R6
      0x781A0005,  //  001C  JMPF	R6	#0023
      0x8C180532,  //  001D  GETMET	R6	R2	K50
      0x5C200A00,  //  001E  MOVE	R8	R5
      0x5824000A,  //  001F  LDCONST	R9	K10
      0x7C180600,  //  0020  CALL	R6	3
      0x00180D21,  //  0021  ADD	R6	R6	K33
      0x98080A06,  //  0022  SETIDX	R2	R5	R6
      0x7001FFEF,  //  0023  JMP		#0014
      0x580C003A,  //  0024  LDCONST	R3	K58
      0xAC0C0200,  //  0025  CATCH	R3	1	0
      0xB0080000,  //  0026  RAISE	2	R0	R0
      0x600C0012,  //  0027  GETGBL	R3	G18
      0x7C0C0000,  //  0028  CALL	R3	0
      0x60100010,  //  0029  GETGBL	R4	G16
      0x8C140538,  //  002A  GETMET	R5	R2	K56
      0x7C140200,  //  002B  CALL	R5	1
      0x7C100200,  //  002C  CALL	R4	1
      0xA8020008,  //  002D  EXBLK	0	#0037
      0x5C140800,  //  002E  MOVE	R5	R4
      0x7C140000,  //  002F  CALL	R5	0
      0x94180405,  //  0030  GETIDX	R6	R2	R5
      0x1C180D0A,  //  0031  EQ	R6	R6	K10
      0x781A0002,  //  0032  JMPF	R6	#0036
      0x8C180739,  //  0033  GETMET	R6	R3	K57
      0x5C200A00,  //  0034  MOVE	R8	R5
      0x7C180400,  //  0035  CALL	R6	2
      0x7001FFF6,  //  0036  JMP		#002E
      0x5810003A,  //  0037  LDCONST	R4	K58
      0xAC100200,  //  0038  CATCH	R4	1	0
      0xB0080000,  //  0039  RAISE	2	R0	R0
      0x60100010,  //  003A  GETGBL	R4	G16
      0x5C140600,  //  003B  MOVE	R5	R3
      0x7C100200,  //  003C  CALL	R4	1
      0xA802000D,  //  003D  EXBLK	0	#004C
      0x5C140800,  //  003E  MOVE	R5	R4
      0x7C140000,  //  003F  CALL	R5	0
      0xB81A4A00,  //  0040  GETNGBL	R6	K37
      0x881C0B80,  //  0041  GETMBR	R7	R5	K128
      0x001EFE07,  //  0042  ADD	R7	K127	R7
      0x58200061,  //  0043  LDCONST	R8	K97
      0x7C180400,  //  0044  CALL	R6	2
      0x8C180B54,  //  0045  GETMET	R6	R5	K84
      0x7C180200,  //  0046  CALL	R6	1
      0x8818016C,  //  0047  GETMBR	R6	R0	K108
      0x8C180D72,  //  0048  GETMET	R6	R6	K114
      0x88200B80,  //  0049  GETMBR	R8	R5	K128
      0x7C180400,  //  004A  CALL	R6	2
      0x7001FFF1,  //  004B  JMP		#003E
      0x5810003A,  //  004C  LDCONST	R4	K58
      0xAC100200,  //  004D  CATCH	R4	1	0
      0xB0080000,  //  004E  RAISE	2	R0	R0
      0x80000000,  //  004F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: start
********************************************************************/
be_local_closure(class_Matter_Device_start,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(start),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88040108,  //  0000  GETMBR	R1	R0	K8
      0x78060000,  //  0001  JMPF	R1	#0003
      0x80000200,  //  0002  RET	0
      0x8C040181,  //  0003  GETMET	R1	R0	K129
      0x7C040200,  //  0004  CALL	R1	1
      0x8C040182,  //  0005  GETMET	R1	R0	K130
      0x880C0183,  //  0006  GETMBR	R3	R0	K131
      0x7C040400,  //  0007  CALL	R1	2
      0x88040112,  //  0008  GETMBR	R1	R0	K18
      0x8C040384,  //  0009  GETMET	R1	R1	K132
      0x7C040200,  //  000A  CALL	R1	1
      0x50040200,  //  000B  LDBOOL	R1	1	0
      0x90021001,  //  000C  SETMBR	R0	K8	R1
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: msg_send
********************************************************************/
be_local_closure(class_Matter_Device_msg_send,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(msg_send),
    &be_const_str_solidified,
    ( &(const binstruction[ 5]) {  /* code */
      0x88080173,  //  0000  GETMBR	R2	R0	K115
      0x8C080585,  //  0001  GETMET	R2	R2	K133
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x80040400,  //  0004  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: button_handler
********************************************************************/
be_local_closure(class_Matter_Device_button_handler,   /* name */
  be_nested_proto(
    14,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(button_handler),
    &be_const_str_solidified,
    ( &(const binstruction[25]) {  /* code */
      0x5814000A,  //  0000  LDCONST	R5	K10
      0xA41AFA00,  //  0001  IMPORT	R6	K125
      0x601C000C,  //  0002  GETGBL	R7	G12
      0x8820010B,  //  0003  GETMBR	R8	R0	K11
      0x7C1C0200,  //  0004  CALL	R7	1
      0x141C0A07,  //  0005  LT	R7	R5	R7
      0x781E0010,  //  0006  JMPF	R7	#0018
      0x881C010B,  //  0007  GETMBR	R7	R0	K11
      0x941C0E05,  //  0008  GETIDX	R7	R7	R5
      0x8C200D6D,  //  0009  GETMET	R8	R6	K109
      0x5C280E00,  //  000A  MOVE	R10	R7
      0x582C0020,  //  000B  LDCONST	R11	K32
      0x7C200600,  //  000C  CALL	R8	3
      0x78220007,  //  000D  JMPF	R8	#0016
      0x8820010B,  //  000E  GETMBR	R8	R0	K11
      0x94201005,  //  000F  GETIDX	R8	R8	R5
      0x8C201120,  //  0010  GETMET	R8	R8	K32
      0x5C280200,  //  0011  MOVE	R10	R1
      0x5C2C0400,  //  0012  MOVE	R11	R2
      0x5C300600,  //  0013  MOVE	R12	R3
      0x5C340800,  //  0014  MOVE	R13	R4
      0x7C200A00,  //  0015  CALL	R8	5
      0x00140B21,  //  0016  ADD	R5	R5	K33
      0x7001FFE9,  //  0017  JMP		#0002
      0x80000000,  //  0018  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: add_read_sensors_schedule
********************************************************************/
be_local_closure(class_Matter_Device_add_read_sensors_schedule,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(add_read_sensors_schedule),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x88080186,  //  0000  GETMBR	R2	R0	K134
      0x4C0C0000,  //  0001  LDNIL	R3
      0x1C080403,  //  0002  EQ	R2	R2	R3
      0x740A0002,  //  0003  JMPT	R2	#0007
      0x88080186,  //  0004  GETMBR	R2	R0	K134
      0x24080401,  //  0005  GT	R2	R2	R1
      0x780A0005,  //  0006  JMPF	R2	#000D
      0x90030C01,  //  0007  SETMBR	R0	K134	R1
      0xB80A0600,  //  0008  GETNGBL	R2	K3
      0x8C080588,  //  0009  GETMET	R2	R2	K136
      0x5C100200,  //  000A  MOVE	R4	R1
      0x7C080400,  //  000B  CALL	R2	2
      0x90030E02,  //  000C  SETMBR	R0	K135	R2
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: stop
********************************************************************/
be_local_closure(class_Matter_Device_stop,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(stop),
    &be_const_str_solidified,
    ( &(const binstruction[10]) {  /* code */
      0xB8060200,  //  0000  GETNGBL	R1	K1
      0x8C040389,  //  0001  GETMET	R1	R1	K137
      0x5C0C0000,  //  0002  MOVE	R3	R0
      0x7C040400,  //  0003  CALL	R1	2
      0x88040173,  //  0004  GETMBR	R1	R0	K115
      0x78060002,  //  0005  JMPF	R1	#0009
      0x88040173,  //  0006  GETMBR	R1	R0	K115
      0x8C04038A,  //  0007  GETMET	R1	R1	K138
      0x7C040200,  //  0008  CALL	R1	1
      0x80000000,  //  0009  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _start_udp
********************************************************************/
be_local_closure(class_Matter_Device__start_udp,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    2,                          /* argc */
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
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(_start_udp),
    &be_const_str_solidified,
    ( &(const binstruction[27]) {  /* code */
      0x88080173,  //  0000  GETMBR	R2	R0	K115
      0x780A0000,  //  0001  JMPF	R2	#0003
      0x80000400,  //  0002  RET	0
      0x4C080000,  //  0003  LDNIL	R2
      0x1C080202,  //  0004  EQ	R2	R1	R2
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x540615A3,  //  0006  LDINT	R1	5540
      0xB80A4A00,  //  0007  GETNGBL	R2	K37
      0x600C0008,  //  0008  GETGBL	R3	G8
      0x5C100200,  //  0009  MOVE	R4	R1
      0x7C0C0200,  //  000A  CALL	R3	1
      0x000F1603,  //  000B  ADD	R3	K139	R3
      0x58100022,  //  000C  LDCONST	R4	K34
      0x7C080400,  //  000D  CALL	R2	2
      0xB80A0600,  //  000E  GETNGBL	R2	K3
      0x8C08058C,  //  000F  GETMET	R2	R2	K140
      0x5C100000,  //  0010  MOVE	R4	R0
      0x58140057,  //  0011  LDCONST	R5	K87
      0x5C180200,  //  0012  MOVE	R6	R1
      0x7C080800,  //  0013  CALL	R2	4
      0x9002E602,  //  0014  SETMBR	R0	K115	R2
      0x88080173,  //  0015  GETMBR	R2	R0	K115
      0x8C08058D,  //  0016  GETMET	R2	R2	K141
      0x84100000,  //  0017  CLOSURE	R4	P0
      0x7C080400,  //  0018  CALL	R2	2
      0xA0000000,  //  0019  CLOSE	R0
      0x80000000,  //  001A  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: MtrInfo
********************************************************************/
be_local_closure(class_Matter_Device_MtrInfo,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(MtrInfo),
    &be_const_str_solidified,
    ( &(const binstruction[40]) {  /* code */
      0x1C140757,  //  0000  EQ	R5	R3	K87
      0x7815FFFF,  //  0001  JMPF	R5	#0002
      0x1C140757,  //  0002  EQ	R5	R3	K87
      0x7816000D,  //  0003  JMPF	R5	#0012
      0x60140010,  //  0004  GETGBL	R5	G16
      0x8818010B,  //  0005  GETMBR	R6	R0	K11
      0x7C140200,  //  0006  CALL	R5	1
      0xA8020005,  //  0007  EXBLK	0	#000E
      0x5C180A00,  //  0008  MOVE	R6	R5
      0x7C180000,  //  0009  CALL	R6	0
      0x8C1C018E,  //  000A  GETMET	R7	R0	K142
      0x88240D3F,  //  000B  GETMBR	R9	R6	K63
      0x7C1C0400,  //  000C  CALL	R7	2
      0x7001FFF9,  //  000D  JMP		#0008
      0x5814003A,  //  000E  LDCONST	R5	K58
      0xAC140200,  //  000F  CATCH	R5	1	0
      0xB0080000,  //  0010  RAISE	2	R0	R0
      0x70020011,  //  0011  JMP		#0024
      0x60140004,  //  0012  GETGBL	R5	G4
      0x5C180800,  //  0013  MOVE	R6	R4
      0x7C140200,  //  0014  CALL	R5	1
      0x1C140B8F,  //  0015  EQ	R5	R5	K143
      0x78160003,  //  0016  JMPF	R5	#001B
      0x8C14018E,  //  0017  GETMET	R5	R0	K142
      0x5C1C0800,  //  0018  MOVE	R7	R4
      0x7C140400,  //  0019  CALL	R5	2
      0x70020008,  //  001A  JMP		#0024
      0x8C140190,  //  001B  GETMET	R5	R0	K144
      0x5C1C0600,  //  001C  MOVE	R7	R3
      0x7C140400,  //  001D  CALL	R5	2
      0x4C180000,  //  001E  LDNIL	R6
      0x20180A06,  //  001F  NE	R6	R5	R6
      0x781A0002,  //  0020  JMPF	R6	#0024
      0x8C18018E,  //  0021  GETMET	R6	R0	K142
      0x88200B3F,  //  0022  GETMBR	R8	R5	K63
      0x7C180400,  //  0023  CALL	R6	2
      0xB8160200,  //  0024  GETNGBL	R5	K1
      0x8C140B3D,  //  0025  GETMET	R5	R5	K61
      0x7C140200,  //  0026  CALL	R5	1
      0x80000000,  //  0027  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: MtrInfo_one
********************************************************************/
be_local_closure(class_Matter_Device_MtrInfo_one,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(MtrInfo_one),
    &be_const_str_solidified,
    ( &(const binstruction[20]) {  /* code */
      0x8C080176,  //  0000  GETMET	R2	R0	K118
      0x5C100200,  //  0001  MOVE	R4	R1
      0x7C080400,  //  0002  CALL	R2	2
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0403,  //  0004  EQ	R3	R2	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x80000600,  //  0006  RET	0
      0x8C0C0591,  //  0007  GETMET	R3	R2	K145
      0x7C0C0200,  //  0008  CALL	R3	1
      0x780E0008,  //  0009  JMPF	R3	#0013
      0x60100018,  //  000A  GETGBL	R4	G24
      0x58140092,  //  000B  LDCONST	R5	K146
      0x5C180600,  //  000C  MOVE	R6	R3
      0x7C100400,  //  000D  CALL	R4	2
      0xB8160200,  //  000E  GETNGBL	R5	K1
      0x8C140B93,  //  000F  GETMET	R5	R5	K147
      0x5C1C0800,  //  0010  MOVE	R7	R4
      0x58200057,  //  0011  LDCONST	R8	K87
      0x7C140600,  //  0012  CALL	R5	3
      0x80000000,  //  0013  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_network
********************************************************************/
be_local_closure(class_Matter_Device_check_network,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(check_network),
    &be_const_str_solidified,
    ( &(const binstruction[16]) {  /* code */
      0x88040108,  //  0000  GETMBR	R1	R0	K8
      0x78060000,  //  0001  JMPF	R1	#0003
      0x80000200,  //  0002  RET	0
      0xB8060200,  //  0003  GETNGBL	R1	K1
      0x8C040394,  //  0004  GETMET	R1	R1	K148
      0x7C040200,  //  0005  CALL	R1	1
      0x94040395,  //  0006  GETIDX	R1	R1	K149
      0x74060004,  //  0007  JMPT	R1	#000D
      0xB8060200,  //  0008  GETNGBL	R1	K1
      0x8C040396,  //  0009  GETMET	R1	R1	K150
      0x7C040200,  //  000A  CALL	R1	1
      0x94040395,  //  000B  GETIDX	R1	R1	K149
      0x78060001,  //  000C  JMPF	R1	#000F
      0x8C04018D,  //  000D  GETMET	R1	R0	K141
      0x7C040200,  //  000E  CALL	R1	1
      0x80000000,  //  000F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: _trigger_read_sensors
********************************************************************/
be_local_closure(class_Matter_Device__trigger_read_sensors,   /* name */
  be_nested_proto(
    8,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(_trigger_read_sensors),
    &be_const_str_solidified,
    ( &(const binstruction[48]) {  /* code */
      0xA4068600,  //  0000  IMPORT	R1	K67
      0xB80A0200,  //  0001  GETNGBL	R2	K1
      0x8C080597,  //  0002  GETMET	R2	R2	K151
      0x7C080200,  //  0003  CALL	R2	1
      0xB80E0200,  //  0004  GETNGBL	R3	K1
      0x8C0C0798,  //  0005  GETMET	R3	R3	K152
      0x58140061,  //  0006  LDCONST	R5	K97
      0x7C0C0400,  //  0007  CALL	R3	2
      0x780E0006,  //  0008  JMPF	R3	#0010
      0xB80E4A00,  //  0009  GETNGBL	R3	K37
      0x60100008,  //  000A  GETGBL	R4	G8
      0x5C140400,  //  000B  MOVE	R5	R2
      0x7C100200,  //  000C  CALL	R4	1
      0x00133204,  //  000D  ADD	R4	K153	R4
      0x58140061,  //  000E  LDCONST	R5	K97
      0x7C0C0400,  //  000F  CALL	R3	2
      0x4C0C0000,  //  0010  LDNIL	R3
      0x1C0C0403,  //  0011  EQ	R3	R2	R3
      0x780E0000,  //  0012  JMPF	R3	#0014
      0x80000600,  //  0013  RET	0
      0x8C0C035B,  //  0014  GETMET	R3	R1	K91
      0x5C140400,  //  0015  MOVE	R5	R2
      0x7C0C0400,  //  0016  CALL	R3	2
      0x4C100000,  //  0017  LDNIL	R4
      0x20100604,  //  0018  NE	R4	R3	R4
      0x7812000D,  //  0019  JMPF	R4	#0028
      0x5810000A,  //  001A  LDCONST	R4	K10
      0x6014000C,  //  001B  GETGBL	R5	G12
      0x8818010B,  //  001C  GETMBR	R6	R0	K11
      0x7C140200,  //  001D  CALL	R5	1
      0x14140805,  //  001E  LT	R5	R4	R5
      0x78160006,  //  001F  JMPF	R5	#0027
      0x8814010B,  //  0020  GETMBR	R5	R0	K11
      0x94140A04,  //  0021  GETIDX	R5	R5	R4
      0x8C140B9A,  //  0022  GETMET	R5	R5	K154
      0x5C1C0600,  //  0023  MOVE	R7	R3
      0x7C140400,  //  0024  CALL	R5	2
      0x00100921,  //  0025  ADD	R4	R4	K33
      0x7001FFF3,  //  0026  JMP		#001B
      0x70020006,  //  0027  JMP		#002F
      0xB8124A00,  //  0028  GETNGBL	R4	K37
      0x60140008,  //  0029  GETGBL	R5	G8
      0x5C180400,  //  002A  MOVE	R6	R2
      0x7C140200,  //  002B  CALL	R5	1
      0x00173605,  //  002C  ADD	R5	K155	R5
      0x58180061,  //  002D  LDCONST	R6	K97
      0x7C100400,  //  002E  CALL	R4	2
      0x80000000,  //  002F  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_plugin_class_arg
********************************************************************/
be_local_closure(class_Matter_Device_get_plugin_class_arg,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(get_plugin_class_arg),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8808019C,  //  0000  GETMBR	R2	R0	K156
      0x8C080532,  //  0001  GETMET	R2	R2	K50
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x880C059D,  //  0005  GETMBR	R3	R2	K157
      0x70020000,  //  0006  JMP		#0008
      0x580C0057,  //  0007  LDCONST	R3	K87
      0x80040600,  //  0008  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: bridge_add_endpoint
********************************************************************/
be_local_closure(class_Matter_Device_bridge_add_endpoint,   /* name */
  be_nested_proto(
    16,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(bridge_add_endpoint),
    &be_const_str_solidified,
    ( &(const binstruction[68]) {  /* code */
      0x880C019C,  //  0000  GETMBR	R3	R0	K156
      0x8C0C0732,  //  0001  GETMET	R3	R3	K50
      0x5C140200,  //  0002  MOVE	R5	R1
      0x7C0C0400,  //  0003  CALL	R3	2
      0x4C100000,  //  0004  LDNIL	R4
      0x1C100604,  //  0005  EQ	R4	R3	R4
      0x78120008,  //  0006  JMPF	R4	#0010
      0xB8124A00,  //  0007  GETNGBL	R4	K37
      0x60140008,  //  0008  GETGBL	R5	G8
      0x5C180200,  //  0009  MOVE	R6	R1
      0x7C140200,  //  000A  CALL	R5	1
      0x00173C05,  //  000B  ADD	R5	K158	R5
      0x00140B9F,  //  000C  ADD	R5	R5	K159
      0x58180061,  //  000D  LDCONST	R6	K97
      0x7C100400,  //  000E  CALL	R4	2
      0x80000800,  //  000F  RET	0
      0x8810010E,  //  0010  GETMBR	R4	R0	K14
      0x60140008,  //  0011  GETGBL	R5	G8
      0x5C180800,  //  0012  MOVE	R6	R4
      0x7C140200,  //  0013  CALL	R5	1
      0x5C180600,  //  0014  MOVE	R6	R3
      0x5C1C0000,  //  0015  MOVE	R7	R0
      0x5C200800,  //  0016  MOVE	R8	R4
      0x5C240400,  //  0017  MOVE	R9	R2
      0x7C180600,  //  0018  CALL	R6	3
      0x881C010B,  //  0019  GETMBR	R7	R0	K11
      0x8C1C0F39,  //  001A  GETMET	R7	R7	K57
      0x5C240C00,  //  001B  MOVE	R9	R6
      0x7C1C0400,  //  001C  CALL	R7	2
      0x601C0013,  //  001D  GETGBL	R7	G19
      0x7C1C0000,  //  001E  CALL	R7	0
      0x981F4001,  //  001F  SETIDX	R7	K160	R1
      0x60200010,  //  0020  GETGBL	R8	G16
      0x8C240538,  //  0021  GETMET	R9	R2	K56
      0x7C240200,  //  0022  CALL	R9	1
      0x7C200200,  //  0023  CALL	R8	1
      0xA8020004,  //  0024  EXBLK	0	#002A
      0x5C241000,  //  0025  MOVE	R9	R8
      0x7C240000,  //  0026  CALL	R9	0
      0x94280409,  //  0027  GETIDX	R10	R2	R9
      0x981C120A,  //  0028  SETIDX	R7	R9	R10
      0x7001FFFA,  //  0029  JMP		#0025
      0x5820003A,  //  002A  LDCONST	R8	K58
      0xAC200200,  //  002B  CATCH	R8	1	0
      0xB0080000,  //  002C  RAISE	2	R0	R0
      0xB8224A00,  //  002D  GETNGBL	R8	K37
      0x60240018,  //  002E  GETGBL	R9	G24
      0x582800A1,  //  002F  LDCONST	R10	K161
      0x5C2C0800,  //  0030  MOVE	R11	R4
      0x5C300200,  //  0031  MOVE	R12	R1
      0x8C3401A2,  //  0032  GETMET	R13	R0	K162
      0x5C3C0400,  //  0033  MOVE	R15	R2
      0x7C340400,  //  0034  CALL	R13	2
      0x7C240800,  //  0035  CALL	R9	4
      0x58280022,  //  0036  LDCONST	R10	K34
      0x7C200400,  //  0037  CALL	R8	2
      0x8820014E,  //  0038  GETMBR	R8	R0	K78
      0x98200A07,  //  0039  SETIDX	R8	R5	R7
      0x50200200,  //  003A  LDBOOL	R8	1	0
      0x90021808,  //  003B  SETMBR	R0	K12	R8
      0x8820010E,  //  003C  GETMBR	R8	R0	K14
      0x00201121,  //  003D  ADD	R8	R8	K33
      0x90021C08,  //  003E  SETMBR	R0	K14	R8
      0x8C20016B,  //  003F  GETMET	R8	R0	K107
      0x7C200200,  //  0040  CALL	R8	1
      0x8C2001A3,  //  0041  GETMET	R8	R0	K163
      0x7C200200,  //  0042  CALL	R8	1
      0x80040800,  //  0043  RET	1	R4
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: signal_endpoints_changed
********************************************************************/
be_local_closure(class_Matter_Device_signal_endpoints_changed,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(signal_endpoints_changed),
    &be_const_str_solidified,
    ( &(const binstruction[14]) {  /* code */
      0x8C0401A4,  //  0000  GETMET	R1	R0	K164
      0x580C000A,  //  0001  LDCONST	R3	K10
      0x5412001C,  //  0002  LDINT	R4	29
      0x58140061,  //  0003  LDCONST	R5	K97
      0x50180000,  //  0004  LDBOOL	R6	0	0
      0x7C040A00,  //  0005  CALL	R1	5
      0x8C0401A4,  //  0006  GETMET	R1	R0	K164
      0xB80E0600,  //  0007  GETNGBL	R3	K3
      0x880C07A5,  //  0008  GETMBR	R3	R3	K165
      0x5412001C,  //  0009  LDINT	R4	29
      0x58140061,  //  000A  LDCONST	R5	K97
      0x50180000,  //  000B  LDBOOL	R6	0	0
      0x7C040A00,  //  000C  CALL	R1	5
      0x80000000,  //  000D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_50ms
********************************************************************/
be_local_closure(class_Matter_Device_every_50ms,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(every_50ms),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8C0401A6,  //  0000  GETMET	R1	R0	K166
      0x7C040200,  //  0001  CALL	R1	1
      0x88040109,  //  0002  GETMBR	R1	R0	K9
      0x00040321,  //  0003  ADD	R1	R1	K33
      0x90021201,  //  0004  SETMBR	R0	K9	R1
      0x88040118,  //  0005  GETMBR	R1	R0	K24
      0x8C0403A7,  //  0006  GETMET	R1	R1	K167
      0x7C040200,  //  0007  CALL	R1	1
      0x80000000,  //  0008  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: reset_param
********************************************************************/
be_local_closure(class_Matter_Device_reset_param,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(reset_param),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x50040000,  //  0000  LDBOOL	R1	0	0
      0x90021801,  //  0001  SETMBR	R0	K12	R1
      0x8804010F,  //  0002  GETMBR	R1	R0	K15
      0x90021C01,  //  0003  SETMBR	R0	K14	R1
      0x8C04016B,  //  0004  GETMET	R1	R0	K107
      0x7C040200,  //  0005  CALL	R1	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: conf_to_log
********************************************************************/
be_local_closure(class_Matter_Device_conf_to_log,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(conf_to_log),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x58040037,  //  0000  LDCONST	R1	K55
      0x58080057,  //  0001  LDCONST	R2	K87
      0x600C0010,  //  0002  GETGBL	R3	G16
      0x8C1003A8,  //  0003  GETMET	R4	R1	K168
      0x5C180000,  //  0004  MOVE	R6	R0
      0x7C100400,  //  0005  CALL	R4	2
      0x7C0C0200,  //  0006  CALL	R3	1
      0xA802000B,  //  0007  EXBLK	0	#0014
      0x5C100600,  //  0008  MOVE	R4	R3
      0x7C100000,  //  0009  CALL	R4	0
      0x1C1409A0,  //  000A  EQ	R5	R4	K160
      0x78160000,  //  000B  JMPF	R5	#000D
      0x7001FFFA,  //  000C  JMP		#0008
      0x60140018,  //  000D  GETGBL	R5	G24
      0x581800A9,  //  000E  LDCONST	R6	K169
      0x5C1C0800,  //  000F  MOVE	R7	R4
      0x94200004,  //  0010  GETIDX	R8	R0	R4
      0x7C140600,  //  0011  CALL	R5	3
      0x00080405,  //  0012  ADD	R2	R2	R5
      0x7001FFF3,  //  0013  JMP		#0008
      0x580C003A,  //  0014  LDCONST	R3	K58
      0xAC0C0200,  //  0015  CATCH	R3	1	0
      0xB0080000,  //  0016  RAISE	2	R0	R0
      0x80040400,  //  0017  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: k2l_num
********************************************************************/
be_local_closure(class_Matter_Device_k2l_num,   /* name */
  be_nested_proto(
    9,                          /* nstack */
    1,                          /* argc */
    12,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(k2l_num),
    &be_const_str_solidified,
    ( &(const binstruction[52]) {  /* code */
      0x58040037,  //  0000  LDCONST	R1	K55
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x4C0C0000,  //  0003  LDNIL	R3
      0x1C0C0003,  //  0004  EQ	R3	R0	R3
      0x780E0000,  //  0005  JMPF	R3	#0007
      0x80040400,  //  0006  RET	1	R2
      0x600C0010,  //  0007  GETGBL	R3	G16
      0x8C100138,  //  0008  GETMET	R4	R0	K56
      0x7C100200,  //  0009  CALL	R4	1
      0x7C0C0200,  //  000A  CALL	R3	1
      0xA8020007,  //  000B  EXBLK	0	#0014
      0x5C100600,  //  000C  MOVE	R4	R3
      0x7C100000,  //  000D  CALL	R4	0
      0x8C140539,  //  000E  GETMET	R5	R2	K57
      0x601C0009,  //  000F  GETGBL	R7	G9
      0x5C200800,  //  0010  MOVE	R8	R4
      0x7C1C0200,  //  0011  CALL	R7	1
      0x7C140400,  //  0012  CALL	R5	2
      0x7001FFF7,  //  0013  JMP		#000C
      0x580C003A,  //  0014  LDCONST	R3	K58
      0xAC0C0200,  //  0015  CATCH	R3	1	0
      0xB0080000,  //  0016  RAISE	2	R0	R0
      0x600C0010,  //  0017  GETGBL	R3	G16
      0x6010000C,  //  0018  GETGBL	R4	G12
      0x5C140400,  //  0019  MOVE	R5	R2
      0x7C100200,  //  001A  CALL	R4	1
      0x04100921,  //  001B  SUB	R4	R4	K33
      0x40124204,  //  001C  CONNECT	R4	K33	R4
      0x7C0C0200,  //  001D  CALL	R3	1
      0xA8020010,  //  001E  EXBLK	0	#0030
      0x5C100600,  //  001F  MOVE	R4	R3
      0x7C100000,  //  0020  CALL	R4	0
      0x94140404,  //  0021  GETIDX	R5	R2	R4
      0x5C180800,  //  0022  MOVE	R6	R4
      0x241C0D0A,  //  0023  GT	R7	R6	K10
      0x781E0008,  //  0024  JMPF	R7	#002E
      0x041C0D21,  //  0025  SUB	R7	R6	K33
      0x941C0407,  //  0026  GETIDX	R7	R2	R7
      0x241C0E05,  //  0027  GT	R7	R7	R5
      0x781E0004,  //  0028  JMPF	R7	#002E
      0x041C0D21,  //  0029  SUB	R7	R6	K33
      0x941C0407,  //  002A  GETIDX	R7	R2	R7
      0x98080C07,  //  002B  SETIDX	R2	R6	R7
      0x04180D21,  //  002C  SUB	R6	R6	K33
      0x7001FFF4,  //  002D  JMP		#0023
      0x98080C05,  //  002E  SETIDX	R2	R6	R5
      0x7001FFEE,  //  002F  JMP		#001F
      0x580C003A,  //  0030  LDCONST	R3	K58
      0xAC0C0200,  //  0031  CATCH	R3	1	0
      0xB0080000,  //  0032  RAISE	2	R0	R0
      0x80040400,  //  0033  RET	1	R2
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: MtrUpdate
********************************************************************/
be_local_closure(class_Matter_Device_MtrUpdate,   /* name */
  be_nested_proto(
    18,                          /* nstack */
    5,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(MtrUpdate),
    &be_const_str_solidified,
    ( &(const binstruction[126]) {  /* code */
      0x4C140000,  //  0000  LDNIL	R5
      0x1C140805,  //  0001  EQ	R5	R4	R5
      0x78160004,  //  0002  JMPF	R5	#0008
      0xB8160200,  //  0003  GETNGBL	R5	K1
      0x8C140BAA,  //  0004  GETMET	R5	R5	K170
      0x581C00AB,  //  0005  LDCONST	R7	K171
      0x7C140400,  //  0006  CALL	R5	2
      0x80040A00,  //  0007  RET	1	R5
      0xB8160200,  //  0008  GETNGBL	R5	K1
      0x8C140BAC,  //  0009  GETMET	R5	R5	K172
      0x5C1C0800,  //  000A  MOVE	R7	R4
      0x582000AD,  //  000B  LDCONST	R8	K173
      0x7C140600,  //  000C  CALL	R5	3
      0xB81A0200,  //  000D  GETNGBL	R6	K1
      0x8C180DAC,  //  000E  GETMET	R6	R6	K172
      0x5C200800,  //  000F  MOVE	R8	R4
      0x582400AE,  //  0010  LDCONST	R9	K174
      0x7C180600,  //  0011  CALL	R6	3
      0x74160000,  //  0012  JMPT	R5	#0014
      0x781A0064,  //  0013  JMPF	R6	#0079
      0x4C1C0000,  //  0014  LDNIL	R7
      0x78160010,  //  0015  JMPF	R5	#0027
      0x60200009,  //  0016  GETGBL	R8	G9
      0x94240805,  //  0017  GETIDX	R9	R4	R5
      0x7C200200,  //  0018  CALL	R8	1
      0x1824110A,  //  0019  LE	R9	R8	K10
      0x78260004,  //  001A  JMPF	R9	#0020
      0xB8260200,  //  001B  GETNGBL	R9	K1
      0x8C2413AA,  //  001C  GETMET	R9	R9	K170
      0x582C00AF,  //  001D  LDCONST	R11	K175
      0x7C240400,  //  001E  CALL	R9	2
      0x80041200,  //  001F  RET	1	R9
      0x8C240176,  //  0020  GETMET	R9	R0	K118
      0x5C2C1000,  //  0021  MOVE	R11	R8
      0x7C240400,  //  0022  CALL	R9	2
      0x5C1C1200,  //  0023  MOVE	R7	R9
      0x8C240972,  //  0024  GETMET	R9	R4	K114
      0x5C2C0A00,  //  0025  MOVE	R11	R5
      0x7C240400,  //  0026  CALL	R9	2
      0x781A0009,  //  0027  JMPF	R6	#0032
      0x4C200000,  //  0028  LDNIL	R8
      0x1C200E08,  //  0029  EQ	R8	R7	R8
      0x78220003,  //  002A  JMPF	R8	#002F
      0x8C200190,  //  002B  GETMET	R8	R0	K144
      0x94280806,  //  002C  GETIDX	R10	R4	R6
      0x7C200400,  //  002D  CALL	R8	2
      0x5C1C1000,  //  002E  MOVE	R7	R8
      0x8C200972,  //  002F  GETMET	R8	R4	K114
      0x5C280C00,  //  0030  MOVE	R10	R6
      0x7C200400,  //  0031  CALL	R8	2
      0x4C200000,  //  0032  LDNIL	R8
      0x1C200E08,  //  0033  EQ	R8	R7	R8
      0x78220004,  //  0034  JMPF	R8	#003A
      0xB8220200,  //  0035  GETNGBL	R8	K1
      0x8C2011AA,  //  0036  GETMET	R8	R8	K170
      0x582800B0,  //  0037  LDCONST	R10	K176
      0x7C200400,  //  0038  CALL	R8	2
      0x80041000,  //  0039  RET	1	R8
      0x88200FB1,  //  003A  GETMBR	R8	R7	K177
      0x74220004,  //  003B  JMPT	R8	#0041
      0xB8220200,  //  003C  GETNGBL	R8	K1
      0x8C2011AA,  //  003D  GETMET	R8	R8	K170
      0x582800B2,  //  003E  LDCONST	R10	K178
      0x7C200400,  //  003F  CALL	R8	2
      0x80041000,  //  0040  RET	1	R8
      0x8C200FB3,  //  0041  GETMET	R8	R7	K179
      0x7C200200,  //  0042  CALL	R8	1
      0x60240013,  //  0043  GETGBL	R9	G19
      0x7C240000,  //  0044  CALL	R9	0
      0x60280010,  //  0045  GETGBL	R10	G16
      0x8C2C0938,  //  0046  GETMET	R11	R4	K56
      0x7C2C0200,  //  0047  CALL	R11	1
      0x7C280200,  //  0048  CALL	R10	1
      0xA8020016,  //  0049  EXBLK	0	#0061
      0x5C2C1400,  //  004A  MOVE	R11	R10
      0x7C2C0000,  //  004B  CALL	R11	0
      0xB8320200,  //  004C  GETNGBL	R12	K1
      0x8C3019B4,  //  004D  GETMET	R12	R12	K180
      0x5C381000,  //  004E  MOVE	R14	R8
      0x5C3C1600,  //  004F  MOVE	R15	R11
      0x7C300600,  //  0050  CALL	R12	3
      0x4C340000,  //  0051  LDNIL	R13
      0x1C34180D,  //  0052  EQ	R13	R12	R13
      0x78360008,  //  0053  JMPF	R13	#005D
      0xB8360200,  //  0054  GETNGBL	R13	K1
      0x8C341BAA,  //  0055  GETMET	R13	R13	K170
      0x603C0018,  //  0056  GETGBL	R15	G24
      0x584000B5,  //  0057  LDCONST	R16	K181
      0x5C441600,  //  0058  MOVE	R17	R11
      0x7C3C0400,  //  0059  CALL	R15	2
      0x7C340400,  //  005A  CALL	R13	2
      0xA8040001,  //  005B  EXBLK	1	1
      0x80001A00,  //  005C  RET	0
      0x9434100C,  //  005D  GETIDX	R13	R8	R12
      0x9438080B,  //  005E  GETIDX	R14	R4	R11
      0x98241A0E,  //  005F  SETIDX	R9	R13	R14
      0x7001FFE8,  //  0060  JMP		#004A
      0x5828003A,  //  0061  LDCONST	R10	K58
      0xAC280200,  //  0062  CATCH	R10	1	0
      0xB0080000,  //  0063  RAISE	2	R0	R0
      0x8C280FB6,  //  0064  GETMET	R10	R7	K182
      0x5C301200,  //  0065  MOVE	R12	R9
      0x7C280400,  //  0066  CALL	R10	2
      0x8C280F91,  //  0067  GETMET	R10	R7	K145
      0x7C280200,  //  0068  CALL	R10	1
      0x782A000A,  //  0069  JMPF	R10	#0075
      0x602C0018,  //  006A  GETGBL	R11	G24
      0x583000B7,  //  006B  LDCONST	R12	K183
      0x5C340200,  //  006C  MOVE	R13	R1
      0x5C381400,  //  006D  MOVE	R14	R10
      0x7C2C0600,  //  006E  CALL	R11	3
      0xB8320200,  //  006F  GETNGBL	R12	K1
      0x8C3019B8,  //  0070  GETMET	R12	R12	K184
      0x5C381600,  //  0071  MOVE	R14	R11
      0x7C300400,  //  0072  CALL	R12	2
      0x80041800,  //  0073  RET	1	R12
      0x70020003,  //  0074  JMP		#0079
      0xB82E0200,  //  0075  GETNGBL	R11	K1
      0x8C2C173D,  //  0076  GETMET	R11	R11	K61
      0x7C2C0200,  //  0077  CALL	R11	1
      0x80041600,  //  0078  RET	1	R11
      0xB81E0200,  //  0079  GETNGBL	R7	K1
      0x8C1C0FAA,  //  007A  GETMET	R7	R7	K170
      0x582400B9,  //  007B  LDCONST	R9	K185
      0x7C1C0400,  //  007C  CALL	R7	2
      0x80000000,  //  007D  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: event_fabrics_saved
********************************************************************/
be_local_closure(class_Matter_Device_event_fabrics_saved,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(event_fabrics_saved),
    &be_const_str_solidified,
    ( &(const binstruction[12]) {  /* code */
      0x88040115,  //  0000  GETMBR	R1	R0	K21
      0x8C0403BA,  //  0001  GETMET	R1	R1	K186
      0x7C040200,  //  0002  CALL	R1	1
      0x2404030A,  //  0003  GT	R1	R1	K10
      0x78060005,  //  0004  JMPF	R1	#000B
      0x8804010C,  //  0005  GETMBR	R1	R0	K12
      0x74060003,  //  0006  JMPT	R1	#000B
      0x50040200,  //  0007  LDBOOL	R1	1	0
      0x90021801,  //  0008  SETMBR	R0	K12	R1
      0x8C04016B,  //  0009  GETMET	R1	R0	K107
      0x7C040200,  //  000A  CALL	R1	1
      0x80000000,  //  000B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: process_attribute_expansion
********************************************************************/
be_local_closure(class_Matter_Device_process_attribute_expansion,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    3,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(process_attribute_expansion),
    &be_const_str_solidified,
    ( &(const binstruction[28]) {  /* code */
      0x880C033F,  //  0000  GETMBR	R3	R1	K63
      0x88100340,  //  0001  GETMBR	R4	R1	K64
      0x88140341,  //  0002  GETMBR	R5	R1	K65
      0xB81A0600,  //  0003  GETNGBL	R6	K3
      0x8C180DBB,  //  0004  GETMET	R6	R6	K187
      0x5C200000,  //  0005  MOVE	R8	R0
      0x7C180400,  //  0006  CALL	R6	2
      0x8C1C0D8D,  //  0007  GETMET	R7	R6	K141
      0x5C240600,  //  0008  MOVE	R9	R3
      0x5C280800,  //  0009  MOVE	R10	R4
      0x5C2C0A00,  //  000A  MOVE	R11	R5
      0x7C1C0800,  //  000B  CALL	R7	4
      0x8C1C0DBC,  //  000C  GETMET	R7	R6	K188
      0x7C1C0200,  //  000D  CALL	R7	1
      0x4C200000,  //  000E  LDNIL	R8
      0x8C240DBD,  //  000F  GETMET	R9	R6	K189
      0x7C240200,  //  0010  CALL	R9	1
      0x5C201200,  //  0011  MOVE	R8	R9
      0x4C280000,  //  0012  LDNIL	R10
      0x2024120A,  //  0013  NE	R9	R9	R10
      0x78260005,  //  0014  JMPF	R9	#001B
      0x5C240400,  //  0015  MOVE	R9	R2
      0x8C280DBE,  //  0016  GETMET	R10	R6	K190
      0x7C280200,  //  0017  CALL	R10	1
      0x5C2C1000,  //  0018  MOVE	R11	R8
      0x7C240400,  //  0019  CALL	R9	2
      0x7001FFF3,  //  001A  JMP		#000F
      0x80000000,  //  001B  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: invoke_request
********************************************************************/
be_local_closure(class_Matter_Device_invoke_request,   /* name */
  be_nested_proto(
    12,                          /* nstack */
    4,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(invoke_request),
    &be_const_str_solidified,
    ( &(const binstruction[24]) {  /* code */
      0x5810000A,  //  0000  LDCONST	R4	K10
      0x8814073F,  //  0001  GETMBR	R5	R3	K63
      0x6018000C,  //  0002  GETGBL	R6	G12
      0x881C010B,  //  0003  GETMBR	R7	R0	K11
      0x7C180200,  //  0004  CALL	R6	1
      0x14180806,  //  0005  LT	R6	R4	R6
      0x781A000C,  //  0006  JMPF	R6	#0014
      0x8818010B,  //  0007  GETMBR	R6	R0	K11
      0x94180C04,  //  0008  GETIDX	R6	R6	R4
      0x881C0D3F,  //  0009  GETMBR	R7	R6	K63
      0x1C1C0E05,  //  000A  EQ	R7	R7	R5
      0x781E0005,  //  000B  JMPF	R7	#0012
      0x8C1C0DBF,  //  000C  GETMET	R7	R6	K191
      0x5C240200,  //  000D  MOVE	R9	R1
      0x5C280400,  //  000E  MOVE	R10	R2
      0x5C2C0600,  //  000F  MOVE	R11	R3
      0x7C1C0800,  //  0010  CALL	R7	4
      0x80040E00,  //  0011  RET	1	R7
      0x00100921,  //  0012  ADD	R4	R4	K33
      0x7001FFED,  //  0013  JMP		#0002
      0xB81A0600,  //  0014  GETNGBL	R6	K3
      0x88180D78,  //  0015  GETMBR	R6	R6	K120
      0x900EEE06,  //  0016  SETMBR	R3	K119	R6
      0x80000000,  //  0017  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: update_remotes_info
********************************************************************/
be_local_closure(class_Matter_Device_update_remotes_info,   /* name */
  be_nested_proto(
    7,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(update_remotes_info),
    &be_const_str_solidified,
    ( &(const binstruction[33]) {  /* code */
      0x60040013,  //  0000  GETGBL	R1	G19
      0x7C040000,  //  0001  CALL	R1	0
      0x8808016C,  //  0002  GETMBR	R2	R0	K108
      0x4C0C0000,  //  0003  LDNIL	R3
      0x20080403,  //  0004  NE	R2	R2	R3
      0x780A0018,  //  0005  JMPF	R2	#001F
      0x60080010,  //  0006  GETGBL	R2	G16
      0x880C016C,  //  0007  GETMBR	R3	R0	K108
      0x8C0C0738,  //  0008  GETMET	R3	R3	K56
      0x7C0C0200,  //  0009  CALL	R3	1
      0x7C080200,  //  000A  CALL	R2	1
      0xA802000F,  //  000B  EXBLK	0	#001C
      0x5C0C0400,  //  000C  MOVE	R3	R2
      0x7C0C0000,  //  000D  CALL	R3	0
      0x8810016C,  //  000E  GETMBR	R4	R0	K108
      0x94100803,  //  000F  GETIDX	R4	R4	R3
      0x8C1009C0,  //  0010  GETMET	R4	R4	K192
      0x7C100200,  //  0011  CALL	R4	1
      0x4C140000,  //  0012  LDNIL	R5
      0x20140805,  //  0013  NE	R5	R4	R5
      0x78160005,  //  0014  JMPF	R5	#001B
      0x6014000C,  //  0015  GETGBL	R5	G12
      0x5C180800,  //  0016  MOVE	R6	R4
      0x7C140200,  //  0017  CALL	R5	1
      0x24140B0A,  //  0018  GT	R5	R5	K10
      0x78160000,  //  0019  JMPF	R5	#001B
      0x98040604,  //  001A  SETIDX	R1	R3	R4
      0x7001FFEF,  //  001B  JMP		#000C
      0x5808003A,  //  001C  LDCONST	R2	K58
      0xAC080200,  //  001D  CATCH	R2	1	0
      0xB0080000,  //  001E  RAISE	2	R0	R0
      0x90021A01,  //  001F  SETMBR	R0	K13	R1
      0x80040200,  //  0020  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: get_plugin_class_displayname
********************************************************************/
be_local_closure(class_Matter_Device_get_plugin_class_displayname,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(get_plugin_class_displayname),
    &be_const_str_solidified,
    ( &(const binstruction[ 9]) {  /* code */
      0x8808019C,  //  0000  GETMBR	R2	R0	K156
      0x8C080532,  //  0001  GETMET	R2	R2	K50
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C080400,  //  0003  CALL	R2	2
      0x780A0001,  //  0004  JMPF	R2	#0007
      0x880C05C1,  //  0005  GETMBR	R3	R2	K193
      0x70020000,  //  0006  JMP		#0008
      0x580C0057,  //  0007  LDCONST	R3	K87
      0x80040600,  //  0008  RET	1	R3
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: bridge_remove_endpoint
********************************************************************/
be_local_closure(class_Matter_Device_bridge_remove_endpoint,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    2,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(bridge_remove_endpoint),
    &be_const_str_solidified,
    ( &(const binstruction[56]) {  /* code */
      0xA40A8600,  //  0000  IMPORT	R2	K67
      0x600C0008,  //  0001  GETGBL	R3	G8
      0x5C100200,  //  0002  MOVE	R4	R1
      0x7C0C0200,  //  0003  CALL	R3	1
      0x4C100000,  //  0004  LDNIL	R4
      0x4C140000,  //  0005  LDNIL	R5
      0x8818014E,  //  0006  GETMBR	R6	R0	K78
      0x8C180D6D,  //  0007  GETMET	R6	R6	K109
      0x5C200600,  //  0008  MOVE	R8	R3
      0x7C180400,  //  0009  CALL	R6	2
      0x741A0004,  //  000A  JMPT	R6	#0010
      0xB81A4A00,  //  000B  GETNGBL	R6	K37
      0x001F8403,  //  000C  ADD	R7	K194	R3
      0x58200061,  //  000D  LDCONST	R8	K97
      0x7C180400,  //  000E  CALL	R6	2
      0x80000C00,  //  000F  RET	0
      0xB81A4A00,  //  0010  GETNGBL	R6	K37
      0x601C0018,  //  0011  GETGBL	R7	G24
      0x582000C3,  //  0012  LDCONST	R8	K195
      0x5C240200,  //  0013  MOVE	R9	R1
      0x7C1C0400,  //  0014  CALL	R7	2
      0x58200022,  //  0015  LDCONST	R8	K34
      0x7C180400,  //  0016  CALL	R6	2
      0x8818014E,  //  0017  GETMBR	R6	R0	K78
      0x8C180D72,  //  0018  GETMET	R6	R6	K114
      0x5C200600,  //  0019  MOVE	R8	R3
      0x7C180400,  //  001A  CALL	R6	2
      0x50180200,  //  001B  LDBOOL	R6	1	0
      0x90021806,  //  001C  SETMBR	R0	K12	R6
      0x5818000A,  //  001D  LDCONST	R6	K10
      0x601C000C,  //  001E  GETGBL	R7	G12
      0x8820010B,  //  001F  GETMBR	R8	R0	K11
      0x7C1C0200,  //  0020  CALL	R7	1
      0x141C0C07,  //  0021  LT	R7	R6	R7
      0x781E000D,  //  0022  JMPF	R7	#0031
      0x881C010B,  //  0023  GETMBR	R7	R0	K11
      0x941C0E06,  //  0024  GETIDX	R7	R7	R6
      0x8C1C0F23,  //  0025  GETMET	R7	R7	K35
      0x7C1C0200,  //  0026  CALL	R7	1
      0x1C1C0207,  //  0027  EQ	R7	R1	R7
      0x781E0005,  //  0028  JMPF	R7	#002F
      0x881C010B,  //  0029  GETMBR	R7	R0	K11
      0x8C1C0F72,  //  002A  GETMET	R7	R7	K114
      0x5C240C00,  //  002B  MOVE	R9	R6
      0x7C1C0400,  //  002C  CALL	R7	2
      0x70020002,  //  002D  JMP		#0031
      0x70020000,  //  002E  JMP		#0030
      0x00180D21,  //  002F  ADD	R6	R6	K33
      0x7001FFEC,  //  0030  JMP		#001E
      0x8C1C01C4,  //  0031  GETMET	R7	R0	K196
      0x7C1C0200,  //  0032  CALL	R7	1
      0x8C1C016B,  //  0033  GETMET	R7	R0	K107
      0x7C1C0200,  //  0034  CALL	R7	1
      0x8C1C01A3,  //  0035  GETMET	R7	R0	K163
      0x7C1C0200,  //  0036  CALL	R7	1
      0x80000000,  //  0037  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: check_config_ep
********************************************************************/
be_local_closure(class_Matter_Device_check_config_ep,   /* name */
  be_nested_proto(
    10,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(check_config_ep),
    &be_const_str_solidified,
    ( &(const binstruction[77]) {  /* code */
      0x50040000,  //  0000  LDBOOL	R1	0	0
      0x60080012,  //  0001  GETGBL	R2	G18
      0x7C080000,  //  0002  CALL	R2	0
      0x600C0010,  //  0003  GETGBL	R3	G16
      0x8810014E,  //  0004  GETMBR	R4	R0	K78
      0x8C100938,  //  0005  GETMET	R4	R4	K56
      0x7C100200,  //  0006  CALL	R4	1
      0x7C0C0200,  //  0007  CALL	R3	1
      0xA8020007,  //  0008  EXBLK	0	#0011
      0x5C100600,  //  0009  MOVE	R4	R3
      0x7C100000,  //  000A  CALL	R4	0
      0x8C140539,  //  000B  GETMET	R5	R2	K57
      0x601C0009,  //  000C  GETGBL	R7	G9
      0x5C200800,  //  000D  MOVE	R8	R4
      0x7C1C0200,  //  000E  CALL	R7	1
      0x7C140400,  //  000F  CALL	R5	2
      0x7001FFF7,  //  0010  JMP		#0009
      0x580C003A,  //  0011  LDCONST	R3	K58
      0xAC0C0200,  //  0012  CATCH	R3	1	0
      0xB0080000,  //  0013  RAISE	2	R0	R0
      0x600C0010,  //  0014  GETGBL	R3	G16
      0x5C100400,  //  0015  MOVE	R4	R2
      0x7C0C0200,  //  0016  CALL	R3	1
      0xA8020030,  //  0017  EXBLK	0	#0049
      0x5C100600,  //  0018  MOVE	R4	R3
      0x7C100000,  //  0019  CALL	R4	0
      0x1C14090A,  //  001A  EQ	R5	R4	K10
      0x7816000B,  //  001B  JMPF	R5	#0028
      0xB8164A00,  //  001C  GETNGBL	R5	K37
      0x581800C5,  //  001D  LDCONST	R6	K197
      0x581C0022,  //  001E  LDCONST	R7	K34
      0x7C140400,  //  001F  CALL	R5	2
      0x8814014E,  //  0020  GETMBR	R5	R0	K78
      0x8C140B72,  //  0021  GETMET	R5	R5	K114
      0x601C0008,  //  0022  GETGBL	R7	G8
      0x5C200800,  //  0023  MOVE	R8	R4
      0x7C1C0200,  //  0024  CALL	R7	1
      0x7C140400,  //  0025  CALL	R5	2
      0x50040200,  //  0026  LDBOOL	R1	1	0
      0x7002001F,  //  0027  JMP		#0048
      0xB8160600,  //  0028  GETNGBL	R5	K3
      0x88140BA5,  //  0029  GETMBR	R5	R5	K165
      0x1C140805,  //  002A  EQ	R5	R4	R5
      0x7816001B,  //  002B  JMPF	R5	#0048
      0x50040200,  //  002C  LDBOOL	R1	1	0
      0xB8164A00,  //  002D  GETNGBL	R5	K37
      0x60180018,  //  002E  GETGBL	R6	G24
      0x581C00C6,  //  002F  LDCONST	R7	K198
      0x5C200800,  //  0030  MOVE	R8	R4
      0x8824010E,  //  0031  GETMBR	R9	R0	K14
      0x7C180600,  //  0032  CALL	R6	3
      0x581C0022,  //  0033  LDCONST	R7	K34
      0x7C140400,  //  0034  CALL	R5	2
      0x60140008,  //  0035  GETGBL	R5	G8
      0x8818010E,  //  0036  GETMBR	R6	R0	K14
      0x7C140200,  //  0037  CALL	R5	1
      0x8818014E,  //  0038  GETMBR	R6	R0	K78
      0x601C0008,  //  0039  GETGBL	R7	G8
      0x5C200800,  //  003A  MOVE	R8	R4
      0x7C1C0200,  //  003B  CALL	R7	1
      0x8820014E,  //  003C  GETMBR	R8	R0	K78
      0x941C1007,  //  003D  GETIDX	R7	R8	R7
      0x98180A07,  //  003E  SETIDX	R6	R5	R7
      0x8814014E,  //  003F  GETMBR	R5	R0	K78
      0x8C140B72,  //  0040  GETMET	R5	R5	K114
      0x601C0008,  //  0041  GETGBL	R7	G8
      0x5C200800,  //  0042  MOVE	R8	R4
      0x7C1C0200,  //  0043  CALL	R7	1
      0x7C140400,  //  0044  CALL	R5	2
      0x8814010E,  //  0045  GETMBR	R5	R0	K14
      0x00140B21,  //  0046  ADD	R5	R5	K33
      0x90021C05,  //  0047  SETMBR	R0	K14	R5
      0x7001FFCE,  //  0048  JMP		#0018
      0x580C003A,  //  0049  LDCONST	R3	K58
      0xAC0C0200,  //  004A  CATCH	R3	1	0
      0xB0080000,  //  004B  RAISE	2	R0	R0
      0x80040200,  //  004C  RET	1	R1
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: read_sensors_scheduler
********************************************************************/
be_local_closure(class_Matter_Device_read_sensors_scheduler,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(read_sensors_scheduler),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x88040186,  //  0000  GETMBR	R1	R0	K134
      0x4C080000,  //  0001  LDNIL	R2
      0x1C040202,  //  0002  EQ	R1	R1	R2
      0x78060000,  //  0003  JMPF	R1	#0005
      0x80000200,  //  0004  RET	0
      0x88040187,  //  0005  GETMBR	R1	R0	K135
      0x1C04030A,  //  0006  EQ	R1	R1	K10
      0x74060004,  //  0007  JMPT	R1	#000D
      0xB8060200,  //  0008  GETNGBL	R1	K1
      0x8C0403C7,  //  0009  GETMET	R1	R1	K199
      0x880C0187,  //  000A  GETMBR	R3	R0	K135
      0x7C040400,  //  000B  CALL	R1	2
      0x78060006,  //  000C  JMPF	R1	#0014
      0x8C0401C8,  //  000D  GETMET	R1	R0	K200
      0x7C040200,  //  000E  CALL	R1	1
      0xB8060200,  //  000F  GETNGBL	R1	K1
      0x8C0403C9,  //  0010  GETMET	R1	R1	K201
      0x880C0186,  //  0011  GETMBR	R3	R0	K134
      0x7C040400,  //  0012  CALL	R1	2
      0x90030E01,  //  0013  SETMBR	R0	K135	R1
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: save_before_restart
********************************************************************/
be_local_closure(class_Matter_Device_save_before_restart,   /* name */
  be_nested_proto(
    3,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(save_before_restart),
    &be_const_str_solidified,
    ( &(const binstruction[ 7]) {  /* code */
      0x88040112,  //  0000  GETMBR	R1	R0	K18
      0x8C04033C,  //  0001  GETMET	R1	R1	K60
      0x7C040200,  //  0002  CALL	R1	1
      0x88040112,  //  0003  GETMBR	R1	R0	K18
      0x8C0403CA,  //  0004  GETMET	R1	R1	K202
      0x7C040200,  //  0005  CALL	R1	1
      0x80000000,  //  0006  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: adjust_next_ep
********************************************************************/
be_local_closure(class_Matter_Device_adjust_next_ep,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(adjust_next_ep),
    &be_const_str_solidified,
    ( &(const binstruction[21]) {  /* code */
      0x60040010,  //  0000  GETGBL	R1	G16
      0x8808014E,  //  0001  GETMBR	R2	R0	K78
      0x8C080538,  //  0002  GETMET	R2	R2	K56
      0x7C080200,  //  0003  CALL	R2	1
      0x7C040200,  //  0004  CALL	R1	1
      0xA802000A,  //  0005  EXBLK	0	#0011
      0x5C080200,  //  0006  MOVE	R2	R1
      0x7C080000,  //  0007  CALL	R2	0
      0x600C0009,  //  0008  GETGBL	R3	G9
      0x5C100400,  //  0009  MOVE	R4	R2
      0x7C0C0200,  //  000A  CALL	R3	1
      0x8810010E,  //  000B  GETMBR	R4	R0	K14
      0x28100604,  //  000C  GE	R4	R3	R4
      0x78120001,  //  000D  JMPF	R4	#0010
      0x00100721,  //  000E  ADD	R4	R3	K33
      0x90021C04,  //  000F  SETMBR	R0	K14	R4
      0x7001FFF4,  //  0010  JMP		#0006
      0x5804003A,  //  0011  LDCONST	R1	K58
      0xAC040200,  //  0012  CATCH	R1	1	0
      0xB0080000,  //  0013  RAISE	2	R0	R0
      0x80000000,  //  0014  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: every_250ms
********************************************************************/
be_local_closure(class_Matter_Device_every_250ms,   /* name */
  be_nested_proto(
    4,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(every_250ms),
    &be_const_str_solidified,
    ( &(const binstruction[15]) {  /* code */
      0x8C0401CB,  //  0000  GETMET	R1	R0	K203
      0x7C040200,  //  0001  CALL	R1	1
      0x5804000A,  //  0002  LDCONST	R1	K10
      0x6008000C,  //  0003  GETGBL	R2	G12
      0x880C010B,  //  0004  GETMBR	R3	R0	K11
      0x7C080200,  //  0005  CALL	R2	1
      0x14080202,  //  0006  LT	R2	R1	R2
      0x780A0005,  //  0007  JMPF	R2	#000E
      0x8808010B,  //  0008  GETMBR	R2	R0	K11
      0x94080401,  //  0009  GETIDX	R2	R2	R1
      0x8C0805CC,  //  000A  GETMET	R2	R2	K204
      0x7C080200,  //  000B  CALL	R2	1
      0x00040321,  //  000C  ADD	R1	R1	K33
      0x7001FFF4,  //  000D  JMP		#0003
      0x80000000,  //  000E  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified function: autoconf_device
********************************************************************/
be_local_closure(class_Matter_Device_autoconf_device,   /* name */
  be_nested_proto(
    5,                          /* nstack */
    1,                          /* argc */
    10,                          /* varg */
    0,                          /* has upvals */
    NULL,                       /* no upvals */
    0,                          /* has sup protos */
    NULL,                       /* no sub protos */
    1,                          /* has constants */
    &be_ktab_class_Matter_Device,     /* shared constants */
    be_str_weak(autoconf_device),
    &be_const_str_solidified,
    ( &(const binstruction[50]) {  /* code */
      0xA4068600,  //  0000  IMPORT	R1	K67
      0x6008000C,  //  0001  GETGBL	R2	G12
      0x880C010B,  //  0002  GETMBR	R3	R0	K11
      0x7C080200,  //  0003  CALL	R2	1
      0x2408050A,  //  0004  GT	R2	R2	K10
      0x780A0000,  //  0005  JMPF	R2	#0007
      0x80000400,  //  0006  RET	0
      0x880801CD,  //  0007  GETMBR	R2	R0	K205
      0x4C0C0000,  //  0008  LDNIL	R3
      0x1C080403,  //  0009  EQ	R2	R2	R3
      0x780A0004,  //  000A  JMPF	R2	#0010
      0xB80A0600,  //  000B  GETNGBL	R2	K3
      0x8C0805CE,  //  000C  GETMET	R2	R2	K206
      0x5C100000,  //  000D  MOVE	R4	R0
      0x7C080400,  //  000E  CALL	R2	2
      0x90039A02,  //  000F  SETMBR	R0	K205	R2
      0x8808010C,  //  0010  GETMBR	R2	R0	K12
      0x740A000F,  //  0011  JMPT	R2	#0022
      0x880801CD,  //  0012  GETMBR	R2	R0	K205
      0x8C0805CF,  //  0013  GETMET	R2	R2	K207
      0x7C080200,  //  0014  CALL	R2	1
      0x90029C02,  //  0015  SETMBR	R0	K78	R2
      0x60080013,  //  0016  GETGBL	R2	G19
      0x7C080000,  //  0017  CALL	R2	0
      0x90021A02,  //  0018  SETMBR	R0	K13	R2
      0x8C080162,  //  0019  GETMET	R2	R0	K98
      0x7C080200,  //  001A  CALL	R2	1
      0xB80A4A00,  //  001B  GETNGBL	R2	K37
      0x600C0008,  //  001C  GETGBL	R3	G8
      0x8810014E,  //  001D  GETMBR	R4	R0	K78
      0x7C0C0200,  //  001E  CALL	R3	1
      0x000FA003,  //  001F  ADD	R3	K208	R3
      0x58100061,  //  0020  LDCONST	R4	K97
      0x7C080400,  //  0021  CALL	R2	2
      0x880801CD,  //  0022  GETMBR	R2	R0	K205
      0x8C0805D1,  //  0023  GETMET	R2	R2	K209
      0x8810014E,  //  0024  GETMBR	R4	R0	K78
      0x7C080400,  //  0025  CALL	R2	2
      0x8808010C,  //  0026  GETMBR	R2	R0	K12
      0x740A0008,  //  0027  JMPT	R2	#0031
      0x88080115,  //  0028  GETMBR	R2	R0	K21
      0x8C0805BA,  //  0029  GETMET	R2	R2	K186
      0x7C080200,  //  002A  CALL	R2	1
      0x2408050A,  //  002B  GT	R2	R2	K10
      0x780A0003,  //  002C  JMPF	R2	#0031
      0x50080200,  //  002D  LDBOOL	R2	1	0
      0x90021802,  //  002E  SETMBR	R0	K12	R2
      0x8C08016B,  //  002F  GETMET	R2	R0	K107
      0x7C080200,  //  0030  CALL	R2	1
      0x80000000,  //  0031  RET	0
    })
  )
);
/*******************************************************************/


/********************************************************************
** Solidified class: Matter_Device
********************************************************************/
be_local_class(Matter_Device,
    23,
    NULL,
    be_nested_map(80,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(probe_sensor_time, 5), be_const_var(21) },
        { be_const_key_weak(button_pressed, -1), be_const_closure(class_Matter_Device_button_pressed_closure) },
        { be_const_key_weak(button_multi_pressed, -1), be_const_closure(class_Matter_Device_button_multi_pressed_closure) },
        { be_const_key_weak(root_passcode, -1), be_const_var(16) },
        { be_const_key_weak(find_plugin_by_endpoint, 75), be_const_closure(class_Matter_Device_find_plugin_by_endpoint_closure) },
        { be_const_key_weak(every_250ms, 9), be_const_closure(class_Matter_Device_every_250ms_closure) },
        { be_const_key_weak(UDP_PORT, -1), be_const_int(5540) },
        { be_const_key_weak(remove_fabric, 47), be_const_closure(class_Matter_Device_remove_fabric_closure) },
        { be_const_key_weak(msg_received, 64), be_const_closure(class_Matter_Device_msg_received_closure) },
        { be_const_key_weak(plugins_classes, 61), be_const_simple_instance(be_nested_simple_instance(&be_class_map, {
        be_const_map( *     be_nested_map(53,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_const_key_weak(occupancy, -1), be_const_class(be_class_Matter_Plugin_Sensor_Occupancy) },
        { be_const_key_weak(pressure, -1), be_const_class(be_class_Matter_Plugin_Sensor_Pressure) },
        { be_const_key_weak(http_light2, -1), be_const_class(be_class_Matter_Plugin_Bridge_Light2) },
        { be_const_key_weak(v_contact, 36), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Contact) },
        { be_const_key_weak(v_occupancy, -1), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Occupancy) },
        { be_const_key_weak(flow, 8), be_const_class(be_class_Matter_Plugin_Sensor_Flow) },
        { be_const_key_weak(http_contact, -1), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Contact) },
        { be_const_key_weak(shutter, -1), be_const_class(be_class_Matter_Plugin_Shutter) },
        { be_const_key_weak(v_pressure, -1), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Pressure) },
        { be_const_key_weak(http_rain, -1), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Rain) },
        { be_const_key_weak(http_pressure, 12), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Pressure) },
        { be_const_key_weak(v_light2, -1), be_const_class(be_class_Matter_Plugin_Virt_Light2) },
        { be_const_key_weak(v_flow, -1), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Flow) },
        { be_const_key_weak(relay, -1), be_const_class(be_class_Matter_Plugin_OnOff) },
        { be_const_key_weak(v_illuminance, 29), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Illuminance) },
        { be_const_key_weak(v_airquality, -1), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Air_Quality) },
        { be_const_key_weak(temperature, 20), be_const_class(be_class_Matter_Plugin_Sensor_Temp) },
        { be_const_key_weak(waterleak, -1), be_const_class(be_class_Matter_Plugin_Sensor_Waterleak) },
        { be_const_key_weak(v_fan, -1), be_const_class(be_class_Matter_Plugin_Virt_Fan) },
        { be_const_key_weak(http_occupancy, 6), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Occupancy) },
        { be_const_key_weak(http_relay, -1), be_const_class(be_class_Matter_Plugin_Bridge_OnOff) },
        { be_const_key_weak(fan, -1), be_const_class(be_class_Matter_Plugin_Fan) },
        { be_const_key_weak(light1, -1), be_const_class(be_class_Matter_Plugin_Light1) },
        { be_const_key_weak(root, -1), be_const_class(be_class_Matter_Plugin_Root) },
        { be_const_key_weak(illuminance, -1), be_const_class(be_class_Matter_Plugin_Sensor_Illuminance) },
        { be_const_key_weak(v_temp, 15), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Temp) },
        { be_const_key_weak(v_light1, -1), be_const_class(be_class_Matter_Plugin_Virt_Light1) },
        { be_const_key_weak(v_waterleak, -1), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Waterleak) },
        { be_const_key_weak(v_relay, -1), be_const_class(be_class_Matter_Plugin_Virt_OnOff) },
        { be_const_key_weak(contact, 52), be_const_class(be_class_Matter_Plugin_Sensor_Contact) },
        { be_const_key_weak(light2, 27), be_const_class(be_class_Matter_Plugin_Light2) },
        { be_const_key_weak(http_light1, -1), be_const_class(be_class_Matter_Plugin_Bridge_Light1) },
        { be_const_key_weak(gensw_btn, -1), be_const_class(be_class_Matter_Plugin_Sensor_GenericSwitch_Btn) },
        { be_const_key_weak(onoff, -1), be_const_class(be_class_Matter_Plugin_Sensor_OnOff) },
        { be_const_key_weak(v_light0, -1), be_const_class(be_class_Matter_Plugin_Virt_Light0) },
        { be_const_key_weak(light3, -1), be_const_class(be_class_Matter_Plugin_Light3) },
        { be_const_key_weak(http_humidity, -1), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Humidity) },
        { be_const_key_weak(http_illuminance, -1), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Illuminance) },
        { be_const_key_weak(v_rain, 43), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Rain) },
        { be_const_key_weak(http_light0, -1), be_const_class(be_class_Matter_Plugin_Bridge_Light0) },
        { be_const_key_weak(http_waterleak, 45), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Waterleak) },
        { be_const_key_weak(shutter_X2Btilt, -1), be_const_class(be_class_Matter_Plugin_ShutterTilt) },
        { be_const_key_weak(v_light3, -1), be_const_class(be_class_Matter_Plugin_Virt_Light3) },
        { be_const_key_weak(airquality, 35), be_const_class(be_class_Matter_Plugin_Sensor_Air_Quality) },
        { be_const_key_weak(http_flow, -1), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Flow) },
        { be_const_key_weak(humidity, -1), be_const_class(be_class_Matter_Plugin_Sensor_Humidity) },
        { be_const_key_weak(http_temperature, 41), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Temp) },
        { be_const_key_weak(http_light3, -1), be_const_class(be_class_Matter_Plugin_Bridge_Light3) },
        { be_const_key_weak(v_humidity, 1), be_const_class(be_class_Matter_Plugin_Virt_Sensor_Humidity) },
        { be_const_key_weak(http_airquality, -1), be_const_class(be_class_Matter_Plugin_Bridge_Sensor_Air_Quality) },
        { be_const_key_weak(aggregator, 11), be_const_class(be_class_Matter_Plugin_Aggregator) },
        { be_const_key_weak(rain, -1), be_const_class(be_class_Matter_Plugin_Sensor_Rain) },
        { be_const_key_weak(light0, -1), be_const_class(be_class_Matter_Plugin_Light0) },
    }))    ) } )) },
        { be_const_key_weak(add_read_sensors_schedule, -1), be_const_closure(class_Matter_Device_add_read_sensors_schedule_closure) },
        { be_const_key_weak(register_commands, 59), be_const_closure(class_Matter_Device_register_commands_closure) },
        { be_const_key_weak(k2l, -1), be_const_static_closure(class_Matter_Device_k2l_closure) },
        { be_const_key_weak(http_remotes, -1), be_const_var(14) },
        { be_const_key_weak(MtrJoin, -1), be_const_closure(class_Matter_Device_MtrJoin_closure) },
        { be_const_key_weak(attribute_updated, -1), be_const_closure(class_Matter_Device_attribute_updated_closure) },
        { be_const_key_weak(find_plugin_by_friendly_name, 32), be_const_closure(class_Matter_Device_find_plugin_by_friendly_name_closure) },
        { be_const_key_weak(save_before_restart, 57), be_const_closure(class_Matter_Device_save_before_restart_closure) },
        { be_const_key_weak(save_param, 6), be_const_closure(class_Matter_Device_save_param_closure) },
        { be_const_key_weak(load_param, -1), be_const_closure(class_Matter_Device_load_param_closure) },
        { be_const_key_weak(register_http_remote, -1), be_const_closure(class_Matter_Device_register_http_remote_closure) },
        { be_const_key_weak(FILENAME, -1), be_nested_str_weak(_matter_device_X2Ejson) },
        { be_const_key_weak(sort_distinct, 77), be_const_static_closure(class_Matter_Device_sort_distinct_closure) },
        { be_const_key_weak(read_sensors_scheduler, -1), be_const_closure(class_Matter_Device_read_sensors_scheduler_closure) },
        { be_const_key_weak(received_ack, -1), be_const_closure(class_Matter_Device_received_ack_closure) },
        { be_const_key_weak(every_second, -1), be_const_closure(class_Matter_Device_every_second_closure) },
        { be_const_key_weak(plugins, -1), be_const_var(1) },
        { be_const_key_weak(get_plugin_remote_info, 54), be_const_closure(class_Matter_Device_get_plugin_remote_info_closure) },
        { be_const_key_weak(sessions, -1), be_const_var(10) },
        { be_const_key_weak(resolve_attribute_read_solo, -1), be_const_closure(class_Matter_Device_resolve_attribute_read_solo_closure) },
        { be_const_key_weak(clean_remotes, -1), be_const_closure(class_Matter_Device_clean_remotes_closure) },
        { be_const_key_weak(start, -1), be_const_closure(class_Matter_Device_start_closure) },
        { be_const_key_weak(next_ep, 1), be_const_var(19) },
        { be_const_key_weak(msg_send, -1), be_const_closure(class_Matter_Device_msg_send_closure) },
        { be_const_key_weak(button_handler, -1), be_const_closure(class_Matter_Device_button_handler_closure) },
        { be_const_key_weak(init, 10), be_const_closure(class_Matter_Device_init_closure) },
        { be_const_key_weak(plugins_persist, -1), be_const_var(2) },
        { be_const_key_weak(stop, -1), be_const_closure(class_Matter_Device_stop_closure) },
        { be_const_key_weak(_start_udp, -1), be_const_closure(class_Matter_Device__start_udp_closure) },
        { be_const_key_weak(probe_sensor_timestamp, 73), be_const_var(22) },
        { be_const_key_weak(ipv4only, 60), be_const_var(17) },
        { be_const_key_weak(VENDOR_ID, -1), be_const_int(65521) },
        { be_const_key_weak(MtrInfo, -1), be_const_closure(class_Matter_Device_MtrInfo_closure) },
        { be_const_key_weak(events, -1), be_const_var(13) },
        { be_const_key_weak(PRODUCT_ID, -1), be_const_int(32768) },
        { be_const_key_weak(MtrInfo_one, -1), be_const_closure(class_Matter_Device_MtrInfo_one_closure) },
        { be_const_key_weak(started, 69), be_const_var(0) },
        { be_const_key_weak(_trigger_read_sensors, -1), be_const_closure(class_Matter_Device__trigger_read_sensors_closure) },
        { be_const_key_weak(EP, -1), be_const_int(2) },
        { be_const_key_weak(disable_bridge_mode, -1), be_const_var(18) },
        { be_const_key_weak(ui, 76), be_const_var(11) },
        { be_const_key_weak(get_plugin_class_arg, -1), be_const_closure(class_Matter_Device_get_plugin_class_arg_closure) },
        { be_const_key_weak(profiler, -1), be_const_var(6) },
        { be_const_key_weak(bridge_add_endpoint, -1), be_const_closure(class_Matter_Device_bridge_add_endpoint_closure) },
        { be_const_key_weak(invoke_request, 50), be_const_closure(class_Matter_Device_invoke_request_closure) },
        { be_const_key_weak(signal_endpoints_changed, -1), be_const_closure(class_Matter_Device_signal_endpoints_changed_closure) },
        { be_const_key_weak(reset_param, -1), be_const_closure(class_Matter_Device_reset_param_closure) },
        { be_const_key_weak(plugins_config, 56), be_const_var(3) },
        { be_const_key_weak(plugins_config_remotes, 17), be_const_var(4) },
        { be_const_key_weak(MtrUpdate, 26), be_const_closure(class_Matter_Device_MtrUpdate_closure) },
        { be_const_key_weak(commissioning, -1), be_const_var(8) },
        { be_const_key_weak(conf_to_log, 53), be_const_static_closure(class_Matter_Device_conf_to_log_closure) },
        { be_const_key_weak(event_fabrics_saved, -1), be_const_closure(class_Matter_Device_event_fabrics_saved_closure) },
        { be_const_key_weak(process_attribute_expansion, -1), be_const_closure(class_Matter_Device_process_attribute_expansion_closure) },
        { be_const_key_weak(every_50ms, -1), be_const_closure(class_Matter_Device_every_50ms_closure) },
        { be_const_key_weak(update_remotes_info, -1), be_const_closure(class_Matter_Device_update_remotes_info_closure) },
        { be_const_key_weak(get_plugin_class_displayname, -1), be_const_closure(class_Matter_Device_get_plugin_class_displayname_closure) },
        { be_const_key_weak(bridge_remove_endpoint, -1), be_const_closure(class_Matter_Device_bridge_remove_endpoint_closure) },
        { be_const_key_weak(autoconf, 43), be_const_var(9) },
        { be_const_key_weak(check_network, -1), be_const_closure(class_Matter_Device_check_network_closure) },
        { be_const_key_weak(udp_server, -1), be_const_var(5) },
        { be_const_key_weak(k2l_num, 23), be_const_static_closure(class_Matter_Device_k2l_num_closure) },
        { be_const_key_weak(debug, 21), be_const_var(20) },
        { be_const_key_weak(message_handler, -1), be_const_var(7) },
        { be_const_key_weak(adjust_next_ep, -1), be_const_closure(class_Matter_Device_adjust_next_ep_closure) },
        { be_const_key_weak(tick, -1), be_const_var(12) },
        { be_const_key_weak(get_active_endpoints, -1), be_const_closure(class_Matter_Device_get_active_endpoints_closure) },
        { be_const_key_weak(root_discriminator, -1), be_const_var(15) },
        { be_const_key_weak(check_config_ep, 3), be_const_closure(class_Matter_Device_check_config_ep_closure) },
        { be_const_key_weak(autoconf_device, -1), be_const_closure(class_Matter_Device_autoconf_device_closure) },
    })),
    be_str_weak(Matter_Device)
);
/********************************************************************/
/* End of solidification */
